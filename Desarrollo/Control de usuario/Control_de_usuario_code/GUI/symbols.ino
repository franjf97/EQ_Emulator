void drawBackSymbol()
{
  tft.fillCircle(25, 25, 15, TFT_WHITE);

  tft.drawLine(18, 25, 35, 25, TFT_BLACK);
  tft.drawLine(18, 24, 35, 24, TFT_BLACK);
  tft.drawLine(18, 26, 35, 26, TFT_BLACK);

  tft.drawLine(18, 25, 25, 32, TFT_BLACK);
  tft.drawLine(18, 24, 25, 31, TFT_BLACK);
  tft.drawLine(18, 26, 25, 33, TFT_BLACK);

  tft.drawLine(18, 25, 25, 18, TFT_BLACK);
  tft.drawLine(18, 24, 25, 17, TFT_BLACK);
  tft.drawLine(18, 26, 25, 19, TFT_BLACK);
}

void drawVolumeSelect()
{
  tft.fillRect(130, 108, 60, 30, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(FSS18);
  tft.drawFloat(volume * 100, 0, 160, 120, GFXFF);
}

void drawVolume()
{
  tft.fillRect(250, 88, 60, 20, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FSS9);
  tft.drawFloat(volume * 100, 0, 280, 95, GFXFF);
}

void drawDoneSymbol()
{
  tft.fillCircle(295, 25, 15, TFT_WHITE);

  tft.drawLine(285, 25, 292, 32, TFT_BLACK);
  tft.drawLine(285, 24, 292, 31, TFT_BLACK);
  tft.drawLine(285, 26, 292, 33, TFT_BLACK);

  tft.drawLine(292, 32, 303, 18, TFT_BLACK);
  tft.drawLine(292, 31, 303, 17, TFT_BLACK);
  tft.drawLine(292, 33, 303, 19, TFT_BLACK);
}
