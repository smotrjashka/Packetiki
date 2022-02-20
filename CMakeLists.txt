cmake_minimum_required(VERSION 3.18)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_executable(bulk main.cpp proccom.cpp proccom.h storage.cpp storage.h thread_handler.cpp thread_handler.h)

target_link_libraries(bulk
        ${CMAKE_THREAD_LIBS_INIT}
        )

set_target_properties(
        bulk PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)


include(CPack)
