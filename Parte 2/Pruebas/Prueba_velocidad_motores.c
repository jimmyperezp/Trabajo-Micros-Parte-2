//Prueba Velocidad Motores

//Básicamente, es para probar distintas velocidades (ciclos de trabajo) de los PWM

//El timer 1 lo usamos para dos PWMs (Conectados en PB5 y PB6)


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

#define d_PWM 250

void setup_timer1(){

	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR1A |= ((1<<COM1A1) | (1<<COM1B1) | (1<<WGM11));
	TCCR1B |= ((1<<WGM13) | (1<<CS11));
	
	//TOP en ICR1 
	ICR1 = 1000;
	
	//OCR1(A y B)
	OCR1A = d_PWM;
	OCR1B = d_PWM;
	
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

int main (void) {
	
	setup_timer1;
	
	
	mover_motor(1,UP_M1);
	
	
	
	
	}