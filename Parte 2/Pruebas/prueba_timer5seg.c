//PRUEBA TIMER 4 (CADA 5 SEGUNDOS)

#include <avr/io.h>
#include <avr/interrupt.h>

#define PORT_L1 PORTL
#define L1 PL6


void setup_timer4(){
	//cuenta 5 segundos
	
	//modo CTC y prescalado de 1024
	cli();
	
	TCCR4B |= ((1 << WGM42) | (1 << CS42) | (1 << CS40));
	
	
	OCR4A = 39060;
	TIMSK4 |= (1 << OCIE4A);

	
	sei();
	
}


ISR(TIMER4_COMPA_vect){

	PORT_L1 ^= (1 <<L1);

}

int main(void){

	DDRL |= ( 1 << L1);
	PORTL |= ( 1 << L1);
	
	setup_timer3();
	
while(1){
	
	

}

}
