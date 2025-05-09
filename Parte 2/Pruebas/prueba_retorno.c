#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SW1 PD0
#define SW5 PD1

//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5

#define UP 1
#define DOWN  0

#define d_PWM 250



#define REBOTE_MS 50       
#define TICKS_PER_MS 1000  


volatile int int_bloqueado[4];  
volatile int bounce_int;


volatile int dir_m5;
volatile int pos_m5;

volatile int retornando;

volatile int habilitar_antirrebotes;

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

void apagar_motor(int nmotor){

	
	switch(nmotor){		
		
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
	TIMSK1 |= ((1 << OCIE1A) | (1 << OCIE1B) );
	
	
}


void setup_timer3(){
	
	
	TCCR3A = 0;
	TCCR3B = (1 << WGM32) | (1 << CS31);  // Modo CTC y prescalado de 8
	OCR3A  = REBOTE_MS * TICKS_PER_MS;
	TCNT3  = 0;
	
	//Limpio bandera antigua y habilito interrupción del compare
	TIFR3  |= (1 << OCF3A);
	TIMSK3 |= (1 << OCIE3A);
}

void retorno(){
	
	// El motor M5 siempre partirá desde abajo: pos_m5 = 0 al comienzo.
	// Y, siempre que llame a retorno, retornando será =  0
	
	if((pos_m5 == 0) && (retornando == 0)){
		
		retornando = 1;
		mover_motor(5,UP);
		
	}
	
	
	if( pos_m5 == 1 && retornando == 1){
		
		mover_motor(5,DOWN);
		retornando = 0;
	}
	
	
	
}

void SW5_bajada(){		//Salta en cada flanco de bajada de SW5. Es análogo a SW1_bajada
	
	antirrebotes(5);
	apagar_motor(5);
	
		if(dir_m5){
			
			pos_m5 = 1;
			
			if(retornando == 1){    //Si ha subido mientras hacia el retorno,
				
				retorno();	//Vuelvo a llamar a retorno, para devolver M5 abajo
				
			}
		}
		
		else{
			
			pos_m5 = 0;
			
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

ISR(INT1_vect){
	
	SW5_bajada();
}

ISR(TIMER3_COMPA_vect){	//El timer3 interrupmirá cada 50mS
	
	cincuenta_ms();
	
}


int main(void)
{
	DDRB |= (1<<M5_EN);
	DDRD |= (1<<M5_DI);
	
	DDRD &= ~(1<<SW5);
	
	pos_m5 = 0;
	dir_m5 = 0;
	bounce_int = 0;
	retornando = 0;
	habilitar_antirrebotes = 0;
	
	retorno();
	
	
    while (1) 
    {
    }
}

