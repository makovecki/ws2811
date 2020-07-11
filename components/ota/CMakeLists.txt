idf_component_register(SRCS "ota.cpp" "otac.c"
                       INCLUDE_DIRS "." "include"
                       PRIV_REQUIRES esp_https_ota app_update wifi
                       )
#EMBED_TXTFILES server_certs/cert.pem