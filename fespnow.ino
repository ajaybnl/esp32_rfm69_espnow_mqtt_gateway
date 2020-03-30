//ESPNOW
/*
   INIT ESP-NOW
*/

void initespnow() {

  WiFi.disconnect(true);
  //AP Params (Not Used)
  const char *SSID = "esp32_now";
  const char *PASS = "59523582h5258";

  bool result = WiFi.softAP(SSID, PASS, CHANNEL, 0);
  if (!result) {
    if (DBG)Serial.println("AP Config failed.");
  } else {
    if (DBG)Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }

  if (esp_now_init() == ESP_OK) {
    if (DBG)Serial.println("ESPNow Init Success");
    if (DBG)Serial.print("GATEWAY MAC: ");
    if (DBG)Serial.println(WiFi.softAPmacAddress());
  } else {
    blink(LED, 50);
    if (DBG)Serial.println("ESPNow Init Failed");
    ESP.restart();
  }

  esp_now_register_recv_cb(OnDataRecv);
}



//Send ESP-Now Message to Mac Address (Payload)
void sendespnow(uint8_t *mac_addr,Payload * thedata) {
  uint16_t packet_size = sizeof(Payload);
  uint8_t msg_data[packet_size];
  memcpy(&msg_data[0], thedata, packet_size);

  esp_err_t status = esp_now_send(mac_addr, msg_data, packet_size);
  if (ESP_OK != status)
  {
    if (DBG)Serial.println("Error sending message");

  }
}



/*
  Callback
*/

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

  if (data_len != sizeof(Payload)) {
    if (DBG)Serial.print("ESPNOW Invalid payload received, not matching Payload struct!");
    if (DBG)Serial.print(" Expected: ");
    if (DBG)Serial.print(sizeof(Payload));
    if (DBG)Serial.print("Received: ");
    if (DBG)Serial.print(data_len);
    if (DBG)Serial.println();
  }
  //else
  // {
  thedata = *(Payload*)data;

  if (DBG) Serial.print("ESPNOW Message [From]: ");
  if (DBG)Serial.print(thedata.id);
  if (DBG)Serial.print(" [Data1]: ");
  if (DBG)Serial.print(thedata.data1);
  if (DBG) Serial.print(" [Data2]: ");
  if (DBG)Serial.println(thedata.data2);

  blink(LED, 1);
  //}
}
