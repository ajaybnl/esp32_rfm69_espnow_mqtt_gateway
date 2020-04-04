
void publishmqtt(char* topic, char* data1) {
unsigned long tt=millis();

//Connect Wifi 
  connectwifi();
  

//--------
if(DBG)Serial.print("Time Taken for WIFI (MS) : ");
if(DBG)Serial.println(millis()-tt);
tt=millis();
//--------

  //Connect Mqtt
  randomSeed(micros());
  client.setServer(MQTTSERVER, 1883);
  client.setCallback(callback);
  reconnect();

//--------
  if(DBG)Serial.print("Time Taken for MQTT (MS) : ");
if(DBG)Serial.println(millis()-tt);
tt=millis();
//--------

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //delay(100);


  //Publish
  client.publish(topic, data1);

  if (DBG) Serial.print("[Topic] ");
  if (DBG) Serial.print(topic);
  if (DBG) Serial.print("  [Data] ");
  if (DBG) Serial.println(data1);
//Disconnect Wifi (TO START ESP-NOW)
  WiFi.disconnect(true);
  
}

// Connect Wifi and Send Mqtt
void wifionsendmqtt() {
  char topic1[25];
  char data1[75];

  snprintf (topic1, 25, "%s%d", TOPIC_MAIN,thedata.id);
  snprintf (data1, 75, "{id:%1d,uptime:%1d,data:%1d}", thedata.id, thedata.data1, thedata.data2);
  publishmqtt(topic1, data1);
  delay(100);
}






//-----------------------------------
void reconnect() {

  unsigned long tout = millis();
  while (!client.connected()) {

    if (((millis() - tout) / 1000) > 60) {
      //Max Time (60s)Trying to connect Mqtt
      return;
    }

    if (DBG) Serial.print("Attempting MQTT connection...");

    String clientId = "15jkh10";
    clientId += String(random(0xffff), HEX);
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      if (DBG) Serial.println("connected");

      //client.subscribe("inTopic");
    } else {
      if (DBG) Serial.print("failed, rc=");
      if (DBG) Serial.print(client.state());
      if (DBG) Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
  if (DBG) Serial.print("Message arrived [");
  if (DBG) Serial.print(topic);
  if (DBG) Serial.print("] ");
  for (int i = 0; i < length; i++) {
    if (DBG) Serial.print((char)payload[i]);
  }
  if (DBG) Serial.println();
}









//WIFI
void connectwifi() {
  if (DBG) Serial.println("Connecting to Wifi...");
  if (!connect_wifi(10)) {
    if (DBG) Serial.println("Cannot Connect.");
  } else {
    if (DBG) Serial.println("Connected");
    if (DBG) Serial.print("IP address: ");
    if (DBG) Serial.println(WiFi.localIP());
    delay(1000);
  }
}
bool connect_wifi(int maxattempts) {
  bool conn1 = false;
  //TRY
  int trys = 0;
  while (!conn1) {
    if (trys > 0) {
      if (DBG) Serial.print("Try: ");
      if (DBG) Serial.print(trys);
    }
    conn1 = connect__wifi(ssid, password);
    trys++;

    if (trys > maxattempts)break;
  }

  return (conn1);
}
bool connect__wifi(char* ssid, char* password) {

  unsigned long timeout = millis();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_MODE_STA);

  //If STATIC IP 
  #if defined(STATICIP)
 
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  if (DBG) Serial.println("Wifi Static IP Failed to configure");
  }
 #endif
 
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    if (((millis() - timeout) / 1000) >= 2) {
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) return (true); else return (false);
}
