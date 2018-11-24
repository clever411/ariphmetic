#ifndef MENU_HPP
#define MENU_HPP

#include <functional>
#include <vector>
#include <string>





// structs
struct Menu
{
	// typedefs
	typedef struct 
	{
		std::string s;
		std::function< std::string() > f;
		std::string keych;
	} Bar;



	// members
	std::string defheader;
	std::string invinpmessage = "Invalid input";
	std::vector<Bar> bars;



	// methods
	Menu &operator()();
};





#endif
