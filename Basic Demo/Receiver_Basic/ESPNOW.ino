void setupESPNOW()
{
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);
  start_ESP_NOW_AccessPoint();
}

void InitESPNow() {
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.print(*data); Serial.println(" bytes");
  //The sensorData struct is stored in credentials.h
  memcpy(&sensorData, data, sizeof(sensorData));
  Serial.print("Received Topic ");
  Serial.println(sensorData.topic);
  Serial.print("Payload  ");
  Serial.println(sensorData.payload);

  if (sensorData.retain)
  {
    Serial.println("Retain True");
  } else {
    Serial.println("Retain False");
  }

  /*
     Publish the topic and payload from the sensor to the MQTT broker with a retain flag as false.
     TO DO: Add a retain Boolean variable to the  sensorData struct to allow the retain flag
     to be set on a packet by packet basis.
  */
  client.publish(sensorData.topic, sensorData.payload, sensorData.retain);

}
void start_ESP_NOW_AccessPoint() {
  /*
     The ESP_NOW_PASSWORD variable is stored in credentials.h
     For ESP-NOW we only use the softAP so the Sensors (Masters)
     can find buy scaning for WiFi access points and get the
     MAC address and channel that the WiFi interface is running.

     The ESP-NOW client does't not make a WiFi connection to the softAP,
     however it is a full blown access point and can be accessed by any
     WiFi client like a smartphone device that knows the SSID and Password.

     If a client does connect to the access point it will be issued a local
     chip based IP address and not an IP address from your WiFi network.
     By default there is not routing code to allow your WiFi network to be accessed.

     To be safe, make the password long and random.

  */
  bool result = WiFi.softAP(full_esp_now_ssid, ESP_NOW_PASSWORD, WiFi.channel());
  if (!result) {
    Serial.println("AP Config failed.");
    ESP.restart();
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(full_esp_now_ssid));
    setupMQTT();
  }

}

