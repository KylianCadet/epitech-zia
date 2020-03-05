project(PHP)

get_filename_component(PHP_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(PHP_SRC
	${CMAKE_CURRENT_LIST_DIR}/PHP.cpp
	${CMAKE_CURRENT_LIST_DIR}/PHP.hpp
)

add_library(${PROJECT_NAME} SHARED ${PHP_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)

if(WIN32)
target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()