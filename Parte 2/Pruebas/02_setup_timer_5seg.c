//PRUEBA TIMER 4 (CADA 5 SEGUNDOS)
/*Es el que usaré para hacer el retorno de manera periódica.
Esta prueba me sirve también, para confirmar el setup de los timers*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define PORT_L1 PORTL
#define L1 PL6

void setup_timer4(){ //cuenta 5 segundos
	
	
	cli(); // Deshabilito las interrupciones temporalmente
	
	TCCR4B |= ((1 << WGM42) | (1 << CS42) | (1 << CS40));
	OCR4A = 39060;
	//modo CTC y prescalado de 1024

	TIMSK4 |= (1 << OCIE4A); //Habilito interrupciones por coincidencia en la cuenta.

	sei(); // Vuelvo a habilitar las interrupciones.
}


ISR(TIMER4_COMPA_vect){

	PORT_L1 ^= (1 <<L1);

}

int main(void){

	DDRL |= ( 1 << L1);
	PORTL |= ( 1 << L1);
	
	setup_timer4();
	
	while(1){
	}

}
