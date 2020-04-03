# A MQTT GATEWAY of RFM69 &amp; ESP-NOW based on ESP32

## Features:
Fully Working, Both RFM69HCW & ESPNOW Simultanious With TX and RX



## You Need:
* 1x RFM69HCW
* 1x ESP32
* 1x Raspberry Pi (Any Linux Based Board) with Node-Red


## Libraries:
- RFM69 By LowPowerLabs (https://github.com/LowPowerLab/RFM69)
- PubSubClient (https://github.com/knolleary/pubsubclient)
- ArduinoJson (https://github.com/bblanchon/ArduinoJson)



### Current Version : 0.5 Fully Tested & Working (SEE Node-red Code Below)



## ESP32 Pinout: 

```
![ESP32 SPI Pinout](https://i.ibb.co/6bB3b4R/esp32-spi-pins.jpg)
```




## If using Old Version RFM69 ibrary:
Patch on RFM69 Library from Lowpowerlabs (FOR ESP32, OLD VERSION LIBRARY):

### RFM69.cpp:
Replace SPI.begin(); with:
```
 #if defined(ESP32)
 //Choose One SPI PORT
  SPI.begin(18,19,23,5);   //SPI3  (SCK,MISO,MOSI,CS)
// SPI.begin(14,12,13,15);   //SPI2   (SCK,MISO,MOSI,CS) 
  #else
SPI.begin();
  #endif
  ```
### IN RFM69.h, Define : 
  ```
  #define RF69_IRQ_PIN    2
#define RF69_SPI_CS     5
```



## Node-Red Script:
```
[{"id":"e6ad0d04.1d775","type":"comment","z":"344ad79e.ebd1a","name":"Serial To Influxdb","info":"//{\"type\":1,\"id\":2,\"data1\":768,\"data2\":0}","x":220,"y":60,"wires":[]},{"id":"f6438d5e.74b92","type":"debug","z":"344ad79e.ebd1a","name":"","active":false,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","x":850,"y":200,"wires":[]},{"id":"bae68f94.30b09","type":"function","z":"344ad79e.ebd1a","name":"JSON2OBJ","func":"var t = msg.payload;\nt = t.replace(\"\\r\",\"\");\nt = t.replace(\"\\n\",\"\");\nvar obj = JSON.parse(t);\nvar nodeid = obj.nodeid;\nmsg.payload=obj;\nmsg.measurement=\"node\"+nodeid;\nreturn msg;","outputs":1,"noerr":0,"x":550,"y":200,"wires":[["f6438d5e.74b92"]]},{"id":"4ec5d3c3.4da0fc","type":"mqtt out","z":"344ad79e.ebd1a","name":"","topic":"","qos":"0","retain":"","broker":"b778e59e.e51278","x":830,"y":260,"wires":[]},{"id":"7e029392.865394","type":"function","z":"344ad79e.ebd1a","name":"JSON2MQTT","func":"var t = msg.payload;\nt = t.replace(\"\\r\",\"\");\nt = t.replace(\"\\n\",\"\");\nvar obj = JSON.parse(t);\nvar nodeid = obj.nodeid;\nmsg.payload=t;\nmsg.topic=\"feeds/nodes/\"+nodeid;\nreturn msg;","outputs":1,"noerr":0,"x":550,"y":260,"wires":[["4ec5d3c3.4da0fc"]]},{"id":"e2c47750.fda978","type":"serial in","z":"344ad79e.ebd1a","name":"","serial":"ce3207cb.28cb3","x":150,"y":260,"wires":[["7a45ccfe.8703ac"]]},{"id":"7a45ccfe.8703ac","type":"function","z":"344ad79e.ebd1a","name":"ERR Filter","func":"var str = msg.payload;\nvar n = str.startsWith(\"!ERR\");\nif(n){\n//Error\nreturn [null,msg];    \n}else{\n//OK\nreturn [msg,null];\n}","outputs":2,"noerr":0,"x":350,"y":260,"wires":[["bae68f94.30b09","7e029392.865394"],["9e38d50b.e260e","d3e18215.ba549","982b3f2e.5cbf5"]],"inputLabels":["MSG"],"outputLabels":["OK","ERR"],"icon":"node-red/comment.png"},{"id":"9e38d50b.e260e","type":"debug","z":"344ad79e.ebd1a","name":"","active":false,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","x":390,"y":380,"wires":[]},{"id":"d3e18215.ba549","type":"function","z":"344ad79e.ebd1a","name":"Err to File","func":"\nreturn msg;","outputs":1,"noerr":0,"x":550,"y":340,"wires":[["891382e7.87d678"]]},{"id":"891382e7.87d678","type":"file","z":"344ad79e.ebd1a","name":"File Append","filename":"~/gerr.txt","appendNewline":true,"createDir":true,"overwriteFile":"false","encoding":"none","x":730,"y":340,"wires":[[]]},{"id":"982b3f2e.5cbf5","type":"function","z":"344ad79e.ebd1a","name":"Err to MQTT","func":"msg.topic=\"feeds/nodes/errors\"\nreturn msg;","outputs":1,"noerr":0,"x":570,"y":380,"wires":[["4ec5d3c3.4da0fc"]]},{"id":"b778e59e.e51278","type":"mqtt-broker","z":"","name":"localhost","broker":"localhost","port":"1883","clientid":"","usetls":false,"compatmode":true,"keepalive":"60","cleansession":true,"birthTopic":"","birthQos":"0","birthPayload":"","closeTopic":"","closePayload":"","willTopic":"","willQos":"0","willPayload":""},{"id":"ce3207cb.28cb3","type":"serial-port","z":"","serialport":"/dev/ttyUSB0","serialbaud":"9600","databits":"8","parity":"none","stopbits":"1","waitfor":"","dtr":"none","rts":"none","cts":"none","dsr":"none","newline":"500","bin":"false","out":"interbyte","addchar":"","responsetimeout":"10000"}]
```

