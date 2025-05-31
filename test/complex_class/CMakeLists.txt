add_library(COMPLEX_CLASS_LIB
    src/ComplexClass.cpp
)

target_include_directories(COMPLEX_CLASS_LIB PUBLIC
    include/
)

set(COMPLEX_CLASS_DIR /test/complex_class/) 