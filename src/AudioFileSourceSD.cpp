/*
  AudioFileSourceSPIFFS
  Input SD card "file" to be used by AudioGenerator
  
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
#include "AudioFileSourceSD.h"

AudioFileSourceSD::AudioFileSourceSD()
{
}

AudioFileSourceSD::AudioFileSourceSD(const char *filename)
{
  open(filename);
}

bool AudioFileSourceSD::open(const char *filename)
{
  res = f_open(&f, filename, FA_OPEN_EXISTING | FA_READ);
  if (res == FR_OK){
      return true;
  }else{
      return false;
  }
}

AudioFileSourceSD::~AudioFileSourceSD()
{
  if (res == FR_OK) f_close(&f);
}

uint32_t AudioFileSourceSD::read(void *data, uint32_t len)
{
  UINT byte_read;
  res = f_read(&f, data, len, &byte_read);
  return (uint32_t) byte_read;
}

bool AudioFileSourceSD::seek(int32_t pos, int dir)
{
  if (res != FR_OK) return false;
  if (dir==SEEK_SET) return f_lseek(&f, pos)?false:true;
  else if (dir==SEEK_CUR) return f_lseek(&f, (f_tell(&f) + pos))?false:true;
  else if (dir==SEEK_END) return f_lseek(&f, (f_size(&f) + pos))?false:true;
  return false;
}

bool AudioFileSourceSD::close()
{
  f_close(&f);
  return true;
}

bool AudioFileSourceSD::isOpen()
{
  return res?false:true;
}

uint32_t AudioFileSourceSD::getSize()
{
  if (res != FR_OK) return 0;
  return (uint32_t)f_size(&f);
}

uint32_t AudioFileSourceSD::getPos()
{
  if (res != FR_OK) return 0;
  return (uint32_t)f_tell(&f);
}
