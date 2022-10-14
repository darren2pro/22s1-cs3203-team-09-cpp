#pragma once
#include <string>
#include <vector>

class Declaration {
public:
	enum DesignEntity {
		Statement,
		Read,
		Print,
		While,
		If,
		Assignment,
		Variable,
		Constant,
		Procedure,
		Call,
		NONE
	};

	DesignEntity Type;
	std::string name;

	Declaration(DesignEntity TYPE, std::string name) : Type(TYPE), name(name) {};
	Declaration() : Type(DesignEntity::NONE), name("") {};

	bool operator==(const Declaration& d) const {
		return Type == d.Type && name == d.name;
	}

	/**
	 * Returns the Declaration::DesignEntity that is equivalent to the string.
	 * @returns a Declaration::DesignEntity that is equivalent to the string.
	 */
	static DesignEntity getDesignEntity(std::string str) {
		if (str == "stmt") {
			return DesignEntity::Statement;
		}
		else if (str == "read") {
			return DesignEntity::Read;
		}
		else if (str == "print") {
			return DesignEntity::Print;
		}
		else if (str == "while") {
			return DesignEntity::While;
		}
		else if (str == "if") {
			return DesignEntity::If;
		}
		else if (str == "assign") {
			return DesignEntity::Assignment;
		}
		else if (str == "variable") {
			return DesignEntity::Variable;
		}
		else if (str == "constant") {
			return DesignEntity::Constant;
		}
		else if (str == "procedure") {
			return DesignEntity::Procedure;
		}
		else if (str == "call") {
			return DesignEntity::Call;
		}
		else {
			return DesignEntity::NONE;
		}
	}

};