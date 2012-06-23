
#define METEOCONFIG_START 0

// current date coming from cloud detector

float SkyTemp = 0.0;
float Clarity = 0.0;
float   Rain = 0;
float  Darkness = 0;
float Humidity = 0.0;
float AmbTemp = 0.0;


struct meteoConfig {
  // Threshod value
  float SkyTempThreShold;
  float DarknessTheshold;
  float ClarityThreshold;
  int   RainThreshold;
  float HumidityThresold;
  //check
  boolean SkyTempCheck ;
  boolean ClarityCheck ;
  boolean RainCheck ;
  boolean HumidityCheck ;
  boolean DarknessCheck ;
} meteoValues = {
  5.0,
  18.0,
  20.0,
  2100,
  90,
  true,false,false,false,false
};

long timerMeteo;

  char Anwser[50];
  int i = 0;

void GetMeteoSensor(){
  
  timerMeteo = millis();
  

  if (client_Meteo.connect(MeteoArduino, 80)) {
    // Make a HTTP request:
    client_Meteo.println("GET /jsonSensor HTTP/1.0");
    client_Meteo.println();
  }
  else {
    // kf you didn't get a connection to the server:
#if DEBUG >2
    Serial.println("connection failed to MeteoArduino");
#endif
  }
  delay(50);
  
  memset( &Anwser, 0, 49 ); //clear inString memory
  while (client_Meteo.available()){
    char c = client_Meteo.read();
    if (c != '"' && c != ',')
      Anwser[i++]=c;
    if (c=='\r' || c== '\n'){
      c = client_Meteo.read(); //skip \n
      String line(Anwser);
      i=0;
      memset( &Anwser, 0, 49 ); //clear inString memory
      decodeLine(line,"SkyTemp",&SkyTemp);
      decodeLine(line,"DHTTemp",&AmbTemp);
      decodeLine(line,"Darkness",&Darkness);
      decodeLine(line,"Rain",&Rain);
      decodeLine(line,"DHTHum",&Humidity);
    }
  }
  
  Darkness = 19 - (2.5*log10(1000000.0/Darkness));
//#if DEBUG > 2 

  Serial.print("Time elapse :");
  Serial.println(millis()-timerMeteo);

  Serial << "SkyTemp : " << SkyTemp << " DHTTemp :" << AmbTemp << " Darkness :" << Darkness << "\n";
  Serial << "Rain : " << Rain << " DHTHum : " <<Humidity  << "\n";
//#endif

  // if the server's disconnected, stop the client:
  if (!client_Meteo.connected()) {
#if DEBUG >2
    Serial.println("disconnecting.");
#endif
    client_Meteo.stop();
}

}


void decodeLine(String line,String ref,float *value) {
  
      if (line.startsWith(ref)) {
          *value = Stingtofloat(line.substring(line.indexOf(':')+1));
      }   
     
} 


void loadMeteoConfig(void) {
    for (unsigned int t=0; t<sizeof(meteoConfig); t++)
     *((char*)&meteoValues + t) = EEPROM.read(METEOCONFIG_START + t);

}

void saveMeteoConfig(void) {
   for (unsigned int t=0; t<sizeof(meteoConfig); t++)
      EEPROM.write(METEOCONFIG_START + t, *((char*)&meteoValues + t));
}
  
