set(COMPONENT_REQUIRES "tmc2209")
idf_component_register(SRCS "main.cpp" "movement.c"
                       INCLUDE_DIRS ".")
