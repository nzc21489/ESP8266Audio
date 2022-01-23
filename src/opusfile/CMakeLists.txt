project(ESP8266AudioPico)
        
add_library(opusfile STATIC info.c internal.c opusfile.c stream.c)