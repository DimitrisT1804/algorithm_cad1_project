%{
#include <stdio.h>
#include <stdlib.h>
%}

%union {
    char *str;
    double num;
}

%token <str> HASH_DESIGN HASH_ROWS ROW IO COMPONENT IDENTIFIER
%token <num> FLOAT COLON

%%

file: SEPARATOR design_specification SEPARATOR rows SEPARATOR io_ports SEPARATOR components SEPARATOR
    ;

design_specification: HASH_DESIGN IDENTIFIER
    ;

rows: HASH_ROWS row_list
    ;

row_list: /* empty */
        | row_list ROW row
        ;

row: ROW COLON IDENTIFIER IDENTIFIER FLOAT FLOAT FLOAT FLOAT
    ;

io_ports: IO io_port_list
    ;

io_port_list: /* empty */
            | io_port_list io_port
            ;

io_port: IDENTIFIER COLON FLOAT FLOAT
    ;

components: COMPONENT component_list
    ;

component_list: /* empty */
              | component_list component
              ;

component: IDENTIFIER COLON IDENTIFIER FLOAT FLOAT FLOAT FLOAT
    ;

SEPARATOR: "\n"
    ;

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
