#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(7,8);//Declaramos el pin 2 Tx y 3 Rx
//Declaramos la variables para la obtención de datos
int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
double posicao1;
double posicao2 ;
int dia, mes, ano;
void setup()
{

Serial.begin(9600);//Iniciamos el puerto serie
serialgps.begin(9600);//Iniciamos el puerto serie del gps
//Imprimimos:
Serial.println("");
Serial.println("Arduino Rastreador");
Serial.println(" ---Buscando Sinal--- ");
Serial.println("");
}

void loop()
{
while(serialgps.available()) 
{
int c = serialgps.read(); 
if(gps.encode(c)) 
{
float latitude, longitude;
gps.f_get_position(&latitude, &longitude);
Serial.print("Latitude: "); 
Serial.println(latitude,5); 
Serial.print("Longitude: "); 
Serial.println(longitude,5);
posicao1 = latitude,5;
posicao2 = longitude,5;
gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
Serial.print("Data: "); Serial.print(day, DEC); Serial.print("/"); 
Serial.print(month, DEC); Serial.print("/"); Serial.print(year);
Serial.println("");  
dia = day, DEC;
mes = month, DEC;
ano = year;
Serial.print("Velocidad(kmph): "); Serial.println(gps.f_speed_kmph());
Serial.print("Satelites: "); Serial.println(gps.satellites());
Serial.println();
gps.stats(&chars, &sentences, &failed_checksum);
/* Aqui começa a passagem de valores usando variaveis como parametro */
Serial.print("Variavel Latitude: ");
Serial.println(posicao1,5);
Serial.print("Variavel Longitude: ");
Serial.println(posicao2,5);
Serial.print("Variavel Data: ");
Serial.print(dia);Serial.print("/");
Serial.print(mes);Serial.print("/");
Serial.print(ano);
Serial.println("");
Serial.println("");
/*Aqui termina*/
}
}
}
