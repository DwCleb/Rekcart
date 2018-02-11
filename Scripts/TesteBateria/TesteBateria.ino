#define LED 13
#define resistor 6.9

float capacity=0, value,voltage,current, time=0;

void measure (void) {

value= analogRead(0);

voltage=value/1024*5.0;

current = voltage/resistor;

capacity=capacity+current/3600;

time++;

Serial.print("Voltage= ");
Serial.print(voltage);

Serial.print("V Current= ");
Serial.print(current);

Serial.print("A Capacity= ");
Serial.print(capacity);
Serial.print("Ah ");

Serial.print("Discharging time= ");
Serial.print(time);
Serial.print("s ");

Serial.print("\n");
}

boolean x=false;

ISR(TIMER1_OVF_vect) {
TCNT1=0x0BDC;
x=!x;

measure();

}

void setup() {

pinMode(LED, OUTPUT);

TIMSK1=0x01; // Permitiu interrupção global e temporária de transbordamento;
TCCR1A = 0x00; //operação normal página 148 (modo0);
TCNT1=0x0BDC; // define o valor inicial para remover o erro de tempo (registro contador de 16 bits);
TCCR1B = 0x04; //  temporizador de início / set clock;

Serial.begin(9600);

};

void loop () {

digitalWrite(LED, x);

};


