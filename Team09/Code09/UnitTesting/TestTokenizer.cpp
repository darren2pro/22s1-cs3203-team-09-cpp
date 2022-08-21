#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

#define TEST_MY_TOKENIZER(traitValue) TEST_METHOD_ATTRIBUTE(L"MyTokenizer", traitValue)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestTokenizer) {
            public:

            TEST_METHOD(TestTokenize) {
                std::string str = "procedure computeAverage {\n"
                                  "    read num1;\n"
                                  "    read num2;\n"
                                  "    read num3;\n"
                                  "\n"
                                  "    sum = num1 + num2 + num3;\n"
                                  "    ave = sum / 3;\n"
                                  "\n"
                                  "    print ave;\n"
                                  "}";
				Logger::WriteMessage(("Program to test:\n" + str).c_str());
            }
    };
}
