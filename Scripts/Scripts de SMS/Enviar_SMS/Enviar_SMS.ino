/*
### Send SMS messages
1. This example is used to test DFRobot_SIM808 GPS/GPRS/GSM Shield's send SMS func.
2. Open the SIM808_SendSMS example or copy these code to your project
3. Replace the phoneNumber：#define PHONE_NUMBER  "187******39"    
4. Download and dial the function switch to Arduino, and enjoy it

create on 2016/09/23, version: 1.0
by jason
*/
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

//Mobile phone number,need to change
#define PHONE_NUMBER "+5522997195392"  
 
//The content of messages sent
#define MESSAGE  "Local"

#define PIN_TX  7 // Definindo pino de transmissão serial TX para pino de número 7 - cabo cor azul.
#define PIN_RX  8 // Definindo pino de transmissão serial RX para pino de número 8 - cabo cor amarela.

SoftwareSerial mySerial(PIN_TX,PIN_RX); // Define os pinos de transmissão serial.
DFRobot_SIM808 sim808(&mySerial);       // Cenecta RX,TX,PWR.

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
 
 //******** Initialize sim808 module *************
  while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }  
  Serial.println("Sim808 init success");
  Serial.println("Start to send message ...");

  //******** define phone number and text **********
  sim808.sendSMS(PHONE_NUMBER,MESSAGE); 

  Serial.println("Message sent...");

}

void loop() {
  //nothing to do
}
