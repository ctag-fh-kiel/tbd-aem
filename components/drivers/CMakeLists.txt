if(CONFIG_TBD_PLATFORM_MK2)
    idf_component_register(SRCS fs.cpp led_rgb.cpp codec.cpp display.cpp mk2.cpp
            INCLUDE_DIRS "."
            REQUIRES driver spiffs tarablessd1306)
else()
    idf_component_register(SRCS gpio.cpp adc.cpp fs.cpp led_rgb.cpp codec.cpp display.cpp
            INCLUDE_DIRS "."
            REQUIRES ulp driver spiffs tarablessd1306)

    set(ulp_app_name ulp_${COMPONENT_NAME})

    if(CONFIG_TBD_PLATFORM_STR)
        set(ulp_s_sources ulp/mcp3208.S)
    else()
        set(ulp_s_sources ulp/adc.S)
    endif()

    set(ulp_exp_dep_srcs "adc.cpp")

    ulp_embed_binary(${ulp_app_name} ${ulp_s_sources} ${ulp_exp_dep_srcs})
endif()


