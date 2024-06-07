/**
 * @file buzzer.h
 * @brief Definiciones de notas musicales y configuraciones del sistema de monitoreo y seguridad.
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <Keypad.h>
#include <LiquidMenu.h>
#include "DHT.h"
#include "AsyncTaskLib.h"
#include "StateMachineLib.h"

/// Notas musicales en Hz.
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

/// Configuración de salidas.
#define ledR 7
#define ledG 6
#define ledB 8
#define rs 12
#define en 11
#define d4 5
#define d5 4
#define d6 3
#define d7 2

/// Configuración de pines de sensores.
#define photocellPin A1
#define hallPin A0
#define dhtPin 10
#define buttonPin 9

/// Configuración del tipo de sensor DHT.
#define DHTTYPE DHT11 ///< Tipo de sensor DHT.
DHT dht(dhtPin, DHTTYPE);

/// Configuración de la pantalla LCD.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/// Configuración de la máquina de estados.
StateMachine stateMachine(6, 13);

/// Configuración de variables del teclado.
#define ROWS 4
#define COLS 4
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 24, 26, 28}; ///< Pines de filas del teclado.
byte colPins[COLS] = {30, 32, 34, 36}; ///< Pines de columnas del teclado.

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/// Enumeración de posibles entradas para la máquina de estados.
enum Input {
  correctPassword = 0,
  button = 1,
  timeOut7 = 2,
  timeOut10 = 3,
  timeOut3 = 4,
  timeOut4 = 5,
  tempHigh = 6,
  lightHigh = 7,
  hallHigh = 8,
  sysBlock = 9,
  unknow = 10
};

/// Enumeración de posibles estados de la máquina de estados.
enum State {
  inicio = 0,
  config = 1,
  monitorAmbiental = 2,
  monitorEvents = 3,
  alarm = 4,
  bloqueo = 5
};

/// Variable global de entrada.
Input input;

/// Variables de límites de lectura.
int maxTemp = 40;
int minTemp = 0;
int maxLight = 300;
int minLight = 0;
int maxHall = 1000;

/// Declaraciones de funciones y tareas asincrónicas.
void loopS();
void again();
void limpiarclave();
AsyncTask taskLoop(1, true, loopS);
AsyncTask taskAgain(2000, false, again);
AsyncTask taskStopLoop(1, false, [] () { taskLoop.Stop(); });

void printMonitorAmbiental();
void printMonitorHall();
void alarma();
AsyncTask taskLoopMonitorAmbiental(100, true, printMonitorAmbiental);
AsyncTask taskLoopMonitorHall(100, true, printMonitorHall);
AsyncTask taskAlarma(10, false, alarma);

void loopLiquidMenu();
AsyncTask taskLoopMenu(100, true, loopLiquidMenu);

AsyncTask taskCorrect(2000, false, [](){ input = Input::correctPassword; });
AsyncTask taskSysBlock(10, false, [] () { input = Input::sysBlock; });
AsyncTask taskTimeOut7(7000, false, [](){ input = Input::timeOut7; });
AsyncTask taskTimeOut3(3000, false, [](){ input = Input::timeOut3; });
AsyncTask taskTimeOut4(100, false, [](){ input = Input::timeOut4; });
AsyncTask taskTimeOut10(100, false, []() { input = Input::timeOut10; });
AsyncTask taskButton(10, false, [] () { input = Input::button; });
AsyncTask taskTempHigh(5, false, [](){ input = Input::tempHigh; });
AsyncTask taskLightHigh(5, false, [](){ input = Input::lightHigh; });
AsyncTask taskHallHigh(5, false, [](){ input = Input::hallHigh; });

/**
 * @brief Actualiza todas las tareas asincrónicas.
 */
void updateTask() {
  taskTimeOut10.Update();
  taskLoop.Update();
  taskAgain.Update();
  taskCorrect.Update();
  taskStopLoop.Update();
  taskSysBlock.Update();
  taskLoopMonitorAmbiental.Update();
  taskTimeOut7.Update();
  taskTimeOut3.Update();
  taskButton.Update();
  taskLoopMonitorHall.Update();
  taskTimeOut4.Update();
  taskButton.Update();
  taskTempHigh.Update();
  taskLightHigh.Update();
  taskHallHigh.Update();
  taskLoopMenu.Update();
  taskAlarma.Update();
}

/// Variables para la melodía del buzzer.
int tempo = 60;
int buzzer = 13;
int buzzerBlock[] = { NOTE_DS8, -4 };
int buzzerAlarm[] = { NOTE_AS7, 8, NOTE_D8, 8 };

/**
 * @brief Reproduce una melodía en el buzzer.
 * @param melody Arreglo de notas de la melodía.
 * @param notes Número de notas en la melodía.
 */
void playMelody(int melody[], int notes) {
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // notas con puntillo
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}

//-- SEGURIDAD --

/// Clave de seguridad.
const char password[5] = {'*', '8', '6', 'A', 'B'};
int tries = 0;
char intentoActual[6];
int cantidadCaracteres = 0;
int sistemaBloqueado = 0;

/**
 * @brief Inicia el estado de seguridad, reseteando variables y mostrando un mensaje en la pantalla LCD.
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
 * @brief Bucle de seguridad que maneja la entrada del teclado y verifica la clave.
 */
void loopS() {
  lcd.setCursor(cantidadCaracteres, 1);
  char key = keypad.getKey();
  lcd.cursor();

  if (key) {
    if (cantidadCaracteres < 5)
      intentoActual[cantidadCaracteres] = key;
    lcd.print("*");

    if (strcmp(intentoActual, password) == 0) {
      lcd.clear();
      lcd.print("Clave correcta");
      limpiarclave();
      digitalWrite(ledG, HIGH);
      delay(1000);
      digitalWrite(ledG, LOW);
      taskStopLoop.Start();
      taskCorrect.Start();
      return;
    } else if (cantidadCaracteres + 1 == 8) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clave incorrecta");
      digitalWrite(ledB, HIGH);

      for (size_t i = 0; i < 5; i++)
        intentoActual[i] = '*';
      taskStopLoop.Start();

      if (tries >= 2) {
        taskSysBlock.Start();
        return;
      }
      taskAgain.Start();
      cantidadCaracteres = 0;
    } else {
      cantidadCaracteres++;
    }
  }
}

/**
 * @brief Función que se ejecuta para reintentar la clave después de un intento fallido.
 */
void again() {
  digitalWrite(ledB, LOW);
  lcd.clear();
  lcd.print("Prueba de nuevo");
  tries++;
  taskLoop.Start();
}

/**
 * @brief Limpia la clave ingresada.
 */
void limpiarclave() {
  for (int i = 0; i < 5; i++) {
    intentoActual[i] = '0';
  }
}

#endif // BUZZER_H
