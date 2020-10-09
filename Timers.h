#if !defined(TIEMPO0_H)
#define TIEMPO0_H
#ifdef __cplusplus
extern "C" {
	#endif
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#define limite 155 //limite de conteo del timer 0

	void T_inicie_timer(void); //Se inician los timers

	#ifdef __cplusplus
} // extern "C"
#endif
#endif
