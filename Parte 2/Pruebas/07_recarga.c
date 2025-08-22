/*FUNCIÓN "GLOBAL" RECARGA
Se ocupa de bajar M1, esperar 1 segundo para que caiga la bola, y volver a subir M1 con la bola cargada*/


/*Se ha desarrollado de tal manera que, con llamar 1 vez a recarga, esta se ocupa de hacer todo el movimiento sin bloquear. 
Para hacerlo, activa ciertas banderas a medida que va realizando sus fases del 
movimiento. Se va explicando en el código*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SW1 PD0
#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD
#define M1_EN PB5
#define M1_DI PD4
#define UP 1
#define DOWN 0
#define ESPERA__MS_RECARGA 1000
#define d_PWM 900
#define REBOTE_MS 50UL     
#define TICKS_PER_MS 100UL 

//Variables globales:
volatile uint8_t bounce_int;
volatile uint8_t recargando = 0;

volatile uint8_t espera_recarga;
volatile uint16_t cont_espera_recarga;
volatile uint8_t recarga_terminada;

volatile uint8_t dir_m1;
volatile uint8_t pos_m1;

void setup(){
	
	bounce_int = 0;
	recargando = 0;
	espera_recarga= 0;
	cont_espera_recarga = 0;
	recarga_terminada = 0;
	dir_m1 = 0;
	pos_m1 = 1;
	
	cli();
	
	DDRB |= (1 << M1_EN);
	DDRD |= (1 << M1_DI);
	
	DDRD &= ~(1<<SW1);
	PORTD |= (1 << SW1);
	
	EIMSK |= (1 << INT0);
	EICRA |= (1 << ISC01);
	EICRA &= ~(1 << ISC00);
	
	DDRL |= (1 << PL6);
	
	
	sei();
	
}

void setup_timer0(){	//Sirve para contar hasta 1 mS
		
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);	
	OCR0A = 124;	
	TIMSK0  |= (1 << OCIE0A);	
	
}

void setup_timer1(){   //lo usamos para dos PWMs (Conectados en PB5 y PB6)

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
	
	
}

void setup_timer3(){	//Cuenta 50mS. Es el que usamos en el antirrebotes
	
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  
	OCR3A  = 40000; // REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
	
	
}

void setup_timer4(){ //cuenta 5 segundos
	
	
	TCCR4B |= ((1 << WGM42) | (1 << CS42) | (1 << CS40));
	OCR4A = 39060;
	//modo CTC y prescalado de 1024

	TIMSK4 |= (1 << OCIE4A);
	
}

void setup_timers(){
	
	setup_timer0();
	setup_timer1();
	setup_timer3();
	setup_timer4();
	
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
			//EIFR  |=  (1 << INT1);
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
				EIMSK |= (1 << INT1);	
			break;
			
			
			case 6:  
				PCMSK0 |= (1<<PCINT7);		
			break;

			default:
			break;
		}
	
	
}


void apagar_motor(uint8_t nmotor){

	
	switch(nmotor){
		
		case 1:
			TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
		break;

		default:
		break;
		
	}
}

void mover_motor(uint8_t nmotor, uint8_t direccion){
	

	apagar_motor(nmotor);
	
	switch(nmotor){
		
		case 1:
		
			if (direccion){
				
				antirrebotes(1);
				PORT_M1_DI |= (1 << M1_DI);
				TCCR1A |= ((1 << COM1A1));

				dir_m1 = 1; //Si estaba subiendo, me guardo que está arriba.
				
			}
			
			else{
				
				antirrebotes(1);
				TCCR1A |= ((1 << COM1A1));
				PORT_M1_DI &= ~(1 << M1_DI);
				
				dir_m1 = 0;  //Y si estaba bajando, está abajo.
				
			}
			break;

		
		default:
		break;
		
	}
}

void recarga(){
	
	recarga_terminada = 0;
	
	
	if(pos_m1 == 1 && recargando == 0){ //Se cumple la 1º vez que llamo a esta funcion
		
		recargando = 1;	
		apagar_motor(1)	;
		mover_motor(1,DOWN);
	}
	
	if(pos_m1 == 0 && recargando == 1){  // Esto es cuando haya pasado 1s abajo
		
		apagar_motor(1);
		mover_motor(1,UP);
	}
	
}

void milisegundo_parte2(){
	
	if(espera_recarga == 1){ //Si estoy esperando abajo, 
		
		cont_espera_recarga ++; //cuento 1ms mas
		
		if(cont_espera_recarga > ESPERA__MS_RECARGA){ //Y cuando he llegado a la cuenta, entonces continúo:
			
			cont_espera_recarga = 0;
			espera_recarga = 0;
			
			//Vuelvo a llamar a recarga. Ahora, recargando = 1 y pos_m1 = 0.
			recarga();	
		}
	}
	
}


void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	//apagar_motor(1);
	antirrebotes(1);	//Primero, filtro el rebote

		if (dir_m1){		//Si estaba subiendo, está arriba
			
			pos_m1 = 1;
			
			if(recargando == 1){	//Si ha subido haciendo la recarga, ya ha terminado de recargar.
				
				recarga_terminada = 1;
				recargando = 0;
				
			}

		}
		
		else{  // Y, si dir_m1 es 0, estaba bajando:
			
			pos_m1 = 0;
			
			if(recargando == 1){	//Si ha bajado haciendo la recarga,
				
				espera_recarga = 1; //Activo la espera, porque después tendrá que volver arriba.
				
			}

		}
		
	}
	


ISR(INT0_vect){
	
	antirrebotes(1);
	//apagar_motor(1);
	SW1_bajada();
	
}

ISR(TIMER0_COMPA_vect){	//Se ejecuta cada milisegundo
	
	milisegundo_parte2();
	
}

ISR(TIMER1_COMPA_vect){}
	
ISR(TIMER1_COMPB_vect){}
	
ISR(TIMER3_COMPA_vect){	//El timer3 interrupmirá cada 50mS
	
	cincuenta_ms();
	
}

ISR(TIMER4_COMPA_vect){
	
	PORTL ^= (1<<PL6);
		
}


int main (void){
	
	setup_timers();
	setup();
	
	recarga();
	
	//mover_motor(1,UP);
	sei();
	
	while(1){

			
		}
	
	
}
