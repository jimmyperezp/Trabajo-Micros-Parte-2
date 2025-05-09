//FUNCIONES PARTE 2

#include "antirrebotes.h"
#include "apagar_y_mover_motores.h"


volatile int pos_m1= 0;					// '1' si M1 está arriba, '0' si abajo
volatile int pos_m5= 1;					// '1' si M5 está arriba, '0' si abajo

volatile int juego = 0;					// Dice si el juego ha comenzado. Actuará como "enable general"
volatile int disparo = 0;
volatile int flag_disparo = 0;		    //Me sirve para hacer un retorno por cada disparo que haya habido
volatile int ultimo_disparo= 0;			//Aviso de que es el último disparo.

volatile int retornando = 0;			//Estas variables me sirven para hacer el recorrido completo (de recarga y retorno)
volatile int recargando = 0;			// sin bloquear.

volatile int espera_recarga = 0;		//Es un enable para esperar 1 segundo con el timer0 hasta que baje la bola por la rampa.
volatile int cont_espera_recarga = 0;	//Lo uso para contar los milisegundos necesarios hasta 1 segundo
volatile int recarga_terminada = 0;		//Esta sirve para avisar de que la recarga ha terminado. 
volatile int cont_5seg = 0;				//Lo uso para contar "bloques" de 5 segundos con el timer4

volatile uint8_t current_state_SW6;		// Estas dos, me sirven para ver si el flanco de interrupción de SW6
volatile uint8_t last_state_SW6;		// que está en una PCINT, es de subida o bajada

volatile int inic_m5=0;
volatile int inic_m1=0; 
volatile int finalizado_m1=0;
volatile int finalizado_m5=0;

void iniciar_M1(){
	
	inic_m1=1;
	mover_motor(1,UP_M1);
	
}

void iniciar_M5(){
	
	inic_m5=1;
	mover_motor(5,DOWN_M5);
	
}

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
	recarga_terminada = 0;
	inic_m5=0;
	inic_m1=0;

	

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
	
	finalizado_m1=0;
	finalizado_m5=0;

	iniciar_M1();
	iniciar_M5();
	
/*
	//En el comienzo, M1 arriba y M5 abajo:
	mover_motor(1,UP_M1);
	mover_motor(5,DOWN_M5);
	
	while ( (pos_m1 != 1) || (pos_m5 != 0)){
		//Mientras alguna de las dos no haya llegado, me quedo en el bucle
	}
	*/
	//Salgo del bloqueo cuando ambos motores están en la posición inicial
}

int fin_setup_M1(){
	return finalizado_m1;
}

int fin_setup_M5(){
	return finalizado_m5;
}

void recarga(){
	
	//La posición "default" es pos_m1 = 1;
	
	//if(recarga_terminada == 1){
		
		recarga_terminada = 0;
		
	//}
	if(pos_m1 == 1 && recargando == 0){
		
		recargando = 1;		//Me sirve para evaluar qué hacer cuando salte el SW1 la siguiente vez.
		mover_motor(1,DOWN_M1);
	}
	
	if(pos_m1 == 0 && recargando == 1){
		
		mover_motor(1,UP_M1);
	}
	
}

void retorno(){
	
	// El motor M5 siempre partirá desde abajo: pos_m5 = 0 al comienzo.
	// Y, siempre que llame a retorno, retornando será =  0
	
	if((pos_m5 == 0) && (retornando == 0)){
		
		retornando = 1;
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
		
		if(cont_espera_recarga > ESPERA__MS_RECARGA){
			
			cont_espera_recarga = 0;
			espera_recarga = 0;
			
			recarga();	//Cuando ha llegado a la cuenta máxima, llamo a retorno, para volver a subir M1
		}
	}
	
}

void cincoSeg_parte2(){

	//Salta con el timer4. lo usaré para contar "bloques de 5 segundos" para terminar el juego (si previamente ha empezado)
	// Y para hacer el retorno cada 5segundos después de cada disparo.
	
	if(juego == 1){
		
		cont_5seg ++;
		
		if(flag_disparo == 1){
			
			flag_disparo = 0; 	//Ya se ha recargado ese disparo
			retorno();		//Llamo al retorno pasados 5 segundos desque se disparó
			
		}
		
		if(cont_5seg > 5){		//Cuando haya contado 30 segundos
			
			
			ultimo_disparo = 1;	//Aviso de que el siguiente será el ultimo disparo
		}
		
	}
}




//Interrupciones por flancos de bajada en los fines de carrera

void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	antirrebotes(1);	//Primero, filtro el rebote
	apagar_motor(1);	//Y apago el motor.
	if(inic_m1==1){
		inic_m1=0;
		finalizado_m1=1;
		pos_m1 = 1;
	}
	
	else{
		if (dir_m1){		//Si estaba subiendo, está arriba
		
			pos_m1 = 1;
		
			if(recargando == 1){	//Si ha subido haciendo la recarga, ya ha terminado de recargar.
			
				recarga_terminada = 1;
				recargando = 0;
			
			}

		}
	
		else{
		
			pos_m1 = 0;
		
			if(recargando == 1){	//Si ha bajado haciendo la recarga,
			
				espera_recarga = 1; //Activo la espera, porque después tendrá que volver arriba.
			
			}

		}
	
	}
	
}


void SW5_bajada(){		//Salta en cada flanco de bajada de SW5
	
	antirrebotes(5);
	apagar_motor(5);
	if(inic_m5==1){
			inic_m5=0;
			finalizado_m5=1;
			pos_m5=0;
		}
		
	else{
		
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
}


void SW6_flanco(){  //Esta salta en cada flanco de SW6
	
	antirrebotes(6);
	current_state_SW6 = ((PINB & (1 << SW6))>> SW6); //Registro el valor de Sw6

	if ((last_state_SW6) && (!current_state_SW6)){  //Flanco de bajada
		
		if(juego == 0){ //Si es la primera pulsación, comienzo el juego
			
			juego = 1;
			
		}
		
		else{
			
			disparo = 1;	//Si el juego ha empezado y pulso SW6, activo el disparo
			flag_disparo = 1; //Me sirve para hacer el retorno
			
			if(ultimo_disparo == 1){

				//Cuando haya disparado si era el ultimo disparo, el juego se termina
				juego = 0;
				cont_5seg = 0;
			}
			
		}
	}

	last_state_SW6 = current_state_SW6; //Y actualizo el valor del SW6 para la siguiente comparación
}


//Aviso de terminado la recarga 

int get_estado_recarga(void){
	
	//Devolverá un '1' si ha terminado, y un '0' si no. 
	return recarga_terminada;
}

int get_ultimo_disparo(void){
	
	return ultimo_disparo;
	
}

int get_disparo(void){
	
	return disparo;
	
}

int get_juego(void){
	
	return juego;
}


//Configuro las interrupciones que regulan el funcionamiento:


ISR(INT0_vect){			//INT para el SW1. Salta en cada flanco de bajada
	
	SW1_bajada();
}

ISR(INT1_vect){			//INT para el SW5. Salta en los flancos de bajada de SW5
	
	SW5_bajada();
}

ISR(PCINT0_vect){	//PCINT de SW6. Saltará en flancos de subida y de bajada.
	
	SW6_flanco();
}

ISR(TIMER0_COMPA_vect){	//Se ejecuta cada milisegundo
	
	milisegundo_parte2();
	
}

ISR(TIMER4_COMPA_vect){	//se ejecuta cada 5 segundos
	
	cincoSeg_parte2();
	
}
