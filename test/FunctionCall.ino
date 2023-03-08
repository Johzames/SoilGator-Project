#include <SoftwareSerial.h>

#define RE 4
#define DE 5

const byte Temp[] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xd5, 0xca};//Different depending on sensor
const byte Humi[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0a};//Different depending on sensor
const byte pH[] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0a};//Different depending on sensor
unsigned char Sensor = 0;
float valt;
float valh;
float valp;
byte values[10];
SoftwareSerial mod(10, 11);//Only 10 and 11 work; may be different on MKR

void setup() 
{
  Serial.begin(9600);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

}

void loop() 
{
  Sensor = Serial.read();
  switch(Sensor)
  {
    case '1':
    Serial.println("Reading Temperature");
    valt = SensorQueryTemp();//First call
    delay(30);//Delay is needed 20 wasnt enough
    valt = SensorQueryTemp();//Second call
    Serial.println(valt);
    break;

    case '2':
    Serial.println("Reading Humidity");
    valh = SensorQueryHumidity();//First call
    delay(30);//Delay is needed 20 wasnt enough
    valh = SensorQueryHumidity();//Second call
    Serial.println(valh);
    break;

    case '3':
    Serial.println("Reading pH");
    valp = SensorQuerypH();//First call
    delay(30);//Delay is needed 20 wasnt enough
    valp = SensorQuerypH();//Second call
    Serial.println(valp);
    break;
  }
}

float SensorQueryTemp()
{
  float svalt;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(20);

  if (mod.write(Temp, sizeof(Temp)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 10; i++)//Use ten to over read extra data in register
    {
      values[i] = mod.read();
    }
    svalt = (values[4])/10;
  }
  return svalt;
}

float SensorQueryHumidity()
{
  float svalh;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  if (mod.write(Humi, sizeof(Humi)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 10; i++)//Use ten to over read extra data in register
    {
      values[i] = mod.read();
    }
    svalh = (values[4])/10;
  }
  return svalh;
}

float SensorQuerypH()
{
  float svalp;
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  if (mod.write(pH, sizeof(pH)) == 8)
  {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    for (byte i = 0; i < 10; i++)//Use ten to over read extra data in register
    {
      values[i] = mod.read();
    }
    svalp = (values[4])/10;
  }
  return svalp;
}