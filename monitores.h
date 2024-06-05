/**
 * @brief Monitorea las condiciones ambientales y muestra los datos en la pantalla LCD.
 *
 * Esta función lee la temperatura y la humedad desde el sensor DHT, así como la luz desde el sensor fotocelda.
 * Luego muestra estos valores en la pantalla LCD y verifica si exceden los límites definidos, iniciando las tareas correspondientes si es necesario.
 */
void printMonitorAmbiental() {
  lcd.clear();
  
  // Leer la humedad desde el sensor DHT
  int h = dht.readHumidity();

  // Leer la temperatura desde el sensor DHT
  int t = dht.readTemperature();

  // Leer el valor de la fotocelda
  int photocell = analogRead(photocellPin);

  // Mostrar la temperatura en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.print(t);
  delay(500);

  // Mostrar la humedad en la pantalla LCD
  lcd.setCursor(8, 0);
  lcd.print("HUM:");
  lcd.print(h);
  delay(500);

  // Mostrar la luz en la pantalla LCD
  lcd.setCursor(0, 1);
  lcd.print("LUZ:");
  lcd.print(photocell);
  delay(500);

  // Verificar si la temperatura supera el límite máximo
  if (t > maxTemp) {
    Serial.print("La temperatura supera los ");
    Serial.println(maxTemp);
    taskTempHigh.Start();
  }
  // Verificar si la lectura de la fotocelda supera el límite máximo
  else if (photocell > maxLight) {
    Serial.print("La lectura del photocell es menor a ");
    Serial.println(maxLight);
    taskLightHigh.Start();
  }
  // Verificar si la temperatura es menor que el límite mínimo
  else if (t < minTemp) {
    Serial.print("La temperatura es menor a ");
    Serial.println(minTemp);
    taskTempHigh.Start();
  }
  // Verificar si la lectura de la fotocelda es menor que el límite mínimo
  else if (photocell < minLight) {
    Serial.print("photocell es mayor a ");
    Serial.println(minLight);
    taskLightHigh.Start();
  }
}

/**
 * @brief Monitorea el sensor de efecto Hall y muestra los datos en la pantalla LCD.
 *
 * Esta función lee el valor del sensor de efecto Hall, lo muestra en la pantalla LCD y verifica si excede el límite definido,
 * iniciando la tarea correspondiente si es necesario.
 */
void printMonitorHall() {
  // Leer el valor del sensor de efecto Hall
  int hall = analogRead(hallPin);

  lcd.clear();
  
  // Mostrar el valor del sensor de efecto Hall en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("Hall:");
  lcd.print(hall);

  // Verificar si el valor del sensor de efecto Hall supera el límite máximo
  if (hall > maxHall)
    taskHallHigh.Start();
}
