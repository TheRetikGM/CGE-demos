add_executable(${ACLOCK_TARGET} main.cpp)

set(ACLOCK_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${ACLOCK_TARGET})

add_custom_command(
        TARGET ${ACLOCK_TARGET}
        POST_BUILD
	COMMAND ln -sf ${ACLOCK_BINARY} ${CMAKE_SOURCE_DIR}/bin/analog_clock
)

