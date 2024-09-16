// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

//funciones
void redimensionar();
int frecuencia();
int amplitud();
void tipo ();
void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos);
void liberarMemoria(int* &puntos, int* &tiempos, int &numPunto, int &capPuntos, int&capTiempos);


//variables para leer los datos del generador
int analogPin = A0;
int pulsador1 = 2;
int pulsador2 = 4;

//variables globlales
int dato = 0;
int ptTiempo = 0;

int capPuntos = 10; //=capTiempos
int capTiempos = 10;

int *puntos = nullptr;
int *tiempos = nullptr;

int numPunto=0;//=numTiempo

bool estadoAdquisicion= false;

int datoant;

//setup
void setup() {
    Serial.begin(9600);
    lcd_1.begin(16, 2);
    pinMode (pulsador1, INPUT);
    pinMode (pulsador2, INPUT);
}

//loop
void loop(){


    bool estadoPulsador = digitalRead(pulsador1);

    if (estadoPulsador){

      if (puntos == nullptr && tiempos == nullptr){
         puntos = new int [capPuntos];
         tiempos = new int [capPuntos];
      }

      estadoAdquisicion =! estadoAdquisicion;
      while (digitalRead(pulsador1) == HIGH) {
          delay(10);
      }
    }

    if (estadoAdquisicion){

      dato = analogRead(analogPin);
      ptTiempo = millis();
      Serial.print("dato");
      Serial.println(numPunto);
      adquirirDatos(dato,datoant,ptTiempo,puntos,tiempos,numPunto,capPuntos,capTiempos);

      }

    else{
      liberarMemoria(puntos,tiempos,numPunto,capPuntos,capTiempos);
    }

    if (digitalRead(pulsador2)){
    return;
    }
}//fin loop

//implementacion de funciones

void redimensionar(int *&arr, int &capacidad){
    Serial.println("redimensionar");
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

void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos){

    if (dato != datoant){
        datoant = dato;
        puntos[numPunto] = dato;
        ptTiempo = millis();
        tiempos[numPunto] = ptTiempo;
        numPunto++;
        Serial.println("adquirir");

        if (numPunto>=capPuntos){
          redimensionar(puntos,capPuntos);
          redimensionar (tiempos, capTiempos);
        }
    }
}

void liberarMemoria(int* &puntos, int* &tiempos, int &numPunto, int &capPuntos, int&capTiempos){

  if (puntos != nullptr && tiempos != nullptr){

    delete[] puntos;
    delete[] tiempos;
    puntos = nullptr;
    tiempos = nullptr;
    Serial.println("liberar");
  }

  numPunto = 0;
  capPuntos = 10;
  capTiempos = 10;

}
