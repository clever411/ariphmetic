#include <fstream>

#include "Stat.hpp"

using namespace std;





// main
int main( int argc, char *argv[] )
{
	Stat stat;
	ifstream fin;
	for(int i = 1; i < argc; ++i)
	{
		system("clear");
		fin.open(argv[i]);
		if(!fin)
		{
			cerr << "Error with file: " << argv[i] << endl;
			cin.get();
			continue;
		}
		cout << "filename: " << argv[i] << endl;
		stat.read( fin );
		cout << stat << endl;
		cin.get();
		fin.close();
	}
	



	return 0;
}





// end
