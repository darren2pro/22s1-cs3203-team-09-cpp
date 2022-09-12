#include "EntityExtraction.h"
#include <stack>
#include <memory>
#include <TNode/TNode.h>

using namespace PKB;

EntityExtraction::EntityExtraction(PKBStorage* pkb) {
	pkbFacade = pkb;
}

EntityExtraction::~EntityExtraction() {}

void EntityExtraction::extractEntities(const std::shared_ptr<ProgramNode> astRoot) {
	for (std::shared_ptr<ProcedureNode> proc : astRoot->procList) {
		extractEntities(proc);
	}
}
void EntityExtraction::extractEntities(const std::shared_ptr<ProcedureNode> proc) {
	extractStatements(proc -> stmtList);
	pkbFacade->persistEntity(proc);
}

void EntityExtraction::extractStatements(const std::vector<Stmt> stmts) {
	for (const auto& stmt : stmts) {
		std::visit([this](const auto& s) { extractEntities(s); }, stmt);
	}
}

void EntityExtraction::extractEntities(const std::shared_ptr<AssignmentNode> assign) {
	//TNode::VARIABLE_NODE_PTR var = assign->getAssignedVariableNode();
	//TNode::T_NODE_PTR expressionRootNode = assign->expressionRootNode;

	extractEntities(assign->var);
	extractEntities(assign->expr);
	pkbFacade->persistEntity(assign);
	pkbFacade->persistEntity(assign -> var);
    //if (expressionRootNode->isBinaryOperatorNode()) {
    //    // persist the entities of the binary operator
    //}
}
void EntityExtraction::extractEntities(const Expr exp) {
	std::visit([this](const auto& e) { extractEntities(e); }, exp);
}
void EntityExtraction::extractEntities(const std::shared_ptr<CondExprNode> cond) {
	if (cond == nullptr) {
		return;
	}
	extractEntities(cond->relExpr);
	extractEntities(cond->leftCond);
	extractEntities(cond->rightCond);
}
void EntityExtraction::extractEntities(const std::shared_ptr<RelExprNode> rel) {
	if (rel == nullptr) {
		return;
	}
	extractEntities(rel->leftRel);
	extractEntities(rel->rightRel);
}

void EntityExtraction::extractEntities(const std::shared_ptr<BinOpNode> bin) {
	extractEntities(bin->leftExpr);
	extractEntities(bin->rightExpr);
}
void EntityExtraction::extractEntities(const std::shared_ptr<VariableNode> var) {
	pkbFacade->persistEntity(var);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ConstantNode> cons) {
	// pkbFacade->persistEntity(cons);
}

void EntityExtraction::extractEntities(const std::shared_ptr<IfNode> ifNode) {
	extractEntities(ifNode->condExpr);
	extractStatements(ifNode->thenStmtList);
	extractStatements(ifNode->elseStmtList);
}

void EntityExtraction::extractEntities(const std::shared_ptr<WhileNode> whileNode) {
	extractEntities(whileNode->condExpr);
	extractStatements(whileNode->stmtList);
}
void EntityExtraction::extractEntities(const std::shared_ptr<ReadNode> readNode) {
	extractEntities(readNode->var);
}
void EntityExtraction::extractEntities(const std::shared_ptr<PrintNode> printNode) {
	extractEntities(printNode->var);
}
void EntityExtraction::extractEntities(const std::shared_ptr<CallNode> node) {
	//persist to pkb
}