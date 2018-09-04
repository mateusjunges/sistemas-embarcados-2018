#define pinAnalogico 17

void setup() {
  // put your setup code here, to run once:
  pinMode(pinAnalogico, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int leitura = analogRead(pinAnalogico);  
  Serial.println(leitura);
}
