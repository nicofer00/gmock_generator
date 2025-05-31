add_library(SERIAL_LIB
    serial/src/serial.cc
    serial/src/impl/unix.cc
    serial/src/impl/win.cc
    serial/src/impl/list_ports/list_ports_linux.cc
    serial/src/impl/list_ports/list_ports_osx.cc
    serial/src/impl/list_ports/list_ports_win.cc
    interface/src/serialInterface.cpp
    interface/src/ftdiInterface.cpp
    interface/src/serialFinder.cpp
    ftdi/src/ftdi.cpp)

target_include_directories(SERIAL_LIB PUBLIC
    serial/include/
    serial/include/serial/impl/
    interface/include
    ftdi/include
    driver)

set(SERIAL_DIR /test/)

if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
    target_link_libraries(SERIAL_LIB PUBLIC
        ${FOUNDATION_LIBRARY}
        ${IOKIT_LIBRARY})
elseif(UNIX)
    target_link_libraries(SERIAL_LIB PUBLIC
        rt
        pthread
        ${CMAKE_SOURCE_DIR}${SERIAL_DIR}serial/ftdi/src/libftd2xx.a)
else()
    target_link_libraries(SERIAL_LIB PUBLIC
        setupapi
        pthread
        ${CMAKE_SOURCE_DIR}${SERIAL_DIR}serial/ftdi/src/ftd2xx.dll)
endif()
