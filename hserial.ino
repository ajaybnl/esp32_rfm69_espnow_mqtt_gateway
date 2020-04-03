void checkserialin() {

  //Message like
  //{"type":1,"id":2,"data1":768,"data2":0}

  Serial.setTimeout(100);
  if( Serial.available()){
    
  String sdata = Serial.readStringUntil('\n');// s1 is String type variable.
  
  if (sdata.length() > 0) {

    //We have Serial Data
    //Data Like: "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":48.756080}";

    StaticJsonBuffer<200> jsonBuffer;

    //Convert again to chars
    char json[sdata.length()];
    unsigned int len = sdata.length();
    sdata.toCharArray(json, len);

    //parse
    JsonObject& root = jsonBuffer.parseObject(json);

    //if error quit
    if (!root.success()) {
      //Serial.println("parseObject() failed");
      return;
    }

    //extract our payload
   

    int to = root["type"];
    int id = root["id"];
    int data1 = root["data1"];
    int data2 = root["data2"];

    //Serial.flush();

  
    //Prepare payload
    thedata.id = (byte)NODEID;
    thedata.data1 = data1;
    thedata.data2 = data2;

    

    if (to == 0) {
      //ESP-NOW
      sendespnow();
    } else if (to == 1) {
      //RFM69
      sendrfm(id);

    } else if (to == 2) {
      //RF 433mhz

    } else if (to == 3) {
      //RF 315Mhz

    } else if (to == 4) {
      //Bluetooth

    }


  }
  }
}
