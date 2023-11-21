%{
#include <stdio.h>
#include "parser.tab.h" // Include the header file generated by Bison
%}

%%
"#Design:"    { return HASH_DESIGN; }
"#Rows:"      { return HASH_ROWS; }
"Row:"        { return ROW; }
"IO:"         { return IO; }
"Component:"  { return COMPONENT; }
":"           { return COLON; }
[A-Za-z_][A-Za-z0-9_]*   { yylval.str = strdup(yytext); return IDENTIFIER; }
[0-9]+[.][0-9]+          { yylval.num = atof(yytext); return FLOAT; }
[ \t\n]                  ; // Skip whitespace
.                        { return yytext[0]; } // Any other character
%%

int yywrap() {
    return 1;
}