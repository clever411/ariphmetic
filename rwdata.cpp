/*
 * autor:   clever
 * date:    Nov 20 11:54:42 2018
 */

#include "rwdata.hpp"

#include <ctime>
#include <fstream>
#include <libconfig.h++>

using namespace std;





// objects
libconfig::Config datacfg;





// functions
void try_init_datacfg()
{
	static bool isinit = false;
	if(!isinit)
	{
		try
		{
			datacfg.readFile("data/data.cfg");
			if(!datacfg.exists("list"))
				datacfg.getRoot().add(
					"list", libconfig::Setting::TypeArray
				);
		}
		catch(...)
		{
			datacfg.getRoot().add(
				"list", libconfig::Setting::TypeArray
			);
		}
		isinit = true;
	}
	return;
}

void write_stat( Stat const &stat )
{
	// write in config
	using namespace libconfig;
	time_t atime = time(0);
	struct tm *atm = localtime(&atime);
	char filename[30] = { 0 };
	filename[0] = 'd';
	filename[1] = 'a';
	filename[2] = 't';
	filename[3] = 'a';
	filename[4] = '/';
	strftime( filename+5, 25, "%Y.%m.%d_%H:%M:%S.stat", atm );

	try_init_datacfg();
	Setting &list = datacfg.getRoot().lookup("list");
	list.add( Setting::TypeString ) = filename+5;
	datacfg.writeFile("data/data.cfg");



	// write stat in file
	ofstream fout( filename );
	stat.write(fout);


	return;
}





// end
