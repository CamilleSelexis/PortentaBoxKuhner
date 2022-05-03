void init_pcf8575(int adr){
  PCF8575 pcf8575(adr); //Define the pcf8575 with its adr
  pcf8575.begin();
  pcf8575.pinMode(P0,OUTPUT);
  pcf8575.pinMode(P1,OUTPUT);
  pcf8575.pinMode(P2,OUTPUT);
  pcf8575.pinMode(P3,OUTPUT);
  pcf8575.pinMode(P4,OUTPUT);
  pcf8575.pinMode(P5,OUTPUT);
  pcf8575.pinMode(P6,OUTPUT);
  pcf8575.pinMode(P7,OUTPUT);
  

  pcf8575.digitalWrite(P0,LOW);
  pcf8575.digitalWrite(P1,LOW);
  pcf8575.digitalWrite(P2,LOW);
  pcf8575.digitalWrite(P3,LOW);
  pcf8575.digitalWrite(P4,LOW);
  pcf8575.digitalWrite(P5,LOW);
  pcf8575.digitalWrite(P6,HIGH);
  pcf8575.digitalWrite(P7,HIGH);
}
//Enable the magnet on the given row - row 0 is on the left
bool enable_SSR(byte row){
  pcf8575.pinMode(row,OUTPUT);
  pcf8575.digitalWrite(row,HIGH);
  if(row<4){  //Use Timer0 for row 0-1-2-3
    toggle0 = true;
    ITimer0.restartTimer();
  }           // Use Timer1 for rows 4-5-6-7
  else {
    toggle1 = true;
    ITimer1.restartTimer();
  }
}

//rows is 0 or 4
void disable_SSR(byte rows){
  //Stop the timer and reset the bool varaible
  if(!rows){   
    ITimer0.stopTimer();
    toggle0 = true;
  }
  else {
    ITimer1.stopTimer();
    toggle1 = true;
  }
  //DIsable the SSR
  for (int i = 0; i<4; i++){
    //pcf8575.pinMode(rows+i,OUTPUT);
    pcf8575.digitalWrite(rows+i,LOW);
  }
}
