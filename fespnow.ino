//ESPNOW
//Errors will be sent using "!ERR" Keyword to Node-Red

/*
   INIT ESP-NOW
*/

void initespnow() {
 
   bool err = false;
  char *errstr = "";

//This arrangement is VERY CRITICAL for Receiving & Sending ESPNOW Messages.
//############################################################################
WiFi.mode(WIFI_STA);
 delay(100);
 WiFi.disconnect();


  bool result = WiFi.softAP("FashionFinder3", "725872098650", CHANNEL, 0);
  if (!result) {
    //Critical Error Trace
    errstr = "!ERR AP Config failed ";
    err = true;
  } 
//############################################################################


//Init ESPNOW
  if (esp_now_init() != ESP_OK){
    //Critical Error Trace
    errstr = "!ERR ESPNow Init Failed";
    err = true;
  }

//On Error Restart ESP
if(err)
  Serial.println(errstr);
if(err)ESP.restart();



  // Once ESPNow is successfully Init, we will register for Send CB to

  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
// get the status of Received packet
  esp_now_register_recv_cb(OnDataRecv);

//##################################################################


//ADD PEERS
err = false;

//Empty the SLAVE Pointer
  memset(&slave, 0, sizeof(slave));


//We have added 4 Slaves Now (We can add Upto 20)
  for (int i = 0; i <= 3;i++){
//Prepare SLAVES


  for (int ii = 0; ii < 6; ++ii)
  {
    slave.peer_addr[ii] = mac[i][ii];   
  }
  slave.channel = CHANNEL; // pick a channel
  slave.encrypt = 0;       // no encryption

  
//Check if SLAVE is Paired or Add Peer  


    

      // check if the peer exists
      bool exists = esp_now_is_peer_exist(slave.peer_addr);
      if (exists)
      {
        // Slave already paired.
        //No Err        
      }
      else
      {
        // Slave not paired, attempt pair
        esp_err_t addStatus = esp_now_add_peer(&slave);
        if (addStatus == ESP_OK)
        {
          // Pair success
          //No Err
        }
        else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT)
        {
          // How did we get so far!!
          errstr = "!ERR addpeer ESPNOW not init";
          err = true;
        }
        else if (addStatus == ESP_ERR_ESPNOW_ARG)
        {
          errstr = "!ERR addpeer invalid argument";
          err = true;
        }
        else if (addStatus == ESP_ERR_ESPNOW_FULL)
        {
          errstr = "!ERR addpeer peer list full";
          err = true;
        }
        else if (addStatus == ESP_ERR_ESPNOW_NO_MEM)
        {
          errstr = "!ERR addpeer out of memory";
          err = true;
        }
        else if (addStatus == ESP_ERR_ESPNOW_EXIST)
        {
          errstr = "!ERR addpeer peer exists";
          err = true;
        }
        else
        {
          errstr = "!ERR addpeer not sure what happened";
          err = true;
        }

//Send Error
if(err) Serial.println(errstr);
errstr = "";

      }
    
  }
  
}



//Send ESP-Now Message to All Nodes (Our Payload Structure)
void sendespnow() {
 bool err = false;
  char *errstr = "";
   
  esp_err_t result = esp_now_send(NULL, (unsigned uint8_t *) &thedata, sizeof(Payload));
  
        
      if (result == ESP_OK)
      {
      
      }
      else if (result == ESP_ERR_ESPNOW_NOT_INIT)
      {
      
        errstr="!ERR sendmsg ESPNOW not Init";
        err = true;
      }
      else if (result == ESP_ERR_ESPNOW_ARG)
      {
        errstr="!ERR sendmsg Invalid Argument";
         err = true;
      }
      else if (result == ESP_ERR_ESPNOW_INTERNAL)
      {
        errstr="!ERR sendmsg Internal Error";
         err = true;
      }
      else if (result == ESP_ERR_ESPNOW_NO_MEM)
      {
        errstr="!ERR sendmsg ESP_ERR_ESPNOW_NO_MEM";
         err = true;
      }
      else if (result == ESP_ERR_ESPNOW_NOT_FOUND)
      {
        errstr="!ERR sendmsg Peer not found.";
         err = true;
      }
      else
      {
        errstr="!ERR sendmsg Not sure what happened";
         err = true;
      }

 

if(err)  Serial.println(errstr); 
}



/*
  Callback
*/


// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  if(DBG){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Sent : ");
  Serial.print(macStr);
  Serial.print(" Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "1" : "0");
  Serial.println();
  }
}


// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) 
{

 if (data_len == sizeof(Payload)) {
  thedata = *(Payload *)data;
//Data will be sent to Node-Red in Main Loop
 rxe=true;
 }else{
//Invalid Packet
Serial.println("!ERR espnow_rx ESPNOW payload size not match");
 }

  
  
}