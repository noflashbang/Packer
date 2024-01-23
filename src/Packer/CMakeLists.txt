
add_compile_definitions(AUTOSET_EXPORTS)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

set( externHeaders 
   
)

set( libHeaders 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StandardLib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Pack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ISerial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialOut.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialIn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeReg.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Type.h
)

set( libSource
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Pack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialIn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeReg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Type.cpp
)

add_library (Packer SHARED ${libSource} ${libHeaders} ${externHeaders}) 
target_include_directories( Packer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/Packer )
target_include_directories( Packer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src )

target_sources( Packer PRIVATE ${libSource} ${libHeaders} )

target_sources( Packer PUBLIC FILE_SET HEADERS BASE_DIRS ${PARENT_DIR}/include/Packer FILES ${externHeaders} )

install(TARGETS Packer
        RUNTIME DESTINATION bin          
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        FILE_SET HEADERS COMPONENT Development
)
