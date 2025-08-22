// PRUEBA MOTOR 1:

/*Con esta prueba, comprobamos si controlamos bien el motor 1 y si salta la interrupcion correctamente
El funcionamiento es que vaya de un lado a otro sin parar. Cuando llega al final de un lado, salta la interrupción del 
fin de carrera correspondiente, y cambia de sentido de giro */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define SW1 PD0

#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD

#define M1_EN PB5
#define M1_DI PD4

#define d_PWM 750

//Variables del timer 3
#define REBOTE_MS 50UL       //Número de milisegundos
#define TICKS_PER_MS 1000UL  // para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.

//VARIABLES DEL ANTIRREBOTES
volatile int bounce_int; 		//Variable auxiliar global para avisar a la interrupción del timer sobre qué interrupción de tipo INT reactivar.

//SETUPs
void setup(){
	
	cli(); //Deshabilito interrupciones. 

	bounce_int = 0; //Variable usada para el antirrebotes.
	
	//Los Pines de direccion y enable del motor son salidas
	DDRB |=  (1 << M1_EN);
	DDRD |= (1 << M1_DI);
	
	DDRD &= ~(1<< SW1);	//Y el SW1 es una entrada. (Es el fin de carrera del movimiento del motor M1)
	PORTD |= (1 << SW1);
	
	EIMSK |= (1 << INT0);  //Habilito la interrupcion INT0 (Ahí está el SW1)
	
	//Y hago que salte por flanco de bajada
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);

	//PORT_M1_DI |= (1 << M1_DI);
	sei();	//Habilito las interrupciones externas.
	
}

void setup_timer1(){   //lo usamos para el PWM que mueve los motores 1 y 5

	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR1A |= (1<<WGM11) | (1 << COM1A1);
	TCCR1B |= ((1<<WGM13) | (1<<CS11));
	
	//TOP en ICR1
	ICR1 = 1000;
	
	//OCR1(A y B)
	OCR1A = d_PWM;
	OCR1B = d_PWM;
	
	//Habilito las interrupciones por coincidencia en OCR1A y OCR1B
	TIMSK1 |= ((1 << OCIE1A) | (1 << OCIE1B) );
	
}

void setup_timer3(){	//Es el que uso para el antirrebotes
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	//TCNT3  = 0;	

	TIMSK3 |= (1 << OCIE3A);
}

//Apagar y mover motores
void apagar_motor(){
	
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
}

void encender_motor(){
	
		PORT_M1_DI ^= (1 << M1_DI);
		TCCR1A |= (1 << COM1A1);
}

void antirrebotes(uint8_t inum){
	
	bounce_int = inum;
	
	switch(inum) {
			
		case 0:  
				
		EIMSK &= ~(1 << inum);
		//EIFR  |=  (1 << inum);
		
		break;

		default:
		break;
		
	}	
}

ISR(TIMER3_COMPA_vect) {
	
	// Deshabilito la interrupción de compare.
	// TIMSK3 &= ~(1<<OCIE3A);
	// Esto creo que no hay que hacerlo, porque si no no lo encenderiamos en ningun punto del codigo
	
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
		break;
	}
}

ISR(INT0_vect){	//INT para el SW1
	
	antirrebotes(0);
	apagar_motor();
	_delay_ms(500);
	encender_motor(1);
	
}

int main (void){
		
	setup();
	setup_timer1();
	setup_timer3();

	while(1){
	}	
}