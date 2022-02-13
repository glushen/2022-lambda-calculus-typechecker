#include "lct_parser.h"
#include "parser.h"
#include "declaration.h"
#include <cstdarg>
#include <sstream>
#include <optional>
#include <utility>

extern void yy_set_input_string(const char*);
extern void yy_clear_buffer();

static lctp::Result result;

lctp::Result lctp::parse_sequent(const std::string& input) {
#if YYDEBUG
    yydebug = 1;
#endif

    yy_set_input_string(input.c_str());
    yyparse();
    yy_clear_buffer();

    return result;
}

void yy_accept_ast(lct::Sequent sequent) {
    result.accepted = true;
    result.output = std::move(sequent);
}

void yyerror(const char* format, ...) {
    result.accepted = false;

    va_list arguments;

    va_start(arguments, format);
    char* result_chars = new char[vsnprintf(nullptr, 0, format, arguments) + 1];
    va_end(arguments);

    va_start(arguments, format);
    vsprintf(result_chars, format, arguments);
    va_end(arguments);

    result.error_message = result_chars;
    delete[] result_chars;
}
