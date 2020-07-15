idf_component_register(SRCS "ws2811.cpp" 
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES nvs_flash 
                       )