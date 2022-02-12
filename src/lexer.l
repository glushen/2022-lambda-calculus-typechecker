%option noyywrap nodefault

%{
#include <parser.h>
%}

%%



[\r\n ]  /* ignore spaces */
<<EOF>>  { return END_OF_FILE; }
.        { return yytext[0]; }

%%
