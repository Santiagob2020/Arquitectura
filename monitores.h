/**
 * @brief Imprime los valores de temperatura, humedad y luz en la pantalla LCD y verifica si estos exceden los límites predefinidos.
 */
void printMonitorAmbiental() {
  lcd.clear(); ///< Limpia la pantalla LCD.

  int h = dht.readHumidity(); ///< Lee la humedad del sensor DHT.
  int t = dht.readTemperature(); ///< Lee la temperatura del sensor DHT.
  int photocell = analogRead(photocellPin); ///< Lee el valor del sensor de luz.

  lcd.setCursor(0, 0); ///< Establece el cursor en la posición 0, 0.
  lcd.print("TEMP:"); ///< Imprime "TEMP:".
  lcd.print(t); ///< Imprime la temperatura.

  lcd.setCursor(8, 0); ///< Establece el cursor en la posición 8, 0.
  lcd.print("HUM:"); ///< Imprime "HUM:".
  lcd.print(h); ///< Imprime la humedad.
  
  lcd.setCursor(0, 1); ///< Establece el cursor en la posición 0, 1.
  lcd.print("LUZ:"); ///< Imprime "LUZ:".
  lcd.print(photocell); ///< Imprime el valor del sensor de luz.

  if(t > maxTemp) {
    Serial.print("La temperatura supera los "); ///< Imprime mensaje en el monitor serie.
    Serial.println(maxTemp); ///< Imprime el valor máximo de temperatura en el monitor serie.
    taskTempHigh.Start(); ///< Inicia la tarea asociada a temperatura alta.
  } else if(photocell > maxLight) {
    Serial.print("La lectura del photocell es menor a "); ///< Imprime mensaje en el monitor serie.
    Serial.println(maxLight); ///< Imprime el valor máximo de luz en el monitor serie.
    taskLightHigh.Start(); ///< Inicia la tarea asociada a luz alta.
  } else if(t < minTemp) {
    Serial.print("La temperatura es menor a "); ///< Imprime mensaje en el monitor serie.
    Serial.println(minTemp); ///< Imprime el valor mínimo de temperatura en el monitor serie.
    taskTempHigh.Start(); ///< Inicia la tarea asociada a temperatura baja.
  } else if(photocell < minLight) {
    Serial.print("photocell es mayor a "); ///< Imprime mensaje en el monitor serie.
    Serial.println(minLight); ///< Imprime el valor mínimo de luz en el monitor serie.
    taskLightHigh.Start(); ///< Inicia la tarea asociada a luz baja.
  }
}

/**
 * @brief Imprime el valor del sensor Hall en la pantalla LCD y verifica si excede el límite predefinido.
 */
void printMonitorHall() {
  int hall = analogRead(hallPin); ///< Lee el valor del sensor Hall.

  lcd.clear(); ///< Limpia la pantalla LCD.
  lcd.setCursor(0, 0); ///< Establece el cursor en la posición 0, 0.
  lcd.print("Hall:"); ///< Imprime "Hall:".
  lcd.print(hall); ///< Imprime el valor del sensor Hall.

  if(hall > maxHall)
    taskHallHigh.Start(); ///< Inicia la tarea asociada a valor alto del sensor Hall.
}

/**
 * @brief Activa la alarma, mostrando un mensaje en la pantalla LCD, encendiendo el LED y reproduciendo un sonido.
 */
void alarma() {
  lcd.print("!!Alarma!!"); ///< Imprime "!!Alarma!!" en la pantalla LCD.
  digitalWrite(ledB, HIGH); ///< Enciende el LED azul.
  playMelody(buzzerAlarm, 2); ///< Reproduce una melodía de alarma en el buzzer.
  digitalWrite(ledB, LOW); ///< Apaga el LED azul.
  playMelody(buzzerAlarm, 1); ///< Reproduce una melodía de alarma en el buzzer.
  digitalWrite(ledB, HIGH); ///< Enciende el LED azul.
  playMelody(buzzerAlarm, 2); ///< Reproduce una melodía de alarma en el buzzer.
  digitalWrite(ledB, LOW); ///< Apaga el LED azul.
  playMelody(buzzerAlarm, 1); ///< Reproduce una melodía de alarma en el buzzer.
  digitalWrite(ledB, HIGH); ///< Enciende el LED azul.
  playMelody(buzzerAlarm, 2); ///< Reproduce una melodía de alarma en el buzzer.
  digitalWrite(ledB, LOW); ///< Apaga el LED azul.
  playMelody(buzzerAlarm, 1); ///< Reproduce una melodía de alarma en el buzzer.
}
