#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

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
constexpr char MQTT_CLIENT_ID[] = "F4650BBB3EDC";
constexpr char MQTT_TOPIC_STATUS[] = "F4650BBB3EDC_ALM";
constexpr char MQTT_TOPIC_BOOT[] = "F4650BBB3EDC_ACK";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WiFiManager wm;

bool ledState = false;
bool lastButtonReading = HIGH;
bool debouncedButtonState = HIGH;
uint32_t lastBlinkMs = 0;
uint32_t lastMqttReconnectMs = 0;
uint32_t lastDebounceMs = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  (void)topic;
  (void)payload;
  (void)length;
  // Add subscription handling here when needed.
}

void setupOTA() {
  ArduinoOTA.setHostname(MQTT_CLIENT_ID);
  ArduinoOTA.begin();
}

void connectMqttIfNeeded(uint32_t nowMs) {
  if (mqttClient.connected()) {
    return;
  }

  if ((nowMs - lastMqttReconnectMs) < MQTT_RECONNECT_INTERVAL_MS) {
    return;
  }
  lastMqttReconnectMs = nowMs;

  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
    mqttClient.publish(MQTT_TOPIC_STATUS, "online", true);
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
      mqttClient.publish(MQTT_TOPIC_STATUS, "pressed", false);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  // Starts captive portal if credentials are not already saved.
  wm.autoConnect("ESP32S2-Setup");

  setupOTA();
}

void loop() {
  const uint32_t nowMs = millis();

  ArduinoOTA.handle();
  connectMqttIfNeeded(nowMs);
  mqttClient.loop();
  handleBlink(nowMs);
  handleBootButton(nowMs);
}
