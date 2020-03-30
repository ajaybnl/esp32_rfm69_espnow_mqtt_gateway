# esp32_rfm69_espnow_mqtt_gateway

A MQTT GATEWAY of RFM69 &amp; ESP-NOW based on ESP32


#Patch for RFM69 Library from Lowpowerlabs:

RFM69.cpp:
Replace SPI.begin(); with:

 #if defined(ESP32)
 //Choose One SPI PORT
  SPI.begin(18,19,23,5);   //SPI3  (SCK,MISO,MOSI,CS)
// SPI.begin(14,12,13,15);   //SPI2   (SCK,MISO,MOSI,CS) 
  #else
SPI.begin();
  #endif
  
  IN RFM69.h, Define : 
  #define RF69_IRQ_PIN    2
#define RF69_SPI_CS     5

