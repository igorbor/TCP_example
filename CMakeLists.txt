project (test_tcp)

# set (CMAKE_VERBOSE_MAKEFILE ON)

set (COMMON_CFLAGS_WARNINGS "-Wall -Wno-format -Wno-unknown-pragmas -Wno-long-long ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wchar-subscripts -Wcomment -Wformat ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wformat-nonliteral -Wformat-security ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wformat=2 -Wmissing-braces -Wparentheses ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wreturn-type -Wswitch -Wtrigraphs -Wunused ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow ")
set (COMMON_CFLAGS_WARNINGS "${COMMON_CFLAGS_WARNINGS} -Wwrite-strings -Wconversion -fno-inline")

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_C_COMPILER_VERSION VERSION_GREATER "4")
set (COMMON_CFLAGS_WARNINGS_GCC_4_P "-Winit-self -Wswitch-default -Wswitch-enum -Wendif-labels")
endif()

set (CXXFLAGS "-Woverloaded-virtual -fno-implement-inlines -fno-implicit-inline-templates -fno-default-inline")

set (MY_DEBUG_FLAGS " -g -DDEBUG_CODE=1")


set (CMAKE_C_FLAGS "${MY_DEBUG_FLAGS} ${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter ")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror-implicit-function-declaration ")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CFLAGS_WARNINGS} ${COMMON_CFLAGS_WARNINGS_GCC_4_P}")

set (CMAKE_CXX_FLAGS "${MY_DEBUG_FLAGS} ${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CFLAGS_WARNINGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CFLAGS_WARNINGS_GCC_4_P} ${CXXFLAGS}")

add_executable (test_tcp test_tcp.c)
