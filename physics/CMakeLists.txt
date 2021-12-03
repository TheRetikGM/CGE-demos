add_executable(${PHYSICS_TARGET} main.cpp)

set(PHYSICS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PHYSICS_TARGET})

add_custom_command(
	TARGET ${PHYSICS_TARGET}
        POST_BUILD
	COMMAND ln -sf ${PHYSICS_BINARY} ${CMAKE_SOURCE_DIR}/bin/physics
)

