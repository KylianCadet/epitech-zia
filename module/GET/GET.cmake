project(GET)

get_filename_component(GET_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(GET_SRC
	${CMAKE_CURRENT_LIST_DIR}/GET.cpp
	${CMAKE_CURRENT_LIST_DIR}/GET.hpp
)

add_library(${PROJECT_NAME} SHARED ${GET_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)