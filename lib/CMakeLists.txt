include(GNUInstallDirs)

# Sources
file(GLOB_RECURSE STORMBYTE_MEMORY_SOURCES CONFIGURE_DEPEND "${CMAKE_CURRENT_LIST_DIR}/*.cxx")

# Library
add_library(StormByte-Memory SHARED ${STORMBYTE_MEMORY_SOURCES})
add_library(StormByte::Memory ALIAS StormByte-Memory)
target_link_libraries(StormByte-Memory PUBLIC StormByte)
set_target_properties(StormByte-Memory PROPERTIES
	LINKER_LANGUAGE CXX
	SOVERSION		${CMAKE_PROJECT_VERSION}
	VERSION 		${CMAKE_PROJECT_VERSION}
)

# Compile options
if(MSVC)
	target_compile_options(StormByte-Memory PRIVATE /EHsc)
	target_compile_options(StormByte-Memory PRIVATE $<$<CONFIG:Release>:/O2>)
	target_compile_options(StormByte-Memory PRIVATE $<$<CONFIG:Debug>:/Od>)
	target_link_options(StormByte-Memory PRIVATE $<$<CONFIG:Release>:/LTCG /GL>)
else()
	set(CMAKE_CXX_FLAGS_DEBUG "-pipe -g -ggdb -Wall -Wextra -Wnon-virtual-dtor -pedantic -pedantic-errors -O0")
	target_compile_options(StormByte-Memory PRIVATE -fvisibility=hidden $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>)
endif()

# Include directories
target_include_directories(StormByte-Memory
	SYSTEM BEFORE PUBLIC "${CMAKE_CURRENT_LIST_DIR}/public" "${CMAKE_CURRENT_LIST_DIR}/private"
)

# Install
if (NOT STORMBYTE_AS_DEPENDENCY)
	install(TARGETS StormByte-Memory
		ARCHIVE 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY 		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME 		DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
	install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/public/StormByte/"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StormByte"
		FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	)
endif()
