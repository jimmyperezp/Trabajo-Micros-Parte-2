//Encabezado "global"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//Variables parte 2:

#define SW1 PD0
#define SW5 PD1
#define SW6 PK0
#define M1_DI PL1
#define M5_DI PL2
#define M1_EN PL3
#define M5_EN PL4

volatile int dir_m1 = 0;
volatile int dir_m5 = 0;




void apagar_motor(int nmotor){
	
	switch(nmotor){
		
		case 1: 
			
			PORTL &= ~(1 << M1_EN);
			
		break;
		
		case 2: 
		
			//PORTX &= ~(1 << M2_EN);
			
		break;
		
		case 3: 

			//PORTX &= ~(1 << M3_EN);
			
		break;
		
		case 4: 
		
			//PORTX &= ~(1 << M4_EN);
			
		break;
		
		case 5: 
		
			PORTL &= ~(1 << M5_EN);
			
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
				
				PORTL |= ((1 << M1_EN) | (1 << M1_DI));
				dir_m1 = 1;
			
			}
			
			else{
				
				PORTL |= (1 << M1_EN); 
				dir_m1 = 0;
			}
			
			
		break;
		
		case 2: 
		
			//PORTX &= ~(1 << M2_EN);
			
		break;
		
		case 3: 

			//PORTX &= ~(1 << M3_EN);
			
		break;
		
		case 4: 
		
			//PORTX &= ~(1 << M4_EN);
			
		break;
		
		case 5:
		
			if (direccion == 1){
				
				PORTL |= ((1 << M5_EN) | (1 << M5_DI));
				dir_m5 = 1;
			
			}
			
			else{
				
				PORTL |= (1 << M1_EN); 
				dir_m5 = 0;
			}
	
		break;
		 
		
		default: 
		break;
		
	}
}



void setup_timer0(){
	
	//Timer 0 para contar 1 mS 
	
	cli();
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);
	
	OCR0A = 125;
	
	sei();
	
	
}