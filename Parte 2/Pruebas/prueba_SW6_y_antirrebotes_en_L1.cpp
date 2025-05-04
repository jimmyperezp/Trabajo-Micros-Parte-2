
//Prueba para ver antirrebotes y pulsacion de SW6


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define PORT_L1 PORTL
#define L1 PL6


//Para el control de la PCINT
volatile uint8_t last_state_SW6;
volatile uint8_t current_state_SW6;


setup(){
	
	//Tengo que inicializar el timer que uso para el antirrebotes
}
	
	

ISR(PCINT16){
	
	sw6_flanco();
}

void SW6_flanco(){  //Esta salta en cada flanco de SW6
	

		
		uint8_t current_state_SW6 = ((PINB & (1 << SW6))>> SW6);  
		
		iniciar_antirrebotes(5);
		
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
	
	

