

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.2 # or a later release
)

FetchContent_MakeAvailable(Catch2)



cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

set( externHeaders 
    
)

set( testSource 
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/main_tests.cpp
)

add_executable(PackerTest ${testSource} ${externHeaders})
target_link_libraries(PackerTest PRIVATE Catch2::Catch2WithMain)
target_link_libraries(PackerTest PUBLIC Packer)

add_dependencies(PackerTest Packer)

target_include_directories( PackerTest	PUBLIC ${PARENT_DIR}/Packer/include/Packer )

target_sources( PackerTest PRIVATE 
    ${testSource}
 )

 install(TARGETS PackerTest
        RUNTIME           
          COMPONENT Runtime
          DESTINATION test
)
install(FILES $<TARGET_RUNTIME_DLLS:PackerTest> DESTINATION test)