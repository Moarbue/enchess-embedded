set(COMPONENT_REQUIRES "tmc2209" "ArduinoJson")
idf_component_register(SRCS "recognition.cpp" "BLE.cpp" "main.cpp" "movement.c"
                       INCLUDE_DIRS ".")
