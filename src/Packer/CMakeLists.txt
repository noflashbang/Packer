
add_compile_definitions(AUTOSET_EXPORTS)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

set( externHeaders 
	   ${PARENT_DIR}/include/Packer/IPack.h
       ${PARENT_DIR}/include/Packer/Pack.h
       ${PARENT_DIR}/include/Packer/Packer.h
       ${PARENT_DIR}/include/Packer/TypeBuilder.h
       ${PARENT_DIR}/include/Packer/TypeDef.h
       ${PARENT_DIR}/include/Packer/TypeFactory.h
       ${PARENT_DIR}/include/Packer/TypeReg.h
)

set( libHeaders 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StandardLib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ISerial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialOut.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialIn.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BaseTypeBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VectorTypeBuilder.h
)

set( libSource
	${CMAKE_CURRENT_SOURCE_DIR}/src/Packer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Pack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialOut.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SerialIn.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeReg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TypeBuilder.cpp
)

add_library (Packer STATIC ${libSource} ${libHeaders} ${externHeaders}) 
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
