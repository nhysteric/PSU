cmake_minimum_required(VERSION 3.19)
project(PSU)

set(CMAKE_CXX_STANDARD 20)

set(libOTe_DIR extern/libOTe)

find_package(libOTe HINTS ${libOTe_DIR} REQUIRED)

include_directories(osn)

aux_source_directory(osn osn)

link_directories(extern/libOTe/cryptoTools/thirdparty/unix/lib)

# add_executable(test_benes
# test/test_benes.cpp)
add_executable(test_osn
    test/test_osn.cpp
    ${osn}
)

add_definitions(-DNO_INTEL_ASM_SHA1)
target_link_libraries(test_osn oc::libOTe relic)
target_include_directories(test_osn PUBLIC ${libOTe_DIR})

# target_link_libraries(test_benes benes ${MY_LIBS} ${Boost_LIBRARIES})