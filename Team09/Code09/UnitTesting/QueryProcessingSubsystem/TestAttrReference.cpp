#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include "QueryProcessingSubsystem/AttrReference.h"
#include "QueryProcessingSubsystem/Declaration.h"
#include "QueryProcessingSubsystem/Validator/SyntaxException.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestAttrReference) {

public:
    TEST_METHOD(IdentType) {
        AttrReference ref = AttrReference("\"x123\"");

        Assert::IsTrue(ref.TYPE == AttrReference::Types::IDENT);
        Assert::IsTrue(ref.declaration == Declaration());
        Assert::IsTrue(ref.attr == AttrReference::Attribute::NONE);
        Assert::IsTrue(ref.valueType == AttrReference::ValueType::NAME);
        Assert::IsTrue(ref.value == "x123");

        Assert::IsTrue(ref.isIdent());
        Assert::IsFalse(ref.isInteger());
        Assert::IsFalse(ref.isSynonym());        
        
        Assert::IsFalse(ref.isProcName());
        Assert::IsFalse(ref.isVarName());
        Assert::IsFalse(ref.isValue());
        Assert::IsFalse(ref.isStmtNum());
    }    
    
    TEST_METHOD(IntegerType) {
        AttrReference ref = AttrReference("12");

        Assert::IsTrue(ref.TYPE == AttrReference::Types::INTEGER);
        Assert::IsTrue(ref.declaration == Declaration());
        Assert::IsTrue(ref.attr == AttrReference::Attribute::NONE);
        Assert::IsTrue(ref.valueType == AttrReference::ValueType::INTEGER_V);
        Assert::IsTrue(ref.value == "12");

        Assert::IsFalse(ref.isIdent());
        Assert::IsTrue(ref.isInteger());
        Assert::IsFalse(ref.isSynonym());

        Assert::IsFalse(ref.isProcName());
        Assert::IsFalse(ref.isVarName());
        Assert::IsFalse(ref.isValue());
        Assert::IsFalse(ref.isStmtNum());
    }    
    
    TEST_METHOD(SynonymType) {
        AttrReference ref = AttrReference(Declaration(Declaration::DesignEntity::Procedure, "p"), AttrReference::Attribute::ProcName);

        Assert::IsTrue(ref.TYPE == AttrReference::Types::SYNONYM);
        Assert::IsTrue(ref.declaration == Declaration(Declaration::DesignEntity::Procedure, "p"));
        Assert::IsTrue(ref.attr == AttrReference::Attribute::ProcName);
        Assert::IsTrue(ref.valueType == AttrReference::ValueType::NAME);
        Assert::IsTrue(ref.value == "");

        Assert::IsFalse(ref.isIdent());
        Assert::IsFalse(ref.isInteger());
        Assert::IsTrue(ref.isSynonym());

        Assert::IsTrue(ref.isProcName());
        Assert::IsFalse(ref.isVarName());
        Assert::IsFalse(ref.isValue());
        Assert::IsFalse(ref.isStmtNum());

    }

    TEST_METHOD(WrongAttributeForSynonymError) {          
        bool exceptionThrown = false;
        try
        {
            AttrReference ref = AttrReference(Declaration(Declaration::DesignEntity::Procedure, "p"), AttrReference::Attribute::VarName);
        }
        catch (SyntaxError&)
        {
            exceptionThrown = true;
        }

        Assert::IsTrue(exceptionThrown);        
        
        bool exceptionThrown1 = false;
        try
        {
            AttrReference ref1 = AttrReference(Declaration(Declaration::DesignEntity::Procedure, ""), AttrReference::Attribute::StmtNum);
        }
        catch (SyntaxError&)
        {
            exceptionThrown1 = true;
        }

        Assert::IsTrue(exceptionThrown1);
    }    
    
    TEST_METHOD(ValidAttributeForSynonymError) {
        // procname
        std::vector<AttrReference> v1 = std::vector<AttrReference>();
        v1.push_back(AttrReference(Declaration(Declaration::DesignEntity::Procedure, "x"), AttrReference::Attribute::ProcName));
        v1.push_back(AttrReference(Declaration(Declaration::DesignEntity::Call, "x"), AttrReference::Attribute::ProcName));

        for (AttrReference ref : v1) {
            Assert::IsTrue(ref.valueType == AttrReference::ValueType::NAME);

            Assert::IsFalse(ref.isIdent());
            Assert::IsFalse(ref.isInteger());
            Assert::IsTrue(ref.isSynonym());

            Assert::IsTrue(ref.isProcName());
            Assert::IsFalse(ref.isVarName());
            Assert::IsFalse(ref.isValue());
            Assert::IsFalse(ref.isStmtNum());
        }

        // varname
        std::vector<AttrReference> v2 = std::vector<AttrReference>();
        v2.push_back(AttrReference(Declaration(Declaration::DesignEntity::Variable, "x"), AttrReference::Attribute::VarName));
        v2.push_back(AttrReference(Declaration(Declaration::DesignEntity::Read, "x"), AttrReference::Attribute::VarName));
        v2.push_back(AttrReference(Declaration(Declaration::DesignEntity::Print, "x"), AttrReference::Attribute::VarName));

        for (AttrReference ref : v2) {
            Assert::IsTrue(ref.valueType == AttrReference::ValueType::NAME);

            Assert::IsFalse(ref.isIdent());
            Assert::IsFalse(ref.isInteger());
            Assert::IsTrue(ref.isSynonym());

            Assert::IsFalse(ref.isProcName());
            Assert::IsTrue(ref.isVarName());
            Assert::IsFalse(ref.isValue());
            Assert::IsFalse(ref.isStmtNum());
        }

        // value
        AttrReference ref = AttrReference(Declaration(Declaration::DesignEntity::Constant, "x"), AttrReference::Attribute::Value);

        Assert::IsTrue(ref.valueType == AttrReference::ValueType::INTEGER_V);

        Assert::IsFalse(ref.isIdent());
        Assert::IsFalse(ref.isInteger());
        Assert::IsTrue(ref.isSynonym());

        Assert::IsFalse(ref.isProcName());
        Assert::IsFalse(ref.isVarName());
        Assert::IsTrue(ref.isValue());
        Assert::IsFalse(ref.isStmtNum());

        // stmt#
        std::vector<AttrReference> v3 = std::vector<AttrReference>();
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::Statement, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::Read, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::Print, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::Call, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::While, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::If, "x"), AttrReference::Attribute::StmtNum));
        v3.push_back(AttrReference(Declaration(Declaration::DesignEntity::Assignment, "x"), AttrReference::Attribute::StmtNum));

        for (AttrReference ref : v3) {
            Assert::IsTrue(ref.valueType == AttrReference::ValueType::INTEGER_V);

            Assert::IsFalse(ref.isIdent());
            Assert::IsFalse(ref.isInteger());
            Assert::IsTrue(ref.isSynonym());

            Assert::IsFalse(ref.isProcName());
            Assert::IsFalse(ref.isVarName());
            Assert::IsFalse(ref.isValue());
            Assert::IsTrue(ref.isStmtNum());
        }
        }
    };
}