//Prueba inicializacion

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define SW1 PD0
#define SW5 PD1

#define M1_EN PB5
#define M1_DI PD4

#define M5_EN PB6
#define M5_DI PD5


void apagar_motor(int nmotor){
	
	switch(nmotor){
		
		case 1: 
			
			PORTB &= ~(1 << M1_EN);
			
		break;
				
		case 5: 
		
			PORTB &= ~(1 << M5_EN);
			
		break;
		 
		
		default: 
		break;
		
	}
}


void mover_motor(int nmotor, int direccion){

	
	apagar_motor(nmotor);	//1º Lo apagamos para hacer el cambio de direccion
	
	switch(nmotor){
		
		case 1: 
			
			if (direccion == 1){
				
				PORTB |= (1 << M1_EN);  
				PORTD |= (1 << M1_DI);
				dir_m1 = 1;
			
			}
			
			else{
				
				PORTB |= (1 << M1_EN);
				PORTD &= ~(1 << M1_DI);
				dir_m1 = 0;
			}
			
			
		break;
		

		
		case 5:
		

		
			if (direccion == 1){
				
				PORTB |= (1 << M5_EN);  
				PORTD |= (1 << M5_DI);
				dir_m5 = 1;
			
			}
			
			else{
				
				PORTB |= (1 << M5_EN);
				PORTD &= ~(1 << M5_DI);
				dir_m5 = 0;
			}
	
	
		break;
		 
		
		default: 
		break;
		
	}
}

setup_parte2(){


	cli();
	
	DDRB |= ((1 << M1_EN) | (1 << M5_EN) );
	DDRD |= ((1 << SW1) | (1 << SW5) | (1 << M1_DI) | (1 << M5_DI));

	
	EIMSK |= ((1 << INT1) | (1 << INT0));  //Habilito las interrupciones INT0 e INT1
	EICRA |= ((1 << ISC01)| (1 << ISC11));  //Y hago que ambas salten por flanco de bajada
	
	
	sei();

	mover_motor(1,1);
	mover_motor(5,1);
	
	while ( (pos_m1 != 1) || (pos_m5 != 1)){
		//Mientras alguna de las dos no sea 1, me quedo en el bucle
	}
	
	//Salgo del bloqueo cuando ambos motores están en su posición '1' (arriba)
}

void SW1_bajada(){
		
	apagar_motor(1);
	
	if (dir_m1 == 1){
		
		pos_m1 = 0;

	}

	else{
		pos_m1 = 0;
		//recoger_bolas = 0;
	}
	

}


void SW5_bajada(){
	
	apagar_motor(5);
	
	if(dir_m5 == 1){
		
		pos_m5 = 1;
	}
	
	else{
		
		pos_m5 = 0;
		// retorno = 0;
	}
	
	
}
	
	
ISR(INT0_vect){			//PCINT para el SW1
	
	sw1_bajada();
}

ISR(INT1_vect){			//PCINT para el SW5
	
	sw5_bajada();
}

void main(void){
	
	
	setup_parte2();
	
	while(1){
	}
	
