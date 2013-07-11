
void OpenCover(){
  digitalWrite(relaisFermetureCover,LOW);
  digitalWrite(relaisOuvertureCover,HIGH);
}

void CloseCover(){
   digitalWrite(relaisFermetureCover,HIGH);
  digitalWrite(relaisOuvertureCover,LOW);
}
