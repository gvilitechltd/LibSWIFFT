include(../cmake/swifft_defaults.cmake)

add_executable(swifft_keygen swifft_keygen.cpp)

target_include_directories(swifft_keygen
	PUBLIC
	  ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(
	OUTPUT  swifft_key.c
	COMMAND swifft_keygen ${CMAKE_CURRENT_BINARY_DIR}/swifft_key.c
	DEPENDS swifft_keygen
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/swifft_ver.c
	"const char* SWIFFT_version() { return \"${swifft_VERSION}\"; }"
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/swifft_so_dummy.c
	"extern const char* SWIFFT_version();"
)

set(SWIFFT_SRC_FILES
	${CMAKE_CURRENT_BINARY_DIR}/swifft_ver.c
	${CMAKE_CURRENT_BINARY_DIR}/swifft_key.c
	swifft.c
	swifft_avx.c
	swifft_avx2.c
	swifft_avx512.c
	swifft_object.c
)

set(SWIFFT_HEADER_FILES
	common.h
	swifft_avx2.h
	swifft_avx512.h
	swifft_avx.h
	swifft_common.h
	swifft.h
	swifft.hpp
	swifft_iset.inl
	swifft_object.h
	swifft_ver.h
)
set(SWIFFT_HEADERS_DIR include/libswifft)
foreach(SWIFFT_HEADER_FILE
	${SWIFFT_HEADER_FILES}
)
	install(FILES ${CMAKE_SOURCE_DIR}/${SWIFFT_HEADERS_DIR}/${SWIFFT_HEADER_FILE} DESTINATION ${SWIFFT_HEADERS_DIR})
endforeach()

add_library(swifft_static STATIC ${SWIFFT_SRC_FILES})
install(TARGETS swifft_static DESTINATION lib)
set_target_properties(swifft_static PROPERTIES OUTPUT_NAME swifft)

add_library(swifft_shared SHARED ${CMAKE_CURRENT_BINARY_DIR}/swifft_so_dummy.c)
install(TARGETS swifft_shared DESTINATION lib)
set_target_properties(swifft_shared PROPERTIES OUTPUT_NAME swifft)
set_target_properties(swifft_shared PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(swifft_shared PUBLIC
	-Wl,--whole-archive
	$<TARGET_PROPERTY:swifft_static,NAME>
	-Wl,--no-whole-archive
)


foreach(SWIFFT_FILE
	swifft_keygen.cpp
	${CMAKE_CURRENT_BINARY_DIR}/swifft_so_dummy.c
	${SWIFFT_SRC_FILES}
)
	set_source_files_properties(${SWIFFT_FILE} PROPERTIES COMPILE_FLAGS "${SWIFFT_DEFAULT_FILE_COMPILE_FLAGS}")
endforeach()

set_source_files_properties(swifft_avx.c    PROPERTIES COMPILE_FLAGS "${SWIFFT_DEFAULT_FILE_COMPILE_FLAGS} -mavx")
set_source_files_properties(swifft_avx2.c   PROPERTIES COMPILE_FLAGS "${SWIFFT_DEFAULT_FILE_COMPILE_FLAGS} -mavx2")
set_source_files_properties(swifft_avx512.c PROPERTIES COMPILE_FLAGS "${SWIFFT_DEFAULT_FILE_COMPILE_FLAGS} -mavx512f")

foreach(SWIFFT_TARGET
	swifft_static
	swifft_shared
)
target_include_directories(${SWIFFT_TARGET}
	PUBLIC
	  ${CMAKE_SOURCE_DIR}/include
	  ${CMAKE_SOURCE_DIR}/src
)
endforeach()
