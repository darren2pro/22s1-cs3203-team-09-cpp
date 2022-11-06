#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SPAManager/SPAManager.h"
#include "../source/ProgramKnowledgeBase/PKBStorage.h"
#include "../source/SourceProcessor/DesignExtractor/DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IntegrationTesting {
    TEST_CLASS(TestSPA) {
private:
    string getCurrentProgram() {
        string program1 = "procedure firstProc{\n"
            "        read x;\n" // line 1
            "        read z;\n" // line 2
            "        call secondProc;\n" // line 3
            "}\n"
            "procedure secondProc{\n"
            "        x = 0;\n" // line 4
            "        i = 5;\n" // line 5
            "        while ((i != 0) {\n" // line 6
            "            x = x + 2 * y;\n" // line 7
            "            call thirdProc;\n" // line 8
            "            i = i - 1;\n" // line 9
            "        }\n"
            "        if (x == 1) then {\n" // line 10
            "           x = x + 1;\n" // line 11
            "        } else {\n"
            "           z = 1;" // line 12
            "        }\n"
            "        z = z + x + i;\n"// line 13
            "        y = z + 2;\n" // line 14
            "        x = x * y + z;\n" // line 15
            "}\n"
            "procedure thirdProc{\n"
            "        z = 5;\n" // line 16
            "        v = z;\n" // line 17
            "        print v;\n" // line 18
            "}";

    }
public:
    TEST_METHOD(IntegrationSPA1) {
        string program = getCurrentProgram();

        AST ast = SimpleInterface::getAstFromProgram(program);
        PKBStorage* pkb = new PKBStorage();
        DesignExtractor extractor(pkb);
        extractor.extractDesignAbstractions(ast);

        //entities
        Assert::IsTrue(pkb->entityContains(Declaration::Variable, "x"));
        Assert::IsTrue(pkb->entityContains(Declaration::Variable, "z"));
        Assert::IsTrue(pkb->entityContains(Declaration::Variable, "i"));
        Assert::IsTrue(pkb->entityContains(Declaration::Variable, "y"));
        Assert::IsTrue(pkb->entityContains(Declaration::Variable, "v"));
        Assert::IsTrue(pkb->entityContains(Declaration::Procedure, "firstProc"));
        Assert::IsTrue(pkb->entityContains(Declaration::Procedure, "secondProc"));
        Assert::IsTrue(pkb->entityContains(Declaration::Procedure, "thirdProc"));
        Assert::IsTrue(pkb->entityContains(Declaration::Read, "1"));
        Assert::IsTrue(pkb->entityContains(Declaration::Read, "2"));
        Assert::IsTrue(pkb->entityContains(Declaration::Call, "3"));
        Assert::IsTrue(pkb->entityContains(Declaration::Call, "8"));
        Assert::IsTrue(pkb->entityContains(Declaration::Print, "18"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "4"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "5"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "7"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "9"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "11"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "12"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "13"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "14"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "15"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "16"));
        Assert::IsTrue(pkb->entityContains(Declaration::Assignment, "17"));
        Assert::IsTrue(pkb->entityContains(Declaration::While, "6"));
        Assert::IsTrue(pkb->entityContains(Declaration::If, "10"));



        //relations: follows

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "1", "2"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "1"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "2"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "2", "3"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "2"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "3"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "3", "4"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "3"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "4"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "5", "6"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "5"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "6"));


        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "6", "7"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "6"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "7"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "7", "8"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "7"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "8"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "8", "9"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "8"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "9"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "9", "10"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "9"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "10"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "10", "11"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "10"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "11"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "11", "12"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "11"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "12"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "12", "13"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "12"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "13"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "13", "14"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "13"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "14"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "14", "15"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "14"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "15"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "15", "16"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "15"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "16"));

        Assert::IsTrue(pkb->relationContainsSet(Relation::Follows, "16", "17"));
        Assert::IsTrue(pkb->relationContainsFirst(Relation::Follows, "16"));
        Assert::IsTrue(pkb->relationContainsSecond(Relation::Follows, "17"));

    }

    };
}
