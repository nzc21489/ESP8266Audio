/*
  AudioOutputI2S
  Base class for I2S interface port
  
  Copyright (C) 2017  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma GCC optimize("O2")
#include "AudioOutputI2S.h"
#include "pico_i2s.h"
#include <cstring>

int NUM_BUFFER = (4096 * 4);

int32_t *sample_origin = NULL;
int16_t *sample_gained_16 = NULL;
int32_t sample_gained_32;

AudioOutputI2S::AudioOutputI2S(uint16_t buffer_count)
{
  NUM_BUFFER = buffer_count;
  //set defaults
  mono = false;
  bps = 16;
  channels = 2;
  hertz = 44100;
  SetGain(1.0);
}

AudioOutputI2S::~AudioOutputI2S()
{
#ifdef ESP32
  if (i2sOn)
  {
    audioLogger->printf("UNINSTALL I2S\n");
    i2s_driver_uninstall((i2s_port_t)portNo); //stop & destroy i2s driver
  }
#else
  if (i2sOn)
  {
    buf_num = 0;
    buff_select = 0;
    deinit_i2s();
  }

#endif
  i2sOn = false;
}

bool AudioOutputI2S::SetRate(int hz)
{
  // TODO - have a list of allowable rates from constructor, check them
  this->hertz = hz;
  if (i2sOn)
  {
#ifdef ESP32
    i2s_set_sample_rates((i2s_port_t)portNo, AdjustI2SRate(hz));
#else
    //       i2s_set_rate(AdjustI2SRate(hz));
#endif
  }
  return true;
}

bool AudioOutputI2S::SetBitsPerSample(int bits)
{
  if ((bits != 16) && (bits != 8) && (bits != 24) && (bits != 32))
    return false;
  this->bps = bits;
  return true;
}

bool AudioOutputI2S::SetChannels(int channels)
{
  if ((channels < 1) || (channels > 2))
    return false;
  this->channels = channels;
  return true;
}

bool AudioOutputI2S::SetOutputModeMono(bool mono)
{
  this->mono = mono;
  return true;
}

bool AudioOutputI2S::begin(bool txDAC)
{
  if (!i2sOn)
  {
    i2s_buff_size = NUM_BUFFER;
    if (bps == 16)
    {
      init_i2s(bps);
    }
    else
    {
      init_i2s(32);
    }
    int_count_i2s = 0;
    i2s_buff_count = 1;
    buf_num = 0;
    buff_select = 0;
  }

  i2sOn = true;
  //   SetRate(hertz); // Default
  return true;
}

bool AudioOutputI2S::ConsumeSample(int16_t sample[2])
{
  //return if we haven't called ::begin yet
  if (!i2sOn)
    return false;

  if (i2s_buff_count > 1)
  {
    while (int_count_i2s < (i2s_buff_count - 0))
    {
    }
  }
  else // first buffer
  {
    // set 0 to i2s_buff to avoid noise
    memset(&i2s_buff[0][0], 0, i2s_buff_size * 2);
    memset(&i2s_buff[1][0], 0, i2s_buff_size * 2);
  }

  if (bps <= 16)
  {
#ifdef NO_SOFT_VOL
    i2s_buff[(i2s_buff_count +1) % 2][buf_num + 0] = sample[0];
    i2s_buff[(i2s_buff_count +1) % 2][buf_num + 1] = sample[1];
#else
    i2s_buff[(i2s_buff_count +1) % 2][buf_num + 0] = int16_t(float(sample[0]) * gain);
    i2s_buff[(i2s_buff_count +1) % 2][buf_num + 1] = int16_t(float(sample[1]) * gain);
#endif
    buf_num += 2;
  }
  else // bps = 32
  {
    if (buff_select == 0)
    {
#ifdef NO_SOFT_VOL
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 1] = sample[1];
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 0] = sample[0];
#else
      sample_origin = (int32_t *)sample;
      sample_gained_32 = (int32_t)((double)(*sample_origin) * gain);
      sample_gained_16 = (int16_t *)&sample_gained_32;
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 1] = sample_gained_16[1];
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 0] = sample_gained_16[0];
#endif
    }
    else
    {
#ifdef NO_SOFT_VOL
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 3] = sample[1];
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 2] = sample[0];
#else
      sample_origin = (int32_t *)sample;
      sample_gained_32 = (int32_t)((double)(*sample_origin) * gain);
      sample_gained_16 = (int16_t *)&sample_gained_32;
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 3] = sample_gained_16[1];
      i2s_buff[(i2s_buff_count +1) % 2][buf_num + 2] = sample_gained_16[0];
#endif
    }

    if (buff_select == 1)
    {
      buff_select = 0;
      buf_num += 4;
    }
    else
    {
      buff_select++;
    }
  }

  if (buf_num > i2s_buff_size)
  {
    if (i2s_buff_count == 1)
    {
      i2s_start();
    }

    buff_select = 0;
    buf_num = 0;

    i2s_buff_count++;

    if (int_count_i2s >= i2s_buff_count)
    {
      i2s_buff_count = int_count_i2s + 1;
    }

    return false;
  }
  return true;
}

void AudioOutputI2S::flush()
{
#ifdef ESP32
  // makes sure that all stored DMA samples are consumed / played
  int buffersize = 64 * this->dma_buf_count;
  int16_t samples[2] = {0x0, 0x0};
  for (int i = 0; i < buffersize; i++)
  {
    while (!ConsumeSample(samples))
    {
      delay(10);
    }
  }
#endif
}

bool AudioOutputI2S::stop()
{
  if (!i2sOn)
    return false;

#ifdef ESP32
  i2s_zero_dma_buffer((i2s_port_t)portNo);
#endif
  return true;
}
