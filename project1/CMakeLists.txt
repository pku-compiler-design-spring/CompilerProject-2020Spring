file(GLOB test_src "*.cc")
file(GLOB kernel_src "./kernels/*.cc")
file(GLOB solution_src "./solution/*.cc")
file(GLOB clean_src "./clean/*.cc")

include_directories("../include")

add_executable(test1 ${test_src} ${kernel_src})
add_executable(solution ${solution_src})
target_link_libraries(solution ${LIB_NAME})
add_executable(cleanf ${clean_src})

add_custom_target(solution_run
  COMMAND solution
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "run generated solution in ${CMAKE_CURRENT_SOURCE_DIR}"
  SOURCES ${solution_src}
)

add_custom_target(clean_run
  COMMAND cleanf
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "run generated clean in ${CMAKE_CURRENT_SOURCE_DIR}"
  SOURCES ${clean_src}
)

add_dependencies(solution_run clean_run)
add_dependencies(test1 solution_run)