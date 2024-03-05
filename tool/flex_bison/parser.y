%{
#include <stdio.h>
#include <string.h>

int yylex();
int yyparse();
void yyerror(const char *s);

char* expression; // Variable to store the expression

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
%nonassoc BANG  /* Unary negation */

%type <integer> expr term factor unary

%%

input: expr EOL { }
    ;

expr: expr '+' term    { }
    | term             { }
    | unary            { } // Add unary production rule //
    ;

term: term '*' factor { }
    | term '/' factor { }
    | factor           { }
    ;

factor: NUMBER        { }
      | VARIABLE      { }
      | '(' expr ')'   { }
      | expr '^' expr  { }
      ;

unary: BANG factor    { }
     | BANG expr '+' term { }
     ;

%%

void yyerror(const char *s) 
{
    fprintf(stderr, "Parser error: %s\n", s);
}

int yacc_main(const char* expr) 
{
    expression = strdup(expr); // Copy the expression to the variable
    /* YY_BUFFER_STATE buffer_state; // Declare the buffer_state variable
    buffer_state = yy_scan_string(expression); // Set the input buffer */
    
    yy_scan_string(expression); // Set the input buffer

    if (yyparse() == 0) 
    {
        printf("Parsing is correct!\n");
        return 0;
    }
    else 
    {
        printf("Parsing failed.\n");
        return 1;
    }
}
