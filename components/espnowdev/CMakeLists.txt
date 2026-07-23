if(CONFIG_ESPNOW_INITIATOR)
    list(APPEND srcs "initiator.c")
endif()

if(CONFIG_ESPNOW_RESPONDER)
    list(APPEND srcs "responder.c")
endif()

idf_component_register(
    
    SRCS "${srcs}"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash esp_event esp_wifi
)