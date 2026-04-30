#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>

// Hardware pins (ESP32-S2)
// ESP32 Dev Kit 1 built-in LED is on GPIO2
constexpr uint8_t LED_PIN = 2;
constexpr uint8_t BOOT_BUTTON_PIN = 0;

// Timings (ms)
constexpr uint32_t BLINK_INTERVAL_MS = 500;
constexpr uint32_t MQTT_RECONNECT_INTERVAL_MS = 5000;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 40;

// MQTT configuration
constexpr char MQTT_BROKER[] = "public.cloud.shiftr.io";
constexpr uint16_t MQTT_PORT = 1883;
constexpr char MQTT_USER[] = "public";
constexpr char MQTT_PASSWORD[] = "public";
constexpr char MQTT_TOPIC_STATUS[] = "F4650BBB3EDC_ALM";
constexpr char MQTT_TOPIC_BOOT[] = "F4650BBB3EDC_ACK";
constexpr char PROGRAM_NAME[] = "Project_First_CODEX";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WiFiManager wm;
WebServer webServer(80);
HTTPUpdateServer httpUpdater;
String deviceClientId;

bool ledState = false;
bool lastButtonReading = HIGH;
bool debouncedButtonState = HIGH;
uint32_t lastBlinkMs = 0;
uint32_t lastMqttReconnectMs = 0;
uint32_t lastDebounceMs = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String payloadText;
  payloadText.reserve(length);
  for (unsigned int i = 0; i < length; ++i) {
    payloadText += static_cast<char>(payload[i]);
  }

  Serial.printf("[MQTT RX] Topic: %s | Payload: %s\n", topic, payloadText.c_str());
}

bool publishMqttWithSerial(const char* topic, const char* payload, bool retained) {
  if (!mqttClient.connected()) {
    Serial.printf("[MQTT TX] Skipped publish (not connected) Topic: %s | Payload: %s\n", topic, payload);
    return false;
  }

  const bool published = mqttClient.publish(topic, payload, retained);
  Serial.printf(
    "[MQTT TX] %s Topic: %s | Payload: %s\n",
    published ? "Published" : "Publish failed",
    topic,
    payload);
  return published;
}

void setupOTA() {
  ArduinoOTA.setHostname(deviceClientId.c_str());
  ArduinoOTA.begin();
}

String getDeviceClientIdFromMac() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  mac.toUpperCase();
  return mac;
}

void setupWebServer() {
  webServer.on("/", HTTP_GET, []() {
    webServer.send(
      200,
      "text/html",
      "<!doctype html><html><head><meta charset='utf-8'><title>Project_First_CODEX</title></head>"
      "<body><h1>Project_First_CODEX.ino</h1><p>ESP32 sketch with Wi-Fi Manager, OTA, MQTT, and button/LED handling.</p>"
      "<p>Firmware update page: <a href='/update'>/update</a></p></body></html>");
  });

  httpUpdater.setup(&webServer, "/update");
  webServer.begin();
}

void connectMqttIfNeeded(uint32_t nowMs) {
  if (mqttClient.connected()) {
    return;
  }

  if ((nowMs - lastMqttReconnectMs) < MQTT_RECONNECT_INTERVAL_MS) {
    return;
  }
  lastMqttReconnectMs = nowMs;

  if (mqttClient.connect(deviceClientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
    publishMqttWithSerial(MQTT_TOPIC_STATUS, "online", true);
    mqttClient.subscribe(MQTT_TOPIC_BOOT);
  }
}

void handleBlink(uint32_t nowMs) {
  if ((nowMs - lastBlinkMs) < BLINK_INTERVAL_MS) {
    return;
  }

  lastBlinkMs = nowMs;
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void handleBootButton(uint32_t nowMs) {
  const bool rawReading = digitalRead(BOOT_BUTTON_PIN);

  if (rawReading != lastButtonReading) {
    lastDebounceMs = nowMs;
    lastButtonReading = rawReading;
  }

  if ((nowMs - lastDebounceMs) < BUTTON_DEBOUNCE_MS) {
    return;
  }

  if (debouncedButtonState != rawReading) {
    debouncedButtonState = rawReading;
    if (debouncedButtonState == LOW && mqttClient.connected()) {
      publishMqttWithSerial(MQTT_TOPIC_STATUS, "pressed", false);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println();
  Serial.printf("[%s] Booting...\n", PROGRAM_NAME);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  Serial.println("Wi-Fi startup mode: station with fallback to soft access point (config portal).");
  // Starts captive portal if credentials are not already saved.
  wm.autoConnect("ESP32S2-Setup");

  deviceClientId = getDeviceClientIdFromMac();
  Serial.printf("Device client ID (from MAC): %s\n", deviceClientId.c_str());

  const wifi_mode_t wifiMode = WiFi.getMode();
  if (wifiMode == WIFI_AP || wifiMode == WIFI_AP_STA) {
    Serial.println("Boot mode: soft access point active.");
    Serial.printf("SoftAP IP: %s\n", WiFi.softAPIP().toString().c_str());
  } else {
    Serial.println("Boot mode: station.");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Station connected to LAN.");
    Serial.printf("STA IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("Station not connected to LAN.");
  }

  setupOTA();
  setupWebServer();
}

void loop() {
  const uint32_t nowMs = millis();

  ArduinoOTA.handle();
  webServer.handleClient();
  connectMqttIfNeeded(nowMs);
  mqttClient.loop();
  handleBlink(nowMs);
  handleBootButton(nowMs);
}
