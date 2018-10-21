void setupMQTT()
{
  //The MQTT_HOST and MQTT_PORT variables are stored in credentials.h
  client.setServer(MQTT_HOST, MQTT_PORT);
  mqttconnect();
}

void mqttconnect()
{
  bool result = client.connect(myHostName, MQTT_USERNAME, MQTT_PASSWORD);
  if (result)
  {
    Serial.println("Connected to MQTT");
  } else {
    ESP.restart();
  }
}
