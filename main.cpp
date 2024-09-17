// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

//funciones
void redimensionar(int *&arr, int &capacidad);
void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos);
void liberarMemoria(int* &puntos, int* &tiempos, int &numPunto, int &capPuntos, int&capTiempos);
int tipoOnda(int*& puntos, int *&tiempos, int &numPunto);
int frecuencia(int*& puntos, int *&tiempos);
int amplitud(int*& puntos, int *&tiempos, int &numPunto);


//variables para leer los datos del generador
int analogPin = A0;
int pulsador1 = 2;
int pulsador2 = 4;

//variables globlales
int dato = 0;
int ptTiempo = 0;

int capPuntos = 50; //=capTiempos
int capTiempos = 50;

int *puntos = nullptr;
int *tiempos = nullptr;

int numPunto=0;//=numTiempo

bool estadoAdquisicion= false;

int datoant;

//setup
void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
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
        adquirirDatos(dato,datoant,ptTiempo,puntos,tiempos,numPunto,capPuntos,capTiempos);
    }

    else{
      liberarMemoria(puntos,tiempos,numPunto,capPuntos,capTiempos);
    }

    bool estadoPulsador2 = digitalRead(pulsador2);

    if (estadoPulsador2){
      while (digitalRead(pulsador2) == HIGH) {
          delay(10);
      }
    }

    if (estadoPulsador2){

      int a = tipoOnda(puntos, tiempos, numPunto);
      int frec = frecuencia(puntos,tiempos)

    }
}//fin loop

//implementacion de funciones

void redimensionar(int *&arr, int &capacidad){
    //Serial.println("redimensionar");
    unsigned int nuevaCap = capacidad*2;
    int *nuevoArr = new int [nuevaCap];
    for (unsigned int i = 0; i<capacidad;i++){
        nuevoArr[i] = arr[i];
    }
    delete [] arr;
    arr = nuevoArr;
    capacidad = nuevaCap;
    }

void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos){

    if (dato != datoant){
        datoant = dato;
        puntos[numPunto] = dato;
        ptTiempo = millis();
        tiempos[numPunto] = ptTiempo;
        numPunto++;
        //Serial.println("adquirir");
        //Serial.println(dato);
        //Serial.println(ptTiempo);

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
    //Serial.println("liberar");

    numPunto = 0;
    capPuntos =50;
    capTiempos = 50;
  }
}

float frecuencia (int*& puntos, int *&tiempos){

  int pico = puntos[0];
  int i=0;

  if (puntos[i]<puntos[i+1]){
    while (puntos[i+1]>pico){
        pico = puntos[i];
        i++;
    }
  }else{
    while (puntos[i+1]<pico){
        pico = puntos[i];
        i++;
    }
  }
  int indexTiempoIni = i;

  do{
    i++;
  }while(pico!=puntos[i])

    int indexTiempoFinal = i;

    return (tiempos[indexTiempoFinal]-tiempos[indexTiempoIni])/1000.0;

 }
int tipoOnda(int*& puntos, int *&tiempos, int &numPuntos){
  //Serial.println("Entra a tipo");
  float mi;
  float m;
  //int pendientes[numPuntos/2];
  for (int i = 0; i<numPuntos;i+=2){
    mi = pendiente(tiempos[i],tiempos[i+2],puntos[i],puntos[i+2]);
    m = pendiente (tiempos[i+2],tiempos[i+4],puntos[i+2],puntos[i+4]);

    if (mi == 0 && m == 0){
        return 1 ;
    }else if (mi != m){
        return 2;
    }else{
        return 3;
    }

  }


}



void amplitud (int*& puntos, int *&tiempos, int &numPunto){
  int *amplitudes [];
  int max;
  int min;
  for (int i=0;i<numPunto;i++){
    if(puntos[i]>max){
      max = puntos[i];
    if (puntos[i]<min){
      min = puntos[i];
    }
    }
  }

}

float pendiente(int x1, int x2, int y1, int y2){
  float m = static_cast<float>(y2-y1)/(x2-x1);
  return m;
}

void imprimirlcd(int tipoOnda; int frecuencia, int amplitud){

    lcd.clear();

    lcd.setCursor(0,0);
    if (tipoOnda == 1) {
        lcd.print("Onda Cuadrada");
    } else if (tipoOnda == 2) {
        lcd.print("Onda Triangular");
    } else if (tipoOnda == 3) {
        lcd.print("Onda Senoidal");
    } else {
        lcd.print("Onda Desconocida");
    }

    lcd.setCursor(0,1);
    lcd.print("F:");
    lcd.print(frecuencia,1);
    lcd.print("Hz")

    lcd.setCursor(8,1);
    lcd.print("A:");
    lcd.print(amplitud,1);
    lcd.print("V");
}
