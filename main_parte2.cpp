//main de la parte 2;


#include "encabezado_parte2.h"
#include "encabezado_general.h"



//INTERRUPCIONES

ISR(INT0_vect){			//PCINT para el SW1
	
	sw1_bajada();
}

ISR(INT1_vect){			//PCINT para el SW5
	
	sw5_bajada();
}

ISR(PCINT16){
	
	sw6_flanco();
}

ISR(TIMER0_COMPA_vect){
	//esta interrupción salta cada 1 mS
	
	1ms();
	
	else()
	
}

//BUCLE 
void loop(){
	
}
//Programa Principal

int main(void){
		
	setup_parte2();
	
	while(1){
		
		loop();
	}
	
	
	
	
}
