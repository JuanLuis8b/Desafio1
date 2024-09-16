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
int ptTiempo = 0;

int capPuntos = 100; //=capTiempos
int capTiempos = 100;

int *puntos = nullptr;
int *tiempos = nullptr;

int numPunto=0;//=numTiempo

bool estadoAdquisicion= false;
bool estadoAnteriorPulsador = false;

float datoant = 0.5;

int num = 1;

//setup
void setup() {
    Serial.begin(9600);
    lcd_1.begin(16, 2);
    pinMode (pulsador1, INPUT);
    pinMode (pulsador2, INPUT);
}

//loop
void loop(){

    if (puntos == nullptr && tiempos == nullptr){
      puntos = new int [capPuntos];
      tiempos = new int [capPuntos];
    }

    bool estadoPulsador = digitalRead(pulsador1);

    if (estadoPulsador!=estadoPulsadorAnterior){
        estadoPulsadorAnterior=estadoPulsador;
        estadoAdquisicion = true;
    }

    if (estadoAdquisicion ^ estadoPulsador){
      estadoAdquisicion=!estadoAdquisicion;
      delay(1);
      Serial.print("continua ");
      Serial.println(num);
      num++;

      dato = analogRead(analogPin);

      if (dato != datoant){
        datoant= dato;
        puntos[numPunto] = dato;
        ptTiempo = millis();
        tiempos[numPunto] = ptTiempo;
        numPunto++;

        if (numPunto>=capPuntos){
          redimensionar(puntos,capPuntos);
          redimensionar (tiempos, capTiempos);
        }
      }
    }else{
    return;
    /*   delete [] puntos;
    delete [] tiempos;
    puntos = nullptr;
    tiempos = nullptr;
    numPunto = 0;
    capPuntos = 100;
    capTiempos = 100;
    */
    }
    if (digitalRead(pulsador2)){
    return;
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



bool debounce(int pulsador){

    bool lectura = digitalRead(pulsador1);
    unsigned long debounceDelay = 50; // Tiempo de debounce en milisegundos
    unsigned long ultimoTiempoPulsador = 0;
    bool ultimoEstadoPulsador = LOW;

    if (lectura != ultimoEstadoPulsador) {
        ultimoTiempoPulsador = millis();
    }

    if ((millis() - ultimoTiempoPulsador) > debounceDelay) {
        if (lectura != estadoAdquisicion) {
            estadoAdquisicion = lectura;
            return true;
        }
    }

    ultimoEstadoPulsador = lectura;
    return false;

}
