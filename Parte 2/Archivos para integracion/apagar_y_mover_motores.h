#include "setup_timers.h"

// FUNCIONES DE MOVER Y APAGAR MOTORES

//VARIABLES PARTE 2:
volatile int dir_m1 = 0;
volatile int dir_m5 = 0;




void apagar_motor(int nmotor){

	// Para apagar los motores, desactivo el modo PWM del timer que controla cada motor. 
	// Esto significa que por ese PIN, vaya un '0', es decir, el motor está apagado.
	
	switch(nmotor){
		
		case 1:
		
		TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));

		
		break;
		
		case 2:
		
		TCCR5A &= ~((1 << COM5A1) | (1<<COM5A0));

		
		break;
		
		case 3:
		
		TCCR5A &= ~((1 << COM5B1) | (1<<COM5B0));

		
		break;
		
		case 4:
		
		TCCR5A &= ~((1 << COM5C1) | (1<<COM5C0));

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

	//Y para encenderlo, es el complementario, activo el PWM en modo invertido del PWM que regula cada motor.
	
	switch(nmotor){
		
		case 1:
		
		TCCR1A |= ((1 << COM1A1)); //| (1 << COM1A0)); Depende de si lo quiero en modo invertido o no invertido
		
		if (direccion){
			
			PORT_M1_DI |= (1 << M1_DI);
			dir_m1 = 1;
			
		}
		
		else{
			
			PORT_M1_DI &= ~(1 << M1_DI);
			dir_m1 = 0;
		}
		
		
		break;
		
		case 2:
		
		
		TCCR5A |= ((1 << COM5A1)); // | (1<<COM5A0));
		
		if (direccion){
			
			PORT_M2 |= (1<<M2_DI);
			
		}
		
		else{
			
			PORT_M2 &= ~(1<<M2_DI);
		}
		

		
		break;
		
		case 3:
		
		TCCR5A |= ((1 << COM5B1) ); // | (1<<COM5B0));
		
		if (direccion){
			
			PORT_M3 |= (1<<M3_DI);
			
		}
		
		else{
			

			PORT_M3 &= ~(1<<M3_DI);
		}
		

		
		break;
		
		case 4:

		TCCR5A  |= (1 << COM5C1); // | (1<<COM5C0);
		
		if (direccion){

			PORT_M4 |= (1<<M4_DI);
			
		}
		
		else{
			
			PORT_M4 &= ~(1<<M4_DI);
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
