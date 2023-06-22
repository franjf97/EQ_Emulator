#include "AudioTools.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//State machine variables
#define MENU 0
#define ADAPT_INSTRUCTIONS_1 1
#define ADAPT_INSTRUCTIONS_2 2
#define ADAPT_SET_VOLUME 3
#define ADAPT_SELECT 4
#define ADAPT 5
#define ADAPT_SAVE 6
#define FILTER_INSTRUCTIONS 7
#define FILTER 8
#define LOOPBACK_INSTRUCTIONS 9
#define LOOPBACK 10

int gui_state;

// Display parameters

#define ROTATION_UP 1 //Pins at right
#define ROTATION_DOWN 3 //Pins at left

uint8_t rotation = ROTATION_DOWN;

// Touch constraints and variables
#define TOUCH_THRESHOLD 250
#define RAW_LOW_LIMIT 150
#define RAW_HIGH_LIMIT 4000

uint16_t x, y;
bool pressed;

//MSE Plot parameters
#define draw_step 20  //Plots one out of draw_step samples

#define scan 20000.0f //Time plotted in miliseconds
#define sample_time 5.37f //Time between samples in miliseconds

#define offset_x 20
#define offset_y 220
#define scale_x (220/N_samples)
#define scale_y 1.7

const float N_samples = round(scan / sample_time);
int value_y;
int value_x;

int samples_plot = 0;
int count_display_mse = draw_step;


//Audio variables
float volume = 0.2;  //Sets volume

typedef int16_t sound_t;                                   // sound will be represented as int16_t (with 2 bytes)
uint16_t sample_rate = 48000;
uint8_t channels = 1;                                      // The stream will have 2 channels
NoiseGenerator<sound_t> noise(32000);                       // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<sound_t> sound(noise);
I2SStream out;
VolumeStream audio_volume(sound);
StreamCopy copier(out, audio_volume);                             // copies sound into i2s

//Serial pins
#define RXD2 22
#define TXD2 23

//MSE Serial data variables
#define MAX_THEORIC_MSE 4.0f  //Mse cannot be greater, if we receive mse larger must be corrupted data
#define ALPHA 0.999f  //Recursive filter parameter

String received_mse;  //String raw data
float mse;  //Float raw data
float mse_rec;  //Filtered data
float mse_max = 0;  //Max value found
float mse_porc; //Percentage MSE (plotted)

float last_output = 0;  //Last sample for recursive filter


void setup(void)
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  //I2S Configuration
  auto config = out.defaultConfig(TX_MODE);
  config.sample_rate = sample_rate;
  config.channels = channels;
  config.bits_per_sample = 16;
  config.i2s_format = I2S_STD_FORMAT;
  config.is_master = true;
  config.port_no = 0;
  config.pin_ws = 4;
  config.pin_bck = 16;
  config.pin_data = 17;
  config.pin_data_rx = 5;
  config.pin_mck = 0;
  config.use_apll = true;
  out.begin(config);

  noise.begin();

  //Set initial volume
  audio_volume.begin(config); // we need to provide the bits_per_sample and channels
  audio_volume.setVolume(volume);

  //Screen init
  tft.init();
  tft.setRotation(rotation);
  drawInit();
  drawMenu();
}

void loop()
{
  readTouch();

  switch (gui_state)
  {
    case MENU:
      {
        if (pressed)
        {
          if ((x > 30 && x < 100) && (y > 80 && y < 150))
          {
            delay(100);
            drawInstructionsAdapt_1();
          }
          else if ((x > 220 && x < 290) && (y > 80 && y < 150))
          {
            delay(100);
            drawInstructionsFilter();
          }
          else if ((x > 125 && x < 195) && (y > 145 && y < 215))
          {
            delay(100);
            drawInstructionsLoopback();
          }
        }
      }
      break;
    case ADAPT_INSTRUCTIONS_1:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            drawMenu();
          }
          else if ((x > 110 && x < 210) && (y > 185 && y < 230))
          {
            delay(100);
            drawInstructionsAdapt_2();
          }
        }
      }
      break;
    case ADAPT_INSTRUCTIONS_2:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            drawMenu();
          }
          else if ((x > 110 && x < 210) && (y > 185 && y < 230))
          {
            delay(100);
            Serial2.print("%LB#");
            drawSetVolumeAdapt();
          }
        }
      }
      break;
    case ADAPT_SET_VOLUME:
      {
        copier.copy();
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            Serial2.print("%ST#");
            drawMenu();
          }
          else if ((x > 110 && x < 210) && (y > 185 && y < 230))
          {
            Serial2.print("%ST#");
            drawSelectAdapt();
          }
          else if ((x > 210 && x < 260) && (y > 90 && y < 140))
          {
            if (volume < 1.0)
            {
              volume += 0.001;
              audio_volume.setVolume(volume);
              drawVolumeSelect();
            }
          }
          else if ((x > 50 && x < 100 ) && (y > 90 && y < 140))
          {
            if (volume > 0.0)
            {
              volume -= 0.001;
              audio_volume.setVolume(volume);
              drawVolumeSelect();
            }
          }
        }
      }
      break;
    case ADAPT_SELECT:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            drawMenu();
          }
          else if ((x > 40 && x < 140) && (y > 90 && y < 135))
          {
            delay(100);
            mse_max = 0;
            samples_plot = 0;
            Serial2.print("%AF#");
            drawAdapt();
          }
          else if ((x > 180 && x < 280) && (y > 90 && y < 135))
          {
            delay(100);
            mse_max = 0;
            samples_plot = 0;
            Serial2.print("%AS#");
            drawAdapt();
          }
        }
      }
      break;
    case ADAPT:
      {
        copier.copy();
        drawAdaptValue();
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            Serial2.print("%ST#");
            drawMenu();
          }
          else if ((x > 280 && x < 310) && (y > 10 && y < 40))
          {
            Serial2.print("%ST#");
            drawSaveAdapt();
          }
          else if ((x > 255 && x < 305) && (y > 100 && y < 150))
          {
            if (volume < 1.0)
            {
              volume += 0.001;
              audio_volume.setVolume(volume);
            }
          }
          else if ((x > 255 && x < 305) && (y > 170 && y < 220))
          {
            if (volume > 0.0)
            {
              volume -= 0.001;
              audio_volume.setVolume(volume);
            }
          }
        }
      }
      break;
    case ADAPT_SAVE:
      {
        if (pressed)
        {
          if ((x > 40 && x < 140) && (y > 110 && y < 155))
          {
            Serial2.print("%SV#");
            delay(100);
            drawMenu();
          }
          else if ((x > 180 && x < 280) && (y > 110 && y < 155))
          {
            delay(100);
            drawMenu();
          }
        }
      }
      break;
    case FILTER_INSTRUCTIONS:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            drawMenu();
          }
          else if ((x > 110 && x < 210) && (y > 185 && y < 230))
          {
            delay(100);
            Serial2.print("%FI#");
            drawFilter();
          }
        }
      }
      break;
    case FILTER:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            Serial2.print("%ST#");
            drawMenu();
          }
          else if ((x > 120 && x < 200) && (y > 95 && y < 165))
          {
            delay(100);
            Serial2.print("%LB#");
            drawLoopback();
          }
        }
      }
      break;
    case LOOPBACK_INSTRUCTIONS:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            drawMenu();
          }
          else if ((x > 110 && x < 210) && (y > 185 && y < 230))
          {
            delay(100);
            Serial2.print("%LB#");
            drawLoopback();
          }
        }
      }
      break;
    case LOOPBACK:
      {
        if (pressed)
        {
          if ((x > 10 && x < 40) && (y > 10 && y < 40))
          {
            delay(100);
            Serial2.print("%ST#");
            drawMenu();
          }
          else if ((x > 120 && x < 200) && (y > 95 && y < 165))
          {
            delay(100);
            Serial2.print("%FI#");
            drawFilter();
          }
        }
      }
      break;
    default: Serial2.print("%ST#");
      break;
  }

}
