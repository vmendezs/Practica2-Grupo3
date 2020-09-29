
#if !defined(SERIALU_H)
#define SERIALU_H
#ifdef __cplusplus
extern "C" {
	#endif

	#include <avr/io.h>
	#define BANDERA_TECLAS_ON 0xFFU	// Bandera para comparar si termino
									// transmision

	/*Modos de APAGAR*/
	#define POWER_MODE 'P' 			//Seleccion de Power
	#define DOWN 'D'				// Seleccion Power Down
	#define SAFE 'S'				// Seleccion Power Save

	#define STANDBY_MODE '$'		// Seleccion de standby's
	#define STANDBY 'Y'				// Seleccion de standby
	#define EXTENDED_STANDBY 'E'	// Seleccion de standby extended

	#define	PMASK	0xEF; 			// Mascara de perifericos

	/*Estados Máquina*/
	typedef enum{
		ESTADO0,
		ESTADO1,
		ESTADO2
	}ESTADOS;

	typedef struct Comunicacion Comunicacion; // estructura de serial
	struct Comunicacion{
		ESTADOS estado;
		char tecla;
		char teclado_enable;
	};
	void Su_inicie_uart(uint16_t ubrr, Comunicacion *com);	// Inicializa lo que se necesite
															// Para comunicacion serial.
	void Su_Atencion_Bajo_Consumo(Comunicacion *com);		// Se configura el bajo consumo
	char Su_Hubo_Tecla_Serial(Comunicacion *tec_enable);	// Revisa si se ha recibido y si se
															// recibio guarda el valor
	void Su_Transmision(int8_t *tempUnidades, int8_t *bandera, int8_t *tempDecenas);// Transmite
															// unidades, decenas, centigrados y un espacio
	void Su_Watchdog_Function(void);						// Funcion de watchdog
	void Su_Interrupt_Enable(void);							// Habilita la interrupcion INT0
	void Su_Interrupt_Disable(void);						// Desabilita la interrupcion INT0

	#ifdef __cplusplus
} // extern "C"
#endif
#endif