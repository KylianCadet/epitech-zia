project(zia)
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g3")

include(build/conanbuildinfo.cmake)
conan_basic_setup()

include(openZia/openZia/openZia.cmake)

set(ZIA_NAME zia)
set(ZIA_SRC
src/main.cpp
src/Server.cpp
src/ConfigLoader.cpp
src/misc.cpp
src/Core.cpp
)

set(UT_NAME unit_test)
set (UT_SRC
UT/main.cpp
src/misc.cpp
)

include_directories(include)

add_executable(${ZIA_NAME} ${ZIA_SRC})
target_link_libraries(${ZIA_NAME} ${CONAN_LIBS} ${openZiaLibs})
target_include_directories(${ZIA_NAME} PRIVATE ${openZiaIncludes})

add_executable(${UT_NAME} ${UT_SRC})
target_link_libraries(${UT_NAME} ${CONAN_LIBS})
add_custom_command(TARGET ${UT_NAME} POST_BUILD
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/${UT_NAME}
)

if(WIN32)
target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()


include(module/GET/GET.cmake)
include(module/PUT/PUT.cmake)
include(module/POST/POST.cmake)
include(module/DELETE/DELETE.cmake)
include(module/OPTIONS/OPTIONS.cmake)
include(module/TRACE/TRACE.cmake)
include(module/parseHeader/parseHeader.cmake)
include(module/OpenSSL/OpenSSL.cmake)
include(module/fillByteArray/fillByteArray.cmake)
include(module/PHP/PHP.cmake)

# pipeline workflow

# BeforeParse : OpenSSL::decrypt
# Parse : parseHeader::onInterpret
# OnInterpret : PHP::onInterpret
# OnInterpret : GET::onInterpret (contain HEAD)
# OnInterpret : POST::onInterpret
# OnInterpret : DELETE::onInterpret
# OnInterpret : PUT::onInterpret
# OnInterpret : TRACE::onInterpret
# OnInterpret : OPTIONS::onInterpret
# Completed (ASAP) : fillByteArray::onInterpret
# Completed (Medium) : OpenSSL::encrypt