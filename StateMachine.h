#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include "monitores.h"
#include "menuconfig.h"
#include "Buzzer.h"
#include "security.h"

// Declaración de funciones para manejar las acciones de la máquina de estados.
void runSysBlock();
void runConfig();
void runMonitorAmbiental();
void runMonitorEvents();
void runAlarm();

void leavingMonitorAmbiental();
void leavingMonitorEvents();
void leavingConfig();

/**
 * @brief Configura la máquina de estados, definiendo transiciones y acciones.
 *
 * Esta función configura las transiciones entre estados y las acciones que se
 * ejecutarán al entrar o salir de cada estado. También establece el estado inicial.
 */
void setupMachine(){
  stateMachine.AddTransition(start, config, []() {return input == correctPassword;});
  stateMachine.AddTransition(start, block, []() {return input == sysBlock;});
  
  stateMachine.AddTransition(config, monitorAmbiental, []() {return input == button;});
  
  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == tempHigh;});
  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == lightHigh;});
  stateMachine.AddTransition(monitorAmbiental, monitorEvents, []() {return input == timeOut7;});
  stateMachine.AddTransition(monitorAmbiental, config, []() {return input == button;});

  stateMachine.AddTransition(monitorEvents, alarm, []() {return input == hallHigh;});
  stateMachine.AddTransition(monitorEvents, config, []() {return input == button;});
  stateMachine.AddTransition(monitorEvents, monitorAmbiental, []() {return input == timeOut3;});

  stateMachine.AddTransition(alarm, start, []() {return input == button;});
  stateMachine.AddTransition(alarm, monitorAmbiental, []() {return input == timeOut4;});

  stateMachine.AddTransition(block, start, []() {return input == timeOut10;});

  stateMachine.SetOnEntering(start, seguridad);
  stateMachine.SetOnEntering(block, runSysBlock);
  stateMachine.SetOnEntering(config, startLiquidMenu);
  stateMachine.SetOnEntering(monitorAmbiental, runMonitorAmbiental);
  stateMachine.SetOnEntering(monitorEvents, runMonitorEvents);
  stateMachine.SetOnEntering(alarm, runAlarm);

  stateMachine.SetOnLeaving(monitorAmbiental, leavingMonitorAmbiental);
  stateMachine.SetOnLeaving(monitorEvents, leavingMonitorEvents);
  stateMachine.SetOnLeaving(config, leavingConfig);
  // Se configura estado inicial
  stateMachine.SetState(start, false, true);
}

/**
 * @brief Maneja el estado de bloqueo del sistema.
 *
 * Esta función se ejecuta cuando el sistema entra en el estado de bloqueo.
 * Muestra un mensaje en el LCD, enciende el LED rojo y reproduce una melodía de bloqueo.
 */
void runSysBlock(){
  taskLoop.Stop();
  lcd.setCursor(0, 0);
  lcd.print("Sistema bloqueado");
  digitalWrite(ledB, LOW);
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledR, HIGH);
    playMelody(buzzerBlock, 2);
    delay(20);
    digitalWrite(ledR, LOW);
    playMelody(buzzerBlock, 2);
    delay(20);
  }
  lcd.noCursor();
  taskTimeOut10.Start();
}

/**
 * @brief Maneja el estado de configuración.
 *
 * Esta función se ejecuta cuando el sistema entra en el estado de configuración.
 * Muestra un mensaje de configuración en el LCD.
 */
void runConfig(){
  lcd.clear();
  lcd.print("> Config");
}

/**
 * @brief Maneja el estado de monitoreo ambiental.
 *
 * Esta función se ejecuta cuando el sistema entra en el estado de monitoreo ambiental.
 * Limpia el LCD, inicia las tareas de monitoreo ambiental y configura un tiempo de espera.
 */
void runMonitorAmbiental(){
  lcd.clear();
  taskLoopMonitorAmbiental.Start();
  taskTimeOut7.Start();
}

/**
 * @brief Maneja el estado de monitoreo de eventos.
 *
 * Esta función se ejecuta cuando el sistema entra en el estado de monitoreo de eventos.
 * Limpia el LCD, inicia las tareas de monitoreo de eventos y configura un tiempo de espera.
 */
void runMonitorEvents(){
  lcd.clear();
  taskLoopMonitorHall.Start();
  taskTimeOut3.Start();
}

/**
 * @brief Maneja el estado de alarma.
 *
 * Esta función se ejecuta cuando el sistema entra en el estado de alarma.
 * Muestra un mensaje de alarma en el LCD, enciende el LED azul y reproduce una melodía de alarma.
 */
void runAlarm(){
  lcd.clear();
  lcd.print("> Alarm");
  digitalWrite(ledB, HIGH);
  playMelody(buzzerAlarm, 2);
  digitalWrite(ledB, LOW);
  delay(200);
  digitalWrite(ledB, HIGH);
  playMelody(buzzerAlarm, 2);
  digitalWrite(ledB, LOW);
  delay(200);
  digitalWrite(ledB, HIGH);
  playMelody(buzzerAlarm, 2);
  digitalWrite(ledB, LOW);
  delay(200);
  digitalWrite(ledB, HIGH);
  playMelody(buzzerAlarm, 2);
  digitalWrite(ledB, LOW);
  delay(200);
  taskTimeOut4.Start();
}

/**
 * @brief Acción de salida del estado de monitoreo ambiental.
 *
 * Esta función se ejecuta cuando el sistema sale del estado de monitoreo ambiental.
 * Detiene la tarea de monitoreo ambiental y muestra un mensaje en el serial.
 */
void leavingMonitorAmbiental(){
  taskLoopMonitorAmbiental.Stop();
  Serial.println("Saliendo del estado Monitor Ambiental");
}

/**
 * @brief Acción de salida del estado de monitoreo de eventos.
 *
 * Esta función se ejecuta cuando el sistema sale del estado de monitoreo de eventos.
 * Detiene la tarea de monitoreo de eventos.
 */
void leavingMonitorEvents(){
  taskLoopMonitorHall.Stop();
}

/**
 * @brief Acción de salida del estado de configuración.
 *
 * Esta función se ejecuta cuando el sistema sale del estado de configuración.
 * Detiene la tarea de configuración del menú.
 */
void leavingConfig(){
  taskLoopMenu.Stop();
}

#endif // STATEMACHINE_H
