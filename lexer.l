%option yylineno

%{
#include <stdio.h>
#include "parser.tab.h"
extern void yyerror(const char *s);
int printOutput = 0 ;
%}

%%

"main" {
    if(printOutput) printf("MAIN_KEYWORD");
    return MAIN_KEYWORD;
}

"s" {
    if(printOutput) printf("STRUCT_KEYWORD\n"); 
    return STRUCT_KEYWORD;
}

"?" {
    if(printOutput) printf("IF_KEYWORD\n");
    return IF_KEYWORD;
}

"e" {
    if(printOutput) printf("ELSE_KEYWORD\n");
    return ELSE_KEYWORD;
}

"r" {
    if(printOutput) printf("RETURN_KEYWORD\n");
    return RETURN_KEYWORD;
}

"(" {
    if(printOutput) printf("OPEN_BRACKET\n");
    return OPEN_BRACKET;
}

")" {
    if(printOutput) printf("CLOSE_BRACKET\n");
    return CLOSE_BRACKET;
}

"{" {
    if(printOutput) printf("OPEN_CURLY_BRACKET\n");
    return OPEN_CURLY_BRACKET;
}

"}" {
    if(printOutput) printf("CLOSE_CURLY_BRACKET\n");
    return CLOSE_CURLY_BRACKET;
}

";" {
    if(printOutput) printf("SEMICOLON\n");
    return SEMICOLON;
}

"." {
    if(printOutput) printf("DOT\n");
    return DOT;
}

".." {
    if(printOutput) printf("DOUBLE_DOT\n");
    return DOUBLE_DOT;
}

"=" {
    if(printOutput) printf("ASIGNMENT_OPERATOR\n");
    return ASIGNMENT_OPERATOR;
}

"," {
    if(printOutput) printf("COMMA\n");
    return COMMA;
}

"?e" {
    if(printOutput) printf("ELIF_KEYWORD");
    return ELIF_KEYWORD;
}

"l" {
    if(printOutput) printf("LOOP_KEYWORD");
    return LOOP_KEYWORD;
}

"f" {
    if(printOutput) printf("FUNCTION_KEYWORD\n");
    return FUNCTION_KEYWORD;
}

"p" {
    if(printOutput) printf("PRINT_KEYWORD\n");
    return PRINT_KEYWORD;
}

"vf" {
    if(printOutput) printf("VOID_FUNCTION_KEYWORD\n"); 
    return VOID_FUNC_KEYWORD;
}

"I" {
    if(printOutput) printf("TYPE_IDENTIFIER: %s\n", yytext);
    yylval.str = strdup(yytext);
    return TYPE_IDENTIFIER;
}

"D" {
    if(printOutput) printf("TYPE_IDENTIFIER: %s\n", yytext);
    yylval.str = strdup(yytext);
    return TYPE_IDENTIFIER;
}

"B" {
    if(printOutput) printf("TYPE_IDENTIFIER: %s\n", yytext);
    yylval.str = strdup(yytext);
    return TYPE_IDENTIFIER;
}




[TF] {
    if(printOutput) printf("BOOLEAN_LITERAL");
    yylval.str = strdup(yytext);
    return BOOLEAN_LITERAL;
}

"+" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return ADD_OP_TOKEN;
}

"-" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return SUB_OP_TOKEN;
}

"/" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return DIV_OP_TOKEN;
}

"*" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return MULT_OP_TOKEN;
}

"%" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return MOD_OP_TOKEN;
}

"^" {
    if(printOutput) printf("MATH_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return POW_OP_TOKEN;
}

"!" {
    if(printOutput) printf("LOGIC_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return NOT_OP_TOKEN;
}

">="|"<="|"=="|">"|"<"|"\!=" {
    if(printOutput) printf("COMPRARE_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return COMPARE_OPERATOR;
}

"&&" {
    if(printOutput) printf("LOGIC_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return AND_OP_TOKEN; 
}

"||" {
    if(printOutput) printf("LOGIC_OPERATOR: %s\n", yytext);
    yylval.str = strdup(yytext);
    return OR_OP_TOKEN;
}

[a-zA-Z_][a-zA-Z0-9_-]+ { 
    if(printOutput) printf( "IDENTIFIER: %s\n", yytext); 
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[-]?[1-9][0-9]* {
    if(printOutput) printf("Integer: %s\n", yytext); 
    yylval.str = strdup(yytext);
    return INTEGER_LITERAL;
}

"0" {
    if(printOutput) printf("Integer: %s\n", yytext); 
    yylval.str = strdup(yytext);
    return INTEGER_LITERAL;
}

[-]?[1-9][0-9]*\.[0-9]+ {
    if(printOutput) printf("Decimal number: %s\n", yytext);
    yylval.str = strdup(yytext);
    return DECIMAL_LITERAL;
}

"0.0" {
    if(printOutput) printf("Decimal number: %s\n", yytext);
    yylval.str = strdup(yytext);
    return DECIMAL_LITERAL; 
}

[ \t\r]+ {}

[\n] {
    if(printOutput) printf("\n");
}

. {
    if(printOutput) printf("Other: %s\n", yytext); 
    char error_message[256];  // Buffer to hold the error message
    sprintf(error_message, "Unexpected character '%s'", yytext);
    yyerror(error_message);
}
%%
/*
int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--print-output") == 0) {
     //   printOutput = 1;
    }else{
       //  printOutput = 0;
    }
    int token;
    while ((token = yylex()) != 0) {
        // Optionally you can do something with token here
    }
    return 0;
}*/

int yywrap() {
    return 1;
}
