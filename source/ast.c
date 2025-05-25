#include "../include/ast.h"

ASTNode* createNode(ASTNodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

ASTNode* createIntLiteralNode(int value){
    ASTNode* node = createNode(AST_LITERAL_INT);
    node->intValue = value;
    return node;
}

ASTNode* createDecimalLiteralNode(double value){
    ASTNode* node = createNode(AST_LITERAL_DECIMAL);
    node->decimalValue = value;
    return node;
}

ASTNode* createBoolLiteralNode(bool value){
    ASTNode* node = createNode(AST_LITERAL_BOOL);
    node->boolValue = value;
    return node;
}

ASTNode* createVariableDeclaration(char* varName, char varType, ASTNode* value){
    //printf("creating var node: name: %s, type: %c\n", varName, varType);
    ASTNode* node = createNode(AST_VAR_DECL);
    node->varDecl.varName = varName;
    node->varDecl.varType = varType;
    node->varDecl.value = value;
    return node;
}

ASTNode* createBinaryOperationNode(ASTNode* left, ASTNode* right, opType op){
    ASTNode* node = createNode(AST_BIN_OP);
    node->binaryExpr.left = left;
    node->binaryExpr.right = right;
    node->binaryExpr.op = op;
    return node;
}

ASTNode* createCommandNode(ASTNode* command){
    ASTNode* node = createNode(AST_COMMAND);
    node->command.command = command;
    return node;
}

ASTNode* createIdentifierNode(char* name){
    ASTNode* node = createNode(AST_IDENTIFIER);
    node->identifier.name = strdup(name);
    return node;
}

ASTNode* createFunctionCallNode(char* name, ASTNode* firstParam){
    ASTNode* node = createNode(AST_FUNCTION_CALL);
    node->functionCall.funcName = strdup(name);
    node->functionCall.params = firstParam;
    return node;
}

ASTNode* createStructMemberNode(char* structIdentifier, char* fieldName){
    ASTNode* node = createNode(AST_STRUCTURE_MEMBER);
    node->structMember.fieldName = fieldName;
    node->structMember.structIdentifier = structIdentifier;
    return node;
}

ASTNode* createVariableAsignmentNode(char* varIdentifier, ASTNode* value){
    ASTNode* node = createNode(AST_VARIABLE_ASIGNMENT);
    node->variableAsignment.varIdentifier = varIdentifier;
    node->variableAsignment.value = value;
    return node;
}

ASTNode* createForLoopNode(ASTNode* startIndex, ASTNode* endIndex, char* varName, ASTNode* commands){
    ASTNode* node = createNode(AST_FOR_LOOP);
    node->forLoop.startIndex = startIndex;
    node->forLoop.endIndex = endIndex;
    node->forLoop.indexVarName = varName;
    node->forLoop.commandBlock = commands;
    return node;
}
ASTNode* createWhileLoopNode(ASTNode* condition, ASTNode* commands){
    ASTNode* node = createNode(AST_WHILE_LOOP);
    node->whileLoop.condition = condition;
    node->whileLoop.commandBlock = commands;
    return node;
}

ASTNode* createStructMemberAsignmentNode(char* structIdentifier, char* memberName, ASTNode* value){
    ASTNode* node = createNode(AST_STRUCTURE_MEMBER_ASIGNMENT);
    node->structMemberAsignment.structIdentifier = structIdentifier;
    node->structMemberAsignment.structMember = memberName;
    node->structMemberAsignment.value = value;
    return node;
}

ASTNode* createStructDeclarationNode(char* structType, char* structName){
    ASTNode* node = createNode(AST_STRUCTURE_DECLARATION);
    node->structDeclaration.structIdentifier = structName;
    node->structDeclaration.structType = structType;
}

ASTNode* createReturnCommandNode(ASTNode* value){
    ASTNode* node = createNode(AST_RETURN);
    node->nodeValue = value;
    return node;
}

ASTNode* createPrintCommandNode(ASTNode* value){
    ASTNode* node = createNode(AST_PRINT);
    node->nodeValue = value;
    return node;
}

ASTNode* createIfBlockNode(ASTNode* ifBlock, ASTNode* elifBlock, ASTNode* elseBlock){
    ASTNode* node = createNode(AST_IF_BLOCK);
    node->ifBlock.ifBlock = ifBlock;
    node->ifBlock.elifBlock = elifBlock;
    node->ifBlock.elseBlock = elseBlock;
    return node;
}

ASTNode* createElifList(ASTNode* current, ASTNode* next){
    ASTNode* node = createNode(AST_ELIF_LIST);
    node->list.current = current;
    node->list.next = next;
    return node;
}

ASTNode* createifStatementNode(ASTNode* condition, ASTNode* commands){
    ASTNode* node = createNode(AST_IF_STATEMENT);
    node->ifStatement.condition = condition;
    node->ifStatement.commands = commands;
    return node;
}


ASTNode* createListNode(ASTNode* current, ASTNode* list){
    ASTNode* node = createNode(AST_LIST);
    node->list.current = current;
    node->list.next = list;
    return node;
}

ASTNode* createProgramNode(ASTNode* structDefs, ASTNode* mainFunc, ASTNode* funcDefs){
    ASTNode* node = createNode(AST_PROGRAM);
    node->program.structDefs = structDefs;
    node->program.mainFunc = mainFunc;
    node->program.funcDefs = funcDefs;
    return node;
}

ASTNode* createFuncDefinitionNode(char* identifier, char returnType, ASTNode* params, ASTNode* commands){
    ASTNode* node = createNode(AST_FUNCTION_DEFINITION);
    node->functionDef.identifier = identifier;
    node->functionDef.returnType = returnType;
    node->functionDef.paramList = params;
    node->functionDef.commands = commands;
    return node;
}

ASTNode* createStructDefinitionNode(char* identifier, ASTNode* members){
    ASTNode* node = createNode(AST_STRUCTURE_DEFINITION);
    node->structDefinition.structName = identifier;
    node->structDefinition.members = members;
    return node;
}
ASTNode* createMemberDefNode(char type, char* identifier){
    ASTNode* node = createNode(AST_MEMBER_DEFINITION);
    node->memberDefinition.identifier = identifier;
    node->memberDefinition.varType = type;
    return node;
}
ASTNode* createUnaryExpressionNode(opType op, ASTNode* value){
    ASTNode* node = createNode(AST_UNARY_OP);
    node->unaryOp.op = op;
    node->unaryOp.value = value;
    return node;
}

void printIndent(int indent) {
    for (int i = 0; i < indent; i++)
        printf("  ");
}

void printAST(ASTNode* node, int indent) {
    
    if (node == NULL){
        printIndent(indent);
        printf("NULL node\n");
        return;
    }

    //printf("node type: %s\n", getASTNodeTypeName(node->type));
    
    if(node->type != AST_LIST && node->type != AST_COMMAND && node->type != AST_ELIF_LIST) printIndent(indent);

    switch (node->type) {
        case AST_ELIF_LIST:
            if(node->list.current != NULL){
                printAST(node->list.current, indent);
            }
            if(node->list.next != NULL){
                printAST(node->list.next, indent);
            }
            break;

        case AST_ELIF_BLOCK:
            if(node->list.current != NULL){
                printAST(node->list.current, indent);
            }
            if(node->list.next != NULL){
                printAST(node->list.next, indent);
            }
            break;

        case AST_UNARY_OP:
            printf("Unary operation: %s value = \n", getOperationName(node->unaryOp.op));
            printAST(node->unaryOp.value, indent + 1);
            break;
        case AST_FUNCTION_DEFINITION:
            printf("Function definition: identifier = %s return type = %c params = \n", node->functionDef.identifier, node->functionDef.returnType);
            printAST(node->functionDef.paramList, indent + 1);
            printIndent(indent);
            printf("commands = \n");
            printAST(node->functionDef.commands, indent + 1);
            break;

        case AST_STRUCTURE_DEFINITION:
            printf("Structure definition: identifier = %s members = \n", node->structDefinition.structName);
            printAST(node->structDefinition.members, indent + 1);
            break;

        case AST_MEMBER_DEFINITION:
            printf("Member definition: identifier = %s type = %c\n", node->memberDefinition.identifier, node->memberDefinition.varType);
            break;

        case AST_PROGRAM:
            printf("Program:\nStructure definitions = \n");
            printAST(node->program.structDefs, indent + 1);
            printf("Main function = \n");
            printAST(node->program.mainFunc, indent + 1);
            printf("Function definitions = \n");
            printAST(node->program.funcDefs, indent + 1);
            break;

        case AST_LIST:
            if(node->list.current != NULL){
                printAST(node->list.current, indent);
            }
            if(node->list.next != NULL){
                printAST(node->list.next, indent);
            }
            break;

        case AST_IF_BLOCK:
            printf("If block: if statement = \n");
            printAST(node->ifBlock.ifBlock, indent + 1);
            printIndent(indent);
            printf("elif block = \n");
            printAST(node->ifBlock.elifBlock, indent + 1);
            printIndent(indent);
            printf("else block = \n");
            printAST(node->ifBlock.elseBlock, indent + 1);
            break;

        case AST_IF_STATEMENT:
            printf("If statement: condition = \n");
            if(node->ifStatement.condition != NULL){
                printAST(node->ifStatement.condition, indent + 1);
            }else{
                printIndent(indent + 1);
                printf("no condition\n");
            }
            printIndent(indent);
            printf("commands = \n");
            printAST(node->ifStatement.commands, indent + 1);
            break;

        case AST_RETURN:
            printf("Return command: value = \n");
            printAST(node->nodeValue, indent + 1);
            break;
        
        case AST_PRINT:
            printf("Print command: value = \n");
            printAST(node->nodeValue, indent + 1);
            break;

        case AST_STRUCTURE_DECLARATION:
            printf("Structure declaration: type = %s identifier = %s\n", node->structDeclaration.structType, node->structDeclaration.structIdentifier);
            break;

        case AST_STRUCTURE_MEMBER_ASIGNMENT:
            printf("Structure member asignment: struct name = %s member name = %s value = \n", node->structMemberAsignment.structIdentifier, node->structMemberAsignment.structMember);
            printAST(node->structMemberAsignment.value, indent + 1);
            break;

        case AST_FOR_LOOP:
            printf("for loop: index = %s startIdex = \n", node->forLoop.indexVarName);
            printAST(node->forLoop.startIndex, indent + 1);
            printIndent(indent);
            printf("end index = \n");
            printAST(node->forLoop.endIndex, indent + 1);
            printIndent(indent);
            printf("commands = \n");
            printAST(node->forLoop.commandBlock, indent + 1);
            break;

        case AST_WHILE_LOOP:
            printf("While loop: condition = \n");
            printAST(node->whileLoop.condition, indent + 1);
            printIndent(indent);
            printf("commands = \n");
            printAST(node->whileLoop.commandBlock, indent + 1);
            break;

        case AST_VARIABLE_ASIGNMENT:
            printf("Variable asignment: name = %s value = \n", node->variableAsignment.varIdentifier);
            printAST(node->variableAsignment.value, indent + 1);
            break;

        case AST_STRUCTURE_MEMBER:
            printf("Structure member: name = %s field = %s\n", node->structMember.structIdentifier, node->structMember.fieldName);
            break;

        case AST_FUNCTION_CALL:
            printf("Function call: name = %s params = \n", node->functionCall.funcName);
            printAST(node->functionCall.params, indent + 1 );
            break;

       case AST_IDENTIFIER:
            printf("Identifier: %s\n", node->identifier.name);
            break;

        case AST_COMMAND:
            printAST(node->command.command, indent );
            break;

        case AST_VAR_DECL:
            printf("Variable declaration: name = %s type = %c, value = \n", node->varDecl.varName, node->varDecl.varType);
            printAST(node->varDecl.value, indent + 1);
            break;

        case AST_LITERAL_INT:
            printf("Int literal: %d\n", node->intValue);
            break;

        case AST_LITERAL_BOOL:
            printf("Bool literal: %s\n", node->boolValue ? "true" : "false");
            break;

        case AST_LITERAL_DECIMAL:
            printf("Decimal literal: %f\n", node->decimalValue);
            break;

        case AST_BIN_OP:
            printf("Binary operation: %s\n", getOperationName(node->binaryExpr.op));
            printAST(node->binaryExpr.left, indent + 1);
            printAST(node->binaryExpr.right, indent + 1);
            break;

        default:
            printIndent(indent);
            printf("Unknown node type %d\n", node->type);
            break;
    }
}

const char* getOperationName(opType type){
    switch(type) {
        case ADD_OP:  return "+";
        case SUB_OP:  return "-";
        case MULT_OP: return "*";
        case DIV_OP:  return "/";
        case MOD_OP:  return "%";
        case POW_OP:  return "^";
        case AND_OP:  return "&&";
        case OR_OP:   return "||";
        case NOT_OP:  return "!";
        case GT_OP:   return ">";
        case GE_OP:   return ">=";
        case LT_OP:   return "<";
        case LE_OP:   return "<=";
        case EQ_OP:   return "==";
        case NE_OP:   return "!=";
        default:      return "unknown_op";
    }
}

const char* getASTNodeTypeName(ASTNodeType type) {
    switch (type) {
        case AST_PROGRAM: return "AST_PROGRAM";
        case AST_VAR_DECL: return "AST_VAR_DECL";
        case AST_LITERAL_INT: return "AST_LITERAL_INT";
        case AST_LITERAL_BOOL: return "AST_LITERAL_BOOL";
        case AST_LITERAL_DECIMAL: return "AST_LITERAL_DECIMAL";
        case AST_IDENTIFIER: return "AST_IDENTIFIER";
        case AST_BIN_OP: return "AST_BIN_OP";
        case AST_PRINT_STATEMENT: return "AST_PRINT_STATEMENT";
        case AST_COMMAND: return "AST_COMMAND";
        case AST_FUNCTION_CALL: return "AST_FUNCTION_CALL";
        case AST_FUNCTION_CALL_PARAM: return "AST_FUNCTION_CALL_PARAM";
        case AST_STRUCTURE_MEMBER: return "AST_STRUCTURE_MEMBER";
        case AST_VARIABLE_ASIGNMENT: return "AST_VARIABLE_ASIGNMENT";
        case AST_STRUCTURE_MEMBER_ASIGNMENT: return "AST_STRUCTURE_MEMBER_ASIGNMENT";
        case AST_FOR_LOOP: return "AST_FOR_LOOP";
        case AST_WHILE_LOOP: return "AST_WHILE_LOOP";
        case AST_STRUCTURE_DECLARATION: return "AST_STRUCTURE_DECLARATION";
        case AST_RETURN: return "AST_RETURN";
        case AST_PRINT: return "AST_PRINT";
        case AST_IF_BLOCK: return "AST_IF_BLOCK";
        case AST_IF_STATEMENT: return "AST_IF_STATEMENT";
        case AST_ELIF_BLOCK: return "AST_ELIF_BLOCK";
        case AST_LIST: return "AST_LIST";
        case AST_FUNCTION_DEFINITION: return "AST_FUNCTION_DEFINITION";
        case AST_STRUCTURE_DEFINITION: return "AST_STRUCTURE_DEFINITION";
        case AST_MEMBER_DEFINITION: return "AST_MEMBER_DEFINITION";
        case AST_ELIF_LIST: return "AST_ELIF_LIST";
        default: return "UNKNOWN_AST_NODE_TYPE";
    }
}

opType getOpTypeFromStr(const char* opStr){
    if (strcmp(opStr, "+") == 0) return ADD_OP;
    if (strcmp(opStr, "-") == 0) return SUB_OP;
    if (strcmp(opStr, "*") == 0) return MULT_OP;
    if (strcmp(opStr, "/") == 0) return DIV_OP;
    if (strcmp(opStr, "%") == 0) return MOD_OP;
    if (strcmp(opStr, "^") == 0) return POW_OP;
    if (strcmp(opStr, "&&") == 0) return AND_OP;
    if (strcmp(opStr, "||") == 0) return OR_OP;
    if (strcmp(opStr, "!") == 0) return NOT_OP;
    if (strcmp(opStr, ">") == 0) return GT_OP;
    if (strcmp(opStr, ">=") == 0) return GE_OP;
    if (strcmp(opStr, "<") == 0) return LT_OP;
    if (strcmp(opStr, "<=") == 0) return LE_OP;
    if (strcmp(opStr, "==") == 0) return EQ_OP;
    if(strcmp(opStr, "!=") == 0) return NE_OP;
    if(strcmp(opStr, "!") == 0) return NOT_OP;
    return -1;
}




