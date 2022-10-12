#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Declaration.h"
#include "Validator/SyntaxException.h"

class AttrReference {
public:
	enum Types {
		IDENT,			// p.procName = "proc"		<-- "proc" is an IDENT AttrReference
		INTEGER,		// v.value = 12				<-- 12 is an INTEGER AttrReference
		SYNONYM			// p1.procName = p2.procName
	};

	enum Attribute {
		NONE,
		ProcName,
		VarName,
		Value,
		StmtNum
	};

	enum ValueType {
		INTEGER_V,		// For INTEGER & SYNONYM (Value & StmtNum)
		NAME			// For IDENT & SYNONYM (ProcName & VarName)
	};


	Types TYPE;
	Declaration declaration;		// for SYNONYM AttrReference, otherwise will be an empty declaration
	Attribute attr;					// for SYNONYM AttrReference, otherwise will be NONE
	ValueType valueType;
	std::string value;				// for IDENT & INTEGER to store their value, will be "" if it was a synonym

	AttrReference(Declaration declaration, Attribute attr) {
		this->declaration = declaration;
		this->attr = attr;
		this->TYPE = SYNONYM;
		this->valueType = getValueType(attr);
		this->value = "";

		if (!isValidalidAttributeType(attr, declaration)) {
			throw SyntaxError("Invalid synonym type for attribute");
		}
	}

	AttrReference(std::string str) {
		this->declaration = Declaration();
		this->attr = Attribute::NONE;

		if (std::regex_match(str, std::regex("\"[a-zA-Z]([a-zA-Z0-9])*\""))) {		// " IDENT "
			str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
			this->TYPE = Types::IDENT;
			this->valueType = ValueType::NAME;
		}
		else if (std::regex_match(str, std::regex("(0|[1-9]([0-9])*)"))) {		// INTEGER
			this->TYPE = Types::INTEGER;
			this->valueType = ValueType::INTEGER_V;
		}
		else {
			throw SyntaxError("Invalid str");
		}

		this->value = str;
	}

	static Attribute getAttribute(std::string str) {
		if (str == "procName") {
			return Attribute::ProcName;

		}
		else if (str == "varName") {
			return Attribute::VarName;

		}
		else if (str == "value") {
			return Attribute::Value;

		}
		else if (str == "stmt#") {
			return Attribute::StmtNum;

		}
		else {
			return Attribute::NONE;
		}
	}

	bool operator==(const AttrReference& a) const {
		return declaration == a.declaration && attr == a.attr && TYPE == a.TYPE && valueType == a.valueType;
	}

	// Types
	bool isIdent() { return TYPE == Types::IDENT; }
	bool isInteger() { return TYPE == Types::INTEGER; }
	bool isSynonym() { return TYPE == Types::SYNONYM; }

	// Attributes
	bool isProcName() { return attr == Attribute::ProcName; }
	bool isVarName() { return attr == Attribute::VarName; }
	bool isValue() { return attr == Attribute::Value; }
	bool isStmtNum() { return attr == Attribute::StmtNum; }

private:
	ValueType getValueType(Attribute attr) {
		if (attr == Value || attr == StmtNum) {
			return ValueType::INTEGER_V;
		}
		else {
			return ValueType::NAME;
		}
	}

	bool isValidalidAttributeType(Attribute attr, Declaration d) {
		bool flag = false;
		if (attr == ProcName) {
			if (d.TYPE == Declaration::DesignEntity::Procedure || d.TYPE == Declaration::DesignEntity::Call) {
				flag = true;
			}
		}
		else if (attr == VarName) {
			if (d.TYPE == Declaration::DesignEntity::Variable || d.TYPE == Declaration::DesignEntity::Read || d.TYPE == Declaration::DesignEntity::Print) {
				flag = true;
			}
		}
		else if (attr == Value) {
			if (d.TYPE == Declaration::DesignEntity::Constant) {
				flag = true;
			}
		}
		else if (attr == StmtNum) {
			if (d.TYPE == Declaration::DesignEntity::Statement || d.TYPE == Declaration::DesignEntity::Read || d.TYPE == Declaration::DesignEntity::Print || 
				d.TYPE == Declaration::DesignEntity::Call || d.TYPE == Declaration::DesignEntity::While || d.TYPE == Declaration::DesignEntity::If ||
				d.TYPE == Declaration::DesignEntity::Assignment) {
				flag = true;
			}
		}
		return flag;
	}
};