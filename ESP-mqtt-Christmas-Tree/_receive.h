extern String MQTT_CLIENT_ID;

#define relayPin 15
int pin_state;
extern MqttConnector* mqtt;
extern char myName[40];

extern uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  screen = 6;
}

void rainbowCycle(uint8_t wait, int count) {
  uint16_t i, j;

  for (j = 0; j < 256 * count; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
  screen = 6;
}

void clearStrip() {
  uint16_t i, j;
  for (j = 0; j < 256 * 1; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
  }
}

void register_receive_hooks() {
  mqtt->on_subscribe([&](MQTT::Subscribe * sub) -> void {
    Serial.printf("myName = %s \r\n", myName);
    sub->add_topic(MQTT_PREFIX + "/" + myName + "/$/+");
    sub->add_topic(MQTT_PREFIX + "/" + MQTT_CLIENT_ID + "/$/+");
  });

  mqtt->on_before_message_arrived_once([&](void) {
    pinMode(15, OUTPUT);
  });

  mqtt->on_message([&](const MQTT::Publish & pub) { });

  mqtt->on_after_message_arrived([&](String topic, String cmd, String payload) {
    Serial.printf("topic: %s\r\n", topic.c_str());
    Serial.printf("cmd: %s\r\n", cmd.c_str());
    Serial.printf("payload: %s\r\n", payload.c_str());
    if (cmd == "$/command") {
      if (payload == "RGB") {
        screen = 1;
      } else if (payload == "R") {
        screen = 2;
      } else if (payload == "G") {
        screen = 3;
      } else if (payload == "B") {
        screen = 4;
      } else if (payload == "RGBALL") {
        screen = 5;
      }
    }
    else if (cmd == "$/reboot") {
      ESP.reset();
    }
    else {

    }
  });
}
