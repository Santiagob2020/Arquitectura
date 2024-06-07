/**
 * @file main.cpp
 * @brief Este archivo contiene la implementación principal de una máquina de estados con lectura de un botón y control de LED.
 */

#include "variablesBuzzerTareas.h"
#include "StateMachineLib.h"
#include "maquinaEstados.h"

/**
 * @brief Almacena el valor anterior del botón para detectar cambios.
 */
int oldValueButton;

/**
 * @brief Configura la inicialización de la máquina de estados, el puerto serie, el sensor DHT y los pines de los LEDs.
 */
void setup() {
  Serial.begin(9600); ///< Inicializa la comunicación serie a 9600 baudios.
  lcd.begin(16,2); ///< Inicializa la pantalla LCD con 16 columnas y 2 filas.
  setupMachine(); ///< Configura la máquina de estados.
  dht.begin(); ///< Inicializa el sensor DHT.
  pinMode(ledB,OUTPUT); ///< Configura el pin del LED azul como salida.
  pinMode(ledR,OUTPUT); ///< Configura el pin del LED rojo como salida.
  pinMode(ledG,OUTPUT); ///< Configura el pin del LED verde como salida.
  pinMode(buttonPin, INPUT); ///< Configura el pin del botón como entrada.
  input = unknow; ///< Inicializa la variable de entrada como desconocida.
}

/**
 * @brief Bucle principal que lee las entradas, actualiza la máquina de estados y las tareas.
 */
void loop() {
  readInput(); ///< Lee el estado del botón.
  stateMachine.Update(); ///< Actualiza la máquina de estados.
  input = unknow; ///< Restablece la variable de entrada a desconocida.
  updateTask(); ///< Actualiza las tareas.
}

/**
 * @brief Lee el estado del botón y actualiza la entrada de la máquina de estados si se detecta un cambio.
 */
void readInput(){
  Input actualInput = (input != unknow) ? input: unknow; ///< Obtiene la entrada actual o la establece como desconocida si no se ha detectado ninguna.
  int valueButton = digitalRead(buttonPin); ///< Lee el estado del botón.

  if(valueButton != oldValueButton){ ///< Si el estado del botón ha cambiado.
    oldValueButton = valueButton; ///< Actualiza el valor anterior del botón.
    if(valueButton == HIGH){ ///< Si el botón está presionado.
      actualInput = button; ///< Establece la entrada actual como el botón.
      Serial.println("Se ha presionado el boton"); ///< Imprime un mensaje en el puerto serie.
    }
    delay(9); ///< Espera 9 ms para evitar rebotes.
  }
  
  input = actualInput; ///< Actualiza la variable global de entrada.
}
