//Para que funcione, hay que incluir el "setup_timers.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//VARIABLES DEL ANTIRREBOTES

volatile int int_bloqueado[4];  //Bandera para comprobar si la interrupción INT está bloqueada o no. "1" bloqueada y "0" no.
volatile int bounce_int= 0; 		//Variable auxiliar global para avisar a la interrupción del timer sobre qué interrupción de tipo INT reactivar.

volatile int habilitar_antirrebotes = 0;


void antirrebotes(int inum){
	
	
	bounce_int = inum;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		
		
		case 4:  // antirrebotes en PCINT0 (pin PB0) --> ASOCIADO A SW4
		
		
		PCMSK0 &= ~(1<<PCINT0);	// Deshabilitamos esa fuente de interrupción
		

		PCIFR  |= (1<<PCIF0); 	//Limpio bandera
		break;

		
		case 6:  // antirrebotes en PCINT7 (pin PB7) --> ASOCIADO A SW6
		
		PCMSK0 &= ~(1<<PCINT7);		// deshabilitamos PCINT7 (la de SW5)
		PCIFR  |= (1<<PCIF0);	    //Limpio bandera
		
		break;

		default:
		
		EIMSK &= ~(1 << inum);	//Deshabilitamos la INT correspondiente al SW
		EIFR  |=  (1 << inum);
		int_bloqueado[inum] = 1; //bandera para ver qué interrupción INT está bloqueada.
		
		
		break;
	}
	
}

void cincuenta_ms(){
	
	static int numero_ciclos = 0;
	
	if(habilitar_antirrebotes == 1){
		
		numero_ciclos++;
	}
	
	if(numero_ciclos>1){
		
		habilitar_antirrebotes = 0;
		numero_ciclos = 0;
		
		switch(bounce_int) {
			
			case 4:
			PCMSK0 |= (1<<PCINT0); //reactivo PCINT0: ASOCIADO A SW4
			break;
			
			case 6:
			PCMSK0 |= (1<<PCINT7);	 //reactivo PCINT7 --> ASOCIADO A SW6
			break;
			
			default:
			
			EIMSK |= (1<<bounce_int);	// reactivo solo la INT correspondiente
			
			// Y la marco como “no bloqueada”. //Esto solo sirve de cara a tener una flag interna.
			int_bloqueado[bounce_int] = 0;
			
			break;
		}
	}
	
}


ISR(TIMER3_COMPA_vect){	//El timer3 interrupmirá cada 50mS
	
	cincuenta_ms();
	
}


