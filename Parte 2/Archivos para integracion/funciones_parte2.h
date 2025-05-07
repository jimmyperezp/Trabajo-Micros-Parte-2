//FUNCIONES PARTE 2

#include "antirrebotes.h"
#include "apagar_y_mover_motores.h"


volatile int pos_m1= 0;
volatile int pos_m5= 0;

volatile int juego = 0;
volatile int disparo = 0;
volatile int ultimo_disparo= 0;

volatile int retornando = 0;
volatile int recargando = 0;
volatile int flag_disparo = 0;
volatile int espera_recarga = 0;
volatile int cont_espera_recarga = 0;
volatile int cont_5seg = 0;

volatile uint8_t current_state_SW6;
volatile uint8_t last_state_SW6;

void setup_parte2(){
	
	
	cli();
	
	//Inicializo las variables que manejo en esta parte:
	
	juego = 0;
	disparo = 0;
	ultimo_disparo = 0;
	retornando = 0;
	flag_disparo = 0;
	cont_5seg = 0;
	recargando = 0;
	espera_recarga = 0;
	cont_espera_recarga = 0;
	current_state_SW6 = 0;
	last_state_SW6 = 1;
	

	//Direccionamiento y declaración de los pines y puertos
	
	DDRB |= ((1 << M1_EN) | (1 << M5_EN));	//Enables de los motores --> Salidas
	DDRD |= ((1 << M1_DI) | (1 << M5_DI));	//Direcciones motores --> Salidas
	DDRD &= ~((1 << SW1) | (1 << SW5));	//Interruptores 1,5 --> Entradas
	
	DDRB &= ~(1<<SW6);  //SW6 como entrada
	
	//Habilito las interrupciones de todos los SW:
	
	EIMSK |= ((1 << INT1) | (1 << INT0));  //Habilito INT0 e INT1
	EICRA |= ((1 << ISC01)| (1 << ISC11));  //Y hago que ambas salten por flanco de bajada
	
	PCICR |= (1 << PCIE0);  //Habilito el grupo de PCINTS de la 0 a la 7 (SW6 está en PCINT7)
	PCMSK0 |= (1 << PCINT7); // Y habilito sólo la que me interesa (asociada a SW6)
	
	sei();

	//En el comienzo, M1 arriba y M5 abajo:
	mover_motor(1,UP_M1);
	mover_motor(5,DOWN_M5);
	
	while ( (pos_m1 != 1) || (pos_m5 != 0)){
		//Mientras alguna de las dos no haya llegado, me quedo en el bucle
	}
	
	//Salgo del bloqueo cuando ambos motores están en la posición inicial
}

void recarga(){
	
	//La posición "default" es pos_m1 = 1;
	
	if(pos_m1 == 1 && recargando == 0){
		
		recargando = 1;
		mover_motor(1,DOWN_M1);
	}
	
	if(pos_m1 == 0 && recargando == 1){
		
		mover_motor(1,UP_M1);
	}
	
}

void retorno(){
	
	//Suponemos que el motor siempre parte desde una posición baja:
	
	if((pos_m5 == 0) & (retornando == 0)){
		
	//	retornando = 1;
		mover_motor(5,UP_M5);
		
	}
	
	
	if( pos_m5 == 1 && retornando == 1){
		
		mover_motor(5,DOWN_M5);
		retornando = 0;
	}
	
	
	
}


void milisegundo_parte2(){
	
	if(espera_recarga == 1){
		
		cont_espera_recarga ++;
		
		if(cont_espera_recarga > 1000){
			
			cont_espera_recarga = 0;
			espera_recarga = 0;
		
			recarga();
		}
	}
	
}

void cincoSeg_parte2(){
	
	if(juego == 1){
		
		cont_5seg ++;
		
		if(flag_disparo == 1){
			
			flag_disparo = 0; //Ya se ha recargado ese disparo
			retorno();
			
		}
		
		if(cont_5seg > 5){
			
			ultimo_disparo = 1;
		}
		
	}
}




//Interrupciones por flancos de bajada en los fines de carrera

void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	antirrebotes(1);	//Primero, filtro el rebote
	apagar_motor(1);	//Y apago el motor.
	
	if (dir_m1){		//Si estaba subiendo, está arriba
		
		pos_m1 = 1;
		
		if(recargando == 1){
			
			recargando = 0;
			
		}

	}

	else{
		
		pos_m1 = 0;
		
		if(recargando == 1){
			
			espera_recarga = 1; //Es un enable para esperar 1 segundo a que baje la bola
			
		}

	}
	
	
	
}


void SW5_bajada(){		//Salta en cada flanco de bajada de SW5
	
	antirrebotes(5);
	apagar_motor(5);
	
	if(dir_m5){
		
		pos_m5 = 1;
		
		if(retornando == 1){
			
			
			retorno();
			
		}
	}
	
	else{
		
		pos_m5 = 0;
		

		
	}
	
	
}


void SW6_flanco(){  //Esta salta en cada flanco de SW6
	
	antirrebotes(6);
	current_state_SW6 = ((PINB & (1 << SW6))>> SW6);
	


	if ((last_state_SW6) && (!current_state_SW6)){  //Flanco de bajada
		
		if(juego == 0){
			
			juego = 1;
			
		}
		
		else{
			
			disparo = 1;
			flag_disparo = 1; //Me sirve para hacer el retorno
			
			if(ultimo_disparo == 1){
				
				juego = 0;
			}
			
		}
		
		
	}
	
	last_state_SW6 = current_state_SW6;
}




//INTERRUPCIONES
ISR(INT0_vect){			//PCINT para el SW1
	
	SW1_bajada();
}

ISR(INT1_vect){			//PCINT para el SW5
	
	SW5_bajada();
}

ISR(PCINT0_vect){
	
	SW6_flanco();
}

ISR(TIMER0_COMPA_vect){
	
	milisegundo_parte2();
	
}

ISR(TIMER4_COMPA_vect){
	
	cincoSeg_parte2();
	
}
