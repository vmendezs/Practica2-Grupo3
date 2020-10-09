#ifndef FUNCIONES_H
#define FUNCIONES_H


#define FIR1 1
#define FIR2 2
#define FIR3 3
#define FIR_OPT 4
#define IIR 5

/*INICIAN FUNCIONES DE EJEMPLO FILTROS FIR*/

long filtrarFIR1(int in);
long filtrarFIR2(int in);
long filtrarFIR3(int in);
long filtrarFIR_Optimizado(int in);

/*Parametros del filtro IIR*/

typedef struct coef_iir_2_ord {// este va a ser el nombre de la estructura
  float num[3]; // ponga aquí su numerador
  float den[3]; // ponga aquí su denominador
  float w[3];
} coef_iir_2_ord;// este va a ser el nombre del typedef

void inicializar_iir_2_ord(float*num, float*den, float*w, coef_iir_2_ord* ir);

float filtrarIIR(float in,coef_iir_2_ord* ir);
#endif


/***
 *                 _      <-. (`-')_ 
 *       <-.      (_)        \( OO) )
 *    (`-')-----. ,-(`-') ,--./ ,--/ 
 *    (OO|(_\---' | ( OO) |   \ |  | 
 *     / |  '--.  |  |  ) |  . '|  |)
 *     \_)  .--' (|  |_/  |  |\    | 
 *      `|  |_)   |  |'-> |  | \   | 
 *       `--'     `--'    `--'  `--' 
 */
