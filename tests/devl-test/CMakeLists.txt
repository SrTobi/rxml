file(GLOB rxml_test_source "rxml-test/*.cpp")
source_group("tests\\rxml-test" FILES ${rxml_test_source})

set(devl_test_module "devl-test-module.cpp")


source_group("tests" FILES ${test_settings} ${devl_test_module})
source_group("rxml" FILES ${rxml_includes})

add_executable(devl-test ${devl_test_module} ${test_settings} ${rxml_test_source} ${rxml_includes})
target_link_libraries(devl-test ${rxml_dependency_libs})
#add_precompiled_header(devl-test	${tilenet_library_pch_file})
