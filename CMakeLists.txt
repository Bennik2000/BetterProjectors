project(better-projectors)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

if (NOT Qt5Widgets_FOUND OR NOT Qt5Core_FOUND)
    message(FATAL_ERROR "Failed to find Qt5")
endif ()

set(better-projectors_SOURCES 
	src/plugin-main.cpp 
	src/projector-dock.cpp
	src/projector-dock.hpp)

if(WIN32)
	set(MODULE_DESCRIPTION "Better projectors for OBS")
	#configure_file(${CMAKE_SOURCE_DIR}/cmake/winrc/obs-module.rc.in obs-x264.rc)
	#list(APPEND obs-x264_SOURCES
	#	obs-x264.rc)
endif()


add_library(better-projectors MODULE ${better-projectors_SOURCES})

target_include_directories(better-projectors 
	PRIVATE 
	"../../UI/obs-frontend-api"
    ${Qt5Widgets_INCLUDES}
	${Qt5Core_INCLUDES})
	
target_link_libraries(better-projectors
	libobs
    obs-frontend-api
	Qt5::Widgets
    Qt5::Core)
	
set_target_properties(better-projectors PROPERTIES FOLDER "plugins")

install_obs_plugin(better-projectors)
