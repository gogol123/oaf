#include "SPI.h"
#include "Ethernet.h"

#include <utility/w5100.h>

static uint8_t mac[6] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };
IPAddress localIp (192, 168, 200, 177 );
IPAddress dnsIP ( 192,168,200, 1 );                      //live box

IPAddress ntm(192,168,200,195); 
IPAddress _free(212,27,63,110); 

EthernetClient  clientNtm;
EthernetClient  clientFree;

void setup()
{

  Serial.begin(9600);
  Serial.println ("Started");
  Ethernet.begin(mac, localIp,dnsIP);
  Serial.println ("IP started");
 // W5100.setRetransmissionTime(0x07D0);
 // W5100.setRetransmissionCount(3);
}
 
long timer =0 ;

long connectionduration  = 0;
void loop() {
  
 if ( (millis() - timer) > 10000) {
   timer = millis();
   connectionduration = millis();
   Serial.println("connecting to ntm");
   if (clientNtm.connect(ntm, 65432)) {
        Serial.print("Connected in ");
        Serial.print(millis()-connectionduration);
        Serial.println(" ms");

   }
   else {
     Serial.print("connection failed in ");
      Serial.print(millis()-connectionduration);
      Serial.println(" ms");
   }
   clientNtm.stop();

   Serial.println("connecting to free");
   if (clientFree.connect(_free, 80)) {
        Serial.print("Connected in ");
        Serial.print(millis()-connectionduration);
        Serial.println(" ms");

   }
   else {
     Serial.print("connection failed in ");
      Serial.print(millis()-connectionduration);
      Serial.println(" ms");
   }
   clientFree.stop();
 }
}
