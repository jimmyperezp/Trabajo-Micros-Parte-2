//Encabezado Parte 2: Recarga y Retorno


//VARIABLES PARTE 2:

//Variables de temporización:
volatile int cuenta_fin_juego = 0;		 // El cero es el primer valor válido
const int CUENTA_FIN_JUEGO_MAX = 29999;  //El juego dura 30s --> contando 1mS, necesito que la cuenta sea 30*1000;

volatile int cuenta_retorno = 0;		 // El cero es el primer valor válido
const int CUENTA_RETORNO_MAX = 3999;	 //Si hago el retorno cada 4s, y cuento cada 1mS, necesito contar hasta 3999;


//Control del sistema
volatile int retornar_bolas = 0;
volatile int regargar_bola = 0;
int aux_recarga = 0;
int aux_retorno = 0;


//Posiciones de los motores
volatile int pos_m1 = 0;   // '1' Es que está arriba, y '0' está abajo
volatile int pos_m5 = 0;


//Para el control de la PCINT
volatile uint8_t last_state_SW6;
volatile uint8_t current_state_SW6;


//Funciones utilizadas en la parte 2

setup_parte2(){

	cli();
	
	DDRB |= ((1 << M1_EN) | (1 << M5_EN) );
	DDRD |= ((1 << SW1) | (1 << SW5) | (1 << M1_DI) | (1 << M5_DI));
	
	EIMSK |= ((1 << INT1) | (1 << INT0));  //Habilito las interrupciones INT0 e INT1
	EICRA |= ((1 << ISC01)| (1 << ISC11));  //Y hago que ambas salten por flanco de bajada
	
	PCICR |= (1 << PCIE0);  //Habilito el grupo de PCINTS de la 0 a la 7 (SW6 está en PCINT7)
	PCMSK0 |= (1 << PCINT7); // Y habilito sólo la que me interesa (asociada a SW6)
	
	sei();

	mover_motor(1,UP_M1);
	mover_motor(5,DOWN_M5);
	
	while ( (pos_m1 != 1) || (pos_m5 != 0)){
		//Mientras alguna de las dos no sea 1, me quedo en el bucle
	}
	
	//Salgo del bloqueo cuando ambos motores están en su posición '1' (arriba)
}


void SW1_bajada(){	//Salta en cada flanco de bajada de SW1
	
	iniciar_antirrebotes();
	apagar_motor(1);
	
	if (dir_m1 == 1){
		
		pos_m1 = 1;

	}

	else{
		pos_m1 = 0;

	}
	
	
	
}


void SW5_bajada(){		//Salta en cada flanco de bajada de SW5
	
	iniciar_antirrebotes();
	apagar_motor(5);
	
	if(dir_m5 == 1){
		
		pos_m5 = 1;
	}
	
	else{
		
		pos_m5 = 0;
		// retorno = 0;
	}
	
	
}


void SW6_flanco(){  //Esta salta en cada flanco de SW6
	
	if(juego == 1){		//Si se ha pulsado SW6 y estoy jugando, debo hacer el disparo.
		
		//Tengo que ver si la interrupción ha saltado por flanco de subida o de bajada:
		 
		//Lo primero que hago es guardarme el valor actual del pin de SW6:
		
		uint8_t current_state_SW6 = ((PINB & (1 << SW6))>> SW6);  //Leo todo el PINB, y enmascaro solo el bit asociado al SW6
		//Con el desplazamiento hacia la derecha, "normalizo" el resultado, para trabajar solo con '0' o '1'.
		
		//Después, llamo al antirrebotes para no contabilizar más flancos de ruido
		iniciar_antirrebotes(5);
		
		
		//Ahora, lo comparo con el valor anterior, para ver si es un flanco de subida o de bajada:
		
		if (last_state_SW6 == 0 && current_state_SW6 == 1) {
			// Flanco de subida: SW6 pasó de bajo a alto.
			
		}
		else if (last_state_SW6 == 1 && current_state_SW6 == 0) {
			// Flanco de bajada: PB7 pasó de alto a bajo.	
			
			
			disparo=1; //Si el juego está iniciado y se pulsa SW6, se debe hacer el disparo
			
			if(ultimo_disparo==1){		//Voy a disparar. Es el ultimo disparo? --> entonces el juego se termina
				
				juego = 0;
				ultimo_disparo = 0;
			}
		}
		
		last_state_SW6 = current_state_SW6;
	}
	
	
	else{	//Si se ha pulsado SW6 y el juego no está comenzado, lo comienzo.
		
		juego = 1;
	}
	
}

void milisegundo(){
	
		if(juego ==1){
		
		//Aumento las cuentas que debo llevar
		cuenta_fin_juego++;
		cuenta_retorno++;
		
		//Y después, compruebo si han llegado a los valores que necesitamos:
		
		if(cuenta_fin_juego == CUENTA_FIN_JUEGO_MAX){	//Si ya ha contado 30 segundos desde que comenzó el juego
			
			ultimo_disparo = 1; //Pongo para que el siguiente disparo sea el ultimo;
			cuenta_fin_juego = 0;
		}
		
		if (cuenta_retorno == CUENTA_RETORNO_MAX){
			
			retornar_bolas = 1;			// Activo la señal para hacer el retorno

			
		}
		
	}	
}

void retorno(){
	
	
	if(retornar_bolas == 1){
		
		if((pos_m5 == 0) && (aux_retorno == 0)){
			
			mover_motor(5,UP_M5);
			aux_retorno = 1;
		}

		
		if(pos_m5 == 1 && aux_retorno == 1){		//Cuando haya llegado arriba, lo vuelvo a bajar
					
	
				mover_motor(5,DOWN_M5);
				aux_retorno = 2; 		//Así me aseguro de pedirle que baje solo 1 vez.
			}
		
		
		
		if((pos_m5 == 0) && (aux_retorno ==2)){ //ya ha llegado abajo
			
			retornar_bolas = 0; 
			cuenta_retorno = 0;		// Y reseteo la cuenta para hacer el siguiente retorno
		}
	
	}
}

void recarga(){
	
	if(recargar_bola == 1){
		
		if( (pos_m1 == 1) && (aux_recarga = 0)){
			
			mover_motor(1,DOWN_M1);
			aux_recarga = 1;
		
		}

		if((pos_m1 == 0) && (aux_recarga == 1)){
			
			aux_recarga = 2;  //Así, me aseguro de pedirle que suba una sola vez.
			mover_motor(1,UP_M1);
			
		}
		
		if((pos_m1 == 1) && (aux_recarga == 2)){		//Cuando haya llegado arriba, ya ha terminado la secuencia de recarga. 
			
			recargar_bola = 0;
			aux_recarga = 0;
			
		}
	}
	
}


//BUCLE 
void loop(){
	
	if(juego ==1){	//Todas las acciones que debo hacer son si el juego está comenzado
			
		retorno();
		recarga();
		
		if(esperando_bola ==1) {
			
			esperando_bola = 0;
			recargar_bola = 1;

		}
	}

}
