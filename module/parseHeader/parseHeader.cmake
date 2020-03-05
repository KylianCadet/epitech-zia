project(parseHeader)

get_filename_component(PARSE_HEADER_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(PARSE_HEADER_SRC
	${CMAKE_CURRENT_LIST_DIR}/parseHeader.cpp
	${CMAKE_CURRENT_LIST_DIR}/parseHeader.hpp
)

add_library(${PROJECT_NAME} SHARED ${PARSE_HEADER_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)