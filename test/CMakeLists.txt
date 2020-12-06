include(../cmake/swifft_defaults.cmake)

set(SWIFFT_TEST_FILES
       	swifft_catch.cpp
	swifft_io.cpp
)

add_executable(swifft_catch
	catch_main.cpp
	${SWIFFT_TEST_FILES}
)

foreach(SWIFFT_FILE ${SWIFFT_TEST_FILES})
	set_source_files_properties(${SWIFFT_FILE} PROPERTIES COMPILE_FLAGS ${SWIFFT_DEFAULT_FILE_COMPILE_FLAGS})
endforeach()

target_include_directories(swifft_catch
      	PUBLIC
	  ${CMAKE_SOURCE_DIR}/include
	  ${CMAKE_SOURCE_DIR}/src
	  ${CMAKE_SOURCE_DIR}/test
)

find_package(Catch2 REQUIRED)
target_link_libraries(swifft_catch swifft_static Catch2::Catch2)
