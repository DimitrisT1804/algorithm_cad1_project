%{
#include <stdio.h>

int yylex();
int yyparse();
void yyerror(const char *s);
%}

%union 
{
    int integer;
    char variable;
}

%token <integer> NUMBER
%token <variable> VARIABLE
%token BANG
%token EOL

%left '+'
%left '*' '/'
%right '^'
%nonassoc UMINUS
%nonassoc BANG  /* Unary negation */

%type <integer> expr term factor unary

%%

input: expr EOL { }
    ;

expr: expr '+' term    { }
    | term             { }
    ;

term: term '*' factor { }
    | term '/' factor { }
    | factor           { }
    ;

factor: NUMBER        { }
      | VARIABLE      { }
      | '(' expr ')'   { }
      | expr '^' expr  { }
      | unary          { }
      ;

unary: BANG factor    { }
     | BANG expr '+' term { }
     | BANG term      { }
     ;

%%

void yyerror(const char *s) 
{
    fprintf(stderr, "Parser error: %s\n", s);
}

int main() 
{
    if (yyparse() == 0) 
    {
        /* printf("Parsing is correct!\n"); */
        return 0;
    }
    else 
    {
        /* printf("Parsing failed.\n"); */
        return 1;
    }
}
