/*
 * ESP32 Based MQTT Gateway For RFM69HCW & ESP-NOW Both
 * 
 * Connect Sensors with Arduino & ESP8266 / ESP32 With Any Technology (RFM69 Module + ESP-Now)
 * 
 * Created by Ajay Kumar 
 * Email: ajaybnl@gmail.com
 * 
 * Version: 0.1
 * 
 * Update Url : https://github.com/ajaybnl/esp32_rfm69_espnow_mqtt_gateway
 * 
 * 
 * 
 */
 //All Declarations:
#include "include.h"

/*

   SETUP
*/

void setup() {
  
  //Status Led (Blink 1: ESP-Now Message, Blink Twice: RFM69 Message, Blinking Rapidly: ESP-NOW Not Connected)
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //Debug Serial (DBG=true)
  if (DBG)Serial.begin(115200);
  delay(1000);

  //ESP32 Fix (Optional)
  if (nobrownout) WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  if (lowtxpower) WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);


  //Init RFM69
  initrfm();
  //Init ESP-Now
  initespnow();

  
}


/*

   LOOP
*/


void loop() {
  //RFM69 Rx Loop
  rfmloop();

//Received Some Data
  if (rxe | rxr) {
    rxe = false;
    rxr = false;
    
    wifionsendmqtt();
    
    //Start ESP-NOW Again
    initespnow();
  }


//Do Periodic stuff every 5sec
  if ((((unsigned long)(millis() - timer1 )) ) >= 5000) {
    timer1 = millis();
    //do stuff

  }



  


}
