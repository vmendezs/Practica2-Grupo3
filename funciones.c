#include "funciones.h"

//#define BL 8 EJEMPLO
//const char B[BL] = {
//      0,   16,   48,   64,   64,   48,   16,    0
//};

///////////COEFICIENTES PARA FILTRO FIR//////////////////
//FIR ORDEN 7
#define BL 8 //Longitud del vector de coeficientes
const int8_t B[8] = {
      2,   14,   42,   70,   70,   42,   14,    2
};

// //FIR ORDEN 11
// #define BL 12
// const int8_T B[12] = {
//       0,    0,    2,   14,   42,   70,   70,   42,   14,    2,    0,    0
// };

// //FIR ORDEN 17
// #define BL 18
// const int8_T B[18] = {
//       0,    0,    0,    0,    0,    2,   14,   42,   72,   72,   42,   14,
//       2,    0,    0,    0,    0,    0
// };

// //FIR ORDEN 30
// #define BL 31
// const int8_T B[31] = {
//       0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
//       6,   26,   58,   76,   58,   26,    6,    0,    0,    0,    0,    0,
//       0,    0,    0,    0,    0,    0,    0
// };

// ///////////COEFICIENTES PARA FILTRO IIR EN EL MAIN//////////////////


/*Inicio de variables volatiles para pasar datos entre funciones*/
volatile int x[BL];
volatile int k=0;

/*Funcion del filtro FIR*/
long filtrarFIR(int in)
{
  int i = 0;
  x[k] = (int)in; //Señal de entrada asignada al vector
  long y = 0; //Valor de salida del filtro

  //Para verificar si el vector es par
  if((BL%2) == 0)
  {
    for (i = 0; i < (BL/2); i++) //Se recorre el vector de coeficientes hasta la mitad
    {
      y += ((long)B[i]) * ((long)x[i] + (long)x[BL-i]); // verifique que para su filtro no exista overflow.
    }
  }
  else //Como el vector es impar entra aquí
  {
    for (i = 0; i < (BL/2) ; i++) //Se recorre el vector de coeficientes hasta la mitad + 1
    {
      y += ((long)B[i]) * ((long)x[i] + (long)x[BL-i]); // verifique que para su filtro no exista overflow.
    }
      y += ((long)B[(BL/2)+1]) * ((long)x[(BL/2)+1]); // Se obtiene el valor de la mitad del vector que esta solito :c
  }
  return y ; //si no es multiplo de 2^n divida por el factor de normalización adecuado a su filtro.
}

/*Filtro de Pacho*/
// long filtrarFIR1(int in)
// {
//   int i = 1;
//   x[k] = (int)in;
//   long y = 0;
//   for (i = 1; i <= BL; i++) // NOTA, DEBE INICIAR EN 1. EJERCICIO: haga una prueba de escritorio con una respuesta impulso y compruebe...
//   {
//     y += (long)B[i - 1] * (long)x[(i + k) % BL]; // verifique que para su filtro no exista overflow.
//   }

//   k = (k + 1) % BL;
//   return y>>8 ; //si no es multiplo de 2^n divida por el factor de normalización adecuado a su filtro.
// }


/*Funciones del filtro IIR*/

void inicializar_iir_2_ord(float*num, float*den, float*w, coef_iir_2_ord* ir)
{
  for (int i = 0; i < 3; i++) {
    ir->num[i] = num[i];
    ir->den[i] = den[i];
    ir->w[i] = w[i];
  }
}

/*ver la ayuda de matlab de dfilt.df2 para ver la estructura implementada en este filtro*/
float filtrarIIR(float in, coef_iir_2_ord* ir) {
  float y;
  /*Ecuacion implementada:
  w0=a0*x-a1*w1-a2*w2 donde x es la entrada  a denota a los denominadores notese que a0 es siempre 1 por lo que da lo mismo quitarlo
  y =b0*w0+b1*w1+b2*w2
  w2=w1
  w1=w0
  */
  ir->w[0] = (ir->den[0] * in) - (ir->den[1] * ir->w[1]) - (ir->den[2] * ir->w[2]); // OJO QUE EL MENOS YA ESTA EN LA ECUACION ver en  la ayuda de filterDesign en "show filter structure" si esta es o no la ecuación que implementan en ese software
  y = ((ir->num[0] * ir->w[0]) + (ir->num[1] * ir->w[1]) + (ir->num[2] * ir->w[2]));
  ir->w[2] = ir->w[1];//debe ser primero este
  ir->w[1] = ir->w[0];// y luego este para no sobreescribir
  return y;
}


/***
                   _      <-. (`-')_
         <-.      (_)        \( OO) )
      (`-')-----. ,-(`-') ,--./ ,--/
      (OO|(_\---' | ( OO) |   \ |  |
       / |  '--.  |  |  ) |  . '|  |)
       \_)  .--' (|  |_/  |  |\    |
        `|  |_)   |  |'-> |  | \   |
         `--'     `--'    `--'  `--'
*/
