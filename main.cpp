// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

//funciones de apoyo
void copy ();

//funciones
void redimensionar();
int frecuencia();
int amplitud();
void tipo ();
bool presionado();

//variables para leer los datos del generador
int analogPin = A0;
int pulsador1 = 2;
int pulsador2 = 4;

//variables globlales
int dato = 0;
int capDatos = 1000;
int capCiclos = 3;

int **ciclos = new int*[capCiclos];
for (unsigned int i = 0; i < capCiclos; i++){
    ciclos[i] = new int [capDatos]; }

int numelem=0;

//setup
void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  pinMode (pulsador1, INPUT);
  pinMode (pulsador2, INPUT);

}

//loop
void loop(){

 dato = analogRead(analogPin);

//implementacion de funciones

void redimensionar(int**&arr , int &capacidadC, int &capacidadF){

  unsigned int nuevaCapC = capacidadC*2;
  unsigned int nuevaCapF = capacidadF*2;

  int**nuevoArr = new int *[nuevaCapF];
  for (unsigned int i = 0; i < nuevaCapF;i++){
    nuevoArr[i] = new int [nuevaCapC];
  }

  for (unsigned int i = 0; i < capacidadF; i++) {
     for (unsigned int j = 0; j < capacidadC; j++) {
         nuevoArr[i][j] = arr[i][j];
     }
  }

  for (unsigned int i = 0; i < capacidadF;i++){
    delete[] arr[i];
  }
  delete [] arr;

  arr = nuevoArr
  capacidadC = nuevaCapC;
  capacidadF = nuevaCapF;
}

bool presionado(analogPin){



}
