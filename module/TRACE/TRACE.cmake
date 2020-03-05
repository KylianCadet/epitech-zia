project(TRACE)

get_filename_component(TRACE_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(TRACE_SRC
	${CMAKE_CURRENT_LIST_DIR}/TRACE.cpp
	${CMAKE_CURRENT_LIST_DIR}/TRACE.hpp
)

add_library(${PROJECT_NAME} SHARED ${TRACE_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)