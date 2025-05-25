#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define BOOL_TYPE 'B'
#define DECIMAL_TYPE 'D'
#define INT_TYPE 'I'
#define TYPE_NONE 'N'



typedef struct ASTNode ASTNode;

typedef enum {
    ADD_OP,
    SUB_OP,
    MULT_OP, 
    DIV_OP, 
    MOD_OP, 
    POW_OP,
    AND_OP,
    OR_OP,
    NOT_OP,
    GT_OP,
    GE_OP,
    LT_OP,
    LE_OP,
    EQ_OP,
    NE_OP
} opType;

typedef enum {
    AST_PROGRAM,
    AST_VAR_DECL,
    AST_LITERAL_INT,
    AST_LITERAL_BOOL,
    AST_LITERAL_DECIMAL,
    AST_IDENTIFIER,
    AST_BIN_OP,
    AST_PRINT_STATEMENT,
    AST_COMMAND,
    AST_FUNCTION_CALL,
    AST_FUNCTION_CALL_PARAM,
    AST_STRUCTURE_MEMBER,
    AST_VARIABLE_ASIGNMENT,
    AST_STRUCTURE_MEMBER_ASIGNMENT,
    AST_FOR_LOOP,
    AST_WHILE_LOOP,
    AST_STRUCTURE_DECLARATION,
    AST_RETURN,
    AST_PRINT,
    AST_IF_BLOCK,
    AST_IF_STATEMENT,
    AST_ELIF_BLOCK,
    AST_LIST,
    AST_FUNCTION_DEFINITION,
    AST_STRUCTURE_DEFINITION,
    AST_MEMBER_DEFINITION,
    AST_UNARY_OP,
    AST_ELIF_LIST

} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        int intValue;
        double decimalValue;
        bool boolValue;  
        ASTNode* nodeValue;   
        
        struct {
            char* funcName;
            ASTNode* params;
        } functionCall;

        struct {
            opType op;
            ASTNode* value;
        }unaryOp;
        struct {
            char* name;
        } identifier;

        struct {
            opType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binaryExpr;

        struct {
            char varType;
            char* varName;
            struct ASTNode* value;
        } varDecl;

        struct{
            ASTNode* command;
        } command;

        struct{
            char* structIdentifier;
            char* fieldName;
        } structMember;

        struct{
            char* varIdentifier;
            ASTNode* value;
        } variableAsignment;

        struct {
            ASTNode* startIndex;
            ASTNode* endIndex;
            char* indexVarName;
            ASTNode* commandBlock;
        } forLoop;

        struct {
            ASTNode* condition;
            ASTNode* commandBlock;
        } whileLoop;

        struct {
            char* structIdentifier;
            char* structMember;
            ASTNode* value;
        } structMemberAsignment;

        struct {
            char* structType;
            char* structIdentifier;
        } structDeclaration;

        struct {
            ASTNode* condition;
            ASTNode* commands;
        } ifStatement;

        struct {
            ASTNode* ifBlock;
            ASTNode* elifBlock;
            ASTNode* elseBlock;
        } ifBlock;

        struct {
            ASTNode* structDefs;
            ASTNode* mainFunc;
            ASTNode* funcDefs;
        } program;

        struct {
            ASTNode* current;
            ASTNode* next;
        } list;

        struct {
            char* identifier;
            char returnType;
            ASTNode* paramList;
            ASTNode* commands;
        } functionDef;

        struct {
            char* identifier;
            char varType;
        } memberDefinition;

        struct {
            char* structName;
            ASTNode* members;
        } structDefinition;



    };
} ASTNode;

ASTNode* createNode(ASTNodeType type);
ASTNode* createIntLiteralNode(int value);
ASTNode* createDecimalLiteralNode(double value);
ASTNode* createBoolLiteralNode(bool value);
ASTNode* createVariableDeclaration(char* varName, char varType, ASTNode* value);
ASTNode* createBinaryOperationNode(ASTNode* left, ASTNode* right, opType op);
ASTNode* createCommandNode(ASTNode* command);
ASTNode* createFunctionCallNode(char* name, ASTNode* firstParam);
ASTNode* createIdentifierNode(char* name);
ASTNode* createStructMemberNode(char* structIdentifier, char* fieldName);
ASTNode* createVariableAsignmentNode(char* varIdentifier, ASTNode* value);
ASTNode* createForLoopNode(ASTNode* startIndex, ASTNode* endIndex, char* varName, ASTNode* commands);
ASTNode* createWhileLoopNode(ASTNode* condition, ASTNode* commands);
ASTNode* createStructMemberAsignmentNode(char* structIdentifier, char* memberName, ASTNode* value);
ASTNode* createStructDeclarationNode(char* structType, char* structName);
ASTNode* createReturnCommandNode(ASTNode* value);
ASTNode* createPrintCommandNode(ASTNode* value);
ASTNode* createIfBlockNode(ASTNode* ifBlock, ASTNode* elifBlock, ASTNode* elseBlock);
ASTNode* createifStatementNode(ASTNode* condition, ASTNode* commands);
ASTNode* createListNode(ASTNode* current, ASTNode* list);
ASTNode* createProgramNode(ASTNode* structDefs, ASTNode* mainFunc, ASTNode* funcDefs);
ASTNode* createFuncDefinitionNode(char* identifier, char returnType, ASTNode* params, ASTNode* commands);
ASTNode* createStructDefinitionNode(char* identifier, ASTNode* members);
ASTNode* createMemberDefNode(char type, char* identifier);
ASTNode* createUnaryExpressionNode(opType op, ASTNode* value);
ASTNode* createElifList(ASTNode* current, ASTNode* next);
const char* getASTNodeTypeName(ASTNodeType type);
void printAST(ASTNode* node, int indent);
void printIndent(int indent);
const char* getOperationName(opType type);



opType getOpTypeFromStr(const char* opStr);


#endif
