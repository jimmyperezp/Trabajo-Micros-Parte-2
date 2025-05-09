//Prueba inicializacion

//Básicamente voy a probar que el setup funcione en conjunto.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//CONEXIONES DE LOS FINALES DE CARRERA
#define SW1 PD0
#define SW5 PD1

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
#define UP 1
#define DOWN 0


#define REBOTE_MS 50UL       //Número de milisegundos
#define TICKS_PER_MS 1000UL  // para prescaler 8, 8 MHz ? 1 MHz. Número de entradas en la interrupción por milisegundo.

#define d_PWM 750

#define PORT_L1 PORTL
#define L1 PL6

//VARIABLES DEL ANTIRREBOTES

volatile uint8_t int_bloqueado[4];  //Bandera para comprobar si la interrupción INT está bloqueada o no. "1" bloqueada y "0" no.
volatile uint8_t bounce_int = 0 ;
volatile int habilitar_antirrebotes = 0;
//VARIABLES PARTE 2:
volatile int dir_m1 = 0;
volatile int dir_m5 = 0;

//Posiciones de los motores
volatile int pos_m1 = 0;   // '1' Es que está arriba, y '0' está abajo
volatile int pos_m5 = 0;

volatile int setup_M1_iniciado;
volatile int setup_M5_iniciado;
volatile int setup_M1_terminado;
volatile int setup_M5_terminado;

volatile int comenzar_parpadeo;
volatile int aux;

// setup de los timers:


//TIMER 0:
void setup_timer0(){	//Sirve para contar hasta 1 mS
	
	
	cli();
	
	
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS01  | 1 << CS00);
	
	OCR0A = 125;
	
	TIMSK0  |= (1 << OCIE0A);	//Habilito la interrupcion por coincidencia en OCR0A
	
	sei();
	
	
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

//TIMER3
void setup_timer3(){	//Cuenta 50mS. Es el que usamos en el antirrebotes
	
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	
	//Limpio bandera antigua y habilito interrupción del compare
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
}

void setup_timer4(){
	//cuenta 5 segundos
	
	//modo CTC y prescalado de 1024
	cli();
	
	TCCR4B |= ((1 << WGM42) | (1 << CS42) | (1 << CS40));
	
	
	OCR4A = 39060;
	TIMSK4 |= (1 << OCIE4A);

	
	sei();
	
}
// ANTIRREBOTES:

void antirrebotes(int inum){
	
	habilitar_antirrebotes = 1;
	bounce_int = inum;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		
		
		case 4:  // antirrebotes en PCINT0 (pin PB0) --> ASOCIADO A SW4
		
		
		PCMSK0 &= ~(1<<PCINT0);	// Deshabilitamos esa fuente de interrupción
		

		PCIFR  |= (1<<PCIF0); 	//Limpio bandera
		break;

		
		case 5:  // antirrebotes en PCINT7 (pin PB7) --> ASOCIADO A SW5
		
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


//Apagar y mover los motores

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
		
		TCCR1A |= ((1 << COM1A1)); // | (1 << COM1A0));
		
		if (direccion){
			
			
			PORT_M1_DI |= (1 << M1_DI);
			dir_m1 = 1;
			
		}
		
		else{
			
			PORT_M1_DI &= ~(1 << M1_DI);
			dir_m1 = 0;
		}
		
		
		break;
		
		case 5:
		
		TCCR1A |= ((1 << COM1B1)); // | (1 << COM1B0));
		
		if (direccion){
			
			
			PORT_M5_DI |= (1 << M5_DI);
			dir_m5 = 1;
			
		}
		
		else{
			
			PORT_M5_DI &= ~(1 << M5_DI);
			dir_m5 = 0;
		}
		
		break;
		
		
		default:
		break;
		
	}
}


//Interrupciones por flancos de bajada en los fines de carrera

void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	antirrebotes(1);
	apagar_motor(1);
	
	if(setup_M1_iniciado == 1){		//Compruebo si entro aquí cuando estoy haciendo el setup del motor 1:
		
		setup_M1_iniciado = 0;
		setup_M1_terminado = 1;
		pos_m1 = 1;
	}
	
	
	
}
	


int get_setup_parte2(void){

if((setup_M1_terminado) && (setup_M5_terminado)){
	
	return 1;
}

else{
	
	return 0;
}
}
void iniciar_M1(){
	
	setup_M1_iniciado = 1;
	mover_motor(1,UP);
	
}

void iniciar_M5(){
	
	setup_M5_iniciado = 1;
	mover_motor(5,DOWN);
	
}

void SW5_bajada(){		//Salta en cada flanco de bajada de SW5
	
	antirrebotes(5);
	apagar_motor(5);
	
	if(setup_M5_iniciado == 1){
		
		setup_M5_iniciado = 0;  //Ya habrá terminado, así que pongo a '0' el comienzo, y a '1' el final
		setup_M5_terminado = 1;
		pos_m5=0;
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
//INTERRUPCIONES
ISR(INT0_vect){			//PCINT para el SW1
	
	SW1_bajada();
}

ISR(INT1_vect){			//PCINT para el SW5
	
	SW5_bajada();
}

ISR(TIMER3_COMPA_vect) {
	cincuenta_ms();
}

ISR(TIMER4_COMPA_vect){
	
	if(comenzar_parpadeo ==1){
		
		PORT_L1 ^= (1 << L1);	
	}
	
}

void setup(){
	
	comenzar_parpadeo = 0;
	aux = 0;
	cli();

	DDRB |= ((1 << M1_EN) | (1 << M5_EN) );
	DDRD |= ((1 << M1_DI) | (1 << M5_DI));
	DDRD &= ~((1 << SW1) | (1 << SW5));
	DDRL |= (1 << L1);
	
	EIMSK |= ((1 << INT1) | (1 << INT0));  //Habilito las interrupciones INT0 e INT1
	EICRA |= ((1 << ISC01)| (1 << ISC11));  //Y hago que ambas salten por flanco de bajada

	setup_timer0();
	setup_timer1();
	setup_timer3();
	setup_timer4();
	
	setup_M1_iniciado = 1;
	iniciar_M1();
	setup_M5_iniciado = 1;
	iniciar_M5();
	
	
	sei();

}


int main(void) {
	
	

	
	setup();

	//mover_motor(5,UP_M5);
	//mover_motor(1,DOWN_M1);	
	while(1){
		
		aux = get_setup_parte2();
		
		if(aux == 1){
			comenzar_parpadeo = 1;
		}
		
		
	}
	
}
