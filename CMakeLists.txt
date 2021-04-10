cmake_minimum_required (VERSION 3.10)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_BUILD_TYPE Debug)
set (EXECUTABLE_OUTPUT_PATH "..")

# Project name
project (sim-camada-fisica)

set (SOURCE_DIR src)
set (INCLUDE_DIR include)

set (HEADER_FILES 
    ${INCLUDE_DIR}/CamadaEnlace.hpp
    ${INCLUDE_DIR}/CamadaFisica.hpp
)   
set (SOURCE_FILES 
    ${SOURCE_DIR}/CamadaEnlace.cpp
    ${SOURCE_DIR}/CamadaFisica.cpp
    ${SOURCE_DIR}/Simulador.cpp
)
# Executable name
add_executable (sim-camada-fisica ${DOMAINS_SOURCE_FILES}
                            ${SOURCE_FILES}
)
target_include_directories(sim-camada-fisica PUBLIC ${INCLUDE_DIR})
