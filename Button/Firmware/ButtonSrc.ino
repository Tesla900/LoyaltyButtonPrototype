  const byte button1 = 3;         //"Good" button
  const byte button2 = 4;         //"Med" button
  const byte button3 = 5;         //"Bad" button

  const byte led1 = 6;            //"Good" LED
  const byte led2 = 7;            //"Med" LED
  const byte led3 = 8;            //"Bad" LED

  const byte interruptPin = 2;    //Wake-up interrupt
 
  const uint8_t device_id = 0x05; //Device ID (may be set to EEPROM)

void setup() {
  
  
/*Set button pins as inputs*/
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(interruptPin, INPUT);

/*Set LED pins as outputs*/
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
/*Set D2 as interrupt pin at falling edge*/   
  attachInterrupt(digitalPinToInterrupt(interruptPin), ButtonPressed, FALLING);
  
/*Go to sleep mode*/
  GoToSleep(); 
    
}

void loop() {
    
  delay(300);                           // LED delay
  digitalWrite(led1, LOW);              // sets the digital pin 13 off
  digitalWrite(led2, LOW);              // sets the digital pin 13 off
  digitalWrite(led3, LOW);              // sets the digital pin 13 off
 
  GoToSleep();                          // Sleep
  
}

/*Sleep mode setup*/

void GoToSleep(void){  
  ADCSRA &= ~(1<<7);                    //Disable ADC
  SMCR |= (1<<2);                       //Set power down mode
  SMCR |= 1;                            //Enable sleep
  __asm__ __volatile__("sleep");        //activating sleep mode
  
}

/*Send data over UART (or other interface)*/

void DataSend(uint8_t ID, char *data){
  
  Serial.print(":");                    //String start symbol
  Serial.print(String(ID));             //Device ID
  Serial.print("|");                    //Spacer symbol
  
  while(*data){                         //While data does not point to the null which is the string terminator
    Serial.print(*data);                //Print symbol
    data++;                             //Pointer increment
  }
  Serial.println("/");                  //End of line symbol
}

/*Button pressed interrupt*/

void ButtonPressed(void){
  
  Serial.begin(115200);
  
  if (digitalRead(button1)== LOW){
    delayMicroseconds(1000);            //Debounce stability (remove later)
    digitalWrite(led1, HIGH);           // sets the digital pin 13 on  
    DataSend(device_id, "GOOD");        // send "GOOD"
  }
  
  else if (digitalRead(button2)== LOW){
    delayMicroseconds(1000);            //Debounce stability (remove later)
    digitalWrite(led2, HIGH);           //Sets the digital pin 13 on 
    DataSend(device_id, "MEDIUM");      //Send "MEDIUM"
  }
  
  else if (digitalRead(button3)== LOW){
    delayMicroseconds(1000);            //Debounce stability (remove later)
    digitalWrite(led3, HIGH);           //Sets the digital pin 13 on 
    DataSend(device_id, "BAD");         //Send "BAD"
  }
   
}
