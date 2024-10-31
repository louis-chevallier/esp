
idf_component_register(SRCS "test1.cpp" #"one_shot.cpp"
  INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable -Wno-unused-function -DESP=1 -DCONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE=enabled -g)

