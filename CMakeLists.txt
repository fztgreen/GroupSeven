###########################
# CMake Build Environment #
###########################
#
# You can set the following Environment variables:
#
# CMAKE_MODULE_PATH to the search path for cmake modules (e.g., SFML.cmake)
# CMAKE_BUILD_TYPE to override the default build type. Valid options are 'release', 'profile' and 'debug'
#
#########################################

# Minimum Cmake Version (3.14 for FetchContent_MakeAvailable)
cmake_minimum_required(VERSION 3.14)

# Project name
project(csci437)

#####################
# Setup Environment #
#####################
# set to include custom modules
set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})

# set build type if specified by environment
if((NOT CMAKE_BUILD_TYPE) AND (NOT $ENV{CMAKE_BUILD_TYPE} STREQUAL ""))
    set(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
endif()

#################################################
#                                               #
# ADD CUSTOM LIBRARIES HERE.                    #
#                                               #
# Example BOOST:                                #
#  find_package(Boost REQUIRED)                 #
#  include_directories(${Boost_INCLUDE_DIRS})   #
#  link_libraries(${Boost_LIBRARIES})           #
#################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#$target_link_libraries(<math.h> m)

###############
# Build Type  #
###############
# Must be decided before the dependencies are added: single-config generators
# (Makefiles/Ninja) apply it per subdirectory at the point it is added, and
# SFML names its libraries by it (e.g. libsfml-graphics-s-d.a for Debug).
#
# determine build type
# 1) use build type if specified by the user.
# 2) if in-source building, use default (debug) build type.
# 3) if out-source building, determine based on the build directory's name which build type.
#    If build directory is called 'Release' or 'Profile' then use the 'Release' and 'Profile'
#    build type respectively, otherwise use 'debug'.
if(CMAKE_BUILD_TYPE)
  string(TOUPPER "${CMAKE_BUILD_TYPE}" TYPE)
  message("-- Using user specified build type: ${TYPE}")
elseif(${csci437_SOURCE_DIR} STREQUAL ${csci437_BINARY_DIR})
  set(CMAKE_BUILD_TYPE Debug)
  message("-- In-source building. Build type set to: Debug")
else()
  get_filename_component(TYPE ${csci437_BINARY_DIR} NAME)
  string(TOUPPER "${TYPE}" TYPE)
  if(${TYPE} STREQUAL "RELEASE")
    set(CMAKE_BUILD_TYPE Release)
  elseif(${TYPE} STREQUAL "PROFILE")
    set(CMAKE_BUILD_TYPE Profile)
  else()
    set(CMAKE_BUILD_TYPE Debug)
 endif()
 message("-- Out-source building. Build type set to: ${TYPE}")
endif()

# Handle non-default build type 'Profile'
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if(${CMAKE_BUILD_TYPE} STREQUAL "PROFILE")
  set(CMAKE_BUILD_TYPE DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif()

include(FetchContent)

# Download a declared dependency and add it with EXCLUDE_FROM_ALL. Its targets
# still build when the game links them, but its own install() rules (headers,
# static libs, CMake package files) are left out of `cmake --install`.
# (FetchContent_Declare(... EXCLUDE_FROM_ALL) does this too, but needs CMake 3.28.)
if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)  # allow FetchContent_Populate(<name>) on CMake 3.30+
endif()
macro(ge_fetch_dependency name)
  FetchContent_GetProperties(${name})
  if(NOT ${name}_POPULATED)
    FetchContent_Populate(${name})
    add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endmacro()

###################################
# Box2D (downloaded at configure) #
###################################
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED    OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_DOCS       OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG        v2.4.1
  GIT_SHALLOW    TRUE
)
ge_fetch_dependency(box2d)

####################################################
# SFML 2.x (use installed copy, else download it)  #
####################################################
# Set SFML_FORCE_FETCH=ON to ignore any installed SFML and build 2.6.2.
option(SFML_FORCE_FETCH "Always download and build SFML instead of using an installed copy" OFF)

if(NOT SFML_FORCE_FETCH)
  # CONFIG: use SFML's own SFMLConfig.cmake (shipped with SFML 2.5+).
  find_package(SFML 2.5 COMPONENTS graphics window system audio network CONFIG QUIET)
endif()

if(SFML_FOUND)
  message(STATUS "Using installed SFML ${SFML_VERSION}")
  set(SFML_FETCHED OFF)
else()
  # On Linux SFML needs system development packages that CMake cannot install.
  # Check for them up front so the error says exactly what to install.
  if(UNIX AND NOT APPLE)
    find_package(PkgConfig QUIET)
    if(NOT PKG_CONFIG_FOUND)
      message(FATAL_ERROR " * SFML was not found and pkg-config is missing, so its dependencies cannot be checked.\n"
                          " * Install SFML (e.g. 'sudo apt install libsfml-dev'), or install pkg-config and the packages\n"
                          "   listed in README.md so CMake can build SFML from source.")
    endif()

    # pkg-config module -> Debian/Ubuntu package
    set(_sfml_linux_deps
      "x11=libx11-dev"         "xrandr=libxrandr-dev"   "xcursor=libxcursor-dev"
      "gl=libgl1-mesa-dev"     "libudev=libudev-dev"    "freetype2=libfreetype-dev"
      "openal=libopenal-dev"   "flac=libflac-dev"       "ogg=libogg-dev"
      "vorbis=libvorbis-dev"   "vorbisenc=libvorbis-dev" "vorbisfile=libvorbis-dev")
    set(_sfml_missing_mods "")
    set(_sfml_missing_pkgs "")
    foreach(_dep ${_sfml_linux_deps})
      string(REPLACE "=" ";" _dep "${_dep}")
      list(GET _dep 0 _mod)
      list(GET _dep 1 _pkg)
      pkg_check_modules(_SFMLDEP_${_mod} QUIET ${_mod})
      if(NOT _SFMLDEP_${_mod}_FOUND)
        list(APPEND _sfml_missing_mods ${_mod})
        list(APPEND _sfml_missing_pkgs ${_pkg})
      endif()
    endforeach()

    if(_sfml_missing_mods)
      list(REMOVE_DUPLICATES _sfml_missing_pkgs)
      string(REPLACE ";" " " _sfml_missing_mods "${_sfml_missing_mods}")
      string(REPLACE ";" " " _sfml_missing_pkgs "${_sfml_missing_pkgs}")
      message(FATAL_ERROR " * SFML was not found, and building it from source needs these missing libraries: ${_sfml_missing_mods}\n"
                          " * Either install SFML:      sudo apt install libsfml-dev\n"
                          " * Or install its deps:      sudo apt install ${_sfml_missing_pkgs}\n"
                          "   (package names are for Debian/Ubuntu; use your distro's equivalents otherwise)")
    endif()
  endif()

  message(STATUS "SFML not found - downloading and building SFML 2.6.2")
  set(SFML_FETCHED ON)
  set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)  # static SFML: no SFML DLLs to ship
  set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_DOC      OFF CACHE BOOL "" FORCE)
  set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2
    GIT_SHALLOW    TRUE
  )
  ge_fetch_dependency(sfml)
endif()

# Applied to the project's own targets declared below (after the fetched
# dependencies, so they don't inherit these settings).
include_directories(${csci437_SOURCE_DIR}/include)
if(MSVC)
  add_compile_definitions(_USE_MATH_DEFINES)  # M_PI from <cmath>
endif()

##################
# Install layout #
##################
# `cmake --install <build> --prefix <dir>` produces a self-contained folder:
#   Windows:  <dir>/greatEscape.exe, <dir>/openal32.dll, <dir>/data/
#   Linux:    <dir>/bin/greatEscape, <dir>/share/greatescape/
# Both are relocatable: the game finds its data relative to the executable.
include(GNUInstallDirs)
if(WIN32)
  set(_ge_bindir_default  ".")
  set(_ge_datadir_default "data")
else()
  set(_ge_bindir_default  "${CMAKE_INSTALL_BINDIR}")
  set(_ge_datadir_default "${CMAKE_INSTALL_DATADIR}/greatescape")
endif()
set(GE_INSTALL_BINDIR  "${_ge_bindir_default}"  CACHE STRING "Install dir for the executable, relative to the prefix")
set(GE_INSTALL_DATADIR "${_ge_datadir_default}" CACHE STRING "Install dir for the game data, relative to the prefix")
if(IS_ABSOLUTE "${GE_INSTALL_BINDIR}" OR IS_ABSOLUTE "${GE_INSTALL_DATADIR}")
  message(FATAL_ERROR "GE_INSTALL_BINDIR and GE_INSTALL_DATADIR must be relative to the install prefix")
endif()
# Path from the executable's directory to the data directory, compiled into
# src/DataPath.cpp so an installed copy finds its assets wherever it is moved.
file(RELATIVE_PATH GE_DATA_DIR_FROM_BIN "/prefix/${GE_INSTALL_BINDIR}" "/prefix/${GE_INSTALL_DATADIR}")
if(GE_DATA_DIR_FROM_BIN STREQUAL "")
  set(GE_DATA_DIR_FROM_BIN ".")
endif()

# Development fallback: absolute path to the source tree's data/, used when no
# installed data is found next to the executable (see src/DataPath.cpp).
set(GE_DATA_DIR "${csci437_SOURCE_DIR}/data" CACHE PATH "Directory the game loads assets from when not installed")
add_compile_definitions(GE_DATA_DIR="${GE_DATA_DIR}" GE_DATA_DIR_FROM_BIN="${GE_DATA_DIR_FROM_BIN}")

install(DIRECTORY data/ DESTINATION "${GE_INSTALL_DATADIR}"
        PATTERN "ReadMeGuide" EXCLUDE)   # README screenshots, not game assets

# box2d and sfml-* targets carry their own include directories.
link_libraries(box2d sfml-graphics sfml-window sfml-system sfml-audio sfml-network)

# Collected after the dependencies are added: SFML's own CMake scripts use a
# variable called SRC, and would otherwise pick up the game's sources.
# Get CPP files
file(GLOB SRC src/*cpp)

# Get executable files
file(GLOB EXECLIST bin/*cpp)

#######################
# Set Compile Targets #
#######################
# src library (all CPP files in 'src' dir)
if(NOT SRC STREQUAL "")
  get_filename_component(LIBNAME ${csci437_SOURCE_DIR} NAME)
  set(LIBNAME "${LIBNAME}_core")
  add_library(${LIBNAME} ${SRC})
endif()

# executables (any CPP file in 'bin' dir)
foreach(EXEC ${EXECLIST})
  get_filename_component(EXECNAME ${EXEC} NAME_WE)
  add_executable(${EXECNAME} ${EXEC})

  if(NOT SRC STREQUAL "")
    target_link_libraries(${EXECNAME} LINK_PUBLIC ${LIBNAME})
  endif()

  # sfml-audio on Windows loads OpenAL from openal32.dll at runtime; SFML
  # ships it, so put a copy next to the executable.
  if(WIN32 AND SFML_FETCHED)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(_openal_dll "${sfml_SOURCE_DIR}/extlibs/bin/x64/openal32.dll")
    else()
      set(_openal_dll "${sfml_SOURCE_DIR}/extlibs/bin/x86/openal32.dll")
    endif()
    add_custom_command(TARGET ${EXECNAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_openal_dll}" $<TARGET_FILE_DIR:${EXECNAME}>)
    install(FILES "${_openal_dll}" DESTINATION "${GE_INSTALL_BINDIR}")
  endif()

  install(TARGETS ${EXECNAME} RUNTIME DESTINATION "${GE_INSTALL_BINDIR}")

  message("-- Adding executable: ${EXECNAME}")
endforeach(EXEC)
