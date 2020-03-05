project(OPTIONS)

get_filename_component(OPTIONS_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(OPTIONS_SRC
	${CMAKE_CURRENT_LIST_DIR}/OPTIONS.cpp
	${CMAKE_CURRENT_LIST_DIR}/OPTIONS.hpp
)

add_library(${PROJECT_NAME} SHARED ${OPTIONS_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)