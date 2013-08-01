
void OpenCover(){
  digitalWrite(relaisFermetureCover,HIGH);
  digitalWrite(relaisOuvertureCover,LOW);
  Serial.println("relaisOuvertureCover LOW");

}

void CloseCover(){
   digitalWrite(relaisFermetureCover,LOW);
   digitalWrite(relaisOuvertureCover,HIGH);
   Serial.println("relaisFermetureCover LOW");

}

void StopCover(){
   digitalWrite(relaisFermetureCover,HIGH);
   digitalWrite(relaisOuvertureCover,HIGH);
   Serial.println("relaisFermetureCover LOW");
  Serial.println("relaisOuvertureCover LOW");

}
