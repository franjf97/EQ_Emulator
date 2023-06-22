void readTouch()
{
  uint16_t x_raw, y_raw;
  tft.getTouchRaw(&y_raw, &x_raw);
  if  (tft.getTouchRawZ() > TOUCH_THRESHOLD)
    pressed = true;
  else
    pressed = false;
  if (rotation == ROTATION_UP)
  {
    x = map(x_raw, RAW_HIGH_LIMIT, RAW_LOW_LIMIT, 0, 320);
    y = map(y_raw, RAW_HIGH_LIMIT, RAW_LOW_LIMIT, 0, 240);
  }
  else
  {
    x = map(x_raw, RAW_LOW_LIMIT, RAW_HIGH_LIMIT, 0, 320);
    y = map(y_raw, RAW_LOW_LIMIT, RAW_HIGH_LIMIT, 0, 240);
  }
}
