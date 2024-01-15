%{
#include "parser.tab.h"
%}

%%
[ \t\n]              ; // Ignore whitespace
[0-9]+               { yylval.integer = atoi(yytext); return NUMBER; }
[A-Za-z]            { yylval.variable = yytext[0]; return VARIABLE; }
[-+*^!()]            { return yytext[0]; }
.                   { fprintf(stderr, "Invalid character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
