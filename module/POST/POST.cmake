project(POST)

get_filename_component(POST_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(POST_SRC
	${CMAKE_CURRENT_LIST_DIR}/POST.cpp
	${CMAKE_CURRENT_LIST_DIR}/POST.hpp
)

add_library(${PROJECT_NAME} SHARED ${POST_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)