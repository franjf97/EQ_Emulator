float recursiveFilter(float sample)
{
  float output;

  output = ALPHA * last_output + (1 - ALPHA) * sample;
  last_output = output;

  return output;
}

void readSerial()
{
  if (Serial2.available() > 0) {
    received_mse = cleanString(Serial2.readStringUntil('\n'));
    if (received_mse != "-1")
    {
      mse = received_mse.toFloat();
      if (mse < MAX_THEORIC_MSE)
      {
        mse_rec = recursiveFilter(mse);
        if (mse_rec > mse_max)
        {
          mse_max = mse_rec;
        }
        mse_porc = (mse_rec / mse_max) * 100;
      }
    }
  }
}

String cleanString(String received_word)
{
  String interpreted_word = "";
  int start;
  int end;

  start = received_word.indexOf('&');
  end = received_word.indexOf('#', start);

  if ((start != -1) && (end != -1))
  {
    interpreted_word = received_word.substring(start + 1, end);
  }
  else
  {
    interpreted_word = "-1";
  }

  return interpreted_word;
}
