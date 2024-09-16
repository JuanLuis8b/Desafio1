// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

//funciones
void redimensionar2D(int**&arr , int &capacidadC, int &capacidadF);
void redimensionar1D();
int frecuencia();
int amplitud();
void tipo ();

//variables para leer los datos del generador
int analogPin = A0;
int pulsador1 = 2;
int pulsador2 = 4;

//variables globlales
int dato = 0;

int capPuntos = 100; //=capTiempos
int capTiempos = 100;

int *puntos;
int *tiempos;

int numPunto=0;//=numTiempo

bool estadoAdquisicion= false;

float datoant = 0.1;

//setup
void setup() {
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  pinMode (pulsador1, INPUT);
  pinMode (pulsador2, INPUT);

  puntos = new int[capPuntos];
  tiempos = new int [capPuntos];

}

//loop
void loop(){

  bool estadoPulsador = digitalRead(pulsador1);

  if (estadoAdquisicion ^ estadoPulsador){

    estadoAdquisicion=!estadoAdquisicion;

    delay(1);

    dato = analogRead(analogPin);

    if (dato != datoant){

        datoanterior = dato;
        puntos[numPunto] = dato;
        numPunto++;

        ptTiempo = millis();
        tiempos[numPunto] = ptTiempo;

        if (numPunto>=capPuntos){
           redimensionar(puntos,capPuntos);
           redimensionar (tiempos, capTiempos);
        }
      }
    }

   if (digitalRead(pulsador2)){
     continue
  }

  }//fin loop

//implementacion de funciones

void redimensionar(int *&arr, int &capacidad){

 unsigned int nuevaCap = capacidad*2;

  int *nuevoArr = new int [nuevaCap];

  for (unsigned int i = 0; i<capacidad;i++){
    nuevoArr[i] = arr[i];
  }

  delete [] arr;

  arr = nuevoArr;
  capacidad = nuevaCap;
}

void calculo (){


}
