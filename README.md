# Summary
Learning to code with CODEX

## Steps Details
1. Planning to start with a BLINK sketch (From the Arduino examples) but name it "Project_First_CODEX" for compatibility in this folder with the Arduino IDE.
2. Customize for ESP32-S2 Dev K1 with an LED on GPIO 2.  
3. Identify and remove "magic numbers" from body of code, and placing descriptive constant names at the top of file for the magic numbers. 
4. Remove delay() (reference multitasking coding article https://learn.adafruit.com/multi-tasking-the-arduino-part-1 and parts 2, and 3)
5. In setup() inform the user of the progress. Start with the LED on. On the serial port inform the user of the start and end of setup. Set BAUD for 115200.  At end of setup() turn the LED off.
6. Add WiFi Manager and OTA (use libraries tzapu/WiFiManager@^2.0.17 ayushsharma82/ElegantOTA@^3.1.7)
7. Add MQTT PubSub  (use knolleary/PubSubClient@^2.8). Set Broker to "mqtt://public:public@public.cloud.shiftr.io". Publish to "F4650BBB3EDC_ALM" and Subscribe to "F4650BBB3EDC_ACK"
8. Make Boot Button (GPIO0) Publish a payload "a1Lee is in Discord" (Use for debounce libraries cygig/DailyStruggleButton@^0.5.1)
