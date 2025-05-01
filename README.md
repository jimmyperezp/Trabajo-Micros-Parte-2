He añadido los distintos codigos que he escrito.
Los archivos de encabezado tienen una extensión .cpp porque los he escrito en notepad. Al meterlos en microchip studio, deben ser un .h


En la carpeta "programa principal" está el programa tocho, con "todo". Dentro está el encabezado general, el encabezado de nuestra parte, y el main de nuestra parte. 

Encabezado general ->> Encabezado parte2 ->> main Parte 2 

En la carpeta de pruebas, he hecho un par de pruebas segmentadas en las que ir probando cosas poco a poco. Ahí están los siguientes dos archivos:

  1) Prueba motor m1 . Sirve para comprobar que sabemos mover el motor en ambos sentidos, y ver si las interrupciones de los fines de carrera saltan correctamente.
  2) Prueba inicializacion. Esta prueba directamente toda la inicialización del nuestra parte. Es mejor hacer esta una vez hayamos hecho la  anterior.


Después, he añadido la carpeta de setup_timers. Aquí, como indica el nombre, está el setup de los timers, el cual debe ser incluido en el archivo de encabezado general, y llamado mediante la función setup_timers(). 
Dentro de esta carpeta, he metido un PDF de los pasos que he seguido para configurar el timer 5 como salida PWM. He puesto un ciclo de trabajo cualquiera, pero eso lo podemos cambiar.
