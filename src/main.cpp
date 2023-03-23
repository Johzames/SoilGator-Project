#include <Arduino.h>
#include "wiring_private.h"
#include "DHT.h"




// Create the new UART instance assigning it to pin 1 and 0
Uart mySerial (&sercom3, 1, 0, SERCOM_RX_PAD_1, UART_TX_PAD_0);
#define RE 3                            // Control pins for RS485 communication direction
#define DE 4
#define acidPin A0                      // analog pin for pH
#define tempPin 2
#define lightPin A1                     // analog pin for photoresistor
#define moistPin A2

#define DHTTYPE DHT11                   // DHT Type using
#define LIGHT 750                       // tentative value for shade

DHT dht(tempPin, DHTTYPE);              // initialize DHT object

// Global variables
float acidVal = 0;
float tempVal = 0;                      // temperature value
float lightLev = 0;
float moistLev = 0;
//Request command for pH value. Different value for other sensors
const byte pH[] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0a}; 
byte values[10];
byte send = 0;


unsigned long currentMills = 0;
unsigned long prevMills = 0;

void setup() {
  Serial.begin(9600);  
  Serial.println("Start");              // Test the serial monitor
  pinPeripheral(1, PIO_SERCOM);         // Assign RX function to pin 1
  pinPeripheral(0, PIO_SERCOM);         // Assign TX function to pin 0  
  pinMode(RE, OUTPUT);                  // Mode for control pins
  pinMode(DE, OUTPUT);
  delay (2000);                         // give time to measure
}

void loop() {
  /////// pH SECTION ///////
  digitalWrite(DE, HIGH);//Sets the max485 to send data
  digitalWrite(RE, HIGH);//Sets the max485 to send data
  delay(10);

    for (int j = 0; j < 8; j++)         // send pH request
    {
      send = Humi[j];
      mySerial.write(send);
      Serial.print(send, HEX);
    }

    delay(10);

    digitalWrite(DE, LOW);//Sets the max485 to recieve data
    digitalWrite(RE, LOW);//Sets the max485 to recieve data

    while (mySerial.available())       // receive data
    {    
        Serial.println("Receive data:");
        for (byte i = 0; i < 7; i++)
        {
          values[i] = mySerial.read();
          Serial.print(values[i], HEX); // view received data for testing
        }
        Serial.println();
    }

  float acidVal = ((values[3] << 8) | (values[4])) / 10;
  Serial.print("pH: ");
  Serial.println(acidVal);

  /////// TEMPERATURE SECTION ///////
  tempVal = dht.readTemperature(true);  // in Fahrenheit (true)

  /** Failed Case
   if(isnan(t)) {
    Serial.println("Failed Measurement...")
   }
  */

  Serial.print("Temperature: ");        // test in serial monitor
  Serial.println(tempVal);

  
  /////// LIGHT SECTION ///////
  currentMills = millis();
  Serial.print(currentMills);
  Serial.print("     Light Section");

  lightLev = analogRead(lightPin);

  /* start recording time when sun is up
  if(lightLev > LIGHT) {
    upMillis = millis();
    WE MAY NEED A DS1307 MODULE instead
  }
    downMillis = millis();
    float upTime = downMillis - upMillis;
    float totalTime = totalTime + upTime
  */

  /////// MOISTURE SECTION ///////
  moistLev = analogRead(moistPin);
  moistLev = map(moistLev, 0, 550, 0, 100);
  Serial.print("Moisture: ");        // test in serial monitor
  Serial.print(moistLev);
  Serial.println("%");
}

// Attach the interrupt handler to the SERCOM
void SERCOM3_Handler()
{
  mySerial.IrqHandler();
}
