    %{

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "include/ast.h"
    #include "include/interpreter.h"
    #define YYDEBUG 1
    #define MISSING_SEMI_MSG "Missing semicolon at the end of line"
    #define NO_INVALID_TOKEN_OPT 0
    #define INVALID_TOKEN_OPT 1 
    int yylex();
    void yyerror(const char *s);
    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    int value = 0;
    int subtree=0;
    ASTNode* root = NULL;

    void debugPrint(char* s){
        #ifdef YYDEBUG
        printf("%s\n", s);
        #endif
    }

    %}
    //%define parse.error verbose
    %union {
        char* str;
        struct ASTNode* ast;

    }

    %token VOID_FUNC_KEYWORD 
    %token STRUCT_KEYWORD 



    %token IF_KEYWORD 
    %token ELSE_KEYWORD 
    %token OPEN_BRACKET 
    %token CLOSE_BRACKET 
    %token RETURN_KEYWORD 
    %token OPEN_CURLY_BRACKET 
    %token CLOSE_CURLY_BRACKET 
    %token SEMICOLON 
    %token DOT 
    %token DOUBLE_DOT 

    %token COMMA
    %token FUNCTION_KEYWORD 


    %token MAIN_KEYWORD
    %token LOOP_KEYWORD
    %token ELIF_KEYWORD
    %token PRINT_KEYWORD

    %token <str> TYPE_IDENTIFIER 
    %token <str> BOOLEAN_LITERAL 
    %token <str> INTEGER_LITERAL 
    %token <str> DECIMAL_LITERAL

    %token <str> COMPARE_OPERATOR
    %token <str> LOGIC_OPERATOR 
    %token <str> ASIGNMENT_OPERATOR

    %token <str> ADD_OP_TOKEN
    %token <str> SUB_OP_TOKEN
    %token <str> MULT_OP_TOKEN
    %token <str> DIV_OP_TOKEN
    %token <str> MOD_OP_TOKEN
    %token <str> POW_OP_TOKEN
    %token <str> NOT_OP_TOKEN
    %token <str> AND_OP_TOKEN
    %token <str> OR_OP_TOKEN


    %token <str> IDENTIFIER 
    %type <ast> program
    %type <ast> mainFunction
    %type <ast> commandBlock
    %type <ast> commands
    %type <ast> command
    %type <ast> variableDeclaration
    %type <ast> printCommand
    %type <ast> asignmentOptionsNoLiterals
    %type <ast> asignmentOptions
    %type <ast> mathExpression
  
    %type <ast> logicExpression
    %type <ast> compareExpression
    %type <ast> condition
    %type <ast> functionCallNoSemi
    %type <ast> functionCall
    %type <ast> functionParams
    %type <ast> params
    %type <ast> paramOption
    %type <ast> structureMember
    %type <ast> variableAsignment
    %type <ast> forLoop
    %type <ast> whileLoop
    %type <ast> structureMemberAsignment 
    %type <ast> structureDeclaration
    %type <ast> returnCommand
    %type <ast> ifStatement
    %type <ast> elifBlock
    %type <ast> elifBlocks
    %type <ast> elseBlock
    %type <ast> functionDefinition
    %type <ast> functionDefinitions
    %type <ast> functionDefParams
    %type <ast> structureDefinition
    %type <ast> structureDefinitions
    %type <ast> structMemberDeclaration
    %type <ast> structMemberDeclarations
    %type <ast> paramsListFunctionDef
    %type <ast> paramDef
    %type <ast> mathPower
    %type <ast> mathFactor
    %type <ast> mathTerm
    %type <ast> notExpression
    %type <ast> logicAnd
    %type <ast> logicNot


    %type <ast> logicOperand


    %left "+" "-"
    %left "*" "/" "%"
    %left "^"
    %%

    program:
        mainFunction functionDefinitions {
            root = createProgramNode(NULL, $1, $2);
        }
        | structureDefinitions mainFunction {
            root = createProgramNode($1, $2, NULL);         
        }
        | mainFunction {
            root = createProgramNode(NULL, $1, NULL);
        }
        | structureDefinitions mainFunction functionDefinitions {
            root = createProgramNode($1, $2, $3);
        } 


    structureDefinitions:
        structureDefinitions structureDefinition  {
            $$ = createListNode($1, $2);

        }
        | structureDefinition {
            $$ = createListNode($1, NULL);
        }

    structureDefinition: 
        STRUCT_KEYWORD IDENTIFIER OPEN_CURLY_BRACKET structMemberDeclarations CLOSE_CURLY_BRACKET {
            $$ = createStructDefinitionNode($2, $4);
        }

    structMemberDeclarations:
        structMemberDeclarations structMemberDeclaration {
            $$ = createListNode($1, $2);
        }
        | structMemberDeclaration {
            $$ = createListNode($1, NULL);
        }

    structMemberDeclaration:
        TYPE_IDENTIFIER IDENTIFIER SEMICOLON {
            $$ = createMemberDefNode($1[0], $2);
        }
        | TYPE_IDENTIFIER IDENTIFIER {yyerrorNoToken(MISSING_SEMI_MSG);} 
        | TYPE_IDENTIFIER SEMICOLON {yyerrorNoToken("Invalid structure member declaration - no identifier provided");}
        | IDENTIFIER SEMICOLON {yyerrorNoToken("Invalid structure member declaration - no type provided");}

    structureMember:
        IDENTIFIER DOT IDENTIFIER {
            $$ = createStructMemberNode($1, $3);
        }

    mainFunction:
        TYPE_IDENTIFIER FUNCTION_KEYWORD MAIN_KEYWORD OPEN_BRACKET CLOSE_BRACKET commandBlock {
            $$ = $6;
        }

    commandBlock: 
        OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = $2;
        }

    commands:
        {
            $$ = createListNode(NULL, NULL);
        }
        | commands command {
            $$ = createListNode($1, $2);
        }
        | command {
            $$ = createListNode($1, NULL);
        }

    command:
        variableDeclaration {
            $$ = createCommandNode($1);
        }
        | variableAsignment {
            $$ = createCommandNode($1);
        }
        | functionCall {
            $$ = createCommandNode($1);
        }
        | ifStatement {
            $$ = createCommandNode($1);
        }
        | forLoop {
            $$ = createCommandNode($1);
        }
        | whileLoop {
            $$ = createCommandNode($1);
        }
        | structureDeclaration {
            $$ = createCommandNode($1);
        }
        | structureMemberAsignment {
            $$ = createCommandNode($1);
        }
        | returnCommand {
            $$ = createCommandNode($1);
        }
        | printCommand {
            $$ = createCommandNode($1);
        }

    printCommand:
        PRINT_KEYWORD OPEN_BRACKET asignmentOptions CLOSE_BRACKET SEMICOLON {
            $$ = createPrintCommandNode($3);
        }
    
    variableDeclaration:
        TYPE_IDENTIFIER IDENTIFIER SEMICOLON {
            $$ = createVariableDeclaration($2, $1[0], NULL);
        }
        | TYPE_IDENTIFIER IDENTIFIER ASIGNMENT_OPERATOR INTEGER_LITERAL SEMICOLON {
            if (strcmp($1, "I") != 0 && strcmp($1, "D") != 0) {
                char msg[256];
                snprintf(msg, sizeof(msg), "Type mismatch: cannot assign '%s' to type '%s'", $4, $1);
                yyerrorNoToken(msg);
            }else{
                $$ = createVariableDeclaration($2, $1[0], createIntLiteralNode(atoi($4)));
            }
        }
        | TYPE_IDENTIFIER IDENTIFIER ASIGNMENT_OPERATOR DECIMAL_LITERAL SEMICOLON {
            if (strcmp($1, "D") != 0) {
                char msg[256];
                snprintf(msg, sizeof(msg), "Type mismatch: cannot assign '%s' to type '%s'", $4, $1);
                yyerrorNoToken(msg);        
            }else{
                $$ = createVariableDeclaration($2, $1[0], createDecimalLiteralNode(strtod($4, NULL)));
            }
        }
        | TYPE_IDENTIFIER IDENTIFIER ASIGNMENT_OPERATOR BOOLEAN_LITERAL SEMICOLON {
            if (strcmp($1, "B") != 0) {
                char msg[256];
                snprintf(msg, sizeof(msg), "Type mismatch: cannot assign '%s' to type '%s'", $4, $1);
                yyerrorNoToken(msg);
            }else{
                char val = $4[0];
                $$ = createVariableDeclaration($2, $1[0], createBoolLiteralNode((val != NULL && val == 'T')));
            }
        }
        | TYPE_IDENTIFIER IDENTIFIER ASIGNMENT_OPERATOR asignmentOptionsNoLiterals SEMICOLON {
            $$ = createVariableDeclaration($2, $1[0], $4);
        }

    variableAsignment:
        IDENTIFIER ASIGNMENT_OPERATOR asignmentOptions SEMICOLON {
            $$ = createVariableAsignmentNode($1, $3);
        }

    asignmentOptionsNoLiterals:
        mathExpression {
            $$ = $1;
        }
        | logicExpression {
            $$ = $1;
        }
        | compareExpression {
            $$ = $1;
        }
        | structureMember {
            $$ = $1;
        }
        | functionCallNoSemi {
            $$ = $1;
        }
        | IDENTIFIER {
            $$ = createIdentifierNode($1);
        }

    asignmentOptions:
        INTEGER_LITERAL {
            $$ = createIntLiteralNode(atoi($1));
        }
        | BOOLEAN_LITERAL{
            char val = $1[0];
            $$ = createBoolLiteralNode((val != NULL && val == 'T'));
        }
        | DECIMAL_LITERAL {
            $$ = createDecimalLiteralNode(strtod($1, NULL));
        }
        | mathExpression {
            $$ = $1;
        }
        | logicExpression {
            $$ = $1;
        }
        | compareExpression {
            $$ = $1;
        }
        | structureMember {
            $$ = $1;
        }
        | functionCallNoSemi {
            $$ = $1;
        }
        | IDENTIFIER {
            $$ = createIdentifierNode($1);
        }

    functionCall:
        IDENTIFIER functionParams SEMICOLON {
            $$ = createFunctionCallNode($1, $2);
        }

    functionCallNoSemi:
        IDENTIFIER  functionParams {
            $$ = createFunctionCallNode($1, $2);
        }

    functionParams:
        OPEN_BRACKET CLOSE_BRACKET{
            $$ = NULL;
        }
        | OPEN_BRACKET params CLOSE_BRACKET{
            $$ = $2;
        }

    params:
        | params COMMA paramOption {
            $$ = createListNode($1, $3);
        }
        | paramOption {
            $$ = createListNode($1, NULL);
        }

    paramOption:
        IDENTIFIER {
            $$ = createIdentifierNode($1);
        }
        | DECIMAL_LITERAL{
            $$ = createDecimalLiteralNode(strtod($1, NULL));
        }
        | INTEGER_LITERAL {
            $$ = createIntLiteralNode(atoi($1));
        }
        | BOOLEAN_LITERAL {
            char val = $1[0];
            $$ = createBoolLiteralNode((val != NULL && val == 'T'));
        }
        | mathExpression {
            $$ = $1;
        }
        | functionCallNoSemi {
            $$ = $1;
        }
        | logicExpression {
            $$ = $1;
        }


    logicExpression:
        logicExpression OR_OP_TOKEN logicAnd {
            $$ = createBinaryOperationNode($1, $3, OR_OP);
        }
        | logicAnd {
            $$ = $1;
        }

    logicAnd:
        logicAnd AND_OP_TOKEN logicNot {
            $$ = createBinaryOperationNode($1, $3, AND_OP);
        }
        | logicNot {
            $$ = $1;
        }

    logicNot:
        NOT_OP_TOKEN logicOperand {
            $$ = createUnaryExpressionNode(NOT_OP, $2);
        }
        | logicOperand {$$ = $1;}

    logicOperand:
        BOOLEAN_LITERAL {
            char val = $1[0];
            $$ = createBoolLiteralNode((val != NULL && val == 'T'));
        }
        | IDENTIFIER {
            $$ = createIdentifierNode($1);
        }
        | functionCallNoSemi {
            $$ = $1;
        }
        | structureMember {
            $$ = $1;
        }
        | OPEN_BRACKET logicExpression CLOSE_BRACKET {
            $$ = $2;
        }
        | compareExpression {
            $$ = $1;
        }

    compareExpression:
        mathExpression COMPARE_OPERATOR mathExpression {
            opType op = getOpTypeFromStr($2);
            $$ = createBinaryOperationNode($1, $3, op);
        }
        | OPEN_BRACKET mathExpression COMPARE_OPERATOR mathExpression CLOSE_BRACKET{
            opType op = getOpTypeFromStr($2);
            $$ = createBinaryOperationNode($2, $4, op);
        }

    mathExpression:
        mathExpression ADD_OP_TOKEN mathTerm {$$ = createBinaryOperationNode($1, $3, ADD_OP);}
        | mathExpression SUB_OP_TOKEN mathTerm {$$ = createBinaryOperationNode($1, $3, SUB_OP);}
        | mathTerm {$$ = $1;}
    
    mathTerm:
      mathTerm MULT_OP_TOKEN mathFactor    { $$ = createBinaryOperationNode($1, $3, MULT_OP); }
    | mathTerm DIV_OP_TOKEN mathFactor    { $$ = createBinaryOperationNode($1, $3, DIV_OP); }
    | mathTerm MOD_OP_TOKEN mathFactor    { $$ = createBinaryOperationNode($1, $3, MOD_OP); }
    | mathFactor                 { $$ = $1; }

    mathFactor:
      mathPower POW_OP_TOKEN mathFactor   { $$ = createBinaryOperationNode($1, $3, POW_OP); }
    | mathPower                  { $$ = $1; }

    mathPower:
      INTEGER_LITERAL            { $$ = createIntLiteralNode(atoi($1)); }
    | DECIMAL_LITERAL            { $$ = createDecimalLiteralNode(strtod($1, NULL)); }
    | IDENTIFIER                 { $$ = createIdentifierNode($1); }
    | functionCallNoSemi         { $$ = $1; }
    | structureMember            { $$ = $1; }
    | OPEN_BRACKET mathExpression CLOSE_BRACKET { $$ = $2; }

    ifStatement:
        IF_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createIfBlockNode(createifStatementNode($3, $6), NULL, NULL);
        }
        | IF_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET elseBlock {
            $$ = createIfBlockNode(createifStatementNode($3, $6), NULL, $8);
        }
        | IF_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET elifBlocks {
            $$ = createIfBlockNode(createifStatementNode($3, $6), $8, NULL);
        }
        | IF_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET elifBlocks elseBlock {
            $$ = createIfBlockNode(createifStatementNode($3, $6), $8, $9);
        }

    elifBlocks:
        elifBlocks elifBlock {
            $$ = createElifList($1, $2);
        }
        | elifBlock {
            $$ = createListNode($1, NULL);
        }

    elifBlock:
        ELIF_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createifStatementNode($3, $6);
        }

    elseBlock: 
        ELSE_KEYWORD OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createifStatementNode(NULL, $3);
        }

    condition:
        BOOLEAN_LITERAL {
            char val = $1[0];
            $$ = createBoolLiteralNode((val != NULL && val == 'T'));
        }
        | logicExpression {
            $$ = $1;    
        }
        | IDENTIFIER {
            $$ = createIdentifierNode($1);
        }
        | compareExpression {
            $$ = $1;
        }

    forLoop:
        LOOP_KEYWORD OPEN_BRACKET asignmentOptions DOUBLE_DOT IDENTIFIER ASIGNMENT_OPERATOR asignmentOptions CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createForLoopNode($3, $7, $5, $10);
        }
        

    whileLoop:
        LOOP_KEYWORD OPEN_BRACKET condition CLOSE_BRACKET OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createWhileLoopNode($3, $6);
        }

    structureDeclaration:
        STRUCT_KEYWORD IDENTIFIER IDENTIFIER SEMICOLON {
            $$ =  createStructDeclarationNode($2, $3);
        }

    structureMemberAsignment:
        IDENTIFIER DOT IDENTIFIER ASIGNMENT_OPERATOR asignmentOptions SEMICOLON {
            $$ = createStructMemberAsignmentNode($1, $3, $5);
        }

    returnCommand:
        RETURN_KEYWORD asignmentOptions SEMICOLON {
            $$ = createReturnCommandNode($2);
        }

    functionDefinitions:
        | functionDefinitions functionDefinition {
            $$ = createListNode($1, $2);
        }
        | functionDefinition {
            $$ = createListNode($1, NULL);
        }

    functionDefinition:
        TYPE_IDENTIFIER FUNCTION_KEYWORD IDENTIFIER functionDefParams OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createFuncDefinitionNode($3, $1[0], $4, $6);
        }
        | VOID_FUNC_KEYWORD IDENTIFIER functionDefParams OPEN_CURLY_BRACKET commands CLOSE_CURLY_BRACKET {
            $$ = createFuncDefinitionNode($2, 'v', $3, $5);
        }

    functionDefParams:
        OPEN_BRACKET CLOSE_BRACKET {
            $$ = NULL;
        }
        | OPEN_BRACKET paramsListFunctionDef CLOSE_BRACKET {
            $$ = $2;
        }

    paramsListFunctionDef:
        paramsListFunctionDef COMMA paramDef {
            $$ = createListNode($1, $3);
        }
        | paramDef {
            $$ = createListNode($1, NULL);
        }

    paramDef:
        TYPE_IDENTIFIER IDENTIFIER {
            $$ = createMemberDefNode($1[0], $2);
        }
    %%

    void yyerrorNoToken(const char* s){
        printf(s);  
        exit(1);
    }

    void yyerror(const char* s) { 
        printf("Error at line %d: %s, unexpected token: %s\n", yylineno, s, yytext);
        exit(1);
    }

    int main(int argc, char* argv[]){
        yydebug = 0;
        int printASTflag = 0;
        char *inputFilename = NULL;

        for (int i = 1; i < argc; i++) {
            if(strcmp(argv[i], "--print-ast") == 0) {
                printASTflag = 1;
            }
            else if(strcmp(argv[i], "--yydebug") == 0) {
                yydebug = 1;
            }else if (strcmp(argv[i], "-i") == 0) {
                if (i + 1 < argc) {
                    inputFilename = argv[i + 1];
                    i++; 
                } else {
                    printf("Error: -i requires a filename\n");
                    return 1;
                }
            }
        }

        if (inputFilename == NULL) {
            printf("Error: No filename provided");
            return 1;
        }

        FILE *inputFile = fopen(inputFilename, "r");
        yyin = inputFile;

        printf("Started parsing\n");
        printf("----------------------\n");
        yyparse();
        printf("Parsing successful\n");
        if(root == NULL){
            printf("NULL root\n");
            exit(1);
        }else if(printASTflag){
            printf("Printing AST\n");
            printf("----------------------\n");
            printAST(root, 0);
        }
        printf("Interpreting\n");
        printf("----------------------\n");
        interpret(root);
        printf("Program done\n");

        fclose(inputFile);
    }

