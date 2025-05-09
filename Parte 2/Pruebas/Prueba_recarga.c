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
#define d_PWM 500
#define REBOTE_MS 50UL      
#define TICKS_PER_MS 1000UL 


volatile int int_bloqueado[4];
volatile int bounce_int;
volatile int habilitar_antirrebotes;
volatile int recargando = 0;
volatile int espera_recarga;
volatile int cont_espera_recarga;
volatile int recarga_terminada;    
volatile int dir_m1;
volatile int pos_m1;

void setup_timer0(){	//Sirve para contar hasta 1 mS
	
	cli();		
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);	
	OCR0A = 125;	
	TIMSK0  |= (1 << OCIE0A);	
	sei();
	
	
}

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

void setup_timer3(){	//Cuenta 50mS. Es el que usamos en el antirrebotes
	
	cli();
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
	sei();
	
}

void setup_timers(){
	
	setup_timer0();
	setup_timer1();
	setup_timer3();
	
}

void antirrebotes(int inum){
	
	habilitar_antirrebotes = 1;
	bounce_int = inum;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		
		
		case 4:  // antirrebotes en PCINT0 (pin PB0) --> ASOCIADO A SW4
		
		
		PCMSK0 &= ~(1<<PCINT0);	// Deshabilitamos esa fuente de interrupción
		

		PCIFR  |= (1<<PCIF0); 	//Limpio bandera
		break;

		
		case 6:  // antirrebotes en PCINT7 (pin PB7) --> ASOCIADO A SW6
		
		PCMSK0 &= ~(1<<PCINT7);		// deshabilitamos PCINT7 (la de SW5)
		PCIFR  |= (1<<PCIF0);	    //Limpio bandera
		
		break;

		default:
		
		EIMSK &= ~(1 << inum);	//Deshabilitamos la INT correspondiente al SW
		EIFR  |=  (1 << inum);
		int_bloqueado[inum] = 1; //bandera para ver qué interrupción INT está bloqueada.
		
		
		break;
	}
	
}

void cincuenta_ms(){
	
	static int numero_ciclos = 0;
	
	if(habilitar_antirrebotes == 1){
		
		numero_ciclos++;
	}
	
	if(numero_ciclos>1){
		
		habilitar_antirrebotes = 0;
		numero_ciclos = 0;
		
		switch(bounce_int) {
			
			case 4:
			PCMSK0 |= (1<<PCINT0); //reactivo PCINT0: ASOCIADO A SW4
			break;
			
			case 6:
			PCMSK0 |= (1<<PCINT7);	 //reactivo PCINT7 --> ASOCIADO A SW6
			break;
			
			default:
			
			EIMSK |= (1<<bounce_int);	// reactivo solo la INT correspondiente
			
			// Y la marco como “no bloqueada”. //Esto solo sirve de cara a tener una flag interna.
			int_bloqueado[bounce_int] = 0;
			
			break;
		}
	}
	
}
void apagar_motor(int nmotor){

	
	switch(nmotor){
		
		case 1:
		TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
		break;

		default:
		break;
		
	}
}

void mover_motor(int nmotor, int direccion){
	

	apagar_motor(nmotor);
	
	switch(nmotor){
		
		case 1:
		
		TCCR1A |= ((1 << COM1A1));
		
		if (direccion){
			
			PORT_M1_DI |= (1 << M1_DI);
		}
		
		else{
			PORT_M1_DI &= ~(1 << M1_DI);
		}
		
		break;

		
		default:
		break;
		
	}
}

void recarga(){
	
	//La posición "default" es pos_m1 = 1;
	
	//if(recarga_terminada == 1){
	
	recarga_terminada = 0;
	
	//}
	if(pos_m1 == 1 && recargando == 0){
		
		recargando = 1;		//Me sirve para evaluar qué hacer cuando salte el SW1 la siguiente vez.
		mover_motor(1,DOWN);
	}
	
	if(pos_m1 == 0 && recargando == 1){
		
		mover_motor(1,UP);
	}
	
}
void milisegundo_parte2(){
	
	if(espera_recarga == 1){
		
		cont_espera_recarga ++;
		
		if(cont_espera_recarga > ESPERA__MS_RECARGA){
			
			cont_espera_recarga = 0;
			espera_recarga = 0;
			
			recarga();	//Cuando ha llegado a la cuenta máxima, llamo a retorno, para volver a subir M1
		}
	}
	
}





void setup(){
	
	DDRB |= (1<<M1_EN);
	DDRD |= (1<<M1_DI);
	DDRD &= ~(1<<SW1);
	
	EIMSK |= ((1 << INT1) | (1 << INT0));   
	EICRA |= ((1 << ISC01)| (1 << ISC11));  
	
	bounce_int = 0;
	habilitar_antirrebotes= 0;
	recargando = 0;
	espera_recarga= 0;
	cont_espera_recarga = 0;
	recarga_terminada = 0;
	dir_m1 = 0;
	pos_m1 = 1;
	
	setup_timers();
	
}

void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	antirrebotes(1);	//Primero, filtro el rebote
	apagar_motor(1);	//Y apago el motor.
	
		
		
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
	SW1_bajada();
}

ISR(TIMER0_COMPA_vect){	//Se ejecuta cada milisegundo
	
	milisegundo_parte2();
	
}
ISR(TIMER3_COMPA_vect){	//El timer3 interrupmirá cada 50mS
	
	cincuenta_ms();
	
}


int main (void){
	
	setup();
	
	recarga();
	
}
