INCLUDE_DIRECTORIES(
  "${LIBKEEN_DEPS}/catch/include"
  "${LIBKEEN_TESTS}" )

FILE( GLOB test_sources "${LIBKEEN_TESTS}/*.cpp" )
FOREACH( test_source ${test_sources} )
  GET_FILENAME_COMPONENT( test_name ${test_source} NAME_WE )
  SET( test_target_name "test.${test_name}" )
  ADD_EXECUTABLE( "${test_target_name}" ${test_source} )
  TARGET_LINK_LIBRARIES( "${test_target_name}" libkeen.static )
  ADD_TEST( NAME ${test_name} COMMAND "${test_target_name}" )
ENDFOREACH()
