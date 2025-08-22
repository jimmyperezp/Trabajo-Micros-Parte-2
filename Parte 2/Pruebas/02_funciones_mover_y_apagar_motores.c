/*
 * Prueba_movermotores.c
 * Created: 09/05/2025 14:39:52
 * Author : jaime
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//MOVER MOTORES
#define SW1 PD0
#define SW5 PD1
#define SW6 PB7

//Pines, puerto y direcciones del motor 1
#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD
#define M1_EN PB5
#define M1_DI PD4

//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5

#define UP 1
#define DOWN 0
#define d_PWM 500


void setup(){

	DDRB |= ((1<<M1_EN)|(1<<M5_EN));
	DDRD |= ((1<<M1_DI) | (1<<M5_DI));
	
	DDRD &= ~((1<<SW1)|(1<<SW5));
	

}

//TIMER 1:
void setup_timer1(){   //lo usamos para dos PWMs (Conectados en PB5 y PB6)

	cli();
	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR1A |= ((1<<WGM11) );// | (1<<COM1A1) | (1<<COM1B1) );
	TCCR1B |= ((1<<WGM13) | (1<<CS11));
	
	//TOP en ICR1
	ICR1 = 1000;
	
	//OCR1(A y B)
	OCR1A = d_PWM;
	OCR1B = d_PWM;
	
	//Habilito las interrupciones por coincidencia en OCR1A y OCR1B
	TIMSK1 |= ((1 << OCIE1A) | (1 << OCIE1B) );
	
	sei();
	
	
}

// FUNCIONES DE MOVER Y APAGAR MOTORES
void apagar_motor(int nmotor){

	// Para apagar los motores, desactivo el modo PWM del timer que controla cada motor.
	// Esto significa que por ese PIN, vaya un '0', es decir, el motor está apagado.
	
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

	//Y para encenderlo, es el complementario, activo el PWM en modo invertido del PWM que regula cada motor.	
	switch(nmotor){
		
		case 1:
			TCCR1A |= ((1 << COM1A1)); //| (1 << COM1A0)); Depende de si lo quiero en modo invertido o no invertido
			if (direccion){
				
				PORT_M1_DI |= (1 << M1_DI);			
				
			}
			
			else{
				
				PORT_M1_DI &= ~(1 << M1_DI);			
			}
			
		break;

		
		case 5:
		
		TCCR1A |= ((1 << COM1B1)); // | (1 << COM1B0));
		
			if (direccion){		
				PORT_M5_DI |= (1 << M5_DI);			
			}
			
			else{				
				PORT_M5_DI &= ~(1 << M5_DI);
			}		
		break;
				
		default:
		break;
		
	}
}

void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	//antirrebotes(1);	//Primero, filtro el rebote
	apagar_motor(1);	//Y apago el motor.
	
	
}

ISR(INT0_vect){  // Habilito la interrupción de SW1
	
	SW1_bajada();
	
}

int main(void){

	setup(); //Configuro los puertos de E/S
	setup_timer1();
	
	//mover_motor(1,UP);
	//mover_motor(1,DOWN);
	mover_motor(5,UP);
	//mover_motor(5,DOWN);
	while(1){
		
	}	
}