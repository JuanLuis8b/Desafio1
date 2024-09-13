// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

//funciones
void redimensionar();
int frecuencia();
int amplitud();
void tipo ();

//variables para leer los datos del generador
int analogPin = A0;
int dato = 0;

//variables para los pulsadores
int pulsador1 = 2;
int pulsador2 = 4;

void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  pinMode (pulsador1, INPUT);
  pinMode (pulsador2, INPUT);
}

void loop(){
  if (digitalRead(pulsador1) == HIGH){
    dato = analogRead(analogPin);
    Serial.println(dato);
  }
}
