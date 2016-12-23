#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttConnector.h>
#include <Adafruit_NeoPixel.h>

int screen = 0;
#define PIN 14
Adafruit_NeoPixel strip = Adafruit_NeoPixel(104, PIN, NEO_GRB + NEO_KHZ800);
  
#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"


/* WIFI INFO */
#ifndef WIFI_SSID
#define WIFI_SSID        "ESPERT-3020"
#define WIFI_PASSWORD    "espertap"
#endif

String MQTT_HOST        = "mqtt.cmmc.io";
String MQTT_USERNAME    = "";
String MQTT_PASSWORD    = "";
String MQTT_CLIENT_ID   = "";
String MQTT_PREFIX      = "/CMMC";
int    MQTT_PORT        = 1883;

int PUBLISH_EVERY       = 1000;

MqttConnector *mqtt;

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  strip.begin();
  strip.show();
  Serial.println();
  Serial.println("Starting...");
  pinMode(LED_BUILTIN, OUTPUT);
}

void init_wifi() {
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while(WiFi.status() != WL_CONNECTED) {
     Serial.printf ("Connecting to %s:%s\r\n", WIFI_SSID, WIFI_PASSWORD);
     delay(300);
   }
  Serial.println("WiFi Connected.");
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
  mqtt->loop();
  if(screen == 1) rainbowCycle(3, 1);
  else if(screen == 2) colorWipe(strip.Color(255, 0, 0), 10); 
  else if(screen == 3) colorWipe(strip.Color(0, 255, 0), 10); 
  else if(screen == 4) colorWipe(strip.Color(0, 0, 255), 10); 
  else if(screen == 5) rainbowCycle(10, 10);
  else if(screen == 6) clearStrip();
}
