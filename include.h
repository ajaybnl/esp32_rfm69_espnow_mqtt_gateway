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

//Use Raspberry (Serial Mode) 
//#define USEMQTT false

//Use WIFI & Mqtt Only (No Serial to Raspberry)
#define USEMQTT false

//Change USEMQTT from false to true will :
// It will connect to wifi and publish mqtt command for the incoming data. 
// This change will limit rfm69 and ESPNOW data to 2-10 Data Packets / Minute
// It will set serial as debug mode


//Uncomment to use StaticIP 
//#define STATICIP


#if defined(STATICIP)
// Set your Static IP address
IPAddress local_IP(192, 168, 100, 112);
// Set your Gateway IP address
IPAddress gateway(192, 168, 100, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

#endif


WiFiClient espClient;
PubSubClient client(espClient);


//WIFI
char* ssid = "_SSID_HERE_";
char* password = "_PASSWORD HERE_";




//MQTT
const char* MQTTSERVER = "192.168.100.100";

// You will get messages like feeds/nodes/NODEID
#define TOPIC_MAIN "feeds/nodes/"




//NOBROWNOUT
#define nobrownout true


//ESPNOW
esp_now_peer_info_t slave;
#define CHANNEL 1

//
// MAX 20 SLAVE ADDRESSES {0xXX,0xXX,0xXX,0xXX,0xXX,0xXX}
uint8_t mac[][6]= {{0xCA, 0x50, 0xE3, 0x5D, 0x7C, 0xCC},{0xBA, 0xDD, 0xC2, 0x23, 0xFF, 0x74},{0x5A,0xCF,0x7F,0xB9,0x7D,0x28},{0xCA, 0x50, 0xE3, 0x5D, 0x7C, 0xCC}};





//LED Pin
#define LED    13


//SERIAL DEBUG

//this will keep it off in Raspberry Serial Mode
#define DBG USEMQTT







/*
 * 
 * VARIABLES
 */


unsigned long timer1 = 0;

long sec1 = 0;
unsigned long runtime = 0;
bool rxe = false;
bool rxr = false;
