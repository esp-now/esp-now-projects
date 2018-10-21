void putMeToSleep()
{
  /*
   * For those of you who are use to using the Amel chips
   * where the code continues from where it left off when
   * it returns from sleep, this will be a big change.
   * 
   * The the ESP chips return from sleep to go back to the
   * start and call setup.
   * 
   * Enable the chip to wake up when the button attached 
   * to GPIO_NUM_33 (Pin 9) is pressed.
   */
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  bootCount++;
/* The esp_deep_sleep_start() call is instant and if 
 * you still have the button pressed the chip will 
 * wake up again the send another packet. 
 * 
 * So we will wait until you release the button.
 *  
 */
  do
  {

  } while (digitalRead(GPIO_NUM_33) == 0);
    Serial.println("Going to sleep now");
  delay(100);
  esp_deep_sleep_start();
  //The chip does not wake up here like the arduino UNO
  //The chip will return to Setup() when it wakes up.
}

void print_wakeup_reason() {

  //There are many ways to wake this chip from sleep, we are chasing RTC_IO
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Wakeup caused by touchpad"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
