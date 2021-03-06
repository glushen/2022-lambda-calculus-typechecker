project(lambda_calculus_typechecker_lib)

find_package(BISON)
find_package(FLEX)

bison_target(
    parser
    parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
    VERBOSE
)
flex_target(
    lexer
    lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)
add_flex_bison_dependency(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(
    lambda_calculus_typechecker_lib STATIC
    declaration.h declaration.cpp
    parser_helper.h
    lct_parser.cpp lct_parser.h
    lct.cpp lct.h
    ${FLEX_lexer_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)
