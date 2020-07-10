idf_component_register(SRCS "ota.cpp" "otac.c"
                       INCLUDE_DIRS "." "include"
                       EMBED_TXTFILES server_certs/cert.pem
                       PRIV_REQUIRES esp_https_ota app_update wifi
                       )