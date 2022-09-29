#include <string>
#include <vector>
#include "Pattern.h"

class Utils {
public:
    /**
     * Checks whether the provided arg is a valid synonym by referencing the declarations vector from the query adt.
     * @param arg string arg to check whether it is a valid synonym
     * @param declarations vector of declarations from the query adt
     * @return true if arg is a valid synonym, false otherwise
     */
	static bool isSynonym(std::string arg, std::vector<Declaration> declarations) {		// ref.isSynonym()
		for (Declaration decl : declarations) {
			if (decl.name == arg) {
				return true;
			}
		}
		return false;
	};

    /**
     * Gets the declaration object for this synonym
     */
	static Declaration getSynonym(std::string arg, std::vector<Declaration> declarations) {		// ref.declaration
		for (Declaration decl : declarations) {
			if (decl.name == arg) {
				return decl;
			}
		}
		return Declaration(); // Should never be reached
	};

    /**
     * Simply checks whether the given argument contains a quotation mark. If it does then it could either be an
     * arithmetic expression (could be strict or relaxed) or a string literal representing a design entity.
     */
	static bool isBasicQueryString(std::string argument) {		// ref.isString()
		return argument.find_first_of("\"") != std::string::npos; // FFO returns npos if " not found. String should contain " "
	};
	static bool isUnderscore(std::string argument) {
		return argument == "_";
	};

    /**
     * @brief Simply checks whether the given string is a valid integer.<br><br>
     * <strong>Note:</strong> It does not check whether the statement number is within range of the input source program.
     */
    static bool isBasicQueryStatementNumber(std::string argument) {		// ref.isStmtNum()
        for (char c : argument) {
            if (!isdigit(c)) return false;
        }
        return true;
    }
    /**
     * @brief Checks whether the given string is a SIMPLE. A SIMPLE is defined as either a statement number or an
     * arithmetic expression string, or a string literal referencing a design entity.
     *
     * <br><br>A string that returns true for
     * being a basic query simple will never be a synonym because returning true here either means that this is
     * completely a number (not a synonym, which is   IDENT : LETTER ( LETTER | DIGIT )*  ). Or it is a string literal
     * with quotation marks, which is also definitely not allowed in a synonym.
     */
    static bool isBasicQuerySimple(std::string argument) {		// ref.isStmtNum() || ref.isString()
        return isBasicQueryStatementNumber(argument) || isBasicQueryString(argument);
    }
	/**
	 * @brief Checks that the arithmetic expression is a full match one, meaning no underscores. This method
	 * assumes that the string expression indeed is an arithmetic expression.
	 */
	static bool isStrictExpression(std::string expr) {		// expr.isStrict()
		return expr.find_first_of("_") == std::string::npos;
	}
    /**
     * @brief Checks whether the given string is a relaxed arithmetic expression. This method assymes that the string
     * expression indeed is an arithmetic expression.
     */
	static bool isRelaxedExpression(std::string expr) {		// expr.isRelaxed()
		return expr.find_first_of("_") != std::string::npos;
	}

	// NEEDS CHANGING
	//static bool isList(Pattern::Types type) {
	//	return type == Pattern::Types::AssignRelaxed;
	//}
	//static bool isListPair(Pattern::Types type) {
	//	return type == Pattern::Types::AssignRelaxed;
	//}
};
