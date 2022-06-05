cmake_minimum_required(VERSION 3.5.1)
project(IotService)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#message("cout>>${CMAKE_EXE_LINKER_FLAGS}")
#set(CMAKE_C_FLAGS "/MT;/O2;/Ob2;/GF")
#set(CMAKE_CXX_FLAGS "/MT")
#set(CMAKE_EXE_LINKER_FLAGS "/MT;/O2;/Ob2;/GF")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MTd")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# 设置策略CMP0091为NEW，新策略
if (POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif (POLICY CMP0091)

#设置MT/MTd
#MultiThreaded ：对应MT
#MultiThreadedDLL ：对应MD
#MultiThreadedDebug：对应MTd
#MultiThreadedDebugDLL：对应MDd

#set_property(TARGET IotService PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(GLOBAL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(Qt5_SDK_DIR "D:/Qt/Qt5.12.2/5.12.2/msvc2017_static64")
set(Qt5_DIR "${Qt5_SDK_DIR}/lib/cmake/Qt5")

#对应qmake中 QT += core gui widget
#find_package(Qt5 COMPONENTS Core Gui Qml Quick Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets WebSockets REQUIRED)

#message("cout found>>${Qt5Widgets_FOUND}")

message("${Qt5Core_INCLUDE_DIRS}")
message("${Qt5Widgets_INCLUDE_DIRS}")
message("${Qt5Gui_INCLUDE_DIRS}")
message("${Qt5WebSockets_INCLUDE_DIRS}")

#message("cout include>>${Qt5Core_LIBS}")
#message("cout include>>${Qt5Widgets_LIBS}")
#message("cout include>>${Qt5Gui_LIBS}")
#message("cout include>>${Qt5WebSockets_LIBS}")

#message("cout>>${Qt5Widgets_LIBRARY_DIRS}")
#message("cout>>${Qt5::Widgets_LIBRARY}")

include_directories("${Qt5Core_INCLUDE_DIRS}")
include_directories("${Qt5Widgets_INCLUDE_DIRS}")
include_directories("${Qt5Gui_INCLUDE_DIRS}")
include_directories("${Qt5WebSockets_INCLUDE_DIRS}")
include_directories("D:/pgsql/include")

#头文件目录(是个目录的绝对路径)
#find_path(<NAME_INCLUDE_DIRS> NAMES XX.h PATHS /usr/include /usr\local/include/ ...)

#库文件(是个文件的绝对路径)
find_library(Qt5Core_LIBS NAMES Qt5Core PATHS D:/Qt/Qt5.12.2/5.12.2/msvc2017_static64/lib )
find_library(Qt5Widgets_LIBS NAMES Qt5Widgets PATHS D:/Qt/Qt5.12.2/5.12.2/msvc2017_static64/lib )
find_library(Qt5WebSockets_LIBS NAMES Qt5WebSockets PATHS D:/Qt/Qt5.12.2/5.12.2/msvc2017_static64/lib )

message("Qt5Core_LIBS:${Qt5Core_LIBS}")
message("Qt5Widgets_LIBS:${Qt5Widgets_LIBS}")
message("Qt5WebSockets_LIBS:${Qt5WebSockets_LIBS}")

SET(IotService_SOURCES
        main.cpp
        mainwindow.cpp
)

SET(IotService_HEADERS
        mainwindow.h
)

SET(IotService_FORMS
        mainwindow.ui
)

#SET(IotService_RESOURCES
#	img.qrc
#)

include_directories(SYSTEM "inc")
include_directories("./src/util")
include_directories("./src/plugs")
include_directories("./src/wsserver")
include_directories("./src/qtview")

link_directories("./curl/lib")
link_directories("$ENV{QTDIR}/bin")

#调用预编译器moc，需要使用 QT5_WRAP_CPP宏
QT5_WRAP_CPP(IotService_HEADERS_MOC ${IotService_HEADERS})

#使用uic处理.ui文件
QT5_WRAP_UI(IotService_FORMS_HEADERS ${IotService_FORMS})

#使用rcc处理.qrc文件
#QT5_ADD_RESOURCES(qui_RESOURCES_RCC ${qui_RESOURCES})

#这些生成的中间文件都会在build目录下，这样的话，编译器则不能定位由uic程序产生的诸如_ui_mainwindow.h等文件。所以，我们需要把build目录添加到包含目录中
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#STATIC 静态 #添加lib
#add_library(mylib STATIC lib/blah.cpp)

#生成可执行文件
#add_executable(hello_cmake main.cpp)

#链接库文件
#target_link_libraries(hello_cmake PUBLIC mylib)

aux_source_directory(src IOT_SOURCES)

#生成可执行文件
add_executable(IotService
        ${IotService_SOURCES}
        ${IotService_HEADERS_MOC}
        ${IotService_FORMS_HEADERS}
	${IOT_SOURCES}
        #${IotService_RESOURCES_RCC}
)

target_sources(IotService PRIVATE ${IOT_SOURCES})

#为target添加需要链接的共享库
#静态库跳过
#target_link_libraries(IotService Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${Qt5Widgets_LIBRARIES})
#target_link_libraries(IotService ${Qt5Widgets_LIBRARIES})
#message("cout>>${Qt5Widgets_LIBRARIES}")
#target_link_libraries(IotService PRIVATE Qt5::Core Qt5::Gui)

target_link_libraries(IotService PRIVATE ${Qt5Core_LIBS} ${Qt5Widgets_LIBS} ${Qt5WebSockets_LIBS})

INSTALL(TARGETS IotService
    RUNTIME DESTINATION lib
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
