//Encabezado "global"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


//CONEXIONES Y DIRECCIONAMIENTO DE PINES

#define SW1 PD0
#define SW5 PD1
#define SW6 PB7

#define M1_EN PB5
#define M1_DI PD4

#define M2_EN PL3
#define M2_DI PL0
 
#define M3_EN PL4
#define M3_DI PL1

#define M4_EN PL5
#define M4_DI PL2
  
#define M5_EN PB6
#define M5_DI PD5


volatile int dir_m1 = 0;
volatile int dir_m5 = 0;


void apagar_motor(int nmotor){
	
	switch(nmotor){
		
		case 1: 
			
			PORTB &= ~(1 << M1_EN);
			
		break;
		
		case 2: 
		
			PORTL &= ~(1 << M2_EN);
			
		break;
		
		case 3: 

			PORTL &= ~(1 << M3_EN);
			
		break;
		
		case 4: 
		
			PORTL &= ~(1 << M4_EN);
			
		break;
		
		case 5: 
		
			PORTB &= ~(1 << M5_EN);
			
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
				
				PORTB |= (1 << M1_EN);  
				PORTD |= (1 << M1_DI);
				dir_m1 = 1;
			
			}
			
			else{
				
				PORTB |= (1 << M1_EN);
				PORTD &= ~(1 << M1_DI);
				dir_m1 = 0;
			}
			
			
		break;
		
		case 2: 
		
			if (direccion == 1){
				
				PORTL |= ((1<<M2_EN) | (1<<M2_DI));
				
			}
			
			else{
				
				PORTL |= ((1<<M2_EN);
				PORTL &= ~(1<<M2_DI);
			}
			

			
		break;
		
		case 3: 

			if (direccion == 1){
				
				PORTL |= ((1<<M3_EN) | (1<<M3_DI));
				
			}
			
			else{
				
				PORTL |= ((1<<M3_EN);
				PORTL &= ~(1<<M3_DI);
			}
			

			
		break;
		
		case 4: 

			if (direccion == 1){
				
				PORTL |= ((1<<M4_EN) | (1<<M4_DI));
				
			}
			
			else{
				
				PORTL |= ((1<<M4_EN);
				PORTL &= ~(1<<M4_DI);
			}
			
	
			
			
		break;
		
		case 5:
		
			if (direccion == 1){
				
				PORTB |= (1 << M5_EN);  
				PORTD |= (1 << M5_DI);
				dir_m5 = 1;
			
			}
			
			else{
				
				PORTB |= (1 << M5_EN);
				PORTD &= ~(1 << M5_DI);
				dir_m5 = 0;
			}
	
		break;
		 
		
		default: 
		break;
		
	}
}



