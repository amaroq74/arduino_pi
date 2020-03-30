BOARD=generic
UPLOAD_PORT=/dev/ttyACM0
FLASH_DEF=1M0
OTA_ADDR=sonoff4.amaroq.net

LIBS += $(ESP_LIBS)/ESP8266WiFi 
LIBS += $(ESP_LIBS)/ArduinoOTA 
LIBS += $(ESP_LIBS)/ESP8266mDNS
LIBS += $(USER_LIB_PATH)/pubsubclient 
LIBS += $(USER_LIB_PATH)/ArtNet

include $(USER_LIB_PATH)/../makeEspArduino.mk


