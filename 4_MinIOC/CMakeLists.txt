cmake_minimum_required(VERSION 3.12)
project(MinIOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EPICS /usr/local/epics)
set(EPICS_BASE ${EPICS}/epics-base)

find_package(Perl REQUIRED)

# MinIOC.dbd
execute_process(COMMAND ${PERL_EXECUTABLE}
  ${EPICS_BASE}/bin/linux-x86_64/dbdExpand.pl
  -I${EPICS_BASE}/dbd
  -I${EPICS}/pvxs/dbd
  -I${EPICS}/alive/dbd
  -o${PROJECT_SOURCE_DIR}/MinIOC.dbd
  base.dbd pvxsIoc.dbd aliveRecord.dbd)

# MinIOC_registerRecordDeviceDriver.cpp
execute_process(COMMAND ${PERL_EXECUTABLE}
  ${EPICS_BASE}/bin/linux-x86_64/registerRecordDeviceDriver.pl
  -o${PROJECT_SOURCE_DIR}/MinIOC_registerRecordDeviceDriver.cpp
  ${PROJECT_SOURCE_DIR}/MinIOC.dbd
  MinIOC_registerRecordDeviceDriver
  ${PROJECT_SOURCE_DIR})

# MinIOC iocsh commands
file(WRITE  iocsh.txt)
file(APPEND iocsh.txt "epicsEnvSet PVXS_LOG *=INFO\n")
file(APPEND iocsh.txt "dbLoadDatabase MinIOC.dbd\n")
file(APPEND iocsh.txt "MinIOC_registerRecordDeviceDriver pdbbase\n")
file(APPEND iocsh.txt "dbLoadRecords ${EPICS}/alive/db/alive.db 'P=MinIOC:,IOCNM=MinIOC,RHOST=127.0.0.1'\n")
file(APPEND iocsh.txt "iocInit\n")

# MinIOC
add_executable(MinIOC main.cpp MinIOC_registerRecordDeviceDriver.cpp)

# MinIOC : epics-base
target_compile_definitions(MinIOC PRIVATE USE_TYPED_RSET)
target_include_directories(MinIOC PRIVATE ${EPICS_BASE}/include)
target_include_directories(MinIOC PRIVATE ${EPICS_BASE}/include/compiler/gcc)
target_include_directories(MinIOC PRIVATE ${EPICS_BASE}/include/os/Linux)
target_link_libraries(MinIOC PRIVATE ${EPICS_BASE}/lib/linux-x86_64/libCom.so)
target_link_libraries(MinIOC PRIVATE ${EPICS_BASE}/lib/linux-x86_64/libdbCore.so)
target_link_libraries(MinIOC PRIVATE ${EPICS_BASE}/lib/linux-x86_64/libdbRecStd.so)

# MinIOC : pvxs
target_include_directories(MinIOC PRIVATE ${EPICS}/pvxs/include)
target_link_libraries(MinIOC PRIVATE ${EPICS}/pvxs/lib/linux-x86_64/libpvxs.so)
target_link_libraries(MinIOC PRIVATE ${EPICS}/pvxs/lib/linux-x86_64/libpvxsIoc.so)

# MinIOC : alive
target_include_directories(MinIOC PRIVATE ${EPICS}/alive/include)
target_link_libraries(MinIOC PRIVATE ${EPICS}/alive/lib/linux-x86_64/libalive.so)
