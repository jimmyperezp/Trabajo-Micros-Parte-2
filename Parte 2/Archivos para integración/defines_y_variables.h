//CONEXIONES Y PINES EMPLEADOS:

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


//CONEXIONES Y DIRECCIONAMIENTO DE PINES

#define SW1 PD0
#define SW5 PD1
#define SW6 PB7

//Pines, puerto y direcciones del motor 1
#define PORT_M1_EN PORTB
#define PORT_M1_DI PORTD
#define M1_EN PB5
#define M1_DI PD4

//Pines y puerto motor 2
#define PORT_M2 PORTL
#define M2_EN PL3
#define M2_DI PL0

//Pines y puerto motor 3
#define PORT_M3 PORTL
#define M3_EN PL4
#define M3_DI PL1

//Pines y puerto motor 4
#define PORT_M4 PORTL
#define M4_EN PL5
#define M4_DI PL2

//Pines, puerto y direcciones del motor 5
#define PORT_M5_EN PORTB
#define PORT_M5_DI PORTD
#define M5_EN PB6
#define M5_DI PD5

#define UP 1
#define DOWN  0

//Tiempo de espera para hacer la recarga y darle tiempo a la bola a "subirse" al brazo
#define ESPERA__MS_RECARGA 1000

//Ciclo de trabajo de los PWM
#define d_PWM 500

//Constantes para el antirrebotes (TIMER 3)
#define REBOTE_MS 50UL       //Número de milisegundos
#define TICKS_PER_MS 1000UL // para prescaler 8, 8 MHz → 1 MHz. Número de entradas en la interrupción por milisegundo.


//variables del antirrebotes
extern volatile int int_bloqueado[4];  //Bandera para comprobar si la interrupción INT está bloqueada o no. "1" bloqueada y "0" no.
extern volatile int bounce_int;

//Variables que se compartirán entre varias partes:
extern volatile int habilitar_antirrebotes;
extern volatile int juego;
extern volatile int disparo;
extern volatile int ultimo_disparo;
extern volatile int recarga_terminada;    

//Variables de los motores 1 y 5:

extern volatile int dir_m1;
extern volatile int dir_m5;
extern volatile int pos_m1;
extern volatile int pos_m5;

extern volatile int setup_M5_iniciado ;
extern volatile int setup_M1_iniciado ;
extern volatile int setup_M5_terminado;
extern volatile int setup_M1_terminado;


