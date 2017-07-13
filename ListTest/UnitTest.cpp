#include "stdafx.h"
#include "../OrderableList/OrderableList.h"
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace ListTest
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
		void TestMethod1()
		{
			Console::WriteLine("START");
			own::OrderableList<int> int_list = own::OrderableList<int>();
			//std::vector<int> int_vec = std::vector<int>();
			//iterator& front();				// return the iterator to the first element
			//iterator& back();				// return the iterator to the last element
			//iterator& erase();				// deletes the current element and returns the following

			//void push_back(const Val&);		// places an element to the end
			//Val  pop_back();				// deletes and returns the last element
			//Val  pop_front();				// deletes and returns the first element
			//bool empty();					// returns true if the list is empty
			//bool clear();					// clears the list

			for (size_t i = 0; i < 10000; ++i)
			{
				int_list.push_back(i);
			}
			Console::Write(int_list.empty());
			Console::WriteLine(int_list.size());
			
			for (size_t i = 0; i < 100; i++)
			{
				Console::WriteLine(int_list.pop_front());
			}
			Console::WriteLine(int_list.size());
			own::OrderableList<int>::iterator it = int_list.front();
			for (size_t i = 0; i < 10; i++) {
				it.erase();
				++it;
			}

			for (own::OrderableList<int>::iterator it = int_list.front(); it != int_list.back(); ++it) {
				//Console::Write("*it: ");  Console::WriteLine(*it);
				;
			}

			int_list.clear();
			Console::WriteLine(int_list.size());
			Console::WriteLine("FINISHED");

		};
	};
}
