/*
### Read SMS messages
 1. This example is used to test DFRobot_SIM808 GPS/GPRS/GSM Shield's reading SMS
 2. Open the SIM808_SMSread example or copy these code to your project
 3. Download and dial the function switch to Arduino
 4. it will receive SMS Messages and send it to serial

create on 2016/09/23, version: 1.0
by jason
*/

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[16];
char datetime[24];

#define PIN_TX  7 // Definindo pino de transmissão serial TX para pino de número 7 - cabo cor azul.
#define PIN_RX  8 // Definindo pino de transmissão serial RX para pino de número 8 - cabo cor amarela.

SoftwareSerial mySerial(PIN_TX,PIN_RX); // Define os pinos de transmissão serial.
DFRobot_SIM808 sim808(&mySerial);       // Cenecta RX,TX,PWR.


void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);

  //******** Initialize sim808 module *************
  while(!sim808.init()) {
      Serial.print("Sim808 init error\r\n");
      delay(1000);
  }
  delay(3000);  
  Serial.println("Init Success, please send SMS message to me!");
}

void loop() {
  //*********** Detecting unread SMS ************************
   messageIndex = sim808.isSMSunread();
    Serial.print("messageIndex: ");
    Serial.println(messageIndex);

   //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0) { 
      sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
                 
      //***********In order not to full SIM Memory, is better to delete it**********
      sim808.deleteSMS(messageIndex);
      Serial.print("From number: ");
      Serial.println(phone);  
      Serial.print("Datetime: ");
      Serial.println(datetime);        
      Serial.print("Recieved Message: ");
      Serial.println(message);    
   }
}
