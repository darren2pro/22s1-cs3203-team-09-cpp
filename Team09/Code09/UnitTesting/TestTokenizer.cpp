#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include <vector>
#include <SimpleToken.h>

#define TEST_MY_TOKENIZER(traitValue) TEST_METHOD_ATTRIBUTE(L"MyTokenizer", traitValue)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestTokenizer) {
            public:

            TEST_METHOD(TestTokenize) {
                std::string str = "procedure computeAverage {\n"
                                  "    read num1;\n"
                                  "    read num2;\n"
                                  "\n"
                                  "    sum = num1 + num2;\n"
                                  "\n"
                                  "    print sum;\n"
                                  "}";
				Logger::WriteMessage(("Program to test:\n" + str).c_str());
                vector<SimpleToken> expectedResult;
                expectedResult.push_back(SimpleToken("procedure"));
                expectedResult.push_back(SimpleToken("computeAverage"));
                expectedResult.push_back(SimpleToken("{"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("read"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken("="));
                expectedResult.push_back(SimpleToken("num1"));
                expectedResult.push_back(SimpleToken("+"));
                expectedResult.push_back(SimpleToken("num2"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("print"));
                expectedResult.push_back(SimpleToken("sum"));
                expectedResult.push_back(SimpleToken(";"));
                expectedResult.push_back(SimpleToken("}"));
                expectedResult.push_back(SimpleToken("EOF"));

                // TODO: Use your tokenizer to tokenize the above program then compare the result with the expectedResult
            }
    };
}
