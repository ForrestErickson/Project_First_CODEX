# ESP32 Dev Kit 1 Embedded System Starter

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
- `MQTT_BROKER` (`public.cloud.shiftr.io`)
- `MQTT_PORT` (`1883`)
- `MQTT_USER` / `MQTT_PASSWORD` (`public` / `public`)
- `MQTT_CLIENT_ID` (`F4650BBB3EDC`)
- MQTT topics publish/subscribe (`F4650BBB3EDC_ALM`, `F4650BBB3EDC_ACK`)

## Notes
- BOOT button is configured on GPIO `0` with `INPUT_PULLUP`.
- LED pin is set to GPIO `2` for ESP32 Dev Kit 1 built-in LED.
