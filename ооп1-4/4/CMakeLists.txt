cmake_minimum_required(VERSION 3.16)

project(PROG_FOR_OOP LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти необходимые модули Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# Создание исполняемого файла
add_executable(PROG_FOR_OOP
  main.cpp
  application.h application.cpp
  array.h array.cpp
  number.h
  complex.h complex.cpp
  polynom.h polynom.cpp
  interface.h interface.cpp
)

# Связывание с библиотеками Qt
target_link_libraries(PROG_FOR_OOP Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS PROG_FOR_OOP
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
