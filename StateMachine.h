/**
 * @file StateMachine.h
 * @brief Declaración e implementación de la máquina de estados para el sistema.
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "monitores.h"
#include "menu.h"
#include "variablesBuzzerTareas.h"

/**
 * @brief Ejecuta el bloqueo del sistema.
 */
void runSysBlock();

/**
 * @brief Ejecuta la configuración del sistema.
 */
void runConfig();

/**
 * @brief Ejecuta la monitorización ambiental.
 */
void runMonitorAmbiental();

/**
 * @brief Ejecuta la monitorización de eventos.
 */
void runMonitorEvents();

/**
 * @brief Ejecuta la alarma.
 */
void runAlarm();

/**
 * @brief Finaliza la monitorización ambiental.
 */
void exitMonitorAmbiental();

/**
 * @brief Finaliza la monitorización de eventos.
 */
void exitMonitorEvents();

/**
 * @brief Finaliza la configuración.
 */
void exitConfig();

/**
 * @brief Configura la máquina de estados, añadiendo transiciones y configuraciones de entrada/salida.
 */
void setupMachine() {
  stateMachine.AddTransition(inicio, config, []() {return input == correctPassword;});
  stateMachine.AddTransition(inicio, bloqueo, []() {return input == sysBlock;});

  stateMachine.AddTransition(config, monitorAmbiental, []() {return input == button;});

  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == tempHigh;});
  stateMachine.AddTransition(monitorAmbiental, alarm, []() {return input == lightHigh;});
  stateMachine.AddTransition(monitorAmbiental, monitorEvents, []() {return input == timeOut7;});
  stateMachine.AddTransition(monitorAmbiental, config, []() {return input == button;});

  stateMachine.AddTransition(monitorEvents, alarm, []() {return input == hallHigh;});
  stateMachine.AddTransition(monitorEvents, config, []() {return input == button;});
  stateMachine.AddTransition(monitorEvents, monitorAmbiental, []() {return input == timeOut3;});

  stateMachine.AddTransition(alarm, inicio, []() {return input == button;});
  stateMachine.AddTransition(alarm, monitorAmbiental, []() {return input == timeOut4;});

  stateMachine.AddTransition(bloqueo, inicio, []() {return input == timeOut10;});

  stateMachine.SetOnEntering(inicio, seguridad);
  stateMachine.SetOnEntering(bloqueo, runSysBlock);
  stateMachine.SetOnEntering(config, startLiquidMenu);
  stateMachine.SetOnEntering(monitorAmbiental, runMonitorAmbiental);
  stateMachine.SetOnEntering(monitorEvents, runMonitorEvents);
  stateMachine.SetOnEntering(alarm, runAlarm);

  stateMachine.SetOnLeaving(monitorAmbiental, exitMonitorAmbiental);
  stateMachine.SetOnLeaving(monitorEvents, exitMonitorEvents);
  stateMachine.SetOnLeaving(config, exitConfig);

  // Configura el estado inicial
  stateMachine.SetState(inicio, false, true);
}

/**
 * @brief Función que se ejecuta cuando el sistema está bloqueado.
 */
void runSysBlock() {
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
 * @brief Función que se ejecuta para la configuración del sistema.
 */
void runConfig() {
  lcd.clear();
  lcd.print("> Config");
}

/**
 * @brief Función que se ejecuta para la monitorización ambiental.
 */
void runMonitorAmbiental() {
  lcd.clear();
  taskLoopMonitorAmbiental.Start();
  taskTimeOut7.Start();
}

/**
 * @brief Función que se ejecuta para la monitorización de eventos.
 */
void runMonitorEvents() {
  lcd.clear();
  taskLoopMonitorHall.Start();
  taskTimeOut3.Start();
}

/**
 * @brief Función que se ejecuta cuando se activa la alarma.
 */
void runAlarm() {
  lcd.clear();
  taskAlarma.Start();
  taskTimeOut4.Start();
}

/**
 * @brief Función que se ejecuta al salir del estado de monitorización ambiental.
 */
void exitMonitorAmbiental() {
  taskLoopMonitorAmbiental.Stop();
  Serial.println("Saliendo del estado Monitor Ambiental");
}

/**
 * @brief Función que se ejecuta al salir del estado de monitorización de eventos.
 */
void exitMonitorEvents() {
  taskLoopMonitorHall.Stop();
}

/**
 * @brief Función que se ejecuta al salir del estado de configuración.
 */
void exitConfig() {
  taskLoopMenu.Stop();
}

#endif // STATEMACHINE_H
