# ESP32-S2 Embedded System Starter

## Features implemented
- Starts from a **BLINK** baseline using non-blocking timing.
- Replaces magic numbers with `constexpr` constants.
- Removes `delay()` and uses cooperative multitasking patterns.
- Uses **WiFiManager** for Wi-Fi provisioning.
- Enables **OTA** firmware updates with ArduinoOTA.
- Adds **MQTT PubSub** connectivity.
- Publishes a message when the **BOOT button** is pressed.

## Sketch
- `esp32s2_system.ino`

## Required libraries
Install these libraries in Arduino IDE (or PlatformIO equivalents):
- `WiFiManager`
- `PubSubClient`
- `ArduinoOTA` (comes with ESP32 Arduino core)

## Configuration to update
Before flashing, update these constants in the sketch:
- `MQTT_BROKER`
- `MQTT_PORT`
- `MQTT_CLIENT_ID`
- MQTT topics (`MQTT_TOPIC_STATUS`, `MQTT_TOPIC_BOOT`)

## Notes
- BOOT button is configured on GPIO `0` with `INPUT_PULLUP`.
- LED pin uses `LED_BUILTIN`; override if your board uses a different pin.
