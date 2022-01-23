project(ESP8266AudioPico)
        
add_library(celt STATIC 
bands.c
celt.c
celt_decoder.c
celt_encoder.c
celt_lpc.c
cwrs.c
entcode.c
entdec.c
entenc.c
kiss_fft.c
laplace.c
mathops.c
mdct.c
modes.c
pitch.c
quant_bands.c
rate.c
vq.c
)