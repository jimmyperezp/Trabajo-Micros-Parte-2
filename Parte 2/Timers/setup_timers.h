
#define d_PWM 250

//Constantes para el antirrebotes (TIMER 3)

#define REBOTE_MS 50       //Número de milisegundos
#define TICKS_PER_MS 1000  // para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.


//Configuración timer 0. Sirve para contar hasta 1 mS
void setup_timer0(){
	
	//Timer 0 para contar 1 mS 
	
	cli();
	
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);
	
	OCR0A = 125;
	
	TIMSK  |= (1 << OCIE0A);	//Habilito la interrupcion por coincidencia en OCR0A
	
	sei();
	
	
}

//TIMER 1:

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


//TIMER3 

void setup_timer3(){
	
		
		TCCR3A = 0;
		TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
		OCR3A  = REBOTE_MS * TICKS_PER_MS;
		TCNT3  = 0;
		
		//Limpio bandera antigua y habilito interrupción del compare
		TIFR3  |= (1 << OCF3A);
		TIMSK3 |= (1 << OCIE3A);
}

// TIMER 5 
void setup_timer5(){  //usado para el PWM de 3 de los motores (conectados en el puerto L);
	
	// Prescalado de 8 --> CS5(2:0) = 010
	// Modo de operacion 10 --> WGM5(3:0) = 1010
	
	TCCR5A |= (1<<WGM51) | (1<<COM5A1) | (1<<COM5B1) | (1<<COM5C1);
	TCCR5B |= ((1<<WGM53) | (1<<CS51));
	
	//  TOP = ICR5 = 1000.
	ICR5 = 1000;
	
	// OCR5(A,B,C) son para el ciclo de trabajo de cada PWM de salida
	OCR5A = d_PWM5;
	OCR5B = d_PWM5;
	OCR5C = d_PWM5;
	
	
	// Y, por último, habilito las interrupciones por coincidencia en OCR5 A, B y C
	TIMSK5 |= ((1 << OCIE5A) | (1 << OCIE5B) | (1 << OCIE5C));
	
}
