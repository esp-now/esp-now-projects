void sendButtonPress()
{
  //Get the WiFi MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  sprintf(sensorData.topic, "%s%02X%02X%02X%02X%02X%02X", arrTopic, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  /*
   * ESP32/TestButton/240AC40D0DB8
   */
  Serial.println(sensorData.topic);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
 /* 
  *  This demo only has one button. 
  *  My Remote and Photo frame projects have six buttons.
  */
  root["BTN"] = 1;
  root["Vcc"] = getMyVcc(); 
  root.printTo(sensorData.payload);
  Serial.println(sensorData.payload);
  uint8_t bs[sizeof(sensorData)];
  memcpy(bs, &sensorData, sizeof(sensorData));
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, bs, sizeof(sensorData));
  /*
   * A ESP_OK result does not guarantee the packet made it, 
   * it only means that ESP-NOW is working and the chip can 
   * see the receiver, the OnDataSent routine in the ESPNOW 
   * tab tells us if the packet was received OK.
   */
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }


}


double getMyVcc()
{
  int internalBatReading = rom_phy_get_vdd33();
  double vcc = (double)(((uint32_t)internalBatReading * 2960) / 2798) / 1000;
  /*
   * Ever since espressif released there latest arduino core the rom_phy_get_vdd33() 
   * returns a weird number. The best I can tell is it's double the actual reading 
   * so if the result is over 4 volts I half it before returning. 
   * This should allow the code to work both with the new core and the original one.
   */
  if (vcc > 4)
    vcc = (vcc / 2);
  return vcc;
}
