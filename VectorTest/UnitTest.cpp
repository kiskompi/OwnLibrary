#include "stdafx.h"
#include "../vector/dynamic_array.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace VectorTest
{
	[TestClass]
	public ref class UnitTest
	{
	private:
		TestContext^ testContextInstance;

	public: 
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

		#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void VectorTest()
		{
			//
			// TODO: Add test logic here
			//

			own::dynamic_array<int> int_vec = own::dynamic_array<int>();

			for (size_t i = 0; i < 10000; i++)
			{
				int_vec.push_back(i);
			}
			Console::Write("Size: ");
			Console::Write(int_vec.size());
			Console::Write(", capacity: ");
			Console::WriteLine(int_vec.capacity());

			for (size_t i = 0; i < 100; i++)
			{
				int_vec.pop_back();
			}

			for (size_t i = 0; i < 100; i++)
			{
				Console::WriteLine(int_vec.at(i));
			}

			Console::Write("Size: ");
			Console::WriteLine(int_vec.size());

			own::dynamic_array<int>::iterator it = int_vec.begin();
			for (size_t i = 0; i < 10; i++) {
				/*int_vec.erease(it);
				*/++it;
			}

			for (own::dynamic_array<int>::iterator it = int_vec.begin(); it != int_vec.end(); ++it) {
				//Console::Write("*it: ");  Console::WriteLine(*it);
				;
			}
			
			Console::Write("Front: ");
			Console::WriteLine(int_vec.front());

			Console::Write("Back: ");
			Console::WriteLine(int_vec.back());
			int_vec.clear();
			Console::WriteLine("FINISHED");
		};
	};
}
