%{
    #include "../../include/IR.h"
    #define YYSTYPE Boost::Internal::Ref<const Boost::Internal::IRNode>
    #include <stdio.h>
    int yylval_inttmp;
    double yylval_doubletmp;
    #include "../../include/mytest.h"
%}
myinteger ([1-9][0-9]*)
myfloat ({myinteger}\.[0-9]+)
myid ([a-zA-Z_][0-9a-zA-Z_]*)
delim [ \n\r\t]
whitespace ({delim}+)
%%
{whitespace} {}
{myinteger} {sscanf(yytext,"%d",&yylval_inttmp);yylval=MyIntBuilder(yylval_inttmp);return INTEGER;}
{myfloat} {sscanf(yytext,"%lf",&yylval_doubletmp);yylval=MyDoubleBuilder(yylval_doubletmp);return FLOAT;}
{myid} {yylval=MyIdBuilder(yytext);return Id;}
";" {return ';';}
"=" {return '=';}
"," {return ',';}
"+" {return '+';}
"-" {return '-';}
"*" {return '*';}
"/" {return '/';}
"<" {return '<';}
">" {return '>';}
"[" {return '[';}
"]" {return ']';}
"//" {return EXACTLYDIVIDE;}
%%
int yywrap()
{
    return 1;
}
