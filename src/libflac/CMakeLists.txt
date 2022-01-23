project(ESP8266AudioPico)
        
add_library(libflac STATIC bitmath.c bitreader.c cpu.c crc.c fixed.c float.c format.c lpc.c md5.c memory.c window.c stream_decoder.c)