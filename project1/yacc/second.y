%{
#include "../../include/IR.h"
#include "lex.yy.c"
extern double yylval_doubletmp;
extern int yylval_inttmp;
int yyerror(char*);
YYSTYPE myroot;
%}
%token Id INTEGER FLOAT DELIM WHITESPACE LEFTBRACKET RIGHTBRACKET LEFTANGLE RIGHTANGLE 

%left '+' '-'
%left '*' '/' EXACTLYDIVIDE '%'
%nonassoc '='

%%
P :  P S {printf("happy");}
    |S {;}
    ;

S : LHS '=' RHS ';' {;}

LHS : TRef


RHS: RHS '+' RHS {;}
    |RHS '*' RHS {;}
    |RHS '-' RHS {;}
    |RHS '/' RHS {;} 
    |RHS '%' RHS {;} 
    |RHS EXACTLYDIVIDE RHS {;}
    |TRef {;}
    |SRef {;}
    |Const {;}
    ;
    
TRef : Id '<' Clist '>' '[' Alist ']' {;}

SRef : Id '<' Clist '>' {;}

Clist :  Clist ',' INTEGER {;}
        |INTEGER {;}
        ;
Alist :  Alist ',' IdExpr {;}
        |IdExpr {myroot=$1;}
        ;

IdExpr : Id {;}
        |IdExpr '+' IdExpr {;}
        |IdExpr '+' INTEGER {;}
        |IdExpr '*' INTEGER {;}
        |IdExpr EXACTLYDIVIDE INTEGER {;}
        |IdExpr '%' INTEGER {;}
        ;

Const :  FLOAT {;}
        |INTEGER {;}
        ;

%%
YYSTYPE yyparse_string(char * str)
{
        yy_switch_to_buffer(yy_scan_string(str));
        yyparse();
        return myroot;
}
int yyerror(char* s)
{
    printf("%s",s);
    return 0;
}

