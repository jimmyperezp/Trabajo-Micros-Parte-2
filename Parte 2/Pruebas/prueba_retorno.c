#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


#define SW5 PD1

//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5

#define UP 1
#define DOWN 0

#define d_PWM 750



#define REBOTE_MS 50UL
#define TICKS_PER_MS 1000UL


volatile uint8_t bounce_int;

volatile uint8_t dir_m5;
volatile uint8_t pos_m5;
volatile uint8_t retornando;

volatile uint8_t inicio_retorno;

void setup_timer1(){   //lo usamos para dos PWMs (Conectados en PB5 y PB6)

	
	
	TCCR1A |= (1<<WGM11);
	TCCR1B |= ((1<<WGM13) | (1<<CS11));
	
	
	ICR1 = 999;
	
	OCR1A = d_PWM;
	OCR1B = d_PWM;
	
	TIMSK1 |= ((1 << OCIE1A) | (1 << OCIE1B) );
	
	
}

void setup_timer3(){	//Cuenta 50mS. Es el que usamos en el antirrebotes
	
	
	TCCR3A = 0;
	TCCR3B = ((1 << WGM32) | (1 << CS31));
	OCR3A  = 50000; // REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
	
	
}

void setup(){
	
	cli();
	DDRB |= (1<<M5_EN);
	DDRD |= (1<<M5_DI);
	
	DDRD  &= ~(1<<SW5);
	PORTD |= ( 1 << SW5);
	
	EIMSK |= (1 << INT1);   //Habilito la interrupción de INT1
	EICRA |= (1 << ISC11);	// Y la pongo por flanco de bajada.
	EICRA &= ~(1 << ISC10);
	
	pos_m5 = 0;
	dir_m5 = 0;
	bounce_int = 0;
	retornando = 0;

	
	sei();
	
}

void apagar_motor(uint8_t nmotor){

	
	switch(nmotor){
		
		case 5:
		
		TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));
		
		
		break;
		
		
		default:
		break;
		
	}
}

void mover_motor(uint8_t nmotor, uint8_t direccion){
	

	
	
	switch(nmotor){
		
		
		case 5:
		
		if (direccion){
			
			PORT_M5_DI |= (1 << M5_DI);
			TCCR1A |= ((1 << COM1B1));
			
			
			dir_m5 = 1;
			
		}
		
		else{
			
			
			PORT_M5_DI &= ~(1 << M5_DI);
			TCCR1A |= ((1 << COM1B1));
			
			dir_m5 = 0;
		}
		
		break;
		
		
		default:
		break;
		
	}
}

void antirrebotes(uint8_t inum){
	
	bounce_int = inum;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		
		
		case 1: //SW1 está asociado a la INT0
		EIMSK &= ~(1 << INT0);
		//EIFR  |=  (1 << INT0);
		break;
		
		case 2:
		EIMSK &= ~(1 << INT2);	//Deshabilitamos la INT correspondiente al SW
		//EIFR  |=  (1 << INT2);
		break;
		
		case 3:
		EIMSK &= ~(1 << INT3);	//Deshabilitamos la INT correspondiente al SW
		//EIFR  |=  (1 << INT3);
		break;
		
		case 4:  // antirrebotes en PCINT0 (pin PB0) --> ASOCIADO A SW4
		PCMSK0 &= ~(1<<PCINT0);	// Deshabilitamos esa fuente de interrupción
		//PCIFR  |= (1<<PCIF0); 	//Limpio bandera
		break;

		case 5:
		EIMSK &= ~(1 << INT1);	//Deshabilitamos la INT correspondiente al SW
		EIFR  |=  (1 << INT1);
		break;
		
		
		case 6:  // antirrebotes en PCINT7 (pin PB7) --> ASOCIADO A SW6
		PCMSK0 &= ~(1<<PCINT7);		// deshabilitamos PCINT7 (la de SW5)
		//PCIFR  |= (1<<PCIF0);	    //Limpio bandera
		break;

		default:
		break;
	}
	
}


void cincuenta_ms(){
	
	switch(bounce_int) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		
		
		case 1:
		EIMSK |= (1 << INT0);
		break;
		
		case 2:
		EIMSK |= (1 << INT2);

		break;
		
		case 3:
		EIMSK |= (1 << INT3);

		break;
		
		case 4:
		PCMSK0 |= (1<<PCINT0);

		break;

		case 5:
		if(!(PIND & (1<<SW5))){
		EIMSK |= (1 << INT1);
		}
		break;
		
		
		case 6:
		PCMSK0 |= (1<<PCINT7);
		
		break;

		default:
		break;
	}
	
	
}



void retorno(){
	
	if(pos_m5==0){
		
		if(retornando==0) {
			
			antirrebotes(5);
			apagar_motor(5);
			retornando = 1;
			mover_motor(5,UP);
			
			
		}
		else{}
		

	}
	
	else{
		
		if(retornando){
			
			antirrebotes(5);
			//retornando = 0;
			apagar_motor(5);
			mover_motor(5,DOWN);
		}
	}
	
	

	
	
	
}

void SW5_bajada(){		//Salta en cada flanco de bajada de SW5. Es análogo a SW1_bajada
	
	if(inicio_retorno == 1){
		antirrebotes(5);
		inicio_retorno = 0;
		return;
		
	}
	//apagar_motor(5);
	antirrebotes(5);
	if(dir_m5){
		
		pos_m5 = 1;
		
		if(retornando == 1){    //Si ha subido mientras hacia el retorno,
			
			retorno();	//Vuelvo a llamar a retorno, para devolver M5 abajo
			
		}
	}
	
	else{
		
		pos_m5 = 0;
		retornando = 0;
		
	}
}

ISR(INT1_vect){
	
	
	antirrebotes(5);
	SW5_bajada();
	
	
}

ISR(TIMER3_COMPA_vect){	//El timer3 interrupmirá cada 50mS
	
	cincuenta_ms();
	
}

ISR(TIMER1_COMPA_vect){}

ISR(TIMER1_COMPB_vect){}



int main(void)
{
	
	setup();
	setup_timer1();
	setup_timer3();
	inicio_retorno = 1;
	
	retorno();
	//mover_motor(5,DOWN);
	//mover_motor(5,UP);
	
	while (1){
	}
}
