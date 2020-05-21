project(better-projectors)

if(WIN32)
	set(MODULE_DESCRIPTION "Better projectors for OBS")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if (NOT Qt5Widgets_FOUND OR NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Failed to find Qt5")
endif ()


set(better-projectors_UI
        src/ui/window-add-projector.ui)


qt5_wrap_ui(better-projectors_UI_HEADERS ${better-projectors_UI})
#qt5_add_resources(...)


set(better-projectors_SOURCES 
	src/plugin-main.cpp 
	src/projector-dock.cpp
	src/projector-dock.hpp
	src/projector-widget.cpp
	src/projector-widget.hpp
	src/ui/window-add-projector.cpp
	src/ui/window-add-projector.hpp
	${better-projectors_UI_HEADERS})

add_library(better-projectors 
	MODULE 
	${better-projectors_SOURCES})
	
MESSAGE(${better-projectors_UI_HEADERS} " <-----")

target_include_directories(better-projectors 
	PRIVATE 
	"../../UI/obs-frontend-api"
	${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Widgets_INCLUDES}
	${Qt5Core_INCLUDES})
	
target_link_libraries(better-projectors
	libobs
    obs-frontend-api
	Qt5::Widgets
    Qt5::Core)
	
set_target_properties(better-projectors PROPERTIES FOLDER "plugins")

install_obs_plugin(better-projectors)
