//Encabezado "global"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


//CONEXIONES Y DIRECCIONAMIENTO DE PINES

#define SW1 PD0
#define SW5 PD1
#define SW6 PB7

//Pines, puerto y direcciones del motor 1
#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD
#define M1_EN PB5
#define M1_DI PD4

#define UP_M1 1
#define DOWN_M1 0


//Pines y puerto motor 2
#define PORT_M2 PORTL
#define M2_EN PL3
#define M2_DI PL0

//Pines y puerto motor 3
#define PORT_M3 PORTL
#define M3_EN PL4
#define M3_DI PL1

//Pines y puerto motor 4
#define PORT_M4 PORTL
#define M4_EN PL5
#define M4_DI PL2
 
//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5
#define UP_M5 1
#define DOWN_M5 0



volatile int dir_m1 = 0;
volatile int dir_m5 = 0;

volatile int juego = 0;				//Indica si el juego ha comenzado. Lo iniciamos al pulsar SW6 (parte2)

volatile int disparo = 0;
volatile int ultimo_disparo = 0;


void apagar_motor(int nmotor){
	
	switch(nmotor){
		
		case 1: 
			
			PORT_M1_EN &= ~(1 << M1_EN);
			
		break;
		
		case 2: 
		
			PORT_M2 &= ~(1 << M2_EN);
			
		break;
		
		case 3: 

			PORT_M3 &= ~(1 << M3_EN);
			
		break;
		
		case 4: 
		
			PORT_M4 &= ~(1 << M4_EN);
			
		break;
		
		case 5: 
		
			PORT_M5_EN &= ~(1 << M5_EN);
			
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
				
				PORT_M1_EN |= (1 << M1_EN);  
				PORT_M1_DI |= (1 << M1_DI);
				dir_m1 = 1;
			
			}
			
			else{
				
				PORT_M1_EN |= (1 << M1_EN);
				PORT_M1_DI &= ~(1 << M1_DI);
				dir_m1 = 0;
			}
			
			
		break;
		
		case 2: 
		
			if (direccion == 1){
				
				PORT_M2 |= ((1<<M2_EN) | (1<<M2_DI));
				
			}
			
			else{
				
				PORT_M2 |= ((1<<M2_EN);
				PORT_M2 &= ~(1<<M2_DI);
			}
			

			
		break;
		
		case 3: 

			if (direccion == 1){
				
				PORT_M3 |= ((1<<M3_EN) | (1<<M3_DI));
				
			}
			
			else{
				
				PORT_M3 |= ((1<<M3_EN);
				PORT_M3 &= ~(1<<M3_DI);
			}
			

			
		break;
		
		case 4: 

			if (direccion == 1){
				
				PORT_M4 |= ((1<<M4_EN) | (1<<M4_DI));
				
			}
			
			else{
				
				PORT_M4 |= ((1<<M4_EN);
				PORT_M4 &= ~(1<<M4_DI);
			}
			
	
			
			
		break;
		
		case 5:
		
			if (direccion == 1){
				
				PORT_M5_EN |= (1 << M5_EN);  
				PORT_M5_DI |= (1 << M5_DI);
				dir_m5 = 1;
			
			}
			
			else{
				
				PORT_M5_EN |= (1 << M5_EN);
				PORT_M5_DI &= ~(1 << M5_DI);
				dir_m5 = 0;
			}
	
		break;
		 
		
		default: 
		break;
		
	}
}







