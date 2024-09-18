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
        liberarMemoria(puntos);
        liberarMemoria(tiempos);
        numPunto = 0;
        capPuntos = 50;
        capTiempos = 50;
    }

    bool estadoPulsador2 = digitalRead(pulsador2);
    if (estadoPulsador2){
      while (digitalRead(pulsador2) == HIGH) {
          delay(10);
      }
    }

    if (estadoPulsador2){

        int t_defecto = 10;
        int posicion;

        int *tipo = new int [t_defecto];
        float *amplitud = new float [t_defecto];
        float *frecuencia = new float [t_defecto];

        analisis(posicion,tipo,amplitud,frecuencia,t_defecto,numPunto,puntos,tiempos);

        for (int i=0;i<=posicion;i++){

            imprimirlcd(tipo[i],frecuencia[i],amplitud[i]);
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

void redimensionar_(float *&arr, int &capacidad){
    unsigned int nuevaCap = capacidad*2;
    float *nuevoArr = new float [nuevaCap];
    for (unsigned int i = 0; i<capacidad;i++){
        nuevoArr[i] = arr[i];
    }
    delete [] arr;
    arr = nuevoArr;
    capacidad = nuevaCap;
    }

void liberarMemoria(int *&arr){
    delete[]arr;
    arr = nullptr;
}

void liberarMemoria_(float *&arr){
    delete[]arr;
    arr = nullptr;

}

void adquirirDatos(int dato, int &datoant, int ptTiempo, int*&puntos, int*& tiempos, int &numPunto, int&capPuntos, int &capTiempos){

    if (dato != datoant){
        datoant = dato;
        puntos[numPunto] = dato;
        ptTiempo = millis();
        tiempos[numPunto] = ptTiempo;
        numPunto++;

        if (numPunto>=capPuntos){
          redimensionar(puntos,capPuntos);
          redimensionar (tiempos, capTiempos);
        }
    }
}

void imprimirlcd(int tipoOnda, int frecuencia, int amplitud){

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

    lcd.setCursor(8,1);
    lcd.print("A:");
    lcd.print(amplitud,1);
    lcd.print("V");

    delay(200);
}

float pendiente(int x1, int x2, int y1, int y2){
  float m = static_cast<float>(y2-y1)/(x2-x1);
  return m;
}

int calculoPorPendiente(int *& puntos, int *& tiempos,int jinicial,int jfinal){

    int tamanio = (jfinal-jinicial)/2;
    float *pendientes = new float[tamanio];
    int c =0;
    float m;

    for (int i = jinicial;i<=jfinal;i+=2){
        m = pendiente(tiempos[i],tiempos[i+2],puntos[i],puntos[i+2]);
        if (m<=0){
            pendientes[c] = -m;
        }else{
            pendientes[c] = m;
        }
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

    if (maximaRepeticion>(tamanio/2)){
        return 2;
    }
    else if(contador<2){
        return 4;
    }else{
        return 3;
    }
}

void analisis(int &posicion, int*&tipo, float*& amplitud, float *&frecuencia, int t_defecto, int &numPunto, int *&puntos, int*& tiempos){

    posicion = 0;

    for (int i = 0;i<numPunto;i+=2){

        if (puntos[i]==puntos[i+2] && puntos[i] == (-puntos[i+1])){
            tipo[posicion]=1;
            if (puntos[i]<0) {
                amplitud[posicion]=((-puntos[i]+puntos[i+1])/2)/100;
                }
            else {
                amplitud[posicion]=((puntos[i]-puntos[i+1])/2)/100;
            }
            frecuencia[posicion]=(1.0/(tiempos[i+2]-tiempos[i])*1000);
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

            if (pico>0){
                amplitud[posicion]= ((pico-pico2)/2)/100;//porque pico2 es negativo
            }else{
                amplitud[posicion]= ((-pico+pico2)/2)/100;//porque pico2 es positivo
            }

            frecuencia[posicion]=(1.0/(tiempos[jf]-tiempos[j])*1000);

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
