//Encabezado Parte 2: Recarga y Retorno

//Variables parte 2

volatile int ultimo_disparo;
volatile int retorno;

volatile int pos_m1 = 0;   // '1' Es que está arriba, y '0' está abajo
volatile int pos_m5 = 0;


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

void SW6_flanco(){  //Esta salta en cada flanco de SW6
	
	//Aquí pondremos la señal que informa de que se debe efectuar el disparo
}
