//Prueba mover motores.

//Con esta prueba, comprobamos si controlamos bien el motor 1 y si salta la interrupcion correctamente

//El funcionamiento es que vaya de un lado a otro sin parar

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SW1 PD0

#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD

#define M1_EN PB5
#define M1_DI PD4

#define d_PWM 250

//Variables del timer 3

#define REBOTE_MS 50       //Número de milisegundos
#define TICKS_PER_MS 1000  // para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.

//VARIABLES DEL ANTIRREBOTES

volatile uint8_t int_bloqueado[4];  //Bandera para comprobar si la interrupción INT está bloqueada o no. "1" bloqueada y "0" no.
volatile uint8_t bounce_int; 		//Variable auxiliar global para avisar a la interrupción del timer sobre qué interrupción de tipo INT reactivar.




//SETUPs

void setup(){
	
	//Los Pines de direccion y enable son salidas
	
	DDRB |=  (1 << M1_EN);
	DDRD |= (1 << M1_DI);
	
	DDRD &= ~(1<< SW1);	//Y el SW6 es una entrada.
	
	//Ahora, habilito INT0 (Ahí está el SW1):
	
	EIMSK |= (1 << INT0);  //Habilito la interrupcion INT0
	
	//Y hago que salte por flanco de bajada
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);

	sei();	//Habilito las interrupciones externas.
	
}

void setup_timer1(){   //lo usamos para el PWM que mueve los motores 1 y 5

	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR1A |= (1<<WGM11);
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
	
	//Limpio bandera antigua y habilito interrupción del compare
	//TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
}


//Apagar y mover motores

void apagar_motor(){
	
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
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
		// 3) La marco como “no bloqueada”. //Esto solo sirve de cara a tener una flag interna.
		int_bloqueado[bounce_int] = 0;
		break;
	}
}

ISR(INT0_vect){	//INT para el SW1
	
	antirrebotes(1);
	PORT_M1_DI ^= (1 << M1_DI);
	

}

int main (void){
	
	
	setup();
	setup_timer1();
	setup_timer3();


	while(1){

	}
	
}



