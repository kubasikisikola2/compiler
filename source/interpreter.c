#include "../include/interpreter.h"

Scope* scopeVar = NULL;
ScopeStruct* scopeStruct = NULL;
ASTNode* structureDefList = NULL;
ASTNode* funcDefList = NULL;
StructDeclaration* structures = NULL;
Variable* currentFuncRes = NULL;

ValueNode interpret(ASTNode* node) {
    
    if(node == NULL){
        printf("NULL node in interpret"); 
        return (ValueNode) {.type = TYPE_NONE};
    }
  //  if(node->type != AST_LIST) printf("node type in interpret: %s\n", getASTNodeTypeName(node->type));



    switch (node->type) {
        case AST_LITERAL_INT:
            return (ValueNode) {.type = INT_TYPE, .intVal = node->intValue};
            break;

        case AST_LITERAL_BOOL:
            return (ValueNode) {.type = BOOL_TYPE, .boolVal = node->boolValue};
            break;

        case AST_LITERAL_DECIMAL:
            return (ValueNode) {.type = DECIMAL_TYPE, .decimalVal = node->decimalValue};
            break;

        case AST_PRINT:
            interpretPrint(node);
            break;

        case AST_PROGRAM:
            interpretProgram(node);
            break;

        case AST_LIST:
            if(node->list.current != NULL) interpret(node->list.current);
            if(node->list.next != NULL) interpret(node->list.next);
            break;

        case AST_COMMAND:
            interpret(node->command.command);
            break;

        case AST_BIN_OP:
            return interpretBinOp(node);
            break;

        case AST_VAR_DECL:
            declareVariable(node);
            return (ValueNode) {.type = TYPE_NONE};
            break;

        case AST_IDENTIFIER:
            return interpretIdentifier(node);
            break;

        case AST_VARIABLE_ASIGNMENT:
            return interpretAsignment(node);
            break;

        case AST_UNARY_OP:
            return interptretUnaryOp(node);
            break;

        case AST_IF_BLOCK:
            return interpretIfBlock(node);
            break;

        case AST_IF_STATEMENT:
            return interpretIfStatement(node);
            break;

        case AST_FOR_LOOP:
            return interpretForLoop(node);
            break;

        case AST_WHILE_LOOP:
            return interpretWhileLoop(node);
            break;
        
        case AST_STRUCTURE_DECLARATION:
            return interpretStructDeclare(node);
            break;
        
        case AST_STRUCTURE_MEMBER_ASIGNMENT:
            return interpretStructAssignment(node);
            break;
        case AST_STRUCTURE_MEMBER:
            return interpretStructMember(node);
            break;
        
        case AST_FUNCTION_CALL:
            return interpretFunctionCall(node);
            break;
        
        case AST_RETURN:
            return interpretReturn(node);
            break;

    }
    return (ValueNode) {.type = TYPE_NONE};
}

ValueNode interpretReturn(ASTNode* node){
    ValueNode val = interpret(node->nodeValue);

    currentFuncRes = malloc(sizeof(Variable));

    switch(val.type){
        case INT_TYPE:
            currentFuncRes->type = INT_TYPE;
            currentFuncRes->value.intVal = val.intVal;
            break;
        case BOOL_TYPE:
                currentFuncRes->type = BOOL_TYPE;
            currentFuncRes->value.boolVal = val.boolVal;
            break;
        case DECIMAL_TYPE:
                currentFuncRes->type = DECIMAL_TYPE;
            currentFuncRes->value.decimalVal = val.decimalVal;
            break;
    }

    return (ValueNode) {.type = TYPE_NONE};
}

ValueNode interpretFunctionCall(ASTNode* functionCall){

    ASTNode* functionDefinition = findFunctionDef(funcDefList, functionCall->functionCall.funcName);

    if(functionDefinition == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Function '%s' not defined.\n", functionCall->functionCall.funcName);
        interpreterError(buffer);
    }

    Scope* funcVarScope = malloc(sizeof(Scope));
    funcVarScope->currentScope = NULL;
    ScopeStruct* funcStructScope = malloc(sizeof(ScopeStruct));
    funcStructScope->currentScope = NULL;
    addCallParams(functionCall, functionDefinition, funcVarScope);
   
    funcVarScope->next = scopeVar;

    scopeVar = funcVarScope; 

    funcStructScope->next = scopeStruct;
    scopeStruct = funcStructScope;


    interpret(functionDefinition->functionDef.commands);

    clearScope(scopeVar->currentScope);
    clearScope(scopeStruct->currentScope);

    scopeVar = scopeVar->next;
    scopeStruct = scopeStruct->next;

    free(funcVarScope);
    free(funcStructScope);

    if(functionDefinition->functionDef.returnType == 'v'){
        return (ValueNode) {.type = TYPE_NONE};
    }

    ValueNode res;
    res.type = functionDefinition->functionDef.returnType;

    if(currentFuncRes == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Function '%s' does not return value\n", functionCall->functionCall.funcName);
        interpreterError(buffer);
    }

    if(currentFuncRes->type != functionDefinition->functionDef.returnType){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Invalid return type for function '%s'\n", functionCall->functionCall.funcName);
        interpreterError(buffer);
    }

    switch(functionDefinition->functionDef.returnType){
        case INT_TYPE:
            res.intVal = currentFuncRes->value.intVal;
            break;
        case BOOL_TYPE:
            res.boolVal = currentFuncRes->value.boolVal;
            break;
        case DECIMAL_TYPE:
            res.decimalVal = currentFuncRes->value.decimalVal;
            break;
            
    }
    free(currentFuncRes);
    currentFuncRes = NULL;

    return res;
}

void printVarScope(Variable* currentVar){
    Variable* current = currentVar;
    printf("Printing current variable scope\n");

    while(current != NULL){
        printf("Variable name = %s, type = %c\n", current->identifier, current->type);
        current = current->next; 
    }
}

void clearScope(Variable* scope){

    Variable* current = scope;
    while (current != NULL) {
        Variable* next = current->next;

        if (current->identifier) {
            free(current->identifier);
        }
        free(current);
        current = next;
    }
}

void addCallParams(ASTNode* call, ASTNode* definition, Scope* funcVarScope){
    ASTNode* defParams = definition->functionDef.paramList;
    ASTNode* callParams = call->functionCall.params;

    int nParamsDef = getNParams(defParams, 0);
    int nParamsCall = getNParams(callParams, 0);


    if(nParamsDef != nParamsCall){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Invalid number of parameters for function '%s'\n", definition->functionDef.identifier);
        interpreterError(buffer);
    }

    for(int i = 1; i <= nParamsDef; i++){
        int n = nParamsDef - (i - 1);
        ASTNode* currentParamDef = findNthParamNode(defParams, 0,  n);
        ASTNode* currentParamCall = findNthParamNode(callParams, 0,  n);

        ValueNode currentVal = interpret(currentParamCall);

        if(currentVal.type != currentParamDef->memberDefinition.varType){
             char buffer[100];
            snprintf(buffer, sizeof(buffer), "Error: Invalid type for parameter %d when calling function '%s'\n", i, definition->functionDef.identifier);
            interpreterError(buffer);
        }

        Variable* newVar = malloc(sizeof(Variable));
        newVar->hasValue = true;
        newVar->identifier = strdup(currentParamDef->memberDefinition.identifier);
        newVar->type = currentParamDef->memberDefinition.varType;

        newVar->next = funcVarScope->currentScope;
        funcVarScope->currentScope = newVar;

        switch(newVar->type){
            case INT_TYPE:
                newVar->value.intVal = currentVal.intVal;
                break;
            case BOOL_TYPE:
                newVar->value.boolVal = currentVal.boolVal;
            break;
            case DECIMAL_TYPE:
                newVar->value.decimalVal = currentVal.decimalVal;
            break;
        }
    }

}   

int getNParams(ASTNode* current, int currentVal){
    if(current == NULL) return currentVal;

    if (current->type != AST_LIST){
        return currentVal + 1;
    }

    if(current->type == AST_LIST){
        ASTNode* found = getNParams(current->list.current, currentVal + 1);        
        return getNParams(current->list.current, currentVal) + getNParams(current->list.next, currentVal);
    }

    return 0;
}
ASTNode* findNthParamNode(ASTNode* current, int currN, int targetN){
    if(current == NULL) return NULL;

    if (current->type != AST_LIST){
        if(currN == targetN) {
            return current;
        }
    }

    if(current->type == AST_LIST){
        ASTNode* found = findNthParamNode(current->list.current, currN + 1, targetN);
        if(found != NULL){
            return found;
        }
        
        return findNthParamNode(current->list.next, currN + 1, targetN);
    }

    return NULL;
}



ASTNode* findFunctionDef(ASTNode* current, const char* funcName){

   if(current == NULL) return NULL;

    if (current->type == AST_FUNCTION_DEFINITION){

        if(strcmp(current->functionDef.identifier, funcName) == 0) {
            return current;
        }
    }

    if(current->type == AST_LIST){
        ASTNode* found = findFunctionDef(current->list.current, funcName);
        if(found != NULL){
            return found;
        }
        
        return findFunctionDef(current->list.next, funcName);
    }

    return NULL;
}



ValueNode interpretStructAssignment(ASTNode* node){
    StructDeclaration* structDec = findStructDeclaration(node->structMemberAsignment.structIdentifier); 

    if(structDec == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Structure '%s' not declared.\n", node->structMemberAsignment.structIdentifier);
        interpreterError(buffer);
    }

   Variable* currentVar = findMemberVar(structDec, node->structMemberAsignment.structMember);

    if(currentVar == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Structure '%s' has no member '%s'\n", node->structMemberAsignment.structIdentifier, node->structMemberAsignment.structMember);
        interpreterError(buffer);
    }

    ValueNode val = interpret(node->structMemberAsignment.value);

    if(currentVar->type != val.type){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: cannot assign type '%c' to type '%c'\n", val.type, currentVar->type);
        interpreterError(buffer); 
    }

    switch(val.type){
        case INT_TYPE:
            currentVar->value.intVal = val.intVal;
            break;
        case BOOL_TYPE:
            currentVar->value.boolVal = val.boolVal;
            break;
        case DECIMAL_TYPE:
            currentVar->value.decimalVal = val.decimalVal;
            break;

    }

    currentVar->hasValue = true;

    return (ValueNode) {.type = TYPE_NONE};

}

ValueNode interpretStructMember(ASTNode* node){
    StructDeclaration* structDec = findStructDeclaration(node->structMember.structIdentifier);

    if(structDec == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Structure '%s' not declared.\n", node->structMemberAsignment.structIdentifier);
        interpreterError(buffer);
    }

   Variable* currentVar = findMemberVar(structDec, node->structMemberAsignment.structMember);

    if(currentVar == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Structure '%s' has no member '%s'\n", node->structMemberAsignment.structIdentifier, node->structMemberAsignment.structMember);
        interpreterError(buffer);
    }

    if(!currentVar->hasValue){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Structure '%s' has undeclared member '%s'\n", node->structMemberAsignment.structIdentifier, node->structMemberAsignment.structMember);
        interpreterError(buffer);
    }

    ValueNode res;
    res.type = currentVar->type;
    switch (currentVar->type){
    case INT_TYPE:
        res.intVal = currentVar->value.intVal;
        break;
    case BOOL_TYPE:
        res.boolVal = currentVar->value.boolVal;
        break;
    case DECIMAL_TYPE:
        res.decimalVal = currentVar->value.decimalVal;
        break;
    }

    return res;
}

ValueNode interpretStructDeclare(ASTNode* node){
    char* name = node->structDeclaration.structIdentifier;
    if(findStructDeclaration(name) != NULL || findVar(name)){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Variable '%s' already declared.\n", name);
        interpreterError(buffer);
    }



    ASTNode* structDef = findStructDef(node->structDeclaration.structType);
   
    if(structDef == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Unknown structure type '%s'.\n", node->structDeclaration.structType);
        interpreterError(buffer);
    }

    StructDeclaration* declaration = malloc(sizeof(StructDeclaration));
    Variable* current = declaration->member;
    ASTNode* currentDef = structDef->structDefinition.members->list.current;


    Variable* structVar = getVarForStructDef(structDef->structDefinition.members, NULL);
    

    declaration->member = structVar;
    declaration->next = scopeStruct->currentScope;
    scopeStruct->currentScope = declaration;
    declaration->identifier = strdup(name);

    return (ValueNode) {.type = TYPE_NONE};

}

Variable* getVarForStructDef(ASTNode* structDefList, Variable* currVar){
    Variable* newVar;

    if(structDefList->type == AST_MEMBER_DEFINITION){
        newVar = malloc(sizeof(Variable));
        newVar->identifier = strdup(structDefList->memberDefinition.identifier);
        newVar->hasValue = false;
        newVar->type = structDefList->memberDefinition.varType;
        newVar->next = currVar;
        return newVar;
    }

    if(structDefList->list.next != NULL){
        newVar = getVarForStructDef(structDefList->list.next, currVar);
        return getVarForStructDef(structDefList->list.current, newVar); 
    }

    return getVarForStructDef(structDefList->list.current, currVar); 

}

ValueNode interpretWhileLoop(ASTNode* node){
    ValueNode condition = interpret(node->whileLoop.condition);
    bool conditionVal = condition.boolVal;

    while(conditionVal){
        interpret(node->whileLoop.commandBlock);
        condition = interpret(node->whileLoop.condition);
        conditionVal = condition.boolVal;
    }
    
    return (ValueNode) {.type = TYPE_NONE};
}

ValueNode interpretForLoop(ASTNode* node){
    ValueNode startIdx = interpret(node->forLoop.startIndex);
    ValueNode endIdx = interpret(node->forLoop.endIndex);
    
    if(startIdx.type != INT_TYPE || endIdx.type != INT_TYPE){
        interpreterError("Error: invalid for loop index");
    }

    if(findVar(node->forLoop.indexVarName)){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Variable '%s' already declared.\n", node->forLoop.indexVarName);
        interpreterError(buffer);
    }

    Variable* var = malloc(sizeof(Variable));
    var->identifier = strdup(node->forLoop.indexVarName);
    var->type = INT_TYPE;
    var->hasValue = true;
    var->value.intVal = startIdx.intVal;
    var->next = scopeVar->currentScope;
    scopeVar->currentScope = var;
    ASTNode* oneLiteralNode = createIntLiteralNode(1);
    ASTNode* endNode = createIntLiteralNode(endIdx.intVal);
    ASTNode* startNode = createIdentifierNode(node->forLoop.indexVarName);
    ASTNode* loopCondition = createBinaryOperationNode(startNode, endNode, LT_OP);
    ASTNode* addToIdxVar = createBinaryOperationNode(startNode, oneLiteralNode, ADD_OP);
    ASTNode* assignToIdxNode = createVariableAsignmentNode(node->forLoop.indexVarName, addToIdxVar);


    ValueNode condition = interpret(loopCondition);
    bool conditionValue = condition.boolVal;
    while(conditionValue){
        interpret(node->forLoop.commandBlock);
        interpret(assignToIdxNode);
        condition = interpret(loopCondition);
        conditionValue = condition.boolVal;
    }

    free(oneLiteralNode);
    free(endNode);
    free(startNode);
    free(loopCondition);
    free(addToIdxVar);
    return (ValueNode) {.type = TYPE_NONE};

}
ValueNode interpretIfBlock(ASTNode* node){
    ValueNode ifStatement = interpret(node->ifBlock.ifBlock);

    if(ifStatement.boolVal){
        return (ValueNode) {.type = TYPE_NONE};
    }

    if(node->ifBlock.elifBlock != NULL){
        ElifList* elifList = NULL;
        elifList = getElifList(node->ifBlock.elifBlock, elifList);

        ElifList* head = elifList;
        while(head != NULL){
            ValueNode elifStatement = interpret(head->current);
            if(elifStatement.boolVal){
                return (ValueNode) {.type = TYPE_NONE};
            }
            head = head->next;
        }
        freeElifList(elifList);
    
        ValueNode elifRes = interpret(node->ifBlock.elifBlock);
        if(elifRes.boolVal){
            return (ValueNode) {.type = TYPE_NONE};
        }
    }

    if(node->ifBlock.elseBlock != NULL){
        interpret(node->ifBlock.elseBlock);
    }
    return (ValueNode) {.type = TYPE_NONE};
}

ValueNode interpretIfStatement(ASTNode* node){
    ValueNode executed;
    executed.type = BOOL_TYPE;
    if(node->ifStatement.condition == NULL){
        executed.boolVal = true;
        interpret(node->ifStatement.commands);
        return executed;
    }

    ValueNode conditionRes = interpret(node->ifStatement.condition);
    if(conditionRes.type != BOOL_TYPE){
        interpreterError("Error: invalid condition\n");
    }

    if(conditionRes.boolVal){
        interpret(node->ifStatement.commands);
        executed.boolVal = true;
    }else{
        executed.boolVal = false;
    }

    return executed;
}
void printElifList(ElifList* list){
    if(list != NULL){
        printAST(list->current, 0);
    }
    if(list->next != NULL){
        printElifList(list->next);
    }
}

ElifList* getElifList(ASTNode* node, ElifList* currentList){
    ElifList* newList;

    if(node->type == AST_IF_STATEMENT){
        newList = malloc(sizeof(ElifList));
        newList->current = node;
        newList->next = currentList;
        return newList;
    }

    if(node->list.next != NULL){
        newList = getElifList(node->list.next, currentList);
        return getElifList(node->list.current, newList); 
    }

    return getElifList(node->list.current, currentList); 
}

ValueNode interpretAsignment(ASTNode* node){
    Variable* var = findVar(node->variableAsignment.varIdentifier);
    ValueNode val = interpret(node->variableAsignment.value);

    if(var == NULL){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Unknown variable: %s\n", node->variableAsignment.varIdentifier);
        interpreterError(buffer);
    }

    if(var->type != val.type){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: cannot assign type '%c' to type '%c' variable: %s\n", val.type, var->type, node->variableAsignment.varIdentifier);
        interpreterError(buffer);
    }

    var->hasValue = true;

    switch(var->type){
        case INT_TYPE:
            var->value.intVal = val.intVal;
            break;
        case DECIMAL_TYPE:
            var->value.decimalVal = val.decimalVal;
            break;
        case BOOL_TYPE:
            var->value.boolVal = val.boolVal;
            break;
        
    }

}


ValueNode interpretLogicOperation(ASTNode* node){
    ValueNode res;
    res.type = BOOL_TYPE;

    ValueNode left = interpret(node->binaryExpr.left);
    ValueNode right = interpret(node->binaryExpr.right);

    if(left.type != BOOL_TYPE || right.type != BOOL_TYPE){
        interpreterError("Invalid logic expression");
    }

    switch(node->binaryExpr.op){
        case AND_OP:
            res.boolVal = left.boolVal && right.boolVal;
            break;
        case OR_OP:
            res.boolVal = left.boolVal || right.boolVal;
    }

    return res;
}

ValueNode interptretUnaryOp(ASTNode* node){
    ValueNode val = interpret(node->unaryOp.value);
    ValueNode res;
    res.type = BOOL_TYPE;

    switch(node->unaryOp.op){
        case NOT_OP:
            if(val.type != BOOL_TYPE){
                interpreterError("Invalid not operation");
            }
            res.boolVal = !val.boolVal;
            break;
    }

    return res;
}

ValueNode interpretBinOp(ASTNode* node){
    opType op = node->binaryExpr.op;
    if( 
        op == ADD_OP || 
        op == SUB_OP || 
        op == MULT_OP || 
        op == DIV_OP || 
        op == MOD_OP || 
        op == POW_OP){
           return interpretMathOp(node);
    }

    if(
        op == GE_OP ||
        op == GT_OP ||
        op == LT_OP ||
        op == LE_OP ||
        op == EQ_OP ||
        op == NE_OP
    ){
        return interpretCompare(node);
    }

    return interpretLogicOperation(node);

}

ValueNode interpretCompare(ASTNode* node){
    ValueNode left = interpret(node->binaryExpr.left);
    ValueNode right = interpret(node->binaryExpr.right);
    ValueNode res;
    res.type = BOOL_TYPE;

    if(left.type == BOOL_TYPE || right.type == BOOL_TYPE){
        interpreterError("Error: invalid compare expression");
    }

    double leftVal = left.type == INT_TYPE ? (double) left.intVal : left.decimalVal;
    double rightVal = right.type == INT_TYPE ? (double) right.intVal : right.decimalVal;
    switch(node->binaryExpr.op){
        case EQ_OP:
            res.boolVal = leftVal == rightVal;
            break;
        case GE_OP:
            res.boolVal = leftVal >= rightVal;
            break;
        case GT_OP:
            res.boolVal = leftVal > rightVal;
            break;
        case LE_OP:
            res.boolVal = leftVal <= rightVal;
            break;
        case LT_OP:
            res.boolVal = leftVal < rightVal;
            break;
        case NE_OP:
            res.boolVal = leftVal != rightVal;
            break;
    }

    return res;

    
}
 
ValueNode interpretMathOp(ASTNode* node){
    ValueNode left = interpret(node->binaryExpr.left);
    ValueNode right = interpret(node->binaryExpr.right);

    ValueNode res;

    if(left.type == INT_TYPE && right.type == INT_TYPE){
        res.type = INT_TYPE;
        switch(node->binaryExpr.op){
            case ADD_OP:
                res.intVal = left.intVal + right.intVal;
                break;
            case SUB_OP:
                res.intVal = left.intVal - right.intVal;
                break;
            case MULT_OP:
                res.intVal = left.intVal * right.intVal;
                break;
            case DIV_OP:
                res.intVal = left.intVal / right.intVal;
                break;
            case POW_OP:
                res.intVal = pow(left.intVal, right.intVal);
                break;
            case MOD_OP:
                res.intVal = left.intVal % right.intVal;
                break;
        }
    }else{
        res.type = DECIMAL_TYPE;
        double leftVal;
        double rightVal;

        if(left.type == DECIMAL_TYPE){
            leftVal = left.decimalVal;
        }else{
            leftVal = left.intVal;
        }

        if(right.type == DECIMAL_TYPE){
            rightVal = right.decimalVal;
        }else{
            rightVal = right.intVal;
        }

        switch(node->binaryExpr.op){
            case ADD_OP:
                res.decimalVal = leftVal + rightVal;
                break;
            case SUB_OP:
                res.decimalVal = leftVal + rightVal;
                break;
            case MULT_OP:
                res.decimalVal = leftVal + rightVal;
                break;
            case DIV_OP:
                res.decimalVal = leftVal + rightVal;
                break;
            case POW_OP:
                res.intVal = pow(leftVal, rightVal);
                break;
            case MOD_OP:
                interpreterError("invalid mod operation");

        }
    }

    return res;
}

ValueNode interpretIdentifier(ASTNode* node){
    ValueNode res;
    Variable* var = findVar(node->identifier.name);
    if(var == NULL) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Variable '%s' not declared\n", node->identifier.name);
        interpreterError(buffer);
    }

    if(!var->hasValue){
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Variable '%s' has no value\n", node->identifier.name);
        interpreterError(buffer);
    }

    switch(var->type){
        case INT_TYPE:
            res.type = INT_TYPE;
            res.intVal = var->value.intVal;
            break;
        case DECIMAL_TYPE:
            res.type = DECIMAL_TYPE;
            res.decimalVal = var->value.decimalVal;
            break;
        case BOOL_TYPE:
            res.type = BOOL_TYPE;
            res.boolVal = var->value.boolVal;
            break;
    }

    return res;

}

void interpretProgram(ASTNode* program){
    scopeStruct = malloc(sizeof(ScopeStruct));
    scopeVar = malloc(sizeof(Scope));
    scopeStruct->currentScope = NULL;
    scopeVar->currentScope = NULL;
    structureDefList = program->program.structDefs;
    funcDefList = program->program.funcDefs;
    interpret(program->program.mainFunc);
}

void interpretPrint(ASTNode* toPrint){
    ValueNode node = interpret(toPrint->nodeValue);
    switch(node.type){
        case BOOL_TYPE:
            if(node.boolVal){
                printf("true\n");
            }else{
                printf("false\n");
            }
            break;
        case DECIMAL_TYPE:
            printf("%f\n", node.decimalVal);
            break;
        case INT_TYPE:
            printf("%i\n", node.intVal);
            break;
    }
}

ASTNode* findStructDef(const char* name){
    return findStructDefinitionNode(structureDefList, name);
}

ASTNode* findStructDefinitionNode(ASTNode* current, const char* name){

    if(current == NULL) return NULL;

    if (current->type == AST_STRUCTURE_DEFINITION){
        if(strcmp(current->structDefinition.structName, name) == 0) {
            return current;
        }
    }

    if(current->type == AST_LIST){
        ASTNode* found = findStructDefinitionNode(current->list.current, name);
        if(found != NULL){
            return found;
        }
        
        return findStructDefinitionNode(current->list.next, name);
    }

    return NULL;
}

Variable* findMemberVar(StructDeclaration* structDec, const char* identifier){
    Variable* currentVar = structDec->member;

   while(currentVar != NULL){
    if(strcmp(currentVar->identifier, identifier) == 0) return currentVar;
    currentVar = currentVar->next;
   }

   return NULL;
}

StructDeclaration* findStructDeclaration(const char* name){
    StructDeclaration* current = scopeStruct->currentScope;

    while(current != NULL){
        if(strcmp(current->identifier, name) == 0) return current;
        current = current->next;
    }

    return NULL;
}


void assignValToMember(StructDeclaration declaration, ValueNode val, char* memberName){
    Variable* var = declaration.member;

    while(strcmp(var->identifier, memberName) == 0){
        var->hasValue = true;
        //var->//
    }
}

Variable* findVar(const char* name){
    Variable* current = scopeVar->currentScope;
    while(current != NULL){
        if(strcmp(current->identifier, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

void declareVariable(ASTNode* declareNode){
    char* varName = strdup(declareNode->varDecl.varName);
    bool hasValue = false; 

    if(findVar(varName) != NULL) {
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Error: Variable '%s' already declared.\n", varName);
        interpreterError(buffer);
    }
    ValueNode value;
    if(declareNode->varDecl.value != NULL){
        value = interpret(declareNode->varDecl.value);
        hasValue = true;
    }

    Variable* var = malloc(sizeof(Variable));
    var->identifier = strdup(varName);
    var->type = declareNode->varDecl.varType;
    var->hasValue = hasValue;

    if(hasValue){
        if(value.type != declareNode->varDecl.varType){
            char buffer[100];
            snprintf(buffer, sizeof(buffer), "Error: Invalid asignment to variable: %s\n", varName);
            interpreterError(buffer);
        }else{
            switch(var->type){
                case INT_TYPE:
                    var->value.intVal = value.intVal;
                    break;
                case DECIMAL_TYPE:
                    var->value.decimalVal = value.decimalVal;
                    break;
                case BOOL_TYPE:
                    var->value.boolVal = value.boolVal;
                    break;
            }
        }
    }

    var->next = scopeVar->currentScope;
    scopeVar->currentScope = var;
}

void interpreterError(char* str){
    printf(str);
    exit(1);
}

void freeScope(Variable* var){
    if(var == NULL){
        return;
    }
    if(var->next == NULL){
        return;
    }
    freeScope(var->next);
    free(var);
}

void freeElifList(ElifList* list){
    while (list != NULL) {
        ElifList* temp = list;
        list = list->next;
        free(temp);
    }
}