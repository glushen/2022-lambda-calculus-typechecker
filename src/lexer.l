%option noyywrap nodefault

%{
#include <parser.h>
%}

%%

,         { return COMMA; }
\(        { return OPENING_BRACKET; }
\)        { return CLOSING_BRACKET; }
Π|\\\\    { return PI; }
λ|\\      { return LAMBDA; }
\.        { return POINT; }
→|\-\>    { return ARROW; }
\:        { return COLON; }
⊢|\|      { return TURNSTILE; }
\*        { return STAR; }

[a-zA-Z0-9]|α|β|γ|δ|ε|ζ|η|θ|ι|κ|λ|μ|ν|ξ|ο|π|ρ|σ|ς|τ|υ|φ|χ|ψ|ω|Α|Β|Γ|Δ|Ε|Ζ|Η|Θ|Ι|Κ|Λ|Μ|Ν|Ξ|Ο|Π|Ρ|Σ|Τ|Υ|Φ|Χ|Ψ|Ω { yylval.name = new std::string(yytext); return NAME; }

[\r\n ]  /* ignore spaces */
<<EOF>>  { return END_OF_FILE; }
.        { return yytext[0]; }

%%

void yy_set_input_string(const char* in) {
    yy_scan_string(in);
}

void yy_clear_buffer(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}
