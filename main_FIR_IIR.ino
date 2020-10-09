#ifdef __cplusplus
extern "C" {
#include "funciones.h"
#include "generador.h"
#include "nuestrostimers.h"
#include "serialutility.h"
#include "Timers.h"
} // extern "C"
#endif

/*Definiciones del precompilador*/
#define MEDIRTIEMPOS 0//Cambiar a 1 y abrir el monitor serial "de la grafica no se aprecia mucho ya que la columna del centro está en microsegundos"
#define MOSTRARENTRADA 1// cambiar a 1 para mostrar la entrada
#define PERIODOSENAL 100// numero de muestras antes de resetear la respuesta paso e impulso, subir en sistemas lentos

/*Variables globales de configuración*/
//Si selecciona FIR# y IMPULSO o PASO use ENTERA8BITS en la funcion inicializarGenerador para suponer  una entrada con punto fijo a la izquierda del bit 8
//Si selecciona IIR  y IMPULSO o PASO  use UNITARIA en la funcion inicializarGenerador
#define SALIDA FIR_OPT // Seleccione FIR1,FIR2,FIR3,FIR_OPT o IIR ver funciones.h
const long interval = 10;           // Intervalo a medir periodico en milisegundos 100Hz=10ms esto asegura una simulación de un tiempo de muestreo


/*****Variables globales del filtro IIR*****/
coef_iir_2_ord ir1, ir2;/*esta estructura debe ser global y se va a pasar por referencia a la funcion que la inicializa*/
Tm_Periodico sondeoADC,sondeoDisplay;

/*Variables especificas del ejemplo:*/
volatile int walk;
volatile int cont;
unsigned long previousMillis;
unsigned long ts1, ts2;


void setup() {
  cont = 0;
  walk = 0;
  previousMillis = 0;
  pinMode(LED_BUILTIN, OUTPUT); //LED_BUILTIN

  Serial.begin(57600);/*velocidad maxima para que no interfiera en tiempos 1000000*/
  while (!Serial) {
    ; // Esperar a que el puerto inicie
  }
  previousMillis = 0;
  //Coeficientes IIR PASA-BANDAS Fs=100 y corte en 25Hz (Entre 22Hz y 28Hz) @ 3dB
  float num_Section1[3] = { 1, -0.000000000000000124672979561347551519246, 1}; //Numerador Seccion 1
  float den_Section1[3] = {1, 0.2381868958, 0.7661067247}; //Denominador Seccion 1
  float num_Section2[3] = { 1, -0.000000000000000124672979561347551519246, 1}; //Numerador Seccion 2
  float den_Section2[3] = {1, -0.2381868958, 0.7661067247}; //Denominador Seccion 2
  float w1[3] = {0.8749856949, 0, 0};// Ganancia Seccion 1
  float w2[3] = {0.8749856949, 0, 0};// Ganancia Seccion 2inicializar_iir_2_ord(num, den, w, &ir);
  inicializarGenerador(&generar, ENTRADA, ENTERA8BITS, PERIODOSENAL,FRECUENCIA); /*si es iir use UNITARIA si es FIR use ENTERA8BITS para usar paso o impulso de 0 a 255*/
  if (MOSTRARENTRADA)
  {
    Serial.print("Entrada");
    Serial.print(" ");
  }
  if (MEDIRTIEMPOS)
  {
    Serial.print("Tiempo_us");
    Serial.print(" ");
  }
  Serial.println("Salida");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    long sal;//para FIR
    float saliir1, saliir2;// para IIR

    if (generar.entrada&(PERSONAL+ACUMULAR)){ // Ejercicio: averiguar como funciona este condicional? ver en generador.h
      /*Descomente uno de estos a la vez para obtener una de estas respuestas: */
      //SenalPersonalizada(&generar,pgm_read_word_near(&ruido[random(0,1000)]));// lee al azar un lugar en la tabla de numeros al azar distribuidos gausiannos "distribución normal"
      //SenalPersonalizada(&generar,5);// prenda ACUMULAR para hallar una respuesta rampa
      SenalPersonalizada(&generar,random(-3,4));// prenda ACUMULAR para hacer una caminata aleatoria apague para numero aleatorio entre -3 y 3
    }
    walk = generadorSenal(&generar);
    if (MOSTRARENTRADA)
    {
      Serial.print(walk);
      Serial.print(" ");
    }
    switch (SALIDA)
    {
      case FIR1:
        ts1 = micros();
        sal = filtrarFIR1(walk);
        ts2 = micros();
        if (MEDIRTIEMPOS) {
          Serial.print(ts2 - ts1); //timepo en microsegundos
          Serial.print(" ");
        }
        Serial.print(sal);
        Serial.print(" ");
        break;
      case FIR2:
        ts1 = micros();
        sal = filtrarFIR2(walk);
        ts2 = micros();
        if (MEDIRTIEMPOS) {
          Serial.print(ts2 - ts1); //timepo en microsegundos
          Serial.print(" ");
        }
        Serial.print(sal);
        Serial.print(" ");
        break;
      case FIR3:
        ts1 = micros();
        sal = filtrarFIR3(walk);
        ts2 = micros();
        if (MEDIRTIEMPOS) {
          Serial.print(ts2 - ts1); //timepo en microsegundos
          Serial.print(" ");
        }
        Serial.print(sal);
        Serial.print(" ");
        break;
      case FIR_OPT:
        ts1 = micros();
        sal = filtrarFIR_Optimizado(walk);
        ts2 = micros();
        if (MEDIRTIEMPOS) {
          Serial.print(ts2 - ts1); //timepo en microsegundos
          Serial.print(" ");
        }
        Serial.print(sal);
        Serial.print(" ");
        break;
      case IIR:
        ts1 = micros();
        saliir1 = filtrarIIR((float)walk, &ir1);
        saliir2 = filtrarIIR(saliir1, &ir2);
        ts2 = micros();
        if (MEDIRTIEMPOS) {
          Serial.print(ts2 - ts1); //timepo en microsegundos
          Serial.print(" ");
        }
        Serial.print(saliir2, 4); //pinta 4 decimales
        Serial.print(" ");
        break;
    }
    Serial.println("");
  }
}
/***
        ___   ___   _  _
       | __| |_ _| | \| |
       | _|   | |  | .` |
       |_|   |___| |_|\_|

*/
