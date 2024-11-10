cmake_minimum_required(VERSION 3.22.0)
project(server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.81.0 COMPONENTS program_options REQUIRED)

set(SOURCES text_processing.cpp)
set(HEADERS settings.hpp text_processing.hpp util.hpp)

add_executable(server main.cpp ${SOURCES} ${HEADERS})

target_link_libraries(server Boost::program_options)

install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

