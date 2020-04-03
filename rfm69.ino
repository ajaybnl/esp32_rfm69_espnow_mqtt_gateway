//RFM69
//Errors will be sent using "!ERR" Keyword to Node-Red

/*

   INIT RADIO


*/
void initrfm() {
  radio.initialize(FREQUENCY, NODEID, NETWORKID);
#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
  radio.encrypt(ENCRYPTKEY);

#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif

  if(DBG)Serial.println("RFM69HCW Receiver Started");
  //############################################################################


}


/*

   RADIO SEND


*/

void sendrfm(int id) {
  //Nodeid will be different from Data Node ID.
  radio.sendWithRetry(id, &thedata, sizeof(Payload));
  delay(100);
}

/*

   RADIO LOOP


*/


void rfmloop() {

    //RF DATA IN
  if (radio.receiveDone())
  {
    int rssi = radio.RSSI;

    if ((radio.DATALEN) != sizeof(Payload)) {
      Serial.println("!ERR rfm69_rx RFM69 payload size not match");
    }else{
    thedata = *(Payload*)radio.DATA;
    rxr = true;
    //Data will be sent to Node-Red in Main Loop     
    }

    if(DBG)Serial.print("Node [RSSI]: ");
    if(DBG)Serial.print(rssi);

    if (radio.ACKRequested())
    {
      radio.sendACK();
    if(DBG)Serial.print("  [ACK Sent]");
    }
     
     if(DBG)Serial.println();
    
    
  }

}
