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


int main(void){
	
	
	mover_motor(1,UP_M1);
	
}
