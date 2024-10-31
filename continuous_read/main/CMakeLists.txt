
idf_component_register(SRCS "continuous_read_main.cpp" # "one_shot.cpp" # "test1.cpp" #
  INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable -Wno-unused-function -DESP=1 -DCONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE=enabled -g)

