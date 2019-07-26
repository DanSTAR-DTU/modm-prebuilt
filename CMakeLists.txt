PROJECT(modm)

FILE(GLOB_RECURSE ASM_FILES *.sx *.S)
SET_SOURCE_FILES_PROPERTIES(${ASM_FILES} PROPERTIES LANGUAGE CXX)

MACRO(GET_SOURCES OUTPUT SRC_DIR)
	FILE(GLOB_RECURSE ${OUTPUT} ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp ${SRC_DIR}/*.sx ${SRC_DIR}/*.S)
ENDMACRO(GET_SOURCES)

GET_SOURCES(MODM_SRC modm/src)
GET_SOURCES(PRINTF_SRC modm/ext/printf/src)
GET_SOURCES(CRASHCATCHER_SRC modm/ext/crashcatcher/src)

ADD_LIBRARY(modm OBJECT ${MODM_SRC} ${PRINTF_SRC} ${CRASHCATCHER_SRC})

TARGET_INCLUDE_DIRECTORIES(modm
	PUBLIC
		modm/src
		modm/ext/cmsis/core
		modm/ext/cmsis/device
		modm/ext/crashcatcher
		modm/ext/crashcatcher/include
		modm/ext/crashcatcher/src
	PRIVATE
		modm/ext)

SET(MODM_INCLUDE
	${CMAKE_CURRENT_SOURCE_DIR}/modm/src
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/cmsis/core
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/cmsis/device
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/crashcatcher
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/crashcatcher/include
	${CMAKE_CURRENT_SOURCE_DIR}/modm/ext/crashcatcher/src
PARENT_SCOPE)

EXPORT(TARGETS modm FILE modm.cmake)
