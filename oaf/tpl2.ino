

#include <SPI.h>
#include <Ethernet.h>



//#define DEBUG 3



EthernetClient clientNtm;



char tpl2Anwser[60];

// telecope park position

float ParkHA = 0.0;
float ParkDec = -41.0;

long ParkTimer;
boolean isParking = false;

boolean tpl2IsTelescopeParked() {
  
  int i = 0;
  float currentHa=0.0,currentDec=0.0;
#if DEBUG > 2
  Serial.println("Get  telecope position");
#endif 
  clientNtm.println("10 GET DEC.CURRPOS;HA.CURRPOS");
  delay(200);

  memset(&tpl2Anwser,0,59);
  currentHa=0.0;
  currentDec=0.0;
  while(clientNtm.available()) {
     char c = clientNtm.read();
     tpl2Anwser[i++] = c;
     if (c=='\r') {
       clientNtm.read(); //skip \r
       String anwser(tpl2Anwser),anwser2;
       anwser2=anwser.substring(15);
       if (anwser2.startsWith("DEC.CURRPOS")) 
         currentDec = Stingtofloat(anwser2.substring(anwser2.indexOf('=')+1));
       if (anwser2.startsWith("HA.CURRPOS")) 
         currentHa = Stingtofloat(anwser2.substring(anwser2.indexOf('=')+1));
       i=0;
       memset(&tpl2Anwser,0,59);
     }
      
  }
#if DEBUG > 2 
    Serial.print("DEC =");
    Serial.print(currentDec);
    Serial.print(" HA =");
    Serial.println(currentHa);
    Serial.print("time elaspe : ");
#endif 

 if ( (currentHa > (ParkHA -1)) && (currentHa < (ParkHA +1)) &&(currentDec > (ParkDec -1)) && (currentDec < (ParkDec +1)))
  {
    isParking= false;
    return true;
  }
 else
   return false;

}


boolean tpl2PowerStatus() {
  
  int i = 0;
  boolean power = false;
#if DEBUG > 2
  Serial.println("Get  cabinet power status");
#endif 
  clientNtm.println("20 GET CABINET.POWER");
  delay(200);

  memset(&tpl2Anwser,0,59);
  while(clientNtm.available()) {
     char c = clientNtm.read();
     tpl2Anwser[i++] = c;
     if (c=='\r') {
       clientNtm.read(); //skip \r
       String anwser(tpl2Anwser),anwser2;
       anwser2=anwser.substring(15);
       if (anwser2.startsWith("CABINET.POWER=1")) 
         power = true;
        i=0;
        memset(&tpl2Anwser,0,59);
     }
  }
#if DEBUG > 2 
    Serial.print("CABINET.POWER =");
    Serial.print(power);
#endif 

  return power;

}

void tpl2Power(boolean state) {
  
  int i = 0;

#if DEBUG > 2
  Serial.print("Set  cabinet power  :");
  Serial.println(state);
#endif 
  clientNtm.print("30 SET CABINET.POWER=");
  if (state)
    clientNtm.println(1);
  else
    clientNtm.println(0);
  delay(200);
  while(clientNtm.available()) {
       char c = clientNtm.read();
#if DEBUG >2
        Serial.print(c);
#endif
   }

}

void tpl2Park(void) {
  
  if (isParking && (millis()-ParkTimer < 60000)) {
    return; 
  }  
  else {
#if DEBUG > 2
    Serial.println("park telescope");
#endif 
    isParking = true;
    ParkTimer = millis();

    clientNtm.println("40 SET HA.TARGETPOS=0.0");
    clientNtm.println("41 SET DEC.TARGETPOS=-41.0");

    delay(200);
    while(clientNtm.available()) {
       char c = clientNtm.read();
#if DEBUG >2
        Serial.print(c);
#endif
   }
  }
}

boolean tpl2Connect(IPAddress server) {
    Serial.print("connecting to");
     Serial.println(server);

    if (clientNtm.connect(server, 65432)) {
        delay (300);
        Serial.println("connected");
        while(clientNtm.available()) {
           char c = clientNtm.read();
            Serial.print(c);
        }
      // disable info / warning eventy
      clientNtm.println("01 SET SERVER.CONNECTION.EVENTMASK=0");
      delay(100);
      while(clientNtm.available()) {
           char c = clientNtm.read();
            Serial.print(c);
        }
      return true;
    }
    else {
      Serial.println("connection failed");
      return false;
    }
}


void tpl2Disconnect() {
    clientNtm.println("DISCONNECT");
    while(clientNtm.available()) {
       char c = clientNtm.read();
        Serial.print(c);
  }
  clientNtm.stop();
  clientNtm.flush();
}


float Stingtofloat(String  str)
{
      char buffer[str.length()+1];
      str.toCharArray(buffer,str.length()+1);
      return atof(buffer);

}
