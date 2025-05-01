//Prueba mover motores.

//Con esta prueba, comprobamos si controlamos bien el motor 1 y si salta la interrupcion correctamente

//El funcionamiento es que vaya de un lado a otro sin parar

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SW1 PD0

#define M1_DI PL1
#define M1_EN PL3


volatile int fin = 0;
 
 ISR(INT0_vect){			//PCINT para el SW1
	
	fin = 1;
}

 void mover_motor1(){
	 
	 while(fin != 1){
		 
		 PORTL |= ((1<<M1_DI) | (1 << M1_EN));
	 }
	 
	 fin = 0;
	 
	 while(fin!=1){
		 
		 PORTL | (1<< M1_EN);
	 }
	 
	 fin = 0;
 }
 
 
 void main (void){
	 
	 
	DDRL |= ((1<<M1_DI) | (1 << M1_EN) );

	while(1){
		mover_motor1();
	}
	

