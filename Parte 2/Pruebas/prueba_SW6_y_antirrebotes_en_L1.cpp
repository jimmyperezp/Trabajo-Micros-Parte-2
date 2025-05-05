
//Prueba para ver antirrebotes y pulsacion de SW6


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define PORT_L1 PORTL
#define L1 PL6

#define REBOTE_MS 50       //Número de milisegundos
#define TICKS_PER_MS 1000  // para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.



//Para el control de la PCINT
volatile uint8_t last_state_SW6;
volatile uint8_t current_state_SW6;


void setup_timer3(){
	
		
		TCCR3A = 0;
		TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
		OCR3A  = REBOTE_MS * TICKS_PER_MS;
		TCNT3  = 0;
		
		//Limpio bandera antigua y habilito interrupción del compare
		TIFR3  |= (1 << OCF3A);
		TIMSK3 |= (1 << OCIE3A);
}


void antirrebotes(uint8_t inum){
	
	bounce_int = inum;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
			
			
			case 4:  // antirrebotes en PCINT0 (pin PB0)
			
			
			// Enmascaro PCINT0 para deshabilitar esa fuente de interrupción
			PCMSK0 &= ~(1<<PCINT0);
			
			//Limpio bandera
			PCIFR  |= (1<<PCIF0); 
			break;

			case 5:  // antirrebotes en PCINT7 (pin PB7)
			
			// Habilito el grupo PCINT0..7 (PCIE0)
			PCICR  |= (1<<PCIE0);
			// Enmascaro PCINT7 para deshabilitar esa fuente de interrupción
			PCMSK0 &= ~(1<<PCINT7);
			//Limpio bandera
			PCIFR  |= (1<<PCIF0);
			break;

			default:
		
			EIMSK &= ~(1 << inum);
			EIFR  |=  (1 << inum);
			int_bloqueado[inum] = 1; //bandera para ver qué interrupción INT está bloqueada.
	   break;
		}
	
}


ISR(TIMER3_COMPA_vect) {
	
	// Deshabilito la interrupción de compare.
	TIMSK3 &= ~(1<<OCIE3A);
	
	switch(bounce_int) {
		
		case 4: //reactivo PCINT0
		PCMSK0 |= (1<<PCINT0);
		break;
		
		case 5: //reactivo PCINT7
		PCMSK0 |= (1<<PCINT7);
		break;
		
		default:
		// reactivo solo la INT correspondiente
		EIMSK |= (1<<bounce_int);
		// 3) La marco como “no bloqueada”. //Esto solo sirve de cara a tener una flag interna.
		int_bloqueado[bounce_int] = 0;
		break;
	}
}



	

ISR(PCINT16){
	
	sw6_flanco();
}

void SW6_flanco(){  //Esta salta en cada flanco de SW6
	

		
		uint8_t current_state_SW6 = ((PINB & (1 << SW6))>> SW6);  
		
		antirrebotes(6);
		
		if (last_state_SW6 == 0 && current_state_SW6 == 1) {
			// Flanco de subida: SW6 pasó de bajo a alto.
			
		}
		else if (last_state_SW6 == 1 && current_state_SW6 == 0) {
			// Flanco de bajada: PB7 pasó de alto a bajo.	

			PORT_L1 &~ (1 << L1);
			
		}
		
		last_state_SW6 = current_state_SW6;
	}
		

//Programa Principal
int main(void){
				
	while(1){

	}
	
}	
	
	

