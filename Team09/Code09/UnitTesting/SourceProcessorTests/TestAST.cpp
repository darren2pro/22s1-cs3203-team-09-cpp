#include "stdafx.h"
#include "CppUnitTest.h"
#include "SourceProcessor/astBuilder/ProgramNode.h"
#include "SourceProcessor/astBuilder/ProcedureNode.h"
#include "SourceProcessor/astBuilder/StmtLstNode.h"
#include "SourceProcessor/astBuilder/AssignmentNode.h"
#include "SourceProcessor/astBuilder/VariableNode.h"
#include "SourceProcessor/astBuilder/ConstantNode.h"
#include "SourceProcessor/astBuilder/PlusNode.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestAST) {
            public:

            TEST_METHOD(TestProgramNode) {
                Logger::WriteMessage("TestProgramNode");
                const shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>();
                Assert::IsFalse(programNode->isDesignEntity());
                Assert::IsFalse(programNode->isAssignmentNode());
            }

            TEST_METHOD(TestProcedureNode) {
                Logger::WriteMessage("TestProcedureNode");
                const shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>("darrenProcedure");
                Assert::IsTrue(procedureNode->isDesignEntity());
                Assert::IsFalse(procedureNode->isAssignmentNode());
            }

            TEST_METHOD(TestConstantNode) {
                Logger::WriteMessage("TestConstantNode");
                const shared_ptr<ConstantNode> constantNode = make_shared<ConstantNode>("1");
                Assert::IsTrue(constantNode->isDesignEntity());
                Assert::IsFalse(constantNode->isAssignmentNode());
            }

            TEST_METHOD(TestVariableNode) {
                Logger::WriteMessage("TestVariableNode");
                const shared_ptr<VariableNode> variableNode = make_shared<VariableNode>("a");
                Assert::IsTrue(variableNode->isDesignEntity());
                Assert::IsFalse(variableNode->isAssignmentNode());
            }

            TEST_METHOD(TestPlusNode) {
                Logger::WriteMessage("TestPlusNode");
                const shared_ptr<PlusNode> plusNode = make_shared<PlusNode>();
                Assert::IsFalse(plusNode->isDesignEntity());
                Assert::IsFalse(plusNode->isAssignmentNode());
            }

            TEST_METHOD(TestAssignmentNode) {
                Logger::WriteMessage("TestAssignmentNode");
                const shared_ptr<AssignmentNode> assignmentNode = make_shared<AssignmentNode>();
                Assert::IsTrue(assignmentNode->isDesignEntity());
                Assert::IsTrue(assignmentNode->isAssignmentNode());
            }

            TEST_METHOD(TestStmtLstNode) {
                Logger::WriteMessage("TestStmtLstNode");
                const shared_ptr<StmtLstNode> stmtLstNode = make_shared<StmtLstNode>();
                Assert::IsFalse(stmtLstNode->isDesignEntity());
                Assert::IsFalse(stmtLstNode->isAssignmentNode());
            }

            TEST_METHOD(TestAddProcedure) {
                const string procedureName = "darrenProcedure";
                const shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>();
                const shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(procedureName);
                programNode->addProcedure(procedureNode);
                Assert::IsTrue((programNode->procedureList).size() == 1, L"Wrong vector size in procedure list");
                Assert::IsTrue((programNode->procedureList)[0] != nullptr, L"ProcedureNode was not added.");
                Assert::IsTrue((programNode->procedureList)[0]->name == procedureName, L"ProcedureNode name is wrong.");
            }

            TEST_METHOD(TestAddAssignmentNodeToProcedureNode) {
                const string procedureName = "darrenProcedure";
                const shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>();
                const shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(procedureName);
                programNode->addProcedure(procedureNode);
                const shared_ptr<AssignmentNode> assignmentNode = make_shared<AssignmentNode>();
                const shared_ptr<VariableNode> assignedVariable = make_shared<VariableNode>("a");
                procedureNode->addStatement(assignmentNode);
                assignmentNode->addAssignedVariable(assignedVariable);
                const shared_ptr<PlusNode> plusNode = make_shared<PlusNode>();
                assignmentNode->addPlus(plusNode);
                const shared_ptr<VariableNode> variableNode1 = make_shared<VariableNode>("a");
                const shared_ptr<ConstantNode> constantNode = make_shared<ConstantNode>("1");
                plusNode->setLeftSubtree(variableNode1);
                plusNode->setRightSubtree(constantNode);

                const TNode::PROCEDURE_NODE_PTR firstProcedure = (programNode->getProcedureByIndex(0));
                const TNode::ASSIGNMENT_NODE_PTR firstStmt = firstProcedure->getStatementByIndex(0);
                Assert::IsTrue(firstStmt != nullptr, L"AssignmentNode was not added.");
                Assert::IsTrue(firstStmt->isAssignmentNode(), L"Not an assignment node.");
            }

            TEST_METHOD(TestIteratingProceduresStatements) {
                const string procedureName = "darrenProcedure";
                const shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>();
                const shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(procedureName);
                programNode->addProcedure(procedureNode);
                const shared_ptr<AssignmentNode> assignmentNode = make_shared<AssignmentNode>();
                const shared_ptr<VariableNode> assignedVariable = make_shared<VariableNode>("a");
                procedureNode->addStatement(assignmentNode);
                assignmentNode->addAssignedVariable(assignedVariable);
                const shared_ptr<PlusNode> plusNode = make_shared<PlusNode>();
                assignmentNode->addPlus(plusNode);
                const shared_ptr<VariableNode> variableNode1 = make_shared<VariableNode>("a");
                const shared_ptr<ConstantNode> constantNode = make_shared<ConstantNode>("1");
                plusNode->setLeftSubtree(variableNode1);
                plusNode->setRightSubtree(constantNode);

                Assert::IsTrue(programNode->getProcedureCount() == 1, L"Wrong number of procedures.");
                Assert::IsTrue(programNode->getProcedureByIndex(0)->getStatementCount() == 1,
                               L"Wrong number of statements in first procedure.");
            }
    };
}
