#include "Menu.hpp"

#include <iostream>

#define CLEAR_CONSOLE system("clear");





// structs
Menu &Menu::operator()()
{
	// preparation
	using namespace std;
	string input; 
	string toprint;
	int choice;



	// main loop
	while(true)
	{
		// output
		CLEAR_CONSOLE
		if(toprint.empty())
			toprint = defheader;
		cout << toprint << "\n\n";
		for(uint i = 0; i < bars.size(); ++i)
		{
			cout << i+1 << ") " << bars[i].s << '\n';
		}
		cout << bars.size()+1 << ") Quit\n\n> ";


		// input
		if( !( cin >> input ) )
		{
			cin.clear();
			break;
		}
		try
		{
			cin.get(); // clear from \n
			choice = stoi( input );
		}
		catch(...)
		{
			if(
				input == "q" ||
				input == "quit"
			)
				break;
			if(input.size() == 1) for(
				auto b = bars.begin(), e = bars.end();
				b != e; ++b
			)
			{
				if( b->keych.find( input ) != string::npos )
				{
					toprint = b->f();
					goto while_continue_label;
				}
			}
			toprint = invinpmessage;
			continue;
		}
		if(choice < 1 || choice > (int)bars.size())
			break;
		toprint = bars[choice-1].f();
	while_continue_label:
		;
	}



	return *this;
}





// end
