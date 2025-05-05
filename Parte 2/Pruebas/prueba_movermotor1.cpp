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

int fin = 0;
 
 ISR(INT0_vect){	//INT para el SW1
	
	fin = 1;
}

void setup(){	
	
	//Los Pines de direccion y enable son salidas
	PORT_M1_EN |= (1 << M1_EN);
	PORT_M1_DI |= (1 << M1_DI);		
	
	EIMSK |= (1 << INT0);  //Habilito las interrupciones INT0 e INT1
	EICRA |= (1 << ISC01);  //Y hago que salte por flanco de bajada
		
	
	}

void setup_timer1(){   //lo usamos para dos PWMs (Conectados en PB5 y PB6)

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

void apagar_motor(){
	
	TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
}
 
void mover_motor1(){
	
	apagar_motor();
	
	while(fin != 1){  	//La 1ª vez, fin=0, así que entra al bucle
		
		TCCR1A |= (1 << COM1A1);// | (1 << COM1A0));
		PORT_M1_DI |= (1 << M1_DI);
	 }
	 
	 fin = 0;
	 apagar_motor();
	 
	 while(fin!=1){
		 
		TCCR1A |= (1 << COM1A1);// | (1 << COM1A0));
		PORT_M1_DI  &= ~(1 << M1_DI);
	}
	
	fin = 0;
 }
 

 void main (void){
	
	setup();
	setup_timer1();

	while(1){
		mover_motor1();
	}
	
 }
