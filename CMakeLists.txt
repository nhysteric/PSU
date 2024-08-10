cmake_minimum_required(VERSION 3.19)
project(PSU)

set(CMAKE_CXX_STANDARD 20)

set(libOTe_DIR extern/libOTe)

find_package(libOTe HINTS ${libOTe_DIR} REQUIRED)

include_directories(osn)
include_directories(mp_oprf)
include_directories(common)
include_directories(shuffle_receiver)
include_directories(shuffle_receiver_cuckoo)

include_directories(shuffle_sender)
include_directories(shuffle_sender_cuckoo)

aux_source_directory(osn osn)
aux_source_directory(mp_oprf mp_oprf)
aux_source_directory(common DIR_COMMON)
aux_source_directory(shuffle_receiver_cuckoo src)
aux_source_directory(shuffle_receiver sr)
aux_source_directory(shuffle_sender ss)
aux_source_directory(shuffle_sender_cuckoo ssc)
link_directories(extern/libOTe/cryptoTools/thirdparty/unix/lib)
# add_executable(test_benes
# test/test_benes.cpp)
add_executable(test_osn
        test/test_osn.cpp
        ${DIR_COMMON}
        ${osn}
        ${mp_oprf}
        ${sr}
        ${src}
        ${ss}
        ${ssc}
)

add_executable(test_psu
        test/test_psu.cpp
        ${DIR_COMMON}
        ${osn}
        ${mp_oprf}
        ${sr}
        ${src}
        ${ss}
        ${ssc})
add_definitions(-DNO_INTEL_ASM_SHA1)
target_link_libraries(test_psu oc::libOTe relic)
target_link_libraries(test_osn oc::libOTe relic)
target_include_directories(test_psu PUBLIC ${libOTe_DIR})
target_include_directories(test_osn PUBLIC ${libOTe_DIR})

# target_link_libraries(test_benes benes ${MY_LIBS} ${Boost_LIBRARIES})