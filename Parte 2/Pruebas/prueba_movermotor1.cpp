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



volatile int fin = 0;
 
 ISR(INT0_vect){			//INT para el SW1
	
	fin = 1;
}


void apagar_motor(){
	
PORT_M1_EN &= ~(1 << M1_EN);	
}
 
void mover_motor1(){
	
	apagar_motor();
	
	while(fin != 1){
		
		PORT_M1_EN |= (1 << M1_EN);
		PORT_M1_DI |= (1 << M1_DI);
	 }
	 
	 fin = 0;
	 apagar_motor();
	 
	 while(fin!=1){
		 
		 PORT_M1_EN  |= (1<< M1_EN);
		 PORT_M1_DI  &= ~(1 << M1_DI);
	}
	
	fin = 0;
 }
 
 
 void main (void){
	 
	 
	DDRB |= (1 << M1_EN);
	DDRD |= (1 << M1_DI);

	while(1){
		mover_motor1();
	}
	
 }
 
	

