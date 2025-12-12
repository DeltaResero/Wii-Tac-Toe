cmake_minimum_required(VERSION 3.25)
project(Wii-Tac-Toe LANGUAGES C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Default to Release build ---
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' (default)")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Silence warnings from GNUInstallDirs regarding architecture detection
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Library directory name")

# --- Dependencies (FetchContent) ---
include(FetchContent)

# GRRLIB
FetchContent_Declare(grrlib
  GIT_REPOSITORY https://github.com/GRRLIB/GRRLIB.git
  GIT_TAG        c3f2acfee7a4a93e35ade02408daa1508c9309f5
)
set(GRRLIB_INSTALL OFF CACHE BOOL "" FORCE)

# GRRMOD
FetchContent_Declare(grrmod
  URL https://codeload.github.com/GRRLIB/GRRMOD/tar.gz/670f84601a5086c4d3eeb03bd3041e3c0a7f0ace
)
set(GRRMOD_INSTALL OFF CACHE BOOL "" FORCE)
set(GRRMOD_USE_MP3 OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(grrlib grrmod)

# --- System Libraries (with better error checking) ---
find_package(PkgConfig REQUIRED)
# Use IMPORTED_TARGET to create PkgConfig:: targets
pkg_check_modules(PNG REQUIRED libpng IMPORTED_TARGET)
pkg_check_modules(FREETYPE REQUIRED freetype2 IMPORTED_TARGET)
pkg_check_modules(MXML REQUIRED mxml IMPORTED_TARGET)

# Verify critical OGC libraries exist
find_library(AESND_LIB aesnd REQUIRED)
find_library(FAT_LIB fat REQUIRED)
find_library(WIIUSE_LIB wiiuse REQUIRED)
find_library(BTE_LIB bte REQUIRED)
find_library(OGC_LIB ogc REQUIRED)

# --- Asset Conversion ---
# Create directory for generated assets
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gfx)

file(GLOB BIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/gfx/*.png")
set(GENERATED_SOURCES "")

foreach(TEXTURE ${BIN_FILES})
    get_filename_component(TEX_NAME ${TEXTURE} NAME_WE)
    get_filename_component(TEX_FULL_NAME ${TEXTURE} NAME)
    
    # Standard raw2c output names (e.g. input.png -> input.c & input.h)
    set(OUTPUT_H "${CMAKE_CURRENT_BINARY_DIR}/gfx/${TEX_NAME}.h")
    set(OUTPUT_C "${CMAKE_CURRENT_BINARY_DIR}/gfx/${TEX_NAME}.c")
    
    add_custom_command(
        OUTPUT ${OUTPUT_H} ${OUTPUT_C}
        # Copy PNG to build dir so raw2c can find it
        COMMAND ${CMAKE_COMMAND} -E copy ${TEXTURE} ${CMAKE_CURRENT_BINARY_DIR}/gfx/${TEX_FULL_NAME}
        # Run raw2c (it outputs .c and .h to the working directory)
        COMMAND ${DEVKITPRO}/tools/bin/raw2c ${TEX_FULL_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gfx
        DEPENDS ${TEXTURE}
        COMMENT "Converting ${TEX_FULL_NAME} to C/H..."
    )
    list(APPEND GENERATED_SOURCES ${OUTPUT_C} ${OUTPUT_H})
endforeach()

# --- Source Files ---
file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.cpp"
)

# --- Build Target ---
add_executable(Wii-Tac-Toe 
    ${SRC_FILES} 
    ${GENERATED_SOURCES}
)

target_compile_features(Wii-Tac-Toe PRIVATE cxx_std_26)

target_compile_options(Wii-Tac-Toe PRIVATE
  -Werror -Wall -Wunused -Wmisleading-indentation 
  -Wduplicated-cond -Wduplicated-branches
)

target_include_directories(Wii-Tac-Toe PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/source"
  "${CMAKE_CURRENT_BINARY_DIR}/gfx"
)

target_link_libraries(Wii-Tac-Toe PRIVATE
  grrlib
  grrmod
  PkgConfig::PNG
  PkgConfig::FREETYPE
  PkgConfig::MXML
  ${WIIUSE_LIB}
  ${BTE_LIB}
  ${FAT_LIB}
  ${AESND_LIB}
  ${OGC_LIB}
  m
)

# --- Post-Build (ELF -> DOL) ---
add_custom_command(TARGET Wii-Tac-Toe POST_BUILD
    COMMAND ${DEVKITPRO}/tools/bin/elf2dol $<TARGET_FILE:Wii-Tac-Toe> ${CMAKE_CURRENT_BINARY_DIR}/boot.dol
    COMMENT "Converting ELF to DOL..."
    VERBATIM
)

# --- Packaging ---
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/boot.dol" DESTINATION "Wii-Tac-Toe")
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/Wii-Tac-Toe/icon.png"
  "${CMAKE_CURRENT_SOURCE_DIR}/Wii-Tac-Toe/meta.xml"
  DESTINATION "Wii-Tac-Toe"
)

set(CPACK_PACKAGE_NAME "Wii-Tac-Toe")
set(CPACK_GENERATOR "ZIP")
include(CPack)
