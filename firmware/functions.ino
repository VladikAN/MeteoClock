void setLED(byte color) {
  if (!LED_MODE) {
    analogWrite(LED_R_PIN, 0);
    analogWrite(LED_G_PIN, 0);
    analogWrite(LED_B_PIN, 0);
  } else {
    analogWrite(LED_R_PIN, 255);
    analogWrite(LED_G_PIN, 255);
    analogWrite(LED_B_PIN, 255);
  }

  switch (color) {    // 0 off, 1 green, 2 yellow, 3 red
    case 0: break;
    case 1: analogWrite(LED_G_PIN, LED_ON); break;
    case 2:
      analogWrite(LED_R_PIN, LED_ON - 50);
      analogWrite(LED_G_PIN, LED_ON);
      break;
    case 3: analogWrite(LED_R_PIN, LED_ON); break;
  }
}

void loadClock() {
  // Big numbers parts
  uint8_t FL[8] = { 0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111 };
  uint8_t UB[8] = { 0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000 };
  uint8_t LB[8] = { 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111 };
  uint8_t BU[8] = { 0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111 };
  uint8_t BL[8] = { 0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111 };

  lcd.createChar(0, FL);
  lcd.createChar(1, UB);
  lcd.createChar(2, LB);
  lcd.createChar(3, BU);
  lcd.createChar(4, BL);

  // Emotions
  uint8_t good[8]   = { 0b00000, 0b01010, 0b01010, 0b00000, 0b10001, 0b01110, 0b00000, 0b00000 };
  uint8_t normal[8] = { 0b00000, 0b01010, 0b01010, 0b00000, 0b00000, 0b11111, 0b00000, 0b00000 };
  uint8_t bad[8]    = { 0b00000, 0b01010, 0b01010, 0b00000, 0b01110, 0b10001, 0b00000, 0b00000 };
  lcd.createChar(5, good);
  lcd.createChar(6, normal);
  lcd.createChar(7, bad);
}

void loadPlot() {
  byte row8[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row7[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row6[8] = {0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row4[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111};
  byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
  byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
  byte row1[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};

  lcd.createChar(0, row8);
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}

void updateBrightness() {
  if (!AUTO_BRIGHT)
    return;
  
  if (analogRead(PHOTO) < BRIGHT_THRESHOLD) {   // light place
    analogWrite(BACKLIGHT_PIN, LCD_BRIGHT_MIN);
    LED_ON = LED_MODE == 0 ? LED_BRIGHT_MIN : 255 - LED_BRIGHT_MIN;
  } else {                                      // dark place
    analogWrite(BACKLIGHT_PIN, LCD_BRIGHT_MAX);
    LED_ON = LED_MODE == 0 ? LED_BRIGHT_MAX : 255 - LED_BRIGHT_MAX;
  }
}

void readInput() {
  button.tick();

  if (button.isClick()) {
    next_mode++;
    if (next_mode > 8)
      next_mode = 0;
  }

  if (button.isHolded()) {
    next_mode = 0;
  }

  if (display_mode == next_mode)
    return;

  updateScreen();
}

void updateScreen() {
  display_mode = next_mode;
  lcd.clear();

  if (display_mode == 0) {
    drawClock();
    drawData();
    drawSensors();
    return;
  }

  switch (display_mode) {
    case 1: drawPlot(0, 3, 15, 4, TMP_MIN, TMP_MAX, TMP_STP, (int*)tmpHour, "t hr"); break;
    case 2: drawPlot(0, 3, 15, 4, TMP_MIN, TMP_MAX, TMP_STP, (int*)tmpDay, "t day"); break;
    case 3: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, HUM_STP, (int*)humHour, "h hr"); break;
    case 4: drawPlot(0, 3, 15, 4, HUM_MIN, HUM_MAX, HUM_STP, (int*)humDay, "h day"); break;
    case 5: drawPlot(0, 3, 15, 4, PRS_MIN, PRS_MAX, PRS_STP, (int*)prsHour, "p hr"); break;
    case 6: drawPlot(0, 3, 15, 4, PRS_MIN, PRS_MAX, PRS_STP, (int*)prsDay, "p day"); break;
    case 7: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, CO2_STP, (int*)co2Hour, "c hr"); break;
    case 8: drawPlot(0, 3, 15, 4, CO2_MIN, CO2_MAX, CO2_STP, (int*)co2Day, "c day"); break;
  }
}

void readSensors() {
  // Temperature & Humidity & Pressure
  bme.takeForcedMeasurement();
  current_tmp = bme.readTemperature();
  current_hum = bme.readHumidity();
  current_prs = (float)bme.readPressure() * 0.00750062;

  // CO2
  current_co2 = mhz19.getPPM();
  if (current_co2 < 800) setLED(1);
  else if (current_co2 < 1200) setLED(2);
  else if (current_co2 >= 1200) setLED(3);
}

void drawSensors() {
  // refer to https://learn.robotgeek.com/getting-started/59-lcd-special-characters.html for any chars you want
  // Temperature
  lcd.setCursor(0, 2);
  lcd.print(String(current_tmp, 1) + " ");
  lcd.write(223);

  // Humidity
  lcd.setCursor(8, 2);
  lcd.print(String(current_hum) + " %");

  // Pressure
  lcd.setCursor(14, 2);
  lcd.print(String(current_prs) + " mm");

  // CO2
  lcd.setCursor(4, 3);
  lcd.print("CO2 " + String(current_co2) + " ppm");

  // CO2 impression
  lcd.setCursor(17, 3);
  if (current_co2 < 800) lcd.write(5);
  else if (current_co2 < 1200) lcd.write(6);
  else if (current_co2 >= 1200) lcd.write(7);
}

void updateAvgSensorsData() {
  // hourly
  if (hourPlotTimer.isReady()) {
    for (byte i = 0; i < 14; i++) {
      tmpHour[i] = tmpHour[i + 1];
      humHour[i] = humHour[i + 1];
      prsHour[i] = prsHour[i + 1];
      co2Hour[i] = co2Hour[i + 1];
    }

    tmpHour[14] = current_tmp;
    humHour[14] = current_hum;
    co2Hour[14] = current_co2;
    prsHour[14] = current_prs;
  }

  // daily
  if (dayPlotTimer.isReady()) {
    long avgTmp = 0, avgHum = 0, avgPrs = 0, avgCO2 = 0;

    for (byte i = 0; i < 15; i++) {
      avgTmp += tmpHour[i];
      avgHum += humHour[i];
      avgPrs += prsHour[i];
      avgCO2 += co2Hour[i];
    }

    avgTmp /= 15;
    avgHum /= 15;
    avgPrs /= 15;
    avgCO2 /= 15;

    for (byte i = 0; i < 14; i++) {
      tmpDay[i] = tmpDay[i + 1];
      humDay[i] = humDay[i + 1];
      prsDay[i] = prsDay[i + 1];
      co2Day[i] = co2Day[i + 1];
    }

    tmpDay[14] = avgTmp;
    humDay[14] = avgHum;
    prsDay[14] = avgPrs;
    co2Day[14] = avgCO2;
  }
}

void updateTime() {
  now = rtc.now();
  secs = now.second();
  mins = now.minute();
  hrs = now.hour();
}
