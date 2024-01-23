%{
#include "parser.h"
%}

%%

[ \t]              ; // Ignore whitespace
[0-9]+             { yylval.integer = atoi(yytext); return NUMBER; }
[A-Za-z][A-Za-z0-9]* { yylval.variable = strdup(yytext); return VARIABLE; } // Allow variables longer than one character
[+\-*/^()]         { return yytext[0]; }
\n                { return EOL; }
!                 { return BANG; }
.                 { fprintf(stderr, "Invalid character: %s\n", yytext); }

%%

int yywrap() 
{
    return 1;  // indicate end of input
}
