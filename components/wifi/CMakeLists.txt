idf_component_register(SRCS "wifi.cpp"              
                            "udpserver.cpp"
                            "echoprocessor.cpp"
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES nvs_flash esp_netif ota storage
                       )