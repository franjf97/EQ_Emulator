void drawInit()
{
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(45, 62, 230, 40, 10, TFT_BLUE);
  tft.fillRoundRect(50, 67, 220, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB12);
  tft.drawString("EQ EMULATOR", 160, 80, GFXFF);

  delay(1000);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSSB9);
  tft.drawString("Starting      ", 160, 150, GFXFF);

  delay(2000);
  tft.drawString("Starting .    ", 160, 150, GFXFF);
  delay(2000);
  tft.drawString("Starting . .  ", 160, 150, GFXFF);
  delay(2000);
  tft.drawString("Starting . . .", 160, 150, GFXFF);
  delay(2000);
}

void drawMenu()
{
  gui_state = MENU;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(45, 12, 230, 40, 10, TFT_BLUE);
  tft.fillRoundRect(50, 17, 220, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB12);
  tft.drawString("EQ EMULATOR", 160, 30, GFXFF);

  tft.setFreeFont(FSSB9);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  tft.fillCircle(65, 115, 53, TFT_BLUE);
  tft.fillCircle(65, 115, 50, TFT_WHITE);
  tft.drawString("ADAPT", 65, 115, GFXFF);

  tft.fillCircle(255, 115, 53, TFT_BLUE);
  tft.fillCircle(255, 115, 50, TFT_WHITE);
  tft.drawString("FILTER", 255, 115, GFXFF);

  tft.fillCircle(160, 180, 53, TFT_BLUE);
  tft.fillCircle(160, 180, 50, TFT_WHITE);
  tft.drawString("LOOPBACK", 160, 180, GFXFF);
}

void drawInstructionsAdapt_1()
{
  gui_state = ADAPT_INSTRUCTIONS_1;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.setTextDatum(ML_DATUM);
  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1) Connect 'Noise output 1'", 10, 60, GFXFF);
  tft.drawString("   to 'Input 1'", 10, 75, GFXFF);
  tft.drawString("2) Connect 'Noise output 2'", 10, 95, GFXFF);
  tft.drawString("   to your sample device", 10, 110, GFXFF);
  tft.drawString("3) Connect your device", 10, 130, GFXFF);
  tft.drawString("   output to 'Input 2'", 10, 145, GFXFF);
  tft.drawString("4) Press 'OK' when done", 10, 165, GFXFF);

  tft.fillRoundRect(105, 180, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(110, 185, 100, 45, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("OK", 160, 205, GFXFF);

  drawBackSymbol();
}

void drawInstructionsAdapt_2()
{
  gui_state = ADAPT_INSTRUCTIONS_2;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.setTextDatum(ML_DATUM);
  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("5) Check that levels of", 10, 60, GFXFF);
  tft.drawString("   both inputs are similar", 10, 75, GFXFF);
  tft.drawString("6) Use 'Noise Output'", 10, 95, GFXFF);
  tft.drawString("   to regulate 'Input 1'", 10, 110, GFXFF);
  tft.drawString("7) Use your device volume", 10, 130, GFXFF);
  tft.drawString("   to regulate 'Input 2'", 10, 145, GFXFF);
  tft.drawString("4) Press 'OK' when done", 10, 165, GFXFF);

  tft.fillRoundRect(95, 180, 130, 55, 10, TFT_BLUE);
  tft.fillRoundRect(100, 185, 120, 45, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB9);
  tft.drawString("Check levels", 160, 205, GFXFF);

  drawBackSymbol();
}

void drawSetVolumeAdapt()
{
  gui_state = ADAPT_SET_VOLUME;
  volume = 0.2;
  audio_volume.setVolume(volume);
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSS9);
  tft.drawString("Noise Volume", 160,65 , GFXFF);
  drawVolumeSelect();

  tft.fillRoundRect(205, 90, 60, 60, 10, TFT_BLUE);
  tft.fillRoundRect(210, 95, 50, 50, 10, TFT_WHITE);
  tft.fillRoundRect(55, 90, 60, 60, 10, TFT_BLUE);
  tft.fillRoundRect(60, 95, 50, 50, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("+", 230, 115, GFXFF);
  tft.drawString("-", 80, 115, GFXFF);

  tft.fillRoundRect(105, 180, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(110, 185, 100, 45, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("OK", 160, 205, GFXFF);

  drawBackSymbol();
}

void drawSelectAdapt()
{
  gui_state = ADAPT_SELECT;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSSB9);
  tft.drawString("Choose adaptation rate:", 160, 60, GFXFF);

  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB9);
  tft.fillRoundRect(35, 85, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(40, 90, 100, 45, 10, TFT_WHITE);
  tft.drawString("FAST", 90, 112, GFXFF);

  tft.fillRoundRect(175, 85, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(180, 90, 100, 45, 10, TFT_WHITE);
  tft.drawString("SLOW", 230, 112, GFXFF);

  tft.setFreeFont(FS9);
  tft.setTextDatum(ML_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("> Fast convergence", 10, 170, GFXFF);
  tft.drawString("> Less precision", 10, 200, GFXFF);

  tft.drawString("> Slow convergence", 170, 170, GFXFF);
  tft.drawString("> More precision", 170, 200, GFXFF);

  drawBackSymbol();
}

void drawInstructionsFilter()
{
  gui_state = FILTER_INSTRUCTIONS;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("FILTER", 160, 20, GFXFF);

  tft.setTextDatum(ML_DATUM);
  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1) Connect desired source", 10, 75, GFXFF);
  tft.drawString("   to Inputs 1 and 2", 10, 90, GFXFF);
  tft.drawString("2) Connect Outputs 1 and 2", 10, 110, GFXFF);
  tft.drawString("   to audio sink/speaker", 10, 125, GFXFF);
  tft.drawString("4) Press 'OK' when done", 10, 145, GFXFF);

  tft.fillRoundRect(105, 180, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(110, 185, 100, 45, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("OK", 160, 205, GFXFF);

  drawBackSymbol();
}

void drawInstructionsLoopback()
{
  gui_state = LOOPBACK_INSTRUCTIONS;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(95, 2, 130, 40, 10, TFT_BLUE);
  tft.fillRoundRect(100, 7, 120, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("LOOPBACK", 160, 20, GFXFF);

  tft.setTextDatum(ML_DATUM);
  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("1) Connect desired source", 10, 75, GFXFF);
  tft.drawString("   to Inputs 1 and 2", 10, 90, GFXFF);
  tft.drawString("2) Connect Outputs 1 and 2", 10, 110, GFXFF);
  tft.drawString("   to audio sink/speaker", 10, 125, GFXFF);
  tft.drawString("4) Press 'OK' when done", 10, 145, GFXFF);

  tft.fillRoundRect(105, 180, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(110, 185, 100, 45, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("OK", 160, 205, GFXFF);

  drawBackSymbol();
}

void drawAdapt()
{
  gui_state = ADAPT;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.fillRoundRect(250, 110, 60, 60, 10, TFT_BLUE);
  tft.fillRoundRect(255, 115, 50, 50, 10, TFT_WHITE);
  tft.fillRoundRect(250, 175, 60, 60, 10, TFT_BLUE);
  tft.fillRoundRect(255, 180, 50, 50, 10, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB18);
  tft.drawString("+", 280, 135, GFXFF);
  tft.drawString("-", 280, 200, GFXFF);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSS9);
  tft.drawString("Noise", 280, 55, GFXFF);
  tft.drawString("Volume", 280, 75, GFXFF);
  drawVolume();

  drawBackSymbol();

  drawDoneSymbol();

  drawAdaptPlot();
}

void drawAdaptPlot()
{
  tft.fillRect(0, 45, 250, 200, TFT_BLACK);

  tft.drawLine(20, 92, 240, 92, TFT_DARKGREY);
  tft.drawLine(20, 135, 240, 135, TFT_DARKGREY);
  tft.drawLine(20, 177, 240, 177, TFT_DARKGREY);

  tft.drawLine(196, 50, 196, 220, TFT_DARKGREY);
  tft.drawLine(152, 50, 152, 220, TFT_DARKGREY);
  tft.drawLine(108, 50, 108, 220, TFT_DARKGREY);
  tft.drawLine(64, 50, 64, 220, TFT_DARKGREY);

  tft.drawRect(20, 50, 220, 170, TFT_WHITE);

  tft.setFreeFont(TT1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TR_DATUM);
  tft.drawString("100", 20, 50, GFXFF);
  tft.drawString("75", 20, 92, GFXFF);
  tft.drawString("50", 20, 135, GFXFF);
  tft.drawString("25", 20, 177, GFXFF);
  tft.drawString("0", 20, 220, GFXFF);

  tft.drawFloat(scan / 1000, 0, 245, 225, GFXFF);
  tft.drawFloat(0.8 * (scan / 1000), 0, 200, 225, GFXFF);
  tft.drawFloat(0.6 * (scan / 1000), 0, 155, 225, GFXFF);
  tft.drawFloat(0.4 * (scan / 1000), 0, 113, 225, GFXFF);
  tft.drawFloat(0.2 * (scan / 1000), 0, 70, 225, GFXFF);

  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MSE = ", 150, 57, GFXFF);
  tft.drawString("%", 232, 57, GFXFF);
}

void drawAdaptValue()
{
  readSerial();
  if (samples_plot < N_samples)
  {
    value_y = (int)(scale_y * mse_porc);
    value_x = (int)(scale_x * samples_plot);
    digitalWrite(13, !digitalRead(13));   //Utilizado para debuggear el tiempo de recepción del MSE (afecta la constante sample_time)
    if (count_display_mse == draw_step)
    {
      tft.fillCircle(offset_x + value_x, offset_y - value_y, 1, TFT_GREEN);
      tft.setFreeFont(FM9);
      tft.setTextColor(TFT_BLACK, TFT_GREEN);
      tft.fillRect(174, 52, 55, 15, TFT_GREEN);
      tft.drawFloat(mse_porc, 1, 202, 57, GFXFF);
      drawVolume();
      count_display_mse = 0;
    }
    else
    {
      count_display_mse++;
    }
    samples_plot++;
  }
  else
  {
    samples_plot = 0;
    drawAdaptPlot();
  }
}

void drawSaveAdapt()
{
  gui_state = ADAPT_SAVE;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("ADAPT", 160, 20, GFXFF);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSSB9);
  tft.drawString("Save as new filter?", 160, 80, GFXFF);

  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setFreeFont(FSSB9);
  tft.fillRoundRect(35, 105, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(40, 110, 100, 45, 10, TFT_WHITE);
  tft.drawString("YES", 90, 132, GFXFF);

  tft.fillRoundRect(175, 105, 110, 55, 10, TFT_BLUE);
  tft.fillRoundRect(180, 110, 100, 45, 10, TFT_WHITE);
  tft.drawString("NO", 230, 132, GFXFF);

}

void drawFilter()
{
  gui_state = FILTER;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(105, 2, 110, 40, 10, TFT_BLUE);
  tft.fillRoundRect(110, 7, 100, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("FILTER", 160, 20, GFXFF);

  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  tft.drawRect(70, 95, 50, 17, TFT_WHITE);
  tft.drawRect(71, 95, 49, 16, TFT_BLACK);
  tft.fillTriangle(120, 112, 115, 109, 115, 115, TFT_WHITE);
  tft.drawString("Input 1", 70, 85, GFXFF);

  tft.drawRect(70, 148, 50, 17, TFT_WHITE);
  tft.drawRect(71, 149, 49, 16, TFT_BLACK);
  tft.fillTriangle(120, 148, 115, 145, 115, 151, TFT_WHITE);
  tft.drawString("Input 2", 70, 170, GFXFF);

  tft.drawRect(200, 95, 50, 17, TFT_WHITE);
  tft.drawRect(200, 95, 49, 16, TFT_BLACK);
  tft.fillTriangle(250, 95, 253, 100, 247, 100, TFT_WHITE);
  tft.drawString("Output 1", 250, 85, GFXFF);

  tft.drawRect(200, 148, 50, 17, TFT_WHITE);
  tft.drawRect(200, 149, 49, 16, TFT_BLACK);
  tft.fillTriangle(250, 165, 253, 160, 247, 160, TFT_WHITE);
  tft.drawString("Output 2", 250, 170, GFXFF);

  tft.drawRect(120, 95, 80, 70, TFT_WHITE);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(FSSB24);
  tft.drawString("~", 160, 120, GFXFF);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(TT1);
  tft.setFreeFont(FSS9);
  tft.drawString("Press box to change to loopback", 160, 220, GFXFF);

  drawBackSymbol();
}

void drawLoopback()
{
  gui_state = LOOPBACK;
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(95, 2, 130, 40, 10, TFT_BLUE);
  tft.fillRoundRect(100, 7, 120, 30, 10, TFT_SILVER);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_SILVER);
  tft.setFreeFont(FSSB9);
  tft.drawString("LOOPBACK", 160, 20, GFXFF);

  tft.setFreeFont(FM9);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  tft.drawRect(70, 95, 50, 17, TFT_WHITE);
  tft.drawRect(71, 95, 49, 16, TFT_BLACK);
  tft.fillTriangle(120, 112, 115, 109, 115, 115, TFT_WHITE);
  tft.drawString("Input 1", 70, 85, GFXFF);

  tft.drawRect(70, 148, 50, 17, TFT_WHITE);
  tft.drawRect(71, 149, 49, 16, TFT_BLACK);
  tft.fillTriangle(120, 148, 115, 145, 115, 151, TFT_WHITE);
  tft.drawString("Input 2", 70, 170, GFXFF);

  tft.drawRect(200, 95, 50, 17, TFT_WHITE);
  tft.drawRect(200, 95, 49, 16, TFT_BLACK);
  tft.fillTriangle(250, 95, 253, 100, 247, 100, TFT_WHITE);
  tft.drawString("Output 1", 250, 85, GFXFF);

  tft.drawRect(200, 148, 50, 17, TFT_WHITE);
  tft.drawRect(200, 149, 49, 16, TFT_BLACK);
  tft.fillTriangle(250, 165, 253, 160, 247, 160, TFT_WHITE);
  tft.drawString("Output 2", 250, 170, GFXFF);

  tft.drawRect(120, 95, 80, 70, TFT_WHITE);

  tft.drawLine(121, 112, 199, 112, TFT_GREEN);
  tft.drawLine(121, 148, 199, 148, TFT_GREEN);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSS9);
  tft.drawString("Press box to change to filter", 160, 220, GFXFF);

  drawBackSymbol();
}
