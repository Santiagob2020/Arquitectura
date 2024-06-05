#ifndef SECURITY_H
#define SECURITY_H

#include "variables.h"

// Variables de seguridad
const char password[5] = {'0', '1', '2', '3', '4'};
int tries = 0;
char intentoActual[6];
int cantidadCaracteres = 0;
int sistemaBloqueado = 0;

// Tareas asincrónicas

/**
 * @brief Inicializa el sistema de seguridad, reseteando los intentos y limpiando el estado.
 *
 * Esta función configura el sistema para empezar a recibir una nueva contraseña,
 * reseteando el número de intentos, la cantidad de caracteres ingresados y el estado del sistema.
 * Además, apaga todos los LEDs y muestra un mensaje en el LCD solicitando la clave.
 */
void seguridad() {
  Serial.println("Ha entrado a el estado Start");
  tries = 0;
  cantidadCaracteres = 0;
  sistemaBloqueado = 0;
  digitalWrite(ledR, LOW);
  digitalWrite(ledB, LOW);
  digitalWrite(ledG, LOW);
  lcd.clear();
  lcd.print("Ingrese clave");
  
  taskLoop.Start();
}

/**
 * @brief Función de loop para verificar la entrada de la contraseña.
 *
 * Esta función se ejecuta en un loop, leyendo las teclas ingresadas, mostrando asteriscos en el LCD,
 * verificando si la contraseña ingresada es correcta y actuando en consecuencia (correcta o incorrecta).
 */
void loopS() {
  lcd.setCursor(cantidadCaracteres, 1);
  char key = keypad.getKey();
  lcd.cursor();

  if (key) {
    // Recibimos caracter ingresado
    if (cantidadCaracteres < 5)
      intentoActual[cantidadCaracteres] = key;
    lcd.print("*");

    if (strcmp(intentoActual, password) == 0) { // >> CAMBIAR ESTO
      // Configuración para clave correcta
      lcd.clear();
      lcd.print("Clave correcta");
      limpiarclave();
      digitalWrite(ledG, HIGH);
      delay(1000);
      digitalWrite(ledG, LOW);
      taskStopLoop.Start();
      taskCorrect.Start();
      return;
    } else if (cantidadCaracteres + 1 == 5) {
      // Configuración para clave incorrecta
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clave incorrecta");
      digitalWrite(ledB, HIGH);

      for (size_t i = 0; i < 5; i++)
        intentoActual[i] = '*';
      taskStopLoop.Start();
      // Validar si es necesario bloquear el sistema
      if (tries == 2) {
        taskSysBlock.Start();
        return;
      }
      taskAgain.Start();
      cantidadCaracteres = 0;
    } else
      cantidadCaracteres++;
  }
}

/**
 * @brief Función para manejar intentos fallidos de contraseña.
 *
 * Esta función se llama cuando la contraseña ingresada es incorrecta,
 * incrementa el contador de intentos y reinicia el proceso para un nuevo intento.
 */
void again() {
  digitalWrite(ledB, LOW);
  lcd.clear();
  lcd.print("Prueba Again");      
  tries++;
  taskLoop.Start();
}

/**
 * @brief Limpia la contraseña ingresada.
 *
 * Esta función resetea el buffer de la contraseña ingresada a '0'.
 */
void limpiarclave() {
  for (int i = 0; i < 5; i++) {
    intentoActual[i] = '0';
  }
}

#endif // SECURITY_H
