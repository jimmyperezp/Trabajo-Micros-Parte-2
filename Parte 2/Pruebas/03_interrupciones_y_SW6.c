/*Prueba para comprobar declaración de interrupciones y SW6.
Funcionamiento: Cada vez que se pulse el SW6, cambia el estado del LED*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define PORT_L1 PORTL
#define L1 PL6

#define SW6 PB7
#define SW1 PD0

#define REBOTE_MS 50UL    //Número de milisegundos
#define TICKS_PER_MS 1000UL// para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.


//VARIABLES DEL ANTIRREBOTES

volatile uint8_t int_bloqueado[4];  //Bandera para comprobar si la interrupción INT está bloqueada o no. "1" bloqueada y "0" no.
volatile uint8_t bounce_int; 		//Variable auxiliar global para avisar a la interrupción del timer sobre qué interrupción de tipo INT reactivar.

//Para el control de la PCINT

volatile uint8_t last_state_SW6  ;
volatile uint8_t current_state_SW6 ;

int habilitar_antirrebotes = 0;

volatile uint8_t high = 1;
volatile uint8_t low = 0;

void setup(){
	
	cli();
	
	//Inicializo las variables:
	high = 1;
	low = 0;
	last_state_SW6 = 0;
	current_state_SW6 = 0;
	bounce_int = 0;
	
	
	//Direccionamiento de los puertos
	DDRL |=  (1 << L1);
	DDRD &= ~(1 << SW1);
	PORTD |= (1 << SW1);  //Pongo una resistencia de pull-up al SW1
	DDRB &= ~(1 << SW6);
	PORTB |= (1 << SW6);  //Pongo una resistencia de pull-up al SW6

	
	
	
	PCICR |= (1 << PCIE0);   //Habilito el grupo de PCINTS de la 0 a la 7 (SW6 está en PCINT7)
	PCMSK0 |= (1 << PCINT7); // Y habilito sólo la que me interesa (asociada a SW6)
	
	EIMSK |= (1 << INT0);		//Habilito INT0  --> ASOCIADO A SW1
	EICRA |= (1 << ISC01);		//Y la pongo para que salte en flanco de bajada
	EICRA &= ~(1 << ISC00);

	PORT_L1 |= (1 << L1);		//Comienzo con la luz encendida
	
	sei();
	
}

//TIMER3
void setup_timer3(){
	
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;

	TIMSK3 |= (1 << OCIE3A);
}



void antirrebotes(uint8_t inum){
	
	bounce_int = inum;
	habilitar_antirrebotes = 1;
	
	switch(inum) { //Con este switch, trato de manera distinta las interrupciones por INT y por PCINT
		

		case 6:  // antirrebotes en PCINT7 (pin PB7)

		PCMSK0 &= ~(1<<PCINT7);

		break;

		default:
		
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
			
			
			case 6: //reactivo PCINT7
			
			PCMSK0 |= (1<<PCINT7);
			
			break;
			
			default:
			
			break;
		}
	}
	
}

void SW6_flanco(){  //Esta salta en cada flanco de SW6
	
	antirrebotes(6);
	current_state_SW6 = ((PINB & (1 << SW6))>> SW6);
	


	if ((last_state_SW6) && (!current_state_SW6)) {
		// Flanco de bajada: PB7 pasó de alto a bajo.
		PORT_L1 ^= (1 << L1);
		
	}
	
	last_state_SW6 = current_state_SW6;
}


void SW1_bajada(){
	
	PORT_L1 ^= (1 << L1);
}


ISR(TIMER3_COMPA_vect) {
	
	
	cincuenta_ms();

}

ISR(PCINT0_vect){
	
	
	SW6_flanco();
}

ISR(INT0_vect){
	
	SW1_bajada();
}


//Programa Principal
int main(void){

	setup();
	
	setup_timer3();

	while(1){
	}
	
}




