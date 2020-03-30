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

   RADIO LOOP


*/


void rfmloop() {


  if (radio.receiveDone())
  {
    int rssi = radio.RSSI;

    if ((radio.DATALEN) != sizeof(Payload)) {
      if(DBG)Serial.print("RFM69 Invalid payload received, not matching Payload struct!");
      if(DBG)Serial.print(" Expected: "); 
      if(DBG)Serial.print(sizeof(Payload)); 
      if(DBG)Serial.print("Received: ");  
      if(DBG)Serial.print(radio.DATALEN);
      if(DBG)Serial.println();
    }
    // else
    //{
    thedata = *(Payload*)radio.DATA;

    //#######################################################################
    //RF DATA IN


    rxr = true;
    //##############################################################

    if(DBG) Serial.print("RFM69 Message [From]: ");
    if(DBG)Serial.print(thedata.id);
    if(DBG)Serial.print(" [Data1]: ");
    if(DBG)Serial.print(thedata.data1);
    if(DBG) Serial.print(" [Data2]: ");
    if(DBG)Serial.print(thedata.data2);
    if(DBG)Serial.print(" [RSSI]: ");
    if(DBG)Serial.print(rssi);
    
    // }

    if (radio.ACKRequested())
    {
      radio.sendACK();
    if(DBG)Serial.print("  [ACK Sent]");
    }
     if(DBG)Serial.println();
    
    blink(LED, 2);
  }

}
