%{
#include <stdio.h>
#include <math.h>

int yylex();
int yyparse();
void yyerror(const char *s);
%}

%union {
    int integer;
    char variable;
}

%token <integer> NUMBER
%token <variable> VARIABLE

%left '+' '-'
%left '*' '/'
%right '^' '!'
%nonassoc UMINUS

%type <integer> expr term factor exponent unary

%%

input: expr { }
    ;

expr: expr '+' term    { $$ = $1 + $3; }
    | expr '-' term    { $$ = $1 - $3; }
    | term             { $$ = $1; }
    ;

term: term '*' factor { $$ = $1 * $3; }
    | term '/' factor { $$ = $1 / $3; }
    | factor           { $$ = $1; }
    ;

factor: NUMBER        { $$ = $1; }
      | VARIABLE      { $$ = $1; }
      | '(' expr ')'   { $$ = $2; }
      | expr '^' expr  { $$ = pow($1, $3); }
      | expr '!'       { int result = 1; for (int i = 1; i <= $1; ++i) result *= i; $$ = result; }
      | '-' expr %prec UMINUS { $$ = -$2; }
      ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}

int main() {
    if (yyparse() == 0) {
        printf("Correct\n");
    } else {
        printf("False\n");
    }
    return 0;
}
