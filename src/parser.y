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

%token COMMA OPENING_BRACKET CLOSING_BRACKET PI LAMBDA POINT ARROW COLON TURNSTILE STAR
%token <name> NAME
%token END_OF_FILE

%type <type> type colon-type-opt
%type <variable> variable
%type <term> term
%type <context> context context-turnstile-opt
%type <name> name-or-star

%nonassoc ABSTRACTION_REDUCE CONTEXT_REDUCE
%right ARROW
%left OPENING_BRACKET LAMBDA PI NAME
%nonassoc APPLICATION_REDUCE

%%


sequent:
    context-turnstile-opt term colon-type-opt END_OF_FILE  { yy_accept_ast(lct::Sequent(ph::unwrap($1), std::shared_ptr<lct::Term>($2), std::shared_ptr<lct::Type>($3))); YYACCEPT; }

type:
    NAME                                                        { $$ = new lct::TypeVariable(ph::unwrap($1)); }
|   OPENING_BRACKET type CLOSING_BRACKET                        { $$ = $2; }
|   type ARROW type                                             { $$ = new lct::TypeArrow(std::shared_ptr<lct::Type>($1), std::shared_ptr<lct::Type>($3)); }
|   type type %prec APPLICATION_REDUCE                          { $$ = new lct::TypeApplication(std::shared_ptr<lct::Type>($1), std::shared_ptr<lct::Type>($2)); }
|   PI NAME colon-star-opt POINT type %prec ABSTRACTION_REDUCE  { $$ = new lct::TypeAbstraction(ph::unwrap($2), std::shared_ptr<lct::Type>($5)); }


colon-type-opt:
    %empty      { $$ = 0; }
|   COLON type  { $$ = $2; }

colon-star-opt:
    %empty
|   COLON STAR

variable:
    name-or-star COLON type  { $$ = new lct::Variable(ph::unwrap($1), std::shared_ptr<lct::Type>($3)); }

term:
    NAME                                                 { $$ = new lct::TermVariable(ph::unwrap($1)); }
|   OPENING_BRACKET term CLOSING_BRACKET                 { $$ = $2; }
|   term term %prec APPLICATION_REDUCE                   { $$ = new lct::TermApplication(std::shared_ptr<lct::Term>($1), std::shared_ptr<lct::Term>($2)); }
|   LAMBDA variable POINT term %prec ABSTRACTION_REDUCE  { $$ = new lct::TermAbstraction(ph::unwrap($2), std::shared_ptr<lct::Term>($4)); }

context:
    variable                { $$ = new std::vector<lct::Variable>(); $$->push_back(ph::unwrap($1)); }
|   context COMMA variable  { $$ = $1; $$->push_back(ph::unwrap($3)); }

context-turnstile-opt:
    %empty %prec CONTEXT_REDUCE  { $$ = new std::vector<lct::Variable>(); }
|   context TURNSTILE            { $$ = $1; }

name-or-star:
    NAME  { $$ = $1; }
|   STAR  { $$ = new std::string("*"); }
