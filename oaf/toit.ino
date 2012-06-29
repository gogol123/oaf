long timerAeration = 0;

void ManageToit(){
  switch (CurrentState) {
      case StateToitFerme :
#if DEBUG > 2
        Serial << "Toit Ferme \n";
#endif
        // tout les moteurs off
        MoteurOuvertureTympanOff();
        MoteurFermetureTympanOff();
        MoteurOuvertureToitOff();
        MoteurFermetureToitOff();
        if ((UserAction == ActionOuverturePartiel || UserAction==ActionOuvertureTotal  ||UserAction==ActionOuvertureAeration ) && !CapteurTympanOuvert &&CapteurTelescope)
            CurrentState = StateTympanOuverture1;
        break;
      case StateTympanOuverture1 :
#if DEBUG > 2
        Serial <<  "Tympan Ouverture 1 \n";
#endif
        MoteurOuvertureTympanOn(10);
        if(CapteurTympanIntermediaire){
          CurrentState=StateToitOuverture1;
          timerAeration = millis();
        }
        if (UserAction==ActionFermeture &&!CapteurTympanFermer)
          CurrentState=StateTympanFermeture1;
        break;
      case StateTympanOuverture2:
 #if DEBUG > 2
        Serial << " Tympan Ouverture 2 \n";
 #endif
        MoteurOuvertureToitOff();
        MoteurOuvertureTympanOn(46); //46s
        if (CapteurTympanOuvert)
            CurrentState=StateToitOuvert;
         if (UserAction==ActionFermeture)
            CurrentState=StateTympanFermeture2;
        break;
      case StateTympanFermeture1:
#if DEBUG > 2
        Serial << " Tympan Fermeture 1 \n";
#endif
        MoteurOuvertureTympanOff();
        MoteurFermetureToitOff();
        MoteurFermetureTympanOn(10);
        if (CapteurTympanFermer)
          CurrentState=StateToitFerme;
        break;
      case StateToitOuverture1:
#if DEBUG > 2
        Serial << "Toit  Ouverture 1 \n";
#endif
        MoteurOuvertureToitOn(46);
        MoteurOuvertureTympanOff();
        if(  UserAction ==ActionOuverturePartiel || UserAction== ActionOuvertureTotal )
          CurrentState=StateToitOuverture2;
        if (UserAction ==ActionFermeture && !CapteurToitFermer)
          CurrentState = StateToitFermeture;
         if (UserAction == ActionOuvertureAeration && (millis()-timerAeration > 5000))
           CurrentState=StateToitAeration;
       break;
    case StateToitAeration:
#if DEBUG > 2
        Serial << "Toit  Aeration  \n";
#endif
        MoteurOuvertureToitOff();
        MoteurOuvertureTympanOff();
        if (UserAction ==ActionFermeture && !CapteurToitFermer)
          CurrentState = StateToitFermeture;
        if(  UserAction ==ActionOuverturePartiel || UserAction== ActionOuvertureTotal )
          CurrentState=StateToitOuverture2;

       break;
     case StateToitOuverture2:
#if DEBUG > 2
        Serial << "Toit  Ouverture 2 \n";
#endif
        MoteurOuvertureToitOn(36);
        MoteurOuvertureTympanOff();
        if(CapteurToitOuvert && UserAction ==ActionOuverturePartiel)
          CurrentState=StateToitOuvert;
        if (CapteurToitOuvert && UserAction ==ActionOuvertureTotal)
          CurrentState= StateTympanOuverture2;
        if (UserAction ==ActionFermeture && !CapteurToitFermer)
          CurrentState = StateToitFermeture;
       break;
      case StateToitFermeture:
#if DEBUG > 2
        Serial << "Toit Fermeture \n";
#endif
        MoteurFermetureTympanOff();
        MoteurOuvertureTympanOff();

        MoteurOuvertureToitOff();
        MoteurFermetureToitOn(40);
        if (   CapteurToitFermer  ) 
          CurrentState=StateTympanFermeture1;
       break;
      case StateToitOuvert:
#if DEBUG > 2
        Serial << "Toit Ouvert \n";
#endif
      MoteurOuvertureToitOff();
      MoteurOuvertureTympanOff();

       if(UserAction== ActionFermeture && !CapteurToitFermer )
          CurrentState=StateParkTelescope;
        if(UserAction ==ActionFemetureSansPark)
          CurrentState=StateTympanFermeture2;
       break ;
      case StateArretUrgence:
            MoteurFermetureToitOff();
            MoteurFermetureTympanOff();
            MoteurOuvertureToitOff();
            MoteurOuvertureTympanOff();
 #if DEBUG > 2
        Serial << "Arret d'urgence \n";
 #endif
        if (UserAction == ActionFermeture )
          CurrentState=StateParkTelescope;       
        if ((UserAction == ActionOuverturePartiel || UserAction==ActionOuvertureTotal ) ) 
          CurrentState=StateToitFerme;
        if(UserAction ==ActionFemetureSansPark)
          CurrentState=StateTympanFermeture2;
       break;
      case StateTympanFermeture2:
 #if DEBUG > 2
        Serial << "Tympan Fermeture 2 \n";
 #endif
        MoteurFermetureTympanOn(46);
        if (CapteurTympanIntermediaire   )
          CurrentState=StateToitFermeture ;
         if (CapteurTympanFermer)
             CurrentState= StateTympanOuverture3;
       break;   
       case StateParkTelescope:
 #if DEBUG > 2
        Serial << "park telescop \n";
 #endif
         KillAstelOs();
        tpl2Park();
        if (CapteurTelescope){
            tpl2Power(false);
            CurrentState= StateTympanFermeture2;
        }
       break;   
      case StateTympanOuverture3:
  #if DEBUG > 2
        Serial << "RE ouverture tympan intermediaire \n";
 #endif
        MoteurOuvertureTympanOn(10);
        if(CapteurTympanIntermediaire)  
          CurrentState=StateToitFermeture;
         break;

      default :
 #if DEBUG > 2
        Serial << " Error !! \n";
#endif   
        MoteurFermetureToitOff();
        MoteurFermetureTympanOff();
        MoteurOuvertureToitOff();
        MoteurOuvertureTympanOff();
        break;
  
  }
}

long TimerToitOuverture = 0;
long TimerToitFermeture=0;
long TimerTympanOuverture =0;
long TimerTympanFermeture =0;

void MoteurOuvertureToitOn(long  timer){
  if (digitalRead(relaisOuvertureToit)==LOW) {
      digitalWrite(relaisOuvertureToit,HIGH);
      TimerToitOuverture = millis();
  }
  if (millis()-TimerToitOuverture > timer*1000){ 
    CurrentState=StateArretUrgence;
    UserAction = NoAction;
  }
#if DEBUG > 2
  Serial << timer-(millis()-TimerToitOuverture)/1000 << "s avant coupure d'urgence moteur Ouverture Toit\n";
#endif
}


void MoteurOuvertureToitOff(){
  digitalWrite(relaisOuvertureToit,LOW);
}

void MoteurFermetureToitOn(long  timer){
  if (digitalRead(relaisFermetureToit)==LOW){
      digitalWrite(relaisFermetureToit,HIGH);
      TimerToitFermeture=millis();
  }
  if (millis()-TimerToitFermeture > timer*1000){
    CurrentState=StateArretUrgence;
     UserAction = NoAction;
  }
#if DEBUG > 2
  Serial << timer-(millis()-TimerToitFermeture)/1000 << "s avant coupure d'urgence moteur Fermeture Toit\n";
#endif
}


void MoteurFermetureToitOff(){
  digitalWrite(relaisFermetureToit,LOW);
}

void MoteurOuvertureTympanOn(long  timer){

  if (digitalRead(relaisOuvertureTympan)==LOW) {
    digitalWrite(relaisOuvertureTympan,HIGH);
    TimerTympanOuverture=millis();
  }
  if ((millis()-TimerTympanOuverture) > (timer*1000)) {
     CurrentState=StateArretUrgence;
     UserAction = NoAction;
   }
#if DEBUG > 2
  Serial << timer-(millis()-TimerTympanOuverture)/1000 << "s avant coupure d'urgence moteur Ouverture Tympan\n";
#endif
}


void MoteurOuvertureTympanOff(){
  digitalWrite(relaisOuvertureTympan,LOW);
}

void MoteurFermetureTympanOn(long  timer){
  if (digitalRead(relaisFermetureTympan)==LOW) {
    digitalWrite(relaisFermetureTympan,HIGH);
    TimerTympanFermeture=millis();
  }
  if (millis()-TimerTympanFermeture > timer*1000){
    CurrentState=StateArretUrgence;
    UserAction = NoAction;
  }
#if DEBUG > 2
  Serial << timer-(millis()-TimerTympanFermeture)/1000 << "s avant coupure d'urgence moteur fermeture Tympan\n";
#endif
}


void MoteurFermetureTympanOff(){

  digitalWrite(relaisFermetureTympan,LOW);
}




