idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "include" "../components/bno085/include" "../components/Battery/include" "../components/LedStrip/include"
                    REQUIRES bno085 Battery LedStrip)