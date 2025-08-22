# Trabajo Sistemas Microcprocesadores (4º GITI - ETSII UPM) 

Código desarrollado para la elaboración del trabajo de sistemas microprocesadores, asignatura de 4º de GITI, en la especialidad de Automática y Electrónica. 

Estos códigos corresponden a los sistemas de recarga y retorno de la maqueta de la bolera. 

<div style="display: flex; justify-content: center;">
<img src="https://github.com/jimmyperezp/jimmyperezp/blob/main/cpp.svg" alt="c++" width="60" height="60"/>  
<img src="https://github.com/jimmyperezp/jimmyperezp/blob/main/MStudio_logo.png" alt="microchip" width ="60" height="60"/> 
<img src="https://github.com/jimmyperezp/Programacion_de_sistemas/blob/main/logo%20escuela.png" alt="logo industriales" width="300" height="100"/>

</div>

<br><br>

## Contexto: 

**1: Soporte empleado: Microcontrolador**
El objetivo de este trabajo era el control de una maqueta utilizando un microcontrolador. En este caso, se utilizó el AVR ATMEGA 640.

Para conocer y realizar el direccionamiento, conexionado y correcta utilización del micro, se debe utilizar el datasheet del mismo .

En cuanto al software, todo el proyecto se desarrolló en Microchip Studio. En dicho entorno se puede acceder a los puertos del micro para comprobar en todo momento su estado, por lo que era la solución más adecuada.

**Maqueta**
Este trabajo en concreto se encargaba de manejar una maqueta de una bolera. Esta se dividía en 3 partes. 
Los códigos de este repositorio son los desarrollados para gestionar y controlar la segunda de estas partes; encargada de gestionar el sistema de recarga y de retorno de las bolas (ver figura del sistema en la documentación).

**Procedimiento**
En este repositorio se fue trabajando con los códigos exclusivamente de esta segunda parte. Una vez comprobada su funcionalidad, gracias a las pruebas que se encuentran disponibles, se prosiguió con la integración y adaptación del resto de partes de manera remota. 



## Contenido

El desarrollo del código se divide en 3 secciones: 

1. **Pruebas**: A medida que se iban desarrollando nuevas funcionalidades para la maqueta, se han ido probando paulatinamente su funcionamiento sobre partes específicas de la maqueta. Estas pruebas son: 


    - Dirección de motores
    - Mover motor 1 (de una dirección a otra. Así, se comprueba el funcionamiento de los fines de carrera)
    - Mover y apagar motores.
    - Timer de 5 segundos. 
    - Pulsacion de SW6 con antirrebotes en L1.
    - Función "global" RETORNO
    - Función "global" RECARGA
    - Setup realizado en lenguaje ensamblador.

<br><br>

## Estado del repositorio: 
Este repositorio está terminado. El trabajo fue entregado y la asignatura aprobada.

Última modificación: 22/08/2025
