add_executable(${DCLOCK_TARGET} main.cpp)

set(DCLOCK_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${DCLOCK_TARGET})

add_custom_command(
        TARGET ${DCLOCK_TARGET}
        POST_BUILD
	COMMAND ln -sf ${DCLOCK_BINARY} ${CMAKE_SOURCE_DIR}/bin/digital_clock
)

