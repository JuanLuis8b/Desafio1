// C++ code
//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

//funciones
void redimensionar(int *&arr, int &capacidad);
void redimensionar_(float *&arr, int &capacidad);
void liberarMemoria(int *&arr);
void liberarMemoria_(float *& arr);
void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos);
void analisis(int &posicion, int*&tipo, float*& amplitud, float*& frecuencia, int t_defecto, int &numPunto,int *&puntos, int*& tiempos);
int calculoPorPendiente(int *& puntos, int *& tiempos,int jinicial,int jfinal);
void imprimirlcd(int tipoOnda, int frecuencia, int amplitud);
float pendiente(int x1, int x2, int y1, int y2);
float absf(float m);



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
      Serial.println("Adqusion started");
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
        liberarMemoria(puntos);
        liberarMemoria(tiempos);
        numPunto = 0;
        capPuntos = 100;
        capTiempos = 100;
    }

    bool estadoPulsador2 = digitalRead(pulsador2);
    if (estadoPulsador2){
      while (digitalRead(pulsador2) == HIGH) {
          delay(10);
      }
    }

    if (estadoPulsador2){
      Serial.println("Entra al analisis");

        int t_defecto = 10;
        int posicion = 0;

        int *tipo = new int [t_defecto];
        float *amplitud = new float [t_defecto];
        float *frecuencia = new float [t_defecto];

        analisis(posicion,tipo,amplitud,frecuencia,t_defecto,numPunto,puntos,tiempos);

        for (int i=0;i<posicion;i++){

            imprimirlcd(tipo[i],frecuencia[i],amplitud[i]);

          Serial.println(tipo[i]);
        Serial.println(frecuencia[i]);
      Serial.println(amplitud[i]);

        }


        liberarMemoria(puntos);
        liberarMemoria(tiempos);

        liberarMemoria(tipo);
        liberarMemoria_(amplitud);
        liberarMemoria_(frecuencia);
    }
}//fin loop

//implementacion de funciones

void redimensionar(int *&arr, int &capacidad){
    Serial.println("redimensionar");
    int nuevaCap = capacidad*2;
    int *nuevoArr = new int [nuevaCap];
    for (unsigned int i = 0; i<capacidad;i++){
        nuevoArr[i] = arr[i];
    }
    delete [] arr;
    arr = nuevoArr;

    capacidad = nuevaCap;
  Serial.println(nuevaCap);
    }

void redimensionar_(float *&arr, int &capacidad){
    int nuevaCap = capacidad*2;
    float *nuevoArr = new float [nuevaCap];
    for (unsigned int i = 0; i<capacidad;i++){
        nuevoArr[i] = arr[i];
    }
    delete [] arr;
    arr = nuevoArr;
    capacidad = nuevaCap;
    }

void liberarMemoria(int *&arr){
    if (arr != nullptr){
        delete[]arr;
        arr = nullptr;
    }

}

void liberarMemoria_(float *&arr){
    if (arr != nullptr){
        delete[]arr;
        arr = nullptr;
    }

}

void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos){

    if (dato != datoant){
        datoant = dato;
        puntos[numPunto] = dato;
        tiempos[numPunto] = ptTiempo;

                Serial.println(puntos[numPunto]);
        numPunto++;


        }
  if (numPunto>=capPuntos){
    Serial.println(numPunto);
          redimensionar(puntos,capPuntos);
          redimensionar (tiempos, capTiempos);
    }
}

void imprimirlcd(int tipoOnda, float frecuencia, float amplitud){

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
    lcd.print("Hz");

    lcd.setCursor(9,1);
    lcd.print("A:");
    lcd.print(amplitud,1);
    lcd.print("V");

    delay(500);
}

float pendiente(int x1, int x2, int y1, int y2){
  float m = static_cast<float>(y2-y1)/(x2-x1);
  return m;
}
float absf(float m){
    if (m>=0){
        return m;
    }else {
        return -m;
    }
}

int calculoPorPendiente(int *& puntos, int *& tiempos,int jinicial,int jfinal){

    int tamanio = (jfinal-jinicial)/2;
    float *pendientes = new float[tamanio];
    int c =0;
    float m;

    for (int i = jinicial;i<=jfinal;i+=2){
        m = absf(pendiente(tiempos[i],tiempos[i+2],puntos[i],puntos[i+2]));
        c++;

        if (c>=tamanio){
            redimensionar_(pendientes,tamanio);
        }
    }

    int maximaRepeticion = 0;

    for (int i = 0; i<tamanio;i++){
        int contador = 1;
        for (int j = i +1 ; j<tamanio;j++){
            if (pendientes[i]==pendientes[j]){
                contador++;
            }
        }
        if(contador>maximaRepeticion){
            maximaRepeticion = contador;
        }
    }


    liberarMemoria_(pendientes);

    if (maximaRepeticion<3){
        return 1;
    }
    else if (maximaRepeticion>(tamanio*0.7)){
        return 2;
    }
    else{
        return 3;
    }
}

void analisis(int &posicion, int*&tipo, float*& amplitud, float *&frecuencia, int t_defecto, int &numPunto, int *&puntos, int*& tiempos){

    posicion = 0;

    for (int i = 0;i<=numPunto;i+=2){

        if ((puntos[i]==puntos[i+2] && puntos[i]==(abs(puntos[i+1]))) || pendiente(tiempos[i],tiempos[i+2],puntos[i],puntos[i+2])==0){
            tipo[posicion]=1;
            amplitud[posicion] = abs(puntos[i])/100.0;

            frecuencia[posicion]=(1000.0/(tiempos[i+2]-tiempos[i]));
            Serial.println(frecuencia[posicion]);
        }
        else{

            int j=i;
            int pico = puntos[j];

            if (puntos[j]<puntos[j+1]){//va en subida
                while (puntos[j+1]>pico){
                    pico = puntos[j];
                    j++;
                }
            } else{
                while (puntos[j+1]<pico){//va en bajada
                    pico = puntos[j];
                    j++;
                }
            }
            //j es la posicion del primer pico(sup/inf)

            int jf = j;

            int pico2 = puntos[j];

            if (pico<0){//el pico es negativo
                do{
                jf++;
                if (puntos[jf]>pico2){
                    pico2 = puntos[jf];
                }
            }while(pico!=puntos[jf]);
            }else{//el pico es positivo
                do{
                jf++;
                if (puntos[jf]<pico2){
                    pico2 = puntos[jf];
                }
            }while(pico!=puntos[jf]);
            }
            //jf es la posicion final del periodo

            tipo[posicion]=calculoPorPendiente(puntos,tiempos,j,jf);

          if (tipo[posicion] == 1){
            amplitud[posicion]=abs(puntos[i])/100.0;
            frecuencia[posicion]=(1000.0/(tiempos[i+2]-tiempos[i]));
          }else{

            amplitud[posicion]=abs(pico/100.0);

            Serial.println(tiempos[jf]);
            Serial.println(tiempos[j]);
            frecuencia[posicion]=(1000.0/(tiempos[jf]-tiempos[j]));
          }


            i = jf;
        }

        posicion++;

        if (posicion>=t_defecto){
            int t2 = t_defecto;
            int t3 = t_defecto;

            redimensionar(tipo,t_defecto);
            redimensionar_(amplitud,t2);
            redimensionar_(frecuencia,t3);
        }
    }
}
