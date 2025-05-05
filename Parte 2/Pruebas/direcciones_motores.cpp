
//Prueba para ver direcciones de los motores


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//Pines, puerto y direcciones del motor 1
#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD
#define M1_EN PB5
#define M1_DI PD4
#define UP_M1 1
#define DOWN_M1 0

//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5
#define UP_M5 1
#define DOWN_M5 0

//VARIABLES PARTE 2:
volatile int dir_m1 = 0;
volatile int dir_m5 = 0;


void apagar_motor(int nmotor){
	
	switch(nmotor){
		
		case 1: 
			
			TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));

			
		break;
		
		
		case 5: 
			
			TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
			
			
		break;
		 
		
		default: 
		break;
		
	}
}


void mover_motor(int nmotor, int direccion){
	

	apagar_motor(nmotor);	//1º Lo apagamos para hacer el cambio de direccion
	
	switch(nmotor){
		
		case 1: 
			
			if (direccion == 1){
				
				TCCR1A |= ((1 << COM1A1) | (1 << COM1A0));
				PORT_M1_DI |= (1 << M1_DI);
				dir_m1 = 1;
			
			}
			
			else{
				
				TCCR1A |= ((1 << COM1A1) | (1 << COM1A0));
				PORT_M1_DI &= ~(1 << M1_DI);
				dir_m1 = 0;
			}
			
			
	

		break;
		
		case 5:
		
			if (direccion == 1){
				
				TCCR1A |= ((1 << COM1B1) | (1 << COM1B0));
				PORT_M5_DI |= (1 << M5_DI);
				dir_m5 = 1;
			
			}
			
			else{
				
				
				TCCR1A |= ((1 << COM1B1) | (1 << COM1B0));
				PORT_M5_DI &= ~(1 << M5_DI);
				dir_m5 = 0;
			}
	
		break;
		 
		
		default: 
		break;
		
	}
}




int main(void){
	
	
	mover_motor(1,UP_M1);
	
}
