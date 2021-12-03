cmake_minimum_required(VERSION 3.13.4)

project("cge_demos")
set(CMAKE_CXX_STANDARD 17)

# DOWNLOAD SUBMODULES
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if (GIT_SUBMODULE)
                message(STATUS "Submodule update")
                execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
				RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if (NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMODULE_RESULT}.")
		endif()
	endif()
endif()

# CHECK ALL THE SUBMODULES
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/Physics2D/CMakeLists.txt")
	message(FATAL_ERROR "The Physics2D submodule was not downloaded!")
endif()

set(PAINT_TARGET "paint_main")
set(PHYSICS_TARGET "physics_main")

include_directories(
	PUBLIC include
	PUBLIC external/Physics2D/include
	PUBLIC external/cge
)

add_subdirectory("paint")
add_subdirectory("external/Physics2D")
add_subdirectory("physics")

target_link_libraries(${PAINT_TARGET} ncurses)
target_link_libraries(${PHYSICS_TARGET} ncurses Physics2D)

# Create bin folder
execute_process(COMMAND mkdir -p bin WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
