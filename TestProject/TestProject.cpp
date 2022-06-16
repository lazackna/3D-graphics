#include "pch.h"
#include "CppUnitTest.h"

#include "../wk3_fps/util.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProject
{
	TEST_CLASS(TestProject)
	{
	public:
		
		TEST_METHOD(Split_Test)
		{
			std::string input = "Hello!World!I!Am!Splitting";
			std::string delimiter = "!";

			std::vector<std::string> output = splitString(input, delimiter);

			Assert::AreEqual(5, (int)output.size());
			
		}

		TEST_METHOD(Delim_Not_In_String) {
			std::string input = "Hello world";
			std::string delimiter = "!";

			std::vector<std::string> output = splitString(input, delimiter);

			Assert::AreEqual(1, (int)output.size());
		}

		TEST_METHOD(Delim_Is_Empty) {
			std::string input = "Hello world";
			std::string delimiter = "";

			std::vector<std::string> output = splitString(input, delimiter);

			Assert::AreEqual(1, (int)output.size());
		}

		TEST_METHOD(Input_Equals_Delim) {
			std::string input = "Hello world";
			std::string delimiter = "Hello world";

			std::vector<std::string> output = splitString(input, delimiter);

			Assert::AreEqual(1, (int)output.size());
		}

		TEST_METHOD(Input_Empty) {
			std::string input = "";
			std::string delimiter = "!";

			std::vector<std::string> output = splitString(input, delimiter);

			Assert::AreEqual(0, (int)output.size());
		}

		TEST_METHOD(Float_Mod) {
			float input = 3.0f;
			int mod = 2;

			float output = float_mod(input, mod);

			Assert::AreEqual(1.0f, output);
		}

		TEST_METHOD(Float_Mod_With_decimals) {
			float input = 3.5f;
			int mod = 2;

			float output = float_mod(input, mod);

			Assert::AreEqual(1.5f, output);
		}

		TEST_METHOD(Float_Mod_Bigger) {
			float input = 3.0f;
			int mod = 4;

			float output = float_mod(input, mod);

			Assert::AreEqual(3.0f, output);
		}

		TEST_METHOD(Float_Mod_Bigger_With_Decimals) {
			float input = 3.5f;
			int mod = 4.0f;

			float output = float_mod(input, mod);

			Assert::AreEqual(3.5f, output);
		}

		TEST_METHOD(Float_Mod_Same) {
			float input = 3.0f;
			int mod = 3.0f;

			float output = float_mod(input, mod);

			Assert::AreEqual(0.0f, output);
		}

		TEST_METHOD(Float_Mod_Big_Numbers) {
			float input = 2633.36f;
			int mod = 58;

			float output = float_mod(input, mod);

			// Small margin because of floating point arithmatic error.
			Assert::AreEqual(23.36f, output, 0.001f);
		}
	};
}
