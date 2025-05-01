
#define d_PWM 250

//Configuración timer 0. Sirve para contar hasta 1 mS
void setup_timer0(){
	
	//Timer 0 para contar 1 mS 
	
	cli();
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);
	
	OCR0A = 125;
	
	sei();
	
	
}


//El timer 1 lo usamos para dos PWMs (Conectados en PB5 y PB6)
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

//Configuración del timer 5 (lo uso para el PWM de 3 de los motores);
void setup_timer5(){
	
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
	
	
}
