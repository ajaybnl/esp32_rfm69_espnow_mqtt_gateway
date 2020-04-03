#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <RFM69.h>         //get it here: https://github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     
#include <RFM69_OTA.h>     
#include <SPI.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <esp_now.h>



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
  uint16_t id;     
  uint16_t data1; 
  uint16_t data2;  
} Payload;
Payload thedata;

/**************************************************/


//NOBROWNOUT
#define nobrownout true


//ESPNOW
esp_now_peer_info_t slave;
#define CHANNEL 1

//
/*
SLAVE: 204:80:227:93:124:204:  Status: Pair success
SLAVE: 188:221:194:35:255:116:  Status: Pair success
SLAVE: 92:207:127:185:125:40:  Status: Pair success
SLAVE: 195:80:227:93:124:204:  Status: Pair success
*/
uint8_t mac[][6]= {{0xCC, 0x50, 0xE3, 0x5D, 0x7C, 0xCC},{0xBC, 0xDD, 0xC2, 0x23, 0xFF, 0x74},{0x5C,0xCF,0x7F,0xB9,0x7D,0x28},{0xC3, 0x50, 0xE3, 0x5D, 0x7C, 0xCC}};




//WIFI
char* ssid = "FashionFinder";
char* password = "siddharth561";
unsigned long wifitimer = 0;



//MQTT
const char* mqtt_server = "192.168.100.100";

#define RUNTIME "feeds/nodes/runtime"
#define TOPIC_MAIN "feeds/nodes/"
#define TOPIC_SUB_NODE "feeds/nodes/txnode"
#define TOPIC_SUB_DATA "feeds/nodes/txdata"
#define TOPIC_COUNT "feeds/nodes/count"


//PINS
#define LED    13

//Don't change
#define DBG false







/*
 * 
 * VARIABLES
 */


unsigned long timer1 = 0;

long sec1 = 0;
unsigned long runtime = 0;
bool rxe = false;
bool rxr = false;
