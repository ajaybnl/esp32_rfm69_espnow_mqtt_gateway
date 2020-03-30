#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <RFM69.h>         //get it here: https://github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     
#include <RFM69_OTA.h>     
#include <SPI.h>           

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <esp_now.h>



WiFiClient espClient;
PubSubClient client(espClient);


//RFM
#define NODEID          200 //the ID of this node
#define NETWORKID     200 //the network ID of all nodes this node listens/talks to
#define FREQUENCY     RF69_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define ENCRYPTKEY    "x8j5b6c1jk0pp34a" //identical 16 characters/bytes on all nodes, not more not less!
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
#define ACK_TIME       30  // # of ms to wait for an ack packet
//*****************************************************************************************************************************
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -75  //target RSSI for RFM69_ATC (recommended > -80)
//*****************************************************************************************************************************
// Serial baud rate must match your Pi/host computer serial port baud rate!
#define SERIAL_EN     //comment out if you don't want any serial verbose output
#define SERIAL_BAUD  115200
//*****************************************************************************************************************************

#ifdef ENABLE_ATC
RFM69_ATC radio;
#else
RFM69 radio;
#endif


/**************************************************/
//PayLoad Structure

//Values: -32768 to 32767
typedef struct {
  uint16_t id; //store this nodeId
  uint16_t data1; //uptime in ms
  uint16_t data2;  //
} Payload;
Payload thedata;

/**************************************************/


//NOBROWNOUT
#define nobrownout true
#define lowtxpower true



//ESPNOW
#define CHANNEL 1


//WIFI
char* ssid = "FashionFinder";
char* password = "siddharth561";
unsigned long wifitimer = 0;



//MQTT
const char* mqtt_server = "192.168.100.100";

#define RUNTIME "feeds/rfm69/runtime"
#define TOPIC_MAIN "feeds/rfm69/"
#define TOPIC_SUB_NODE "feeds/rfm69/txnode"
#define TOPIC_SUB_DATA "feeds/rfm69/txdata"
#define TOPIC_COUNT "feeds/rfm69/count"


//PINS
#define LED    13

//DBG
#define DBG true







/*
 * 
 * VARIABLES
 */


unsigned long timer1 = 0;

long sec1 = 0;
unsigned long runtime = 0;
bool rxe = false;
bool rxr = false;
