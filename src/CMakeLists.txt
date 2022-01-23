project(ESP8266AudioPico)

add_library(ESP8266AudioPico STATIC
        AudioFileSourceSD.cpp
        AudioGeneratorFLAC.cpp
        AudioGeneratorWAV.cpp
        AudioGeneratorMP3.cpp
        AudioGeneratorAAC.cpp
        AudioGeneratorOpus.cpp
        AudioFileSourceID3.cpp
        AudioOutputI2S.cpp
        )
    target_compile_definitions(ESP8266AudioPico PRIVATE
            PICO_AUDIO_I2S_MONO_OUTPUT=0
            PICO_AUDIO_I2S_MONO_INPUT=0
    )

target_link_libraries(ESP8266AudioPico fatfs sd_pico pico_audio_i2s_sender pico_stdlib pico_multicore libflac libmad libhelix-aac opusfile libopus libogg)


add_library(ESP8266AudioPico_NO_SOFT_VOL STATIC
        AudioFileSourceSD.cpp
        AudioGeneratorFLAC.cpp
        AudioGeneratorWAV.cpp
        AudioGeneratorMP3.cpp
        AudioGeneratorAAC.cpp
        AudioGeneratorOpus.cpp
        AudioFileSourceID3.cpp
        AudioOutputI2S.cpp
        )
    target_compile_definitions(ESP8266AudioPico_NO_SOFT_VOL PRIVATE
            PICO_AUDIO_I2S_MONO_OUTPUT=0
            PICO_AUDIO_I2S_MONO_INPUT=0
            NO_SOFT_VOL=1
    )

target_link_libraries(ESP8266AudioPico_NO_SOFT_VOL fatfs sd_pico pico_audio_i2s_sender pico_stdlib pico_multicore libflac libmad libhelix-aac opusfile libopus libogg)



set_source_files_properties(AudioGeneratorFLAC.cpp PROPERTIES COMPILE_FLAGS -fpermissive)

set(INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/libflac
  ${PROJECT_SOURCE_DIR}/libmad
  ${PROJECT_SOURCE_DIR}/libhelix-aac
  ${PROJECT_SOURCE_DIR}/libogg
  ${PROJECT_SOURCE_DIR}/libopus
  ${PROJECT_SOURCE_DIR}/libopus/celt
  ${PROJECT_SOURCE_DIR}/libopus/silk
  ${PROJECT_SOURCE_DIR}/libopus/silk/fixed
  ${PROJECT_SOURCE_DIR}/pusfile
  ${PICO_SDK_PATH}
)

include_directories("${INCLUDE_DIR}")

add_subdirectory(libflac)
add_subdirectory(libmad)
add_subdirectory(libhelix-aac)
add_subdirectory(libogg)
add_subdirectory(libopus)
add_subdirectory(opusfile)
