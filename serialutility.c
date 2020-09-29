#include "serialutility.h"
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "Consumo.h"

void Su_inicie_uart(uint16_t ubrr, Comunicacion *com)
{
    UBRR0L = (uint8_t)(ubrr & 0xFF);                // Configuracion de la tasa a 9600
    UBRR0H = (uint8_t)(ubrr >> 8);                  //
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);          // se enciende la recepcion y transmision
    com->estado=ESTADO0;                            //inicializacion de estados
}

char Su_Hubo_Tecla_Serial(Comunicacion *tec_enable){
    if(UCSR0A&(1<<7))                               // Condicion de recepcion de serial
    {
        tec_enable->tecla = UDR0;                   // Guarda el valor del serial
        return 1;                                   // Dice que se recibio algo
    }
    else
    {
        return 0;                                   // Dice que no recibio nada
    }
}

void Su_Transmision(int8_t *tempUnidades, int8_t *bandera, int8_t *tempDecenas){
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);          // Se asegura que el serial este activado
	if(*bandera==0)                                 // Condicion para saber que debe enviar
	{
		*bandera= 1;
		UDR0 = *tempDecenas + '0';                  // Envia las unidades
	}
	else if(*bandera==1)
	{
		*bandera=2;
		UDR0 = *tempUnidades + '0';                 // Envia las decenas
	}
	else if (*bandera==2)
	{
		*bandera=3;
		UDR0 = 'C';                                 // Envia los centigrados
	}
	else if(*bandera==3)
	{
		*bandera=0;
		UDR0 = ' ';                                 // Envia un espacio
	}
}

void Su_Atencion_Bajo_Consumo(Comunicacion *com){
    switch (com->estado)
    {
        case ESTADO0:                               // Verifica si ir a Power o Standby
			
            if ((com->tecla)==POWER_MODE){
				
                com->estado=ESTADO1;                // Salida a Power
            }
            else if((com->tecla)==STANDBY_MODE){
                 com->estado=ESTADO2;               // Salida a Standby
            }
            else
            {
                 com->estado=ESTADO0;
                 C_Interrupt_Disable();
            }
                
                       
        break;

        case ESTADO1:                                 // Power mode 
					
            if ((com->tecla)==DOWN){
                C_Power_Down();                       // Realiza Power Down
                com->estado=ESTADO0;
            }
            else if((com->tecla) == SAFE){
                C_Power_Safe();                       // Realiza Power save
                com->estado = ESTADO0;
                }
             else{
				 
                com->estado=ESTADO0;
                C_Interrupt_Disable();
                }         
            
        break;
        
        case ESTADO2: //Standby mode 
            if ((com->tecla)==STANDBY){
                C_Standby();                          // Realiza standby
                com->estado=ESTADO0;
            }
            else if((com->tecla)==EXTENDED_STANDBY){
                C_Extended_Standby();                 // Realiza extended standby
                com->estado=ESTADO0;
                }
            else{
                com->estado=ESTADO0;
                C_Interrupt_Disable();
                }
        break;

        default:
        break;
    }
}
