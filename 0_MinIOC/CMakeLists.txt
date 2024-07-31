cmake_minimum_required(VERSION 3.12)
project(MinIOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EPICS /usr/local/epics)
set(EPICS_BASE ${EPICS}/epics-base)

# MinIOC
add_executable(MinIOC main.cpp)

# MinIOC : epics-base
target_include_directories(MinIOC PRIVATE ${EPICS_BASE}/include)
target_include_directories(MinIOC PRIVATE ${EPICS_BASE}/include/compiler/gcc)
target_link_libraries(MinIOC PRIVATE ${EPICS_BASE}/lib/linux-x86_64/libCom.so)

# MinIOC : pvxs
target_include_directories(MinIOC PRIVATE ${EPICS}/pvxs/include)
target_link_libraries(MinIOC PRIVATE ${EPICS}/pvxs/lib/linux-x86_64/libpvxs.so)
