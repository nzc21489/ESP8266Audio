project(ESP8266AudioPico)

add_library(libhelix-aac STATIC trigtabs.c tns.c stproc.c sbrtabs.c sbrside.c sbrqmf.c sbrmath.c sbrimdct.c sbrhuff.c sbrhfgen.c sbrhfadj.c sbrfreq.c sbrfft.c sbr.c pns.c noiseless.c imdct.c hufftabs.c huffman.c filefmt.c fft.c dequant.c decelmnt.c dct4.c buffers.c bitstream.c aactabs.c aacdec.c)

target_link_libraries(libhelix-aac pico_stdlib)