add_executable(${PAINT_TARGET} main.cpp)

set(PAINT_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PAINT_TARGET})

add_custom_command(
	TARGET ${PAINT_TARGET}
        POST_BUILD
        COMMAND ln -sf ${PAINT_BINARY} ${CMAKE_SOURCE_DIR}/bin/paint
)

