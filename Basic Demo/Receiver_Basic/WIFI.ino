void setupWiFi()
{
  /*
   * WiFi.begin() without any parameters simply enables the radio 
   * so we can get the MAC address (Media Access Control). 
   * Which is unique to each network device making it perfect for use as ID.
   */
  WiFi.begin();
  setMyHostName();
}

void setMyHostName()
{
  byte mac[6];
  WiFi.macAddress(mac);
  sprintf(myHostName, "%s%02X%02X%02X%02X%02X%02X", "ESPNOW", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  sprintf(full_esp_now_ssid, "%s%02X%02X%02X%02X%02X%02X", ESP_NOW_SSID, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println("System ID's");
  Serial.println("Hostname " + String(myHostName));
  Serial.println("ESP-NOW SSID " + String(full_esp_now_ssid));
  connectToHomeNetwork();
}

void connectToHomeNetwork()
{
  //Set the network hostname
  WiFi.setHostname(myHostName);
/*
 * WiFi.mode needs to be WIFI_AP_STA allowing it to be both an 
 * Access Point (ESP-NOW) and a Station (Home WiFi Network) at the same time.
 * 
 */
  WiFi.mode(WIFI_AP_STA);
  //The WIFI_SSID and WIFI_PASSWORD variables are stored in credentials.h
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  IPAddress ip; 
  ip = WiFi.localIP();
  Serial.print("WiFi connected IP: "); Serial.println(ip);
  
  setupESPNOW();
}
