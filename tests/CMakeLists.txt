
set(RXML_CONFIG_TEST_PATH "${PROJECT_SOURCE_DIR}/data")
configure_file(	"test_config.hpp.in"
				"${PROJECT_SOURCE_DIR}/config/test_config.hpp")
include_directories("${PROJECT_SOURCE_DIR}/config")
				
				
				
set(test_settings "${CMAKE_CURRENT_SOURCE_DIR}/test_settings.hpp;${PROJECT_SOURCE_DIR}/config/test_config.hpp")
add_definitions(-DRXML_TESTS)

include_directories(.)
add_subdirectory(devl-test)