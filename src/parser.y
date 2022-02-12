%code requires {
    extern int yylex(void);
    extern void yyerror(const char *s, ...);
}

%union {

}

%token END_OF_FILE

%%


temp:

