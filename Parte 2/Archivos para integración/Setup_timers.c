#include "defines_y_variables.h"

//TIMER 0: Sirve para contar hasta 1 mS
void setup_timer0(){	
	
	cli();
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00); // Usando un prescalado de 64, genero 1 pulso cada 8microsegundos
	
	OCR0A = 125;	//Así que tengo que contar 125 veces 8 microsegundos para llegar a 1 milisegundo
	
	TIMSK0  |= (1 << OCIE0A);	//Habilito la interrupcion por coincidencia en OCR0A
	
	sei();	
	
}


//TIMER 1: lo usamos para dos PWMs (Conectados en PB5 y PB6)
void setup_timer1(){   

	cli();
	// Prescalado de 8 --> CS5(2:0) = 010 --> 1 tick cada 125 nS
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


//TIMER3
void setup_timer3(){	//Cuenta 50mS. Es el que usamos en el antirrebotes
	
	cli();
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	
	//Limpio bandera antigua y habilito interrupción del compare
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
	
	sei();
}


//TIMER 4: Cuenta 5 segundos.
void setup_timer4(){
	//cuenta 5 segundos
	
	//modo CTC y prescalado de 1024
	cli();
	
	TCCR4B |= ((1 << WGM42) | (1 << CS42) | (1 << CS40));
	
	
	OCR4A = 39060;
	TIMSK4 |= (1 << OCIE4A);

	
	sei();
	
}


// TIMER 5
void setup_timer5(){  //usado para el PWM de 3 de los motores (conectados en el puerto L);
	
	cli();
	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	//Los bits de los COM los regulo si quiero encender/apagar un motor.
	
	TCCR5A |= ((1<<WGM51)); //  | (1<<COM5A1) | (1<<COM5B1) | (1<<COM5C1);
	TCCR5B |= ((1<<WGM53) | (1<<CS51));
	
	//  TOP = ICR5 = 1000.
	ICR5 = 1000;
	
	// OCR5(A,B,C) son para el ciclo de trabajo de cada PWM de salida
	OCR5A = d_PWM;
	OCR5B = d_PWM;
	OCR5C = d_PWM;
	
	
	// Y, por último, habilito las interrupciones por coincidencia en OCR5 A, B y C
	TIMSK5 |= ((1 << OCIE5A) | (1 << OCIE5B) | (1 << OCIE5C));
	
	sei();
}

void setup_timers(){
	
	setup_timer0();
	setup_timer1();
	setup_timer3();
	setup_timer4();
	setup_timer5();
}
