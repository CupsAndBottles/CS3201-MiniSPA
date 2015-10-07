#include "TestWrapper.h"
#include "Parser.h";
#include "ParserForPQL.h"
#include "QueryEvaluator.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	this->pkb = pkb;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
	Parser parser;
	parser.openFile(filename);
	cout << "After parser";
	 pkb = parser.getPkb();
	 cout << "after get pkb";
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
	
	//TO 
	try {
		cout << "before parserForPQL";
		ParserForPQL parserForPQL(query, *pkb);
		cout << " after ParserForPQL";
		QueryEvaluator queryEvaluator(*pkb);
		cout << " after evaluator";
		results = queryEvaluator.evaluateQuery(parserForPQL.getQueryTree());
		cout << "after results";
	}
	catch (exception& e) {
		cout << e.what();
	}

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
