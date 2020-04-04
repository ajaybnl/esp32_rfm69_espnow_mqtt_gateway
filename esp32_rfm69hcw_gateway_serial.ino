/*
 * ESP32 Based MQTT Gateway For RFM69HCW & ESP-NOW Both
 * 
 * Connect Sensors with Arduino & ESP8266 / ESP32 With Any Technology (RFM69 Module + ESP-Now)
 * 
 * Created by Ajay Kumar 
 * Email: ajaybnl@gmail.com
 * 
 * v0.5
 * 
 * Dev Url : https://github.com/ajaybnl/esp32_rfm69_espnow_mqtt_gateway
 * 
 * 
 * 
 */


 //All Declarations are int this file:
 
#include "include.h"

/*

   SETUP
*/

void setup() {
  //ESP32 Fix (Optional)  
   if (nobrownout) WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  //Status Led (Blink 1: ESP-Now Message, Blink Twice: RFM69 Message, Blinking Rapidly: ESP-NOW Not Connected)
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);


//This is a ESPNOW & RFM69 Gateway Project, 
//So i am using it on my Raspberry Pi, On Serial Messaging.

// For Raspberry Mode: (USEMQTT = false)
//Compile it on ESP32, 80Mhz Clock Freq. (Raspberry Serial Errors on High Frequency Clock)

  //Debug/Raspberry Serial
     Serial.begin(9600);



  //Init RFM69
  initrfm();

  //Init ESP-Now
  initespnow();

  //Timer to test out ESPNOW Message Send.
  timer1 = millis();
}


/*

   LOOP
*/


void loop() {
  //RFM69 Rx Loop
  rfmloop();

  if(USEMQTT){
  //Mqtt Mode
  }else{
  //Raspberry Serial Mode

  //Check Serial Command
  
  checkserialin();
  }

  
//Received Some Data
  if (rxe || rxr) {
    
    //Received Data from RFM69
    if(rxr)blink(LED, 2);
    
    //Received Data from ESPNOW
    if(rxe)blink(LED, 1);
    
    rxe = false;
    rxr = false;

 if(USEMQTT){
//Mqtt Mode

//Turn on WIFI & send mqtt command and then disconnect.
  wifionsendmqtt();

//Re-Init ESP-NOW  
  initespnow();
   
  }else{
//Raspberry Serial Mode
    
//Json Output    
// Write like this : {"id":value,"string1":"strvalue"}
Serial.print("{\"nodeid\":");
Serial.print(thedata.id);
Serial.print(",\"data1\":");
Serial.print(thedata.data1);
Serial.print(",\"data2\":");
Serial.print(thedata.data2);
Serial.println("}");   
  }
  
  }


//Do Periodic stuff every 5sec
  if ((((unsigned long)(millis() - timer1 )) ) >= 5000) {
  timer1 = millis();
   //Send Message to All ESPNOW Nodes (Paired Ones, Ref: include.h)
   sendespnow();
  }



  


}
