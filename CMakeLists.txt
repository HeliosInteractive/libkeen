CMAKE_MINIMUM_REQUIRED( VERSION 2.8 FATAL_ERROR )

PROJECT( libkeen )

SET( LIBKEEN_MAJOR "0" CACHE STRING "LibKeen major version" )
SET( LIBKEEN_MINOR "0" CACHE STRING "LibKeen minor version" )
SET( LIBKEEN_PATCH "0" CACHE STRING "LibKeen patch version" )
SET( LIBKEEN_VERSION "${LIBKEEN_MAJOR}.${LIBKEEN_MINOR}.${LIBKEEN_PATCH}"
  CACHE STRING "LibKeen version string"  )

SET( LIBKEEN_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH   "root directory" )
SET( LIBKEEN_DEPS "${LIBKEEN_ROOT}/deps" CACHE PATH  "dependencies directory" )
SET( LIBKEEN_TESTS "${LIBKEEN_ROOT}/tests" CACHE PATH  "unit tests directory" )
SET( LIBKEEN_SOURCE  "${LIBKEEN_ROOT}/source"  CACHE PATH "sources directory" )
SET( LIBKEEN_INCLUDE "${LIBKEEN_ROOT}/include" CACHE PATH "include directory" )

ADD_DEFINITIONS( -DASIO_STANDALONE )
IF( MSVC )
  ADD_DEFINITIONS(
    -DVC_EXTRALEAN
    -DWINVER=0x0601
    -D_WIN32_WINNT=0x0601
    -DWIN32_LEAN_AND_MEAN
    -D_CRT_SECURE_NO_WARNINGS )
ENDIF()

ADD_DEFINITIONS(
  -DCURL_STATICLIB
  -DSQLITE_THREADSAFE=3 )

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${LIBKEEN_ROOT}/tools/share/cmake" )
INCLUDE( FindLibCURL REQUIRED )

INCLUDE_DIRECTORIES(
  "${LIBKEEN_DEPS}/asio/asio/include"
  "${LIBKEEN_DEPS}/sqlite"
  "${LIBKEEN_SOURCE}/internal"
  "${LIBKEEN_INCLUDE}"
  "${LibCURL_INCLUDE_DIR}"
)

CONFIGURE_FILE(
  "${LIBKEEN_INCLUDE}/keen/version.hpp.in"
  "${LIBKEEN_INCLUDE}/keen/version.hpp" )

SET( LIBKEEN_HEADERS
  "${LIBKEEN_INCLUDE}/keen/version.hpp"
  "${LIBKEEN_INCLUDE}/keen/config.hpp"
  "${LIBKEEN_INCLUDE}/keen/client.hpp"
  "${LIBKEEN_INCLUDE}/keen/cache.hpp"
  "${LIBKEEN_INCLUDE}/keen/fwd.hpp"
  "${LIBKEEN_INCLUDE}/keen.hpp" )

SET( LIBKEEN_SOURCES
  "${LIBKEEN_DEPS}/sqlite/sqlite3.c"
  "${LIBKEEN_SOURCE}/internal/scoped.hpp"
  "${LIBKEEN_SOURCE}/internal/scoped.cpp"
  "${LIBKEEN_SOURCE}/internal/logger.hpp"
  "${LIBKEEN_SOURCE}/internal/logger.cpp"
  "${LIBKEEN_SOURCE}/internal/curl.hpp"
  "${LIBKEEN_SOURCE}/internal/curl.cpp"
  "${LIBKEEN_SOURCE}/internal/core.hpp"
  "${LIBKEEN_SOURCE}/internal/core.cpp"
  "${LIBKEEN_SOURCE}/keen/version.cpp"
  "${LIBKEEN_SOURCE}/keen/config.cpp"
  "${LIBKEEN_SOURCE}/keen/client.cpp"
  "${LIBKEEN_SOURCE}/keen/cache.cpp"
  "${LIBKEEN_SOURCE}/keen.cpp" )

ADD_LIBRARY(
  libkeen.static STATIC
  ${LIBKEEN_HEADERS}
  ${LIBKEEN_SOURCES} )

ENABLE_TESTING(ON)
ADD_SUBDIRECTORY( ${LIBKEEN_TESTS} )
