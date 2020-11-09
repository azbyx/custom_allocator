cmake_minimum_required(VERSION 3.2)

set(NAME_EXECUTABLE "custom_tuple")
set(NAME_TESTS "gtest")

if($ENV{TRAVIS_BUILD_NUMBER})
	project(${NAME_EXECUTABLE} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER}  LANGUAGES CXX)
else()
	project(${NAME_EXECUTABLE} VERSION 0.0.1  LANGUAGES CXX)
endif()

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest")

add_executable(${NAME_EXECUTABLE} ./source/main.cpp ./header/custom_tuple.h)
#add_executable(${NAME_TESTS} ./source/gtests.cpp)

set_target_properties(${NAME_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${NAME_EXECUTABLE}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/header"
)

if (MSVC)
    
#    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(${NAME_EXECUTABLE} PRIVATE /W4)
#    target_compile_options(${NAME_TESTS} PRIVATE /W4)
else()
    target_compile_options(${NAME_EXECUTABLE} PRIVATE -Wall -Wextra -pedantic -Werror)
#    target_compile_options(${NAME_TESTS} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS ${NAME_EXECUTABLE} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT azbyx@inbox.ru)

include(CPack)

#enable_testing()

#add_test(gltest gltest)
