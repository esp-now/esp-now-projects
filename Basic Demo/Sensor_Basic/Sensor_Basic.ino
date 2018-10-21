extern "C" int rom_phy_get_vdd33();

#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <ArduinoJson.h>

//WiFi Creds
#include "credentials.h"

/*
    The RTC_DATA_ATTR attribute stores the variable in the ESP's
    real time clock memory so it can survive deep sleep.

    The variable is still cleared when the ESP is reset
    or ESP.restart(); is called.
*/
//Deep Sleep
/*
   We increment bootCount each time the setup()
   called but at the end of the day we only
   use it to indicate the first boot
   after reset or restart.
*/
RTC_DATA_ATTR int bootCount = 0;
esp_sleep_wakeup_cause_t wakeup_reason;
//ESP-NOW
RTC_DATA_ATTR esp_now_peer_info_t slave;

//MQTT
/*
   arrTopic is the prefix for the MQTT topic
   and the WiFi MAC address is added to the
   string at packet send time.

   This variable is not stored in credentials.h
   because you may want different topics for
   different types of devices.

   I have
   ESP32/DOOR
   ESP32/MOTION
   ESP32/Remote
   ESP32/PHOTO with no doubt more to come.
*/
char *arrTopic = "ESP32/TestButton/";


void setup() {
  Serial.begin(115200);
  //Set the button pin HIGH
  pinMode(GPIO_NUM_33, INPUT_PULLUP); //Pin 9 on the WROOM-32
  InitESPNow() ; //ESP-NOW Tab
  //Set WiFi chip to max power
  esp_wifi_set_max_tx_power(78);
  if (bootCount == 0)
  {
    ScanForSlave(); //ESP-NOW Tab Remember Slave = Receiver
    putMeToSleep(); //Deep Sleep Tab
  } else {
    esp_wifi_set_channel(slave.channel, WIFI_SECOND_CHAN_NONE);
    manageSlave(); //ESP-NOW Tab
    print_wakeup_reason(); //Deep Sleep Tab
    if (wakeup_reason == 1)
    {
      sendButtonPress(); //MQTT Tab
    } else {
      putMeToSleep(); //Deep Sleep Tab
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() > 20000 && digitalRead(GPIO_NUM_33) == 1)
  {
    putMeToSleep(); //Deep Sleep Tab
  }
}
