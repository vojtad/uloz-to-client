project(uloz-to-client)
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

find_package(Qt4 REQUIRED)

include_directories(src)

set(CMAKE_BUILD_TYPE Debug)

set(QT_USE_QTCORE TRUE)
set(QT_USE_QTGUI TRUE)
set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})

add_definitions(-Wall)

set(SRCS
	src/main.cpp
	src/CDownloadDelegate.cpp
	src/CDownloadModel.cpp
	src/CMainWindow.cpp
	src/CMainWindow_actions.cpp
	src/CMainWindow_communication.cpp
	src/CNewDownloadDialog.cpp
	src/CSearchDialog.cpp
	src/CSearchModel.cpp)

set(MOC_HDRS
	src/CMainWindow.h
	src/CNewDownloadDialog.h
	src/CSearchDialog.h
	src/CSearchModel.h)

set(UIS
	src/CMainWindow.ui
	src/CNewDownloadDialog.ui
	src/CSearchDialog.ui)

set(RCS
	uloz-to-client.qrc)

qt4_wrap_ui(UIS_H ${UIS})
qt4_wrap_cpp(MOC_SRCS ${MOC_HDRS})
qt4_add_resources(RC_FILES ${RCS})

add_executable(uloz-to-client ${SRCS} ${MOC_SRCS} ${UIS_H} ${RC_FILES})
target_link_libraries(uloz-to-client ${QT_LIBRARIES})

install(TARGETS uloz-to-client DESTINATION bin)
