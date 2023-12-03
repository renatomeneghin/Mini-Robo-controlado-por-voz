# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources
    ${CMAKE_SOURCE_DIR}/include/BLE.h
    ${CMAKE_SOURCE_DIR}/include/calendar.h
    ${CMAKE_SOURCE_DIR}/include/clock.h
    ${CMAKE_SOURCE_DIR}/include/clockcalendar.h
    ${CMAKE_SOURCE_DIR}/include/Node.h
    ${CMAKE_SOURCE_DIR}/include/Fila.h
    ${CMAKE_SOURCE_DIR}/include/Motor.h
    ${CMAKE_SOURCE_DIR}/include/Driver_Motores.h
    ${CMAKE_SOURCE_DIR}/include/Microfone.h
    ${CMAKE_SOURCE_DIR}/include/MicrofoneI2S.h
    ${CMAKE_SOURCE_DIR}/include/headers.h
    ${CMAKE_SOURCE_DIR}/src/calendar.cpp
    ${CMAKE_SOURCE_DIR}/src/clock.cpp
    ${CMAKE_SOURCE_DIR}/src/clockcalendar.cpp
    ${CMAKE_SOURCE_DIR}/src/Node.cpp
    ${CMAKE_SOURCE_DIR}/src/Fila.cpp
    ${CMAKE_SOURCE_DIR}/src/Motor.cpp
    ${CMAKE_SOURCE_DIR}/src/Driver_Motores.cpp
    ${CMAKE_SOURCE_DIR}/src/Microfone.cpp
    ${CMAKE_SOURCE_DIR}/src/MicrofoneI2S.cpp
    ${CMAKE_SOURCE_DIR}/src/BLE.*
    ${CMAKE_SOURCE_DIR}/src/main.* 
    #${CMAKE_SOURCE_DIR}/src/main_BLE.*
    #${CMAKE_SOURCE_DIR}/src/host.*
    )

idf_component_register(
    SRCS 
    ${app_sources}
    REQUIRES
    bt
    nvs_flash
    driver
    PRIV_REQUIRES 
    bt
    )
