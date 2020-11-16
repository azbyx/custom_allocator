cmake_minimum_required(VERSION 3.2)

set(NAME_EXECUTABLE "custom_allocator")
set(NAME_TEST "tests_for_list")

if($ENV{TRAVIS_BUILD_NUMBER})
	project(${NAME_EXECUTABLE} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER}  LANGUAGES CXX)
else()
	project(${NAME_EXECUTABLE} VERSION 0.0.1  LANGUAGES CXX)
endif()

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest"
    "googletest"
)


add_executable(${NAME_EXECUTABLE} 
	./main.cpp 
	./headers/Custom_allocator.h
	./headers/list_struct.h
	./headers/simple_strategy.h
	./headers/extensible_simple_strategy.h
	./headers/extensible_realoc_strategy.h
)

add_executable(${NAME_TEST} 
	./gtests.cpp 
	./headers/Custom_allocator.h
	./headers/list_struct.h
	./headers/extensible_realoc_strategy.h
)

target_link_libraries(${NAME_TEST} PRIVATE
    gtest
)

set_target_properties(${NAME_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${NAME_EXECUTABLE}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers"
)

target_include_directories(${NAME_TEST}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/headers"
)

if (MSVC)
    target_compile_options(${NAME_EXECUTABLE} PRIVATE /W4)
    target_compile_options(${NAME_TEST} PRIVATE /W4)
else()
    target_compile_options(${NAME_EXECUTABLE} PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(${NAME_TEST} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS ${NAME_EXECUTABLE} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT azbyx@inbox.ru)

include(CPack)

enable_testing()

add_test(${NAME_TEST} ${NAME_TEST})
