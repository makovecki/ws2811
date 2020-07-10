idf_component_register(SRCS "wifi.cpp"
                            "tcpserver.cpp"
                            "udpserver.cpp"
                            "tcpclient.cpp"
                            "echoprocessor.cpp"
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES nvs_flash esp_netif ota storage
                       )