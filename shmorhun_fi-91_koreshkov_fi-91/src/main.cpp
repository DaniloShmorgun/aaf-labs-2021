#include "Set.h"
#include "Parser.h"
#include "Collection.h"
#include <iostream>
#include <vector>
using namespace std;


int main() {



	Collection C;

	std::cout << "\n*************** + {30, 40} *******************" << std::endl;

	C.Insert(Set({ 30,40 }));
	C.Print(std::cout);

	std::cout << "\n*************** + {30, 40} *******************" << std::endl;

	C.Insert(Set({ 30,40 }));
	C.Print(std::cout);

	std::cout << "\n*************** + {10} + {20} *******************" << std::endl;

	// Subsets
	C.Insert(Set({ 10 }));
	C.Insert(Set({ 20 }));
	C.Print(std::cout);

	std::cout << "\n*********** + { 0,10 } + { 0,20 } *************" << std::endl;

	// Supersets
	C.Insert(Set({ 0,10 }));
	C.Print(std::cout);
	C.Insert(Set({ 0,20 }));
	C.Print(std::cout);


	std::cout << "\n*********** + { 20,30,40 }**************" << std::endl;
	C.Insert(Set({ 20,30,40 }));
	C.Print(std::cout);


	std::cout << "\n*********** + { 10,15,20 } + { 10,25,20 }**************" << std::endl;

	// General
	C.Insert(Set({ 10,15,20 }));
	C.Print(std::cout);

	std::cout << std::endl;

	C.Insert(Set({ 10,25,20 }));
	C.Print(std::cout);

	std::cout << "\n****************************************************" << std::endl;

	//RequestParser prs;
	//std::string input; //= "SEARCH table_1 WHERE INTERSECTS { 1, 2,3, 4  ,  5}";
	//while (input != "exit") {
	//	std::cout << "> ";
	//	input = "";
	//	std::getline(cin, input, ';');
	//
	//	input += ';';
	//	prs.parse(input);	
	//	std::cout << std::endl;
	//	std::cout << "< " << prs.parse(input) << std::endl << std::endl;
	//	std::cout << "< " << prs.parse_error << std::endl << std::endl;
	//}



	return 0;
	
}
