%define parse.error verbose

%code requires {
    #include "parser_helper.h"
    #include "declaration.h"

    extern int yylex(void);
    extern void yyerror(const char *s, ...);
    extern void yy_accept_ast(lct::Sequent);
}

%union {
    std::string* name;
    lct::Type* type;
    lct::Variable* variable;
    lct::Term* term;
    std::vector<lct::Variable>* context;
}

%token COMMA OPENING_BRACKET CLOSING_BRACKET LAMBDA POINT ARROW COLON TURNSTILE
%token <name> NAME
%token END_OF_FILE

%type <type> type color-type-opt
%type <variable> variable
%type <term> term
%type <context> context context-turnstile-opt

%right ARROW

%nonassoc TERM_ABSTRACTION_REDUCE CONTEXT_REDUCE
%left OPENING_BRACKET LAMBDA NAME
%nonassoc TERM_APPLICATION_REDUCE

%%


sequent:
    context-turnstile-opt term color-type-opt END_OF_FILE  { yy_accept_ast(lct::Sequent(ph::unwrap($1), std::shared_ptr<lct::Term>($2), std::shared_ptr<lct::Type>($3))); YYACCEPT; }

type:
    NAME                                  { $$ = new lct::TypeVariable(ph::unwrap($1)); }
|   OPENING_BRACKET type CLOSING_BRACKET  { $$ = $2; }
|   type ARROW type                       { $$ = new lct::TypeArrow(std::shared_ptr<lct::Type>($1), std::shared_ptr<lct::Type>($3)); }

color-type-opt:
    %empty      { $$ = 0; }
|   COLON type  { $$ = $2; }

variable:
    NAME COLON type                       { $$ = new lct::Variable(ph::unwrap($1), std::shared_ptr<lct::Type>($3)); }

term:
    NAME                                                      { $$ = new lct::TermVariable(ph::unwrap($1)); }
|   OPENING_BRACKET term CLOSING_BRACKET                      { $$ = $2; }
|   term term %prec TERM_APPLICATION_REDUCE                   { $$ = new lct::TermApplication(std::shared_ptr<lct::Term>($1), std::shared_ptr<lct::Term>($2)); }
|   LAMBDA variable POINT term %prec TERM_ABSTRACTION_REDUCE  { $$ = new lct::TermAbstraction(ph::unwrap($2), std::shared_ptr<lct::Term>($4)); }

context:
    variable                { $$ = new std::vector<lct::Variable>(); $$->push_back(ph::unwrap($1)); }
|   context COMMA variable  { $$ = $1; $$->push_back(ph::unwrap($3)); }

context-turnstile-opt:
    %empty %prec CONTEXT_REDUCE  { $$ = new std::vector<lct::Variable>(); }
|   context TURNSTILE            { $$ = $1; }
