#include <SoftwareSerial.h>


SoftwareSerial mySerial(4, 0); // Cria um RX e TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Digite o comando AT desejado: ");
  mySerial.begin(9600);
}

void loop() {
  if(mySerial.available())
    Serial.write(mySerial.read());
  if(Serial.available())
    mySerial.write(Serial.read());
}
