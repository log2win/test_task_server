cmake_minimum_required(VERSION 3.16.0)
project(server VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.78.0 COMPONENTS program_options REQUIRED)


add_executable(server main.cpp)

target_link_libraries(server Boost::program_options)

include(GNUInstallDirs)
install(TARGETS server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

