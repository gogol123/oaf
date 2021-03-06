
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
  false,false,false,false,false
};

long timerMeteo;

  char Anwser[50];
  int i = 0;

boolean GetMeteoSensor(){
  
  timerMeteo = millis();
  
  W5100.setRetransmissionTime(0x0DAC);
  W5100.setRetransmissionCount(2);

  if (client_Meteo.connect(MeteoArduino, 80)) {
    // Make a HTTP request:
    client_Meteo.println("GET /jsonSensor HTTP/1.0");
    client_Meteo.println();
  }
  else {
    // kf you didn't get a connection to the server:
    LogOnInternet(2,"connection_to_meteo_failed","system");
#if DEBUG >1
    Serial.println("connectionfailed to MeteoArduino");
#endif
    W5100.setRetransmissionTime(0x07D0);
    W5100.setRetransmissionCount(3);
    return false;
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
  Clarity =  AmbTemp-SkyTemp;
#if DEBUG > 1 
  Serial.print("Time elapse :");
  Serial.println(millis()-timerMeteo);
  Serial << "SkyTemp : " << SkyTemp << " DHTTemp :" << AmbTemp << " Darkness :" << Darkness << "\n";
  Serial << "Rain : " << Rain << " DHTHum : " <<Humidity  << "\n";
#endif

  // if the server's disconnected, stop the client:
  if (!client_Meteo.connected()) {
#if DEBUG > 1
    Serial.println("disconnecting.");
#endif
    client_Meteo.stop();
}

    W5100.setRetransmissionTime(0x07D0);
    W5100.setRetransmissionCount(3);
    return true;

}

boolean IsClosedNeeded(void){
  if (FermetureAuto) {
    char alert[50];
      if (meteoValues.DarknessCheck && (Darkness < meteoValues.DarknessTheshold)){
          sprintf(alert,"Fermeture_auto_darkness_%d_<%d",(int)Darkness,(int)meteoValues.DarknessTheshold);
          LogOnInternet(1,alert,"system");
#if DEBUG > 1
          Serial << " Fermeture Auto - Darness :  " <<Darkness  << "< " << meteoValues.DarknessTheshold <<"\n";
#endif
          return true;
      }
      if (meteoValues.SkyTempCheck && (SkyTemp > meteoValues.SkyTempThreShold)){
          sprintf(alert,"Fermeture_auto_skytemp_%d_<%d",(int)SkyTemp,(int)meteoValues.SkyTempThreShold);
           LogOnInternet(1,alert,"system");
#if DEBUG > 1
          Serial << " Fermeture Auto - SkyTemp :  " <<SkyTemp  << "> " << meteoValues.SkyTempThreShold <<"\n";
#endif 
         return true;
      }
      if (meteoValues.ClarityCheck && (Clarity > meteoValues.ClarityThreshold)){
          sprintf(alert,"Fermeture_auto_Clarity_%d_<%d",(int)Clarity,(int)meteoValues.ClarityThreshold);
           LogOnInternet(1,alert,"system");
#if DEBUG > 1          
          Serial << " Fermeture Auto - Clarity : " << Clarity  << "> " << meteoValues.ClarityThreshold <<"\n";
#endif
         return true;
      }
      if (meteoValues.RainCheck && (Rain > meteoValues.RainThreshold)){
           sprintf(alert,"Fermeture_auto_Rain_%d_<%d",(int)Rain,(int)meteoValues.RainThreshold);
          LogOnInternet(1,alert,"system");
#if DEBUG > 1
          Serial << " Fermeture Auto - Rain :  "  <<Rain  << "> " << meteoValues.RainThreshold <<"\n";
#endif
         return true;
      }
      if (meteoValues.HumidityCheck && (Humidity > meteoValues.HumidityThresold)){
          sprintf(alert,"Fermeture_auto_Humidity_%d_<%d",(int)Humidity,(int)meteoValues.HumidityThresold);
           LogOnInternet(1,alert,"system");
#if DEBUG > 1
         Serial << " Fermeture Auto - Humidity :  "  <<Humidity  << "> " << meteoValues.HumidityThresold <<"\n";
#endif
         return true;
      }
      return false;
  }
}

         
          


void decodeLine(String line,String ref,float *value) {
  
      if (line.startsWith(ref)) {
          *value = Stingtofloat(line.substring(line.indexOf(':')+1));
      }   
     
} 


void loadMeteoConfig(void) {
#if DEBUG > 2
   Serial << "load config \n";
#endif 
    for (unsigned int t=0; t<sizeof(meteoConfig); t++)
     *((char*)&meteoValues + t) = EEPROM.read(METEOCONFIG_START + t);

}

void saveMeteoConfig(void) {
#if DEBUG > 2
     Serial << "save config \n";
#endif
   for (unsigned int t=0; t<sizeof(meteoConfig); t++)
      EEPROM.write(METEOCONFIG_START + t, *((char*)&meteoValues + t));
}
  
