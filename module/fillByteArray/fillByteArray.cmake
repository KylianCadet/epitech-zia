project(FillByteArray)

get_filename_component(FILL_BYTE_ARRAY_SRC_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

set(FILL_BYTE_ARRAY_SRC
	${CMAKE_CURRENT_LIST_DIR}/FillByteArray.cpp
	${CMAKE_CURRENT_LIST_DIR}/FillByteArray.hpp
)

add_library(${PROJECT_NAME} SHARED ${FILL_BYTE_ARRAY_SRC})
target_link_libraries(${PROJECT_NAME} ${openZiaLibs})
target_include_directories(${PROJECT_NAME} PRIVATE ${openZiaIncludes})

set_target_properties(${PROJECT_NAME} PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)