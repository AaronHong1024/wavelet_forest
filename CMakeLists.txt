cmake_minimum_required(VERSION 3.22)
project(wavelet_forest)

set(CMAKE_CXX_STANDARD 17)

add_executable(wavelet_forest main.cpp random_file_generator.cpp)

include(FetchContent)

# sdsl
FetchContent_Declare(
        sdsl
        GIT_REPOSITORY https://github.com/simongog/sdsl-lite
)
FetchContent_GetProperties(sdsl)
if(NOT sdsl_POPULATED)
    FetchContent_Populate(sdsl)
    set(GENERATE_DOC OFF CACHE BOOL "Do not generate doxygen for sdsl-lite")
    add_subdirectory(${sdsl_SOURCE_DIR} ${sdsl_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# divsuffsort
FetchContent_Declare(
        divsufsort
        GIT_REPOSITORY https://github.com/simongog/libdivsufsort.git
        GIT_TAG        2.0.1
)
FetchContent_GetProperties(divsufsort)
if(NOT divsufsort_POPULATED)
    FetchContent_Populate(divsufsort)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build a shared library for libdivsufsort")
    set(BUILD_EXAMPLES OFF CACHE BOOL "Do not build libdivsufsort example")
    set(BUILD_DIVSUFSORT64 ON CACHE BOOL "Build libdivsufsort in 64-bits mode")
    add_subdirectory(${divsufsort_SOURCE_DIR} ${divsufsort_BINARY_DIR} EXCLUDE_FROM_ALL)
    target_include_directories(divsufsort PUBLIC "${divsufsort_BINARY_DIR}/include")
    target_include_directories(divsufsort64 PUBLIC "${divsufsort_BINARY_DIR}/include")
endif()

# malloc count
FetchContent_Declare(
        malloc_count
        GIT_REPOSITORY https://github.com/bingmann/malloc_count.git
        GIT_TAG        v0.7.1
)
FetchContent_GetProperties(malloc_count)
if(NOT malloc_count_POPULATED)
    FetchContent_Populate(malloc_count)
    add_library(malloc_count OBJECT ${malloc_count_SOURCE_DIR}/malloc_count.c ${malloc_count_SOURCE_DIR}/malloc_count.h)
    target_include_directories(malloc_count PUBLIC "${malloc_count_SOURCE_DIR}")
    target_link_libraries(malloc_count dl)
endif()

#include cli11
FetchContent_Declare(
        cli11
        GIT_REPOSITORY "https://github.com/CLIUtils/CLI11.git"
        GIT_TAG "v1.9.0"
)
FetchContent_GetProperties(cli11)
if(NOT cli11_POPULATED)
    FetchContent_Populate(cli11)
endif()
include_directories(${cli11_SOURCE_DIR}/include)
# Openmp
find_package(OpenMP)
if(OpenMP_FOUND OR OPENMP_FOUND)
    message(STATUS "HAVE_OPENMP: OpenMP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(OMP_LIBRARIES "${OpenMP_CXX_LIBRARIES}")
elseif(APPLE AND EXISTS "/opt/local/lib/libomp")
    # official Apple compiler with macports' libomp
    message(STATUS "HAVE_OPENMP: OpenMP[MacPorts]")
    set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp -I/opt/local/include/libomp")
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp -I/opt/local/include/libomp")
    set(OMP_LIBRARIES "-L/opt/local/lib/libomp -lomp")
elseif(APPLE AND EXISTS "/usr/local/opt/libomp")
    # official Apple compiler with homebrew's kegg libomp
    message(STATUS "HAVE_OPENMP: OpenMP[Homebrew kegg]")
    set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include")
    set(OMP_LIBRARIES "-L/usr/local/opt/libomp/lib -lomp")
elseif(APPLE AND EXISTS "/usr/local/lib/libomp.dylib")
    # official Apple compiler with homebrew's libomp
    message(STATUS "HAVE_OPENMP: OpenMP[Homebrew]")
    set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")
    set(OMP_LIBRARIES "-L/usr/local/lib -lomp")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND EXISTS "/usr/local/lib64/libgomp.a")
    # custom static openmp for official linux builds
    message(STATUS "HAVE_OPENMP: OpenMP[static]")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Xpreprocessor -fopenmp")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp")
    set(OMP_LIBRARIES "/usr/local/lib64/libgomp.a")
else()
    message(STATUS "HAVE_OPENMP: NOT FOUND")
endif()


target_include_directories(wavelet_forest PRIVATE  ${cli11_SOURCE_DIR}/include)
target_link_libraries(wavelet_forest PRIVATE malloc_count sdsl divsufsort divsufsort64)
