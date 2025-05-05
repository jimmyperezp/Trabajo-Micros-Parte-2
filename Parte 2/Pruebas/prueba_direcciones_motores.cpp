
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



void setup_timer1(){   //lo usamos para dos PWMs (Conectados en PB5 y PB6)

	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR1A |= ((1<<COM1A1) | (1<<COM1B1) | (1<<WGM11));
	TCCR1B |= ((1<<WGM13) | (1<<CS11));
	
	//TOP en ICR1 
	ICR1 = 1000;
	
	//OCR1(A y B)
	OCR1A = d_PWM;
	OCR1B = d_PWM;
	
	//Habilito las interrupciones por coincidencia en OCR1A y OCR1B
	TIMSK |= ((1 << OCIE1A) | (1 << OCIE1B) ); 	
	 
	
}
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
	
		
	DDRB |= ((1<<M5_EN)|(1<<M1_EN));
	DDRD |= ((1<<M1_DI) | (1 << M5_DI));
	
	setup_timer1();
	mover_motor(1,UP_M1);
	
}
