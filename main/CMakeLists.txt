idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "include" "../components/bno085/include" "../components/Battery/include" "../components/LedStrip/include"
<<<<<<< HEAD
=======
                    PRIV_REQUIRES esp_wifi nvs_flash
>>>>>>> a94b0f0 (update)
                    REQUIRES bno085 Battery LedStrip)