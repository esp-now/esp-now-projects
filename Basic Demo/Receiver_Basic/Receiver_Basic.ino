//Put all includes for all tabs here in the main tab.
//WiFi Creds
#include "credentials.h"
#include <esp_wifi.h>
#include <esp_now.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//WiFi stuff
/*
 * myHostName will be ESPNOW and the chip's WiFi MAC address.  
 * mhHostName will also be used at the MQTT Client ID
 * eg ESPNOW240AC40B1D88
 *
 */
char myHostName[20];
//ESP-NOW
/*
 * full_esp_now_ssid will be a combination of the variable ESP_NOW_SSID 
 * from credentials.h and the chip's WiFi MAC address.  
 * eg ESPX_240AC40B1D88
 *  * 
 */
char full_esp_now_ssid[30];
WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  delay(20);
  Serial.println("Welcome to Tony's basic ESP-NOW receiver (Slave)");
  setupWiFi();
}

void loop() {
//Only thing we need to handle in the loop is the MQTT Client.
client.loop();
}
