#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct {
    ASTNode* current;
    struct ElifList* next;
} ElifList;

typedef struct {
    char type;
    union {
        int intVal;
        bool boolVal;
        double decimalVal;
    };
}ValueNode;

typedef struct {
    char* identifier;
    char type;
    bool hasValue;
    union {
        int intVal;
        bool boolVal;
        double decimalVal
    }value;
    struct Variable* next;
} Variable;

typedef struct {
    char* identifier;
    char* type;
    Variable* member;
    struct StructDeclaration* next;
} StructDeclaration;

typedef struct {
    struct Variable* currentScope;
    struct Scope* next;
} Scope;

typedef struct {
    struct StructDeclaration* currentScope;
    struct ScopeStruct* next;
} ScopeStruct;


ValueNode interpret(ASTNode* node);
void interpretProgram(ASTNode* program);
void interpretPrint(ASTNode* toPrint);
ValueNode interpretBinOp(ASTNode* node);
ValueNode interpretMathOp(ASTNode* node);
Variable* findVar(const char* name);
ValueNode interpretIdentifier(ASTNode* node);
ValueNode interpretAsignment(ASTNode* node);
ValueNode interpretCompare(ASTNode* node);
ValueNode interptretUnaryOp(ASTNode* node);
ValueNode interpretLogicOperation(ASTNode* node);
ValueNode interpretIfBlock(ASTNode* node);
ValueNode interpretIfStatement(ASTNode* node);
ValueNode interpretWhileLoop(ASTNode* node);
ValueNode interpretForLoop(ASTNode* node);
ValueNode interpretStructDeclare(ASTNode* node);
ValueNode interpretStructAssignment(ASTNode* node);
ValueNode interpretStructMember(ASTNode* node);
ValueNode interpretFunctionCall(ASTNode* functionCall);
ValueNode interpretReturn(ASTNode* node);

ASTNode* structHasMember(const char* structType, const char* structMember);
Variable* findMemberVar(StructDeclaration* structDec, const char* identifier);
ASTNode* findStructDef(const char* name);
StructDeclaration* findStructDeclaration(const char* name);
ASTNode* findStructDefinitionNode(ASTNode* current, const char* name);
ASTNode* findFunctionDef(ASTNode* current, const char* funcName);
void addCallParams(ASTNode* call, ASTNode* definition, Scope* funcVarScope);
ASTNode* findNthParamNode(ASTNode* current, int currN, int targetN);
int getNParams(ASTNode* current, int currentVal);
void clearScope(Variable* scope);
Variable* getVarForStructDef(ASTNode* structDefList, Variable* currVar);
ElifList* getElifList(ASTNode* node, ElifList* currentList);
void printElifList(ElifList* list);
void freeElifList(ElifList* list);
