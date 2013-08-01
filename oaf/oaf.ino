#include <EEPROM.h>




#include "SPI.h"
#include "Ethernet.h"


#include "WebServer.h"
#include <avr/wdt.h>

#include <utility/w5100.h>


#define VERSION 0.9.0

void HtmlCmd(WebServer &server, WebServer::ConnectionType type, char *, bool);
void JsonGetCapteur(WebServer &server, WebServer::ConnectionType type, char *, bool);
void setlumiere(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool);
void connectTelecope(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool);
void HtmlAdminCmd(WebServer &server, WebServer::ConnectionType type, char *url, bool);
void HtmlFermetureAuto(WebServer &server, WebServer::ConnectionType type, char *url, bool);


void ManageToit();

void transmitHomeEasy(int blnOn,int recipient);
void itob(unsigned long integer, int length);


//#define DEBUG 2

 long  timer ;
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

#define AdminCredential "YWRtaW46c2Focg==" 


//----------------------------------
// workarround for the arduino mega watchdog bug
// http://www.desert-home.com/2011/10/2560-board-is-not-so-stupid-anymore.html

unsigned long resetTime = 0;
#define TIMEOUTPERIOD 10000     // 8 second watchdog             

#define doggieTickle() resetTime = millis();  // This macro will reset the timer
void(* resetFunc) (void) = 0; //declare reset function @ address 0

void watchdogSetup()
{
  cli();  // disable all interrupts
  wdt_reset(); // reset the WDT timer
  MCUSR &= ~(1<<WDRF);  // because the data sheet said to
  /*
  WDTCSR configuration:
  WDIE = 1 :Interrupt Enable
  WDE = 1  :Reset Enable - I won't be using this on the 2560
  WDP3 = 0 :For 2000ms Time-out
  WDP2 = 1 :bit pattern is
  WDP1 = 1 :0111  change this for a different
  WDP0 = 1 :timeout period.
  */
  // Enter Watchdog Configuration mode:
  WDTCSR = (1<<WDCE) | (1<<WDE);
  // Set Watchdog settings: interrupte enable, 0110 for timer
  WDTCSR = (1<<WDIE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  sei();
  Serial << "finished watchdog setup \n";
}


ISR(WDT_vect) // Watchdog timer interrupt.
{
  if(millis() - resetTime > TIMEOUTPERIOD){
    doggieTickle(); 
    Serial << "Watchdog reset occur \n" ;   // take these lines out
 delay(100);
    resetFunc();     // This will call location zero and cause a reboot.
  }
}

//-----------------------------------


//action

#define NoAction                 0
#define ActionOuvertureTotal     1
#define ActionOuverturePartiel   2
#define ActionFermeture          3
#define ActionStop               4
#define ActionFemetureSansPark   5
#define ActionOuvertureAeration  6


#define StateToitFerme            0
#define StateTympanOuverture1     1
#define StateTympanOuverture2     2
#define StateTympanFermeture1     4
#define StateToitFermeture        5
#define StateToitOuverture1       6
#define StateToitOuvert           7
#define StateArretUrgence         8
#define StateTympanFermeture2     9
#define StateParkTelescope        10
#define StateToitOuverture2       11
#define StateToitAeration         12
#define StateTympanOuverture3     13



byte CurrentState = StateArretUrgence;

boolean TelescopeTpl2Connected = true;
boolean FermetureAuto = false;
byte NbCheckBeforeClose = 0;
byte UserAction = NoAction;


boolean CapteurToitOuvert = false;
boolean CapteurToitFermer = false;
boolean CapteurTympanOuvert = false;
boolean CapteurTympanFermer = false; 
boolean CapteurTympanIntermediaire=false;
boolean CapteurTelescope = false; 
boolean CapteurCoverOuvert = false;
boolean CapteurCoverFermer = false;

//sorties

byte relaisOuvertureToit=22; 
byte relaisFermetureToit=23;
byte relaisOuvertureTympan=24; 
byte relaisFermetureTympan=25; 
byte sortieEmetteurHomeEasy = 9;

byte sortieLumiere = 29; //

byte relaisOuvertureCover = 26;
byte relaisFermetureCover = 27;


//entrees
byte EntreeCapteurToitOuvert = 39; 
byte EntreeCapteurToitFermer = 38; 
byte EntreeCapteurTympanOuvert =42; 
byte EntreeCapteurTympanFermer =40; 
byte EntreeCapteurTympanIntermediaire =41; 
byte EntreeCapteurCoverOuvert = 44;
byte EntreeCapteurCoverFermer = 43;

byte EntreeBoutonOuverture = 51;
byte EntreeBoutonFermeture =53 ;

// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[6] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };
IPAddress localIp (192, 168, 200, 177 );
IPAddress dnsIP ( 192,168,200, 1 );                      //live box

IPAddress OAFServer (192,168,200,82);
IPAddress MeteoArduino (192,168,200,178);
IPAddress ntm(192,168,200,195); 


EthernetClient  client_Meteo;

bool HomeEasyBitKey[26]={};              // 26 bit global to store the home Easy key.
#define HomeEasyKey              5154106


#define PREFIX ""
WebServer webserver(PREFIX, 80);
static long starTimerTelescopeStatus = 0;
static long starTimerMeteo = 0;

void setup()
{

  Serial.begin(9600);
  Serial.println ("Started");
  

  pinMode(relaisOuvertureToit,OUTPUT);
  pinMode(relaisFermetureToit,OUTPUT);
  pinMode(relaisOuvertureTympan,OUTPUT);
  pinMode(relaisFermetureTympan,OUTPUT);
  pinMode(sortieEmetteurHomeEasy,OUTPUT);
   
  digitalWrite(relaisOuvertureToit,HIGH);
  digitalWrite(relaisFermetureToit,HIGH);
  digitalWrite(relaisOuvertureTympan,HIGH);
  digitalWrite(relaisFermetureTympan,HIGH);

  pinMode(relaisOuvertureCover,OUTPUT);
  pinMode(relaisFermetureCover,OUTPUT);
  
  digitalWrite(relaisOuvertureCover,HIGH);
  digitalWrite(relaisFermetureCover,HIGH);

  pinMode(sortieLumiere,OUTPUT);
  digitalWrite(sortieLumiere,HIGH);

  pinMode(EntreeCapteurToitOuvert,INPUT_PULLUP);
  pinMode(EntreeCapteurToitFermer,INPUT_PULLUP);
  pinMode(EntreeCapteurTympanOuvert,INPUT_PULLUP);
  pinMode(EntreeCapteurTympanFermer,INPUT_PULLUP);
  pinMode(EntreeCapteurTympanIntermediaire,INPUT_PULLUP);
  
  pinMode(EntreeCapteurCoverOuvert,INPUT_PULLUP);
  pinMode(EntreeCapteurCoverFermer,INPUT_PULLUP);

  
  pinMode(EntreeBoutonOuverture,INPUT_PULLUP);
  pinMode(EntreeBoutonFermeture,INPUT_PULLUP);

  itob(HomeEasyKey,26);// convert our device code..
  
  // setup the Ehternet library to talk to the Wiznet board
  Ethernet.begin(mac, localIp,dnsIP);
  

  
  InitLog();
  
  webserver.setDefaultCommand(&HtmlCmd);
  webserver.addCommand("json/capteur",&JsonGetCapteur);
  webserver.addCommand("lumiere",&setlumiere);
  webserver.addCommand("connectTelecsope",&connectTelecope);
  webserver.addCommand("admin",&HtmlAdminCmd);
  webserver.addCommand("FermetureAuto",&HtmlFermetureAuto);

  webserver.begin();
 
  TelescopeTpl2Connected= tpl2Connect(ntm);
   starTimerTelescopeStatus = millis();  
   starTimerMeteo = millis();  


  loadMeteoConfig();
  watchdogSetup();
  LogOnInternet(1,"reboot_arduino","system");

  W5100.setRetransmissionTime(0x07D0);
  W5100.setRetransmissionCount(3);
}
boolean notInit=true;

void loop()
{
  
  // process incoming connections one at a time forever
   wdt_reset();   //pat the dog :)
  webserver.processConnection();
  
  if (notInit && (millis()>30000) && !TelescopeTpl2Connected) { //Init after 30s
    TelescopeTpl2Connected= tpl2Connect(ntm);
    InitLog();
    LogOnInternet(1,"reboot_arduino","system");
    notInit = false;
  }


  CapteurToitOuvert = false ;
  if (!digitalRead(EntreeCapteurToitOuvert)){
    delay(100);
    if (!digitalRead(EntreeCapteurToitOuvert))
       CapteurToitOuvert = true ;
  }
  
  CapteurToitFermer = false;
  if (!digitalRead(EntreeCapteurToitFermer)){
    delay(100);
    if (!digitalRead(EntreeCapteurToitFermer))
       CapteurToitFermer = true;
  }
  CapteurTympanOuvert = false;
  if (!digitalRead(EntreeCapteurTympanOuvert)){
    delay(100);
    if (!digitalRead(EntreeCapteurTympanOuvert))
       CapteurTympanOuvert = true;
  }
  CapteurTympanFermer = false;
  if (!digitalRead(EntreeCapteurTympanFermer)){
    delay(100);
    if (!digitalRead(EntreeCapteurTympanFermer))
       CapteurTympanFermer = true;  
  }
  CapteurTympanIntermediaire = false;
  if (!digitalRead(EntreeCapteurTympanIntermediaire)){
    delay(100);
    if (!digitalRead(EntreeCapteurTympanIntermediaire))
       CapteurTympanIntermediaire = true;
  }  
  
  CapteurCoverOuvert = false;
  if (!digitalRead(EntreeCapteurCoverOuvert)){
    delay(100);
    if (!digitalRead(EntreeCapteurCoverOuvert))
       CapteurCoverOuvert = true;
  }  
  
   CapteurCoverFermer = false;
  if (!digitalRead(EntreeCapteurCoverFermer)){
    delay(100);
    if (!digitalRead(EntreeCapteurCoverFermer))
       CapteurCoverFermer = true;
  }  
  
  if (!digitalRead(EntreeBoutonOuverture)){
    delay(100);
    if (!digitalRead(EntreeBoutonOuverture))
    {
        Serial <<  "EntreeBoutonOuverture\n";
        if (CurrentState == StateToitFerme ||CurrentState==StateArretUrgence ){
            UserAction=ActionOuverturePartiel;
            LogOnInternet(1,"ouverture_local","system");
        }
    }
} 

 if (!digitalRead(EntreeBoutonFermeture)){
    delay(100);
    if (!digitalRead(EntreeBoutonFermeture)){
      Serial <<  "EntreeBoutonFermeturee\n";
        if (CurrentState == StateToitOuvert ||CurrentState==StateArretUrgence ){
            UserAction=ActionFermeture;
            LogOnInternet(1,"fermeture_local","system");
        }
    }
} 

#if DEBUG > 2
      Serial.print("Etat capteur :");
      Serial.print(CapteurTympanFermer);
      Serial.print(CapteurTympanIntermediaire);
      Serial.print(CapteurTympanOuvert);
      Serial.print(CapteurToitFermer);
      Serial.print(CapteurToitOuvert);
      Serial.print(" CurrentState:");
      Serial.print(CurrentState);
      Serial.print(" UserAction:");
      Serial.println(UserAction);
#endif
  ManageToit();

  
  //Secutite
  
  if (CapteurTympanFermer)
    digitalWrite(relaisFermetureTympan,HIGH);
  if (CapteurTympanOuvert)
    digitalWrite(relaisOuvertureTympan,HIGH);
  if (CapteurToitFermer)
    digitalWrite(relaisFermetureToit,HIGH);
  if (CapteurToitOuvert)
    digitalWrite(relaisOuvertureToit,HIGH);
    
  if (CapteurCoverOuvert ) {
    digitalWrite(relaisOuvertureCover,HIGH);
  }

  if ( CapteurCoverFermer) {
    digitalWrite(relaisFermetureCover,HIGH);
  }

  if (TelescopeTpl2Connected) {
    if (millis() -starTimerTelescopeStatus > 2000 ){ // check telescope status every 2s
         CapteurTelescope=tpl2IsTelescopeParked();
        starTimerTelescopeStatus = millis();
    }
  }
   else {
     CapteurTelescope=false;
   }
  
  if ( CurrentState==StateToitOuvert && FermetureAuto &&  (millis()-starTimerMeteo >120000)){ // check metreo toutes les 2mn si le toit est ouvert
    if (GetMeteoSensor()){
     if (IsClosedNeeded()){
      NbCheckBeforeClose ++;
      if (NbCheckBeforeClose ==  2){
        Serial << "Femeture du toit sur condition meteo \n";
        UserAction=ActionFermeture;
        LogOnInternet(1,"Femeture_du_toit_sur_condition_meteo","system");
        NbCheckBeforeClose = 0;
      }
    }
    else
      NbCheckBeforeClose=0;
    }
      
    starTimerMeteo=millis();
  }

}


