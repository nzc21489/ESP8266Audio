project(ESP8266AudioPico)
        
add_library(libmad STATIC version.c timer.c synth.c stream.c layer3.c huffman.c frame.c fixed.c decoder.c bit.c)

target_compile_definitions(libmad PUBLIC HAVE_UNISTD_H HAVE_WAITPID HAVE_FCNTL HAVE_PIPE HAVE_FORK)