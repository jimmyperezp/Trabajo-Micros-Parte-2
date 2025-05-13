//FUNCIÓN setup_parte2 EN ENSAMBLADOR



//HACEMOS VISIBLE LA FUNCIÓN PARA C

.global setup_parte2

 //=== Definiciones manuales de registros ===
.equ DDRD    = 0x0A
.equ DDRB    = 0x04
.equ EICRA   = 0x69
.equ EIMSK   = 0x3D
.equ PCICR   = 0x68
.equ PCMSK0  = 0x6B

//=== Importamos variables ===

.extern juego
.extern disparo
.extern flag_disparo
.extern ultimo_disparo
.extern retornando
.extern recargando
.extern espera_recarga
.extern cont_espera_recarga
.extern recarga_terminada
.extern cont_5seg

.extern current_state_SW6
.extern last_state_SW6
.extern setup_M5_iniciado
.extern setup_M1_iniciado
.extern setup_M5_terminado
.extern setup_M1_terminado

// === setup_parte2 ===
setup_parte2:


cli

//Inicializo todas las variables que manejo en esta parte:
ldi r16, 0
sts juego, r16
sts disparo, r16
sts flag_disparo, r16
sts ultimo_disparo, r16
sts retornando, r16
sts recargando, r16
sts espera_recarga, r16
sts cont_espera_recarga, r16
sts recarga_terminada, r16
sts cont_5seg, r16
sts current_state_SW6, r16

ldi r16, 1
sts last_state_SW6, r16

ldi r16, 0
sts setup_M5_iniciado, r16
sts setup_M1_iniciado, r16
sts setup_M5_terminado, r16
sts setup_M1_terminado, r16

// Configuración de direcciones de puertos para EN y DI de motores y SW

ldi r16, 0b00110000         // PD4 (M1_DI), PD5 (M5_DI) como salida
out 0x0A, r16

lds r16, 0x0A               // SW1 (PD0), SW5 (PD1) como entrada
andi r16, 0b11111100
sts 0x0A, r16

ldi r16, 0b01100000         // PB5 (M1_EN), PB6 (M5_EN) como salida
out 0x04, r16

lds r16, 0x04              // SW6 (PB7) como entrada
andi r16, 0b01111111
sts 0x04, r16

//=== Configuración de interrupciones externas ===
lds r16, 0x69
ori r16, 0b00001010          // ISC01 y ISC11 activados (INT0 y INT1 por flanco de bajada)
sts 0x69, r16

lds r16, 0x3D
ori r16, 0b00000011          // Habilita INT0 e INT1
sts 0x3D, r16

//=== Interrupción por pin: PCINT7 (PB7 - SW6) ===
lds r16, 0x68
ori r16, 0b00000001          // Habilita el grupo PCIE0 (PCINT[7:0])
sts 0x68, r16

lds r16, 0x6B
ori r16, 0b10000000          // Habilita PCINT7
sts 0x6B, r16
sei

//Iniciar motores
rcall iniciar_M1
rcall iniciar_M5

ret
