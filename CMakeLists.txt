cmake_minimum_required(VERSION 3.22)
project(nipovpn)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
	add_compile_options(/W4 /permissive- /EHsc)
	add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
else()
	add_compile_options(-Wall -Wextra)
	add_compile_options($<$<CONFIG:Debug>:-g>)
	add_compile_options($<$<CONFIG:RelWithDebInfo>:-g>)
	add_compile_options($<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-ggdb>)
	add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-MMD>)
	add_compile_options($<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-MP>)
endif()

# https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED COMPONENTS regex system)

# /usr/local/lib/cmake/yaml-cpp/yaml-cpp-config.cmake
# declared variables: YAML_CPP_INCLUDE_DIR & YAML_CPP_LIBRARY_DIR
find_package(yaml-cpp REQUIRED)

# https://cmake.org/cmake/help/v3.6/module/FindOpenSSL.html
find_package(OpenSSL REQUIRED)


add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(agent)


#add_custom_target(deb
#        COMMAND ${CMAKE_COMMAND} -E echo "Creating Debian package..."
#        COMMAND dpkg-deb --build ${DEBIANDIR} ${DEBFILE}
#        COMMENT "Building Debian package"
#)

