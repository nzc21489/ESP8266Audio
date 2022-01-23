project(ESP8266AudioPico)
        
add_library(libopus STATIC mapping_matrix.c opus_decoder.c opus_encoder.c opus_multistream.c opus_projection_decoder.c opus_projection_encoder.c opus.c repacketizer.c opus_multistream_decoder.c opus_multistream_encoder.c)

target_link_libraries(libopus celt silk)

add_subdirectory(celt)
add_subdirectory(silk)