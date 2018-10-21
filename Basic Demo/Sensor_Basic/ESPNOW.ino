void InitESPNow() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    esp_now_register_send_cb(OnDataSent);
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  //This routine is called when we get a response back from the receiver after sending it a packet.

  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  if (status == ESP_NOW_SEND_SUCCESS)
  {
    putMeToSleep();
  } else {
    Serial.println("SHIT, something went wrong. Rebooting");
    delay(2000);
    ESP.restart();
  }
}
void ScanForSlave() {
  Serial.println("Scaning for receivers");
  bool slaveFound = false;
  int currentRSSI = -200;
  int8_t scanResults = WiFi.scanNetworks();
  if (scanResults == 0)
  {
    Serial.println("No WiFi devices in AP Mode found");
    // clean up ram
    WiFi.scanDelete();
    ESP.restart();
    return;
  }
  /*
     scanResults will contain the number of access points
     found with scanNetworks(), this will include every
     access point the WiFi Chip can see including your
     home network and maybe even your neighbors.

     We will now search through the list and find any
     that have an SSID starting with the ESP_NOW_SSID variable
     stored in credentials.h, if you have more then one
     receiver (I have three) the code will select the one with
     the best RSSI signal which should be the closest and store
     the MAC address and channel number in the slave variable.

  */
  Serial.println(String(scanResults) + " Access Points found");
  for (int i = 0; i < scanResults; ++i)
  {
    Serial.print("SSID "); Serial.print(WiFi.SSID(i));
    if (WiFi.SSID(i).indexOf(ESP_NOW_SSID) == 0)
    {
      Serial.print(" match found.");
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (slaveFound)
      {
        //We have already found a match but is this one better?
        if (currentRSSI < WiFi.RSSI(i))
        {
          Serial.print("Got a better offer" ); Serial.print(currentRSSI); Serial.print(" "); Serial.println(WiFi.RSSI(i));
          currentRSSI = WiFi.RSSI(i);
          slave.channel = WiFi.channel(i); // pick a channel
          slave.encrypt = 0; // no encryption
          int mac[6];
          if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
            for (int ii = 0; ii < 6; ++ii ) {
              slave.peer_addr[ii] = (uint8_t) mac[ii];
            }
          }
        }
      } else {
        //We have found our first match.
        slaveFound = true;
        currentRSSI = WiFi.RSSI(i);
        slave.channel = WiFi.channel(i); // pick a channel
        slave.encrypt = 0; // no encryption
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }
      }
    }
    Serial.println("");
  }

}


bool manageSlave() {

  /*
     We need to pair the device. This code if from espressif
     and I am not sure what it does or how it works but it
     must be called before sending a packet after wakeup.
  */


  if (bootCount > -1) {
    Serial.print("Slave Status: ");
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}

