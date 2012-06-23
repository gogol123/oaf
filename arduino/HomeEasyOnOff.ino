

void transmitHomeEasy(int blnOn,int recipient)
{
  int i;

  // Do the latch sequence.. 
  digitalWrite(sortieEmetteurHomeEasy, HIGH);
  delayMicroseconds(275);     // bit of radio shouting before we start. 
  digitalWrite(sortieEmetteurHomeEasy, LOW);
  delayMicroseconds(9900);     // low for 9900 for latch 1
  digitalWrite(sortieEmetteurHomeEasy, HIGH);   // high again 
  delayMicroseconds(275);      // wait a moment 275
  digitalWrite(sortieEmetteurHomeEasy, LOW);    // low again for 2675 - latch 2.
  delayMicroseconds(2675);
  // End on a high 
  digitalWrite(sortieEmetteurHomeEasy, HIGH);

  // Send HE Device Address..
  // e.g. 1000010101000110010  272946 in binary.
  for(i=0; i<26;i++)
  {
    sendPair(HomeEasyBitKey[i]);
  }

  // Send 26th bit - group 1/0
  sendPair(false);

  // Send 27th bit - on/off 1/0
  sendPair(blnOn);

  // last 4 bits - recipient   -- button 1 on the HE300 set to 
  // slider position I in this example:

 sendPair(false);
  sendPair(false);
  if (recipient == 2)
      sendPair(true);
  else 
      sendPair(false);
   if (recipient == 1)
          sendPair(true);
  else 
      sendPair(false);
  digitalWrite(sortieEmetteurHomeEasy, HIGH);   // high again (shut up)
  delayMicroseconds(275);      // wait a moment
  digitalWrite(sortieEmetteurHomeEasy, LOW);    // low again for 2675 - latch 2.

}


void sendBit(boolean b) {
  if (b) {
    digitalWrite(sortieEmetteurHomeEasy, HIGH);
    delayMicroseconds(310);   //275 orinally, but tweaked.
    digitalWrite(sortieEmetteurHomeEasy, LOW);
    delayMicroseconds(1340);  //1225 orinally, but tweaked.
  }
  else {
    digitalWrite(sortieEmetteurHomeEasy, HIGH);
    delayMicroseconds(310);   //275 orinally, but tweaked.
    digitalWrite(sortieEmetteurHomeEasy, LOW);
    delayMicroseconds(310);   //275 orinally, but tweaked.
  }
}

void sendPair(boolean b) {
  // Send the Manchester Encoded data 01 or 10, never 11 or 00
  if(b)
  {
    sendBit(true);
    sendBit(false);
  }
  else
  {
  sendBit(false);
  sendBit(true);
  }
}


void itob(unsigned long integer, int length)
{  //needs bit2[length]
  // Convert long device code into binary (stores in global bit2 array.)
 for (int i=0; i<length; i++){
   if ((integer / power2(length-1-i))==1){
     integer-=power2(length-1-i);
     HomeEasyBitKey[i]=1;
   }
   else HomeEasyBitKey[i]=0;
 }
}

unsigned long power2(int power){    //gives 2 to the (power)
 unsigned long integer=1;          
 for (int i=0; i<power; i++){      
   integer*=2;
 }
 return integer;
}
