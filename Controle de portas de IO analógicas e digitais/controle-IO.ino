//PINAGEM ARDUINO
//#define pinMaxLed 13
//#define pinMinLed 12
//#define pinMaxButton 3
//#define pinMinButton 4
//#define pinLedFadeDigital 9 
//#define pinLedFadeAnalogico 5
//#define pinControl 14

//PINAGEM ESP
#define pinMaxLed 0 //D3
#define pinMinLed 05 //D1
#define pinMaxButton 14 //D5
#define pinMinButton 9 //sd2
#define pinLedFadeDigital 15 //D6
#define pinLedFadeAnalogico 04 //D2
#define pinControl A0 //Analogico

//GERAL
#define incremento 1
#define minLed 0
#define maxLed 255
#define delayMaxLed 1000
#define delayMinLed 500


int i = 0;
int ledAnalogico = 0;
int i_afterMap = 0;

void setup(){
  pinMode(pinMaxLed, OUTPUT);
  pinMode(pinMinLed, OUTPUT);
  pinMode(pinMaxButton, INPUT);
  pinMode(pinMinButton, INPUT);
  pinMode(pinControl, INPUT);
  digitalWrite(pinMinLed, LOW);
  digitalWrite(pinMaxLed, LOW);  
  Serial.begin(9600);
}
void loop(){
  if(i == minLed)
    piscaLed(pinMinLed, delayMinLed);

  if(i == maxLed)
    piscaLed(pinMaxLed, delayMaxLed);
    

  if((digitalRead(pinMaxButton) == HIGH) && (i < maxLed)){
      i += incremento;
      i_afterMap = map(i, 0, 1024, 0, 255);
      Serial.println(i);
      digitalWrite(pinLedFadeDigital, i_afterMap);
  }

  if((digitalRead(pinMinButton) == HIGH) && (i > minLed)){
    i -= incremento;
    i_afterMap = map(i, 0, 1024, 0, 255);
    Serial.println(i);
    digitalWrite(pinLedFadeDigital, i_afterMap);  
  }  
 if(analogRead(pinControl)){
    ledAnalogico = analogRead(pinControl);
    ledAnalogico = map(ledAnalogico, 0, 1024, 0, 256);
    Serial.println(ledAnalogico);
    analogWrite(pinLedFadeAnalogico, ledAnalogico);
    delay(100);
  }  
  
}
void piscaLed(int pin, int delayTime){
  digitalWrite(pin, HIGH);
  delay(delayTime);
  digitalWrite(pin, LOW);
  delay(delayTime);
}
