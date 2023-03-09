#include <SoftwareSerial.h>

#define RE 4
#define DE 5

const byte pH[] = {0x01, 0x03, 0x00, 0x0d, 0x00, 0x01, 0x15, 0xc9};//Inquiry frame for JXCT
byte values[10];
float acid;
SoftwareSerial mod(10, 11);// Only 10 and 11 work; may be different on MKR

void setup() {
  Serial.begin(9600);
  mod.begin(9600);//4800 for 5pin 9600 for 2pin
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

}

void loop() {
  digitalWrite(DE, HIGH);//Sets the max485 to send data
  digitalWrite(RE, HIGH);//Sets the max485 to send data
  delay(10);

  if (mod.write(pH, sizeof(pH)) == 8)
  {
    digitalWrite(DE, LOW);//Sets the max485 to recieve data
    digitalWrite(RE, LOW);//Sets the max485 to recieve data
    if (mod.available() > 0) {
        Serial.print("Receive data:");
    }
    for (byte i = 0; i < 10; i++)//Use ten to over read extra data in register
    {
      values[i] = mod.read();
      Serial.print(values[i], HEX);
    }
    Serial.println();
  }
  float acid = float(values[4]) / 10;
  Serial.print("pH: ");
  Serial.println(acid, 1);

  delay(3000);
}
