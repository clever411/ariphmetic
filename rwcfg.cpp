/*
 * autor:   clever
 * date:    Nov 20 11:03:39 2018
 */

#include "rwcfg.hpp"

#include <algorithm>
#include <libconfig.h++>

using namespace std;





template<typename T>
inline void lookup(
	libconfig::Setting const &set, std::string const &name,
	T &value, T const &defvalue
) noexcept
{
	try {
		value = set.lookup(name);
	}
	catch(libconfig::SettingNotFoundException const &e) {
		value = defvalue;
	}
	return;
}


template<>
inline void lookup<string>(
	libconfig::Setting const &set, std::string const &name,
	string &value, string const &defvalue
) noexcept
{
	try {
		value = set.lookup(name).c_str();
	}
	catch(libconfig::SettingNotFoundException const &e) {
		value = defvalue;
	}
	return;
}


void sort_choicearr()
{
	std::sort(
		choicearr, choicearr+choicedis.b()+1,
		[](char lhs, char rhs)->bool
		{
			struct F { static int f(char ch)
			{
				switch(ch)
				{
				case '+':
					return 0;
				case '-':
					return 1;
				case '*':
					return 2;
				case '/':
					return 3;
				default:
					return -1;
				}
			}};
			return F::f(lhs) < F::f(rhs);
		}
	);
	return;
}


void init_choice( std::string const &s )
{
	int discount = 0;
	bool
		wasadd = false, wassub = false,
		wasmult = false, wasdiv = false;
	for(auto b = s.begin(), e = s.end(); b != e; ++b)
	{
		switch(*b)
		{
		case '+': case 'a':
			if(wasadd)
				break;
			wasadd = true;
			choicearr[discount] = '+';
			++discount;
			break;
		case '-': case 's':
			if(wassub)
				break;
			wassub = true;
			choicearr[discount] = '-';
			++discount;
			break;
		case '*': case 'm':
			if(wasmult)
				break;
			wasmult = true;
			choicearr[discount] = '*';
			++discount;
			break;
		case '/': case ':': case 'd':
			if(wasdiv)
				break;
			wasdiv = true;
			choicearr[discount] = '/';
			++discount;
			break;
		default:
			break;
		}
	}
	if(!discount)
	{
		choicearr[0] = '+';
		discount = 1;
	}
	else
		sort_choicearr();
	choicedis.create(0, discount-1);
	return;
}


bool read_config( std::string const &filename )
{
	// preparation
	using namespace libconfig;

	Config cfg;
	try
	{
		cfg.readFile(filename.c_str());
	}
	catch(...)
	{
		return false;
	}
	cfg.setAutoConvert(true);
	Setting const &root = cfg.getRoot();
	int beg, end;
	string strbuf;



	// main
		// addition
	lookup(root, "addlhsdisbeg", beg, 1);
	lookup(root, "addlhsdisend", end, 99);
	addlhsdis.create(beg, end);

	lookup(root, "addrhsdisbeg", beg, 1);
	lookup(root, "addrhsdisend", end, 99);
	addrhsdis.create(beg, end);


		// substraction
	lookup(root, "sublhsdisbeg", beg, 1);
	lookup(root, "sublhsdisend", end, 99);
	sublhsdis.create(beg, end);

	lookup(root, "subrhsdisbeg", beg, 1);
	lookup(root, "subrhsdisend", end, 99);
	subrhsdis.create(beg, end);


		// multiplication
	lookup(root, "multlhsdisbeg", beg, 1);
	lookup(root, "multlhsdisend", end, 29);
	multlhsdis.create(beg, end);

	lookup(root, "multrhsdisbeg", beg, 1);
	lookup(root, "multrhsdisend", end, 9);
	multrhsdis.create(beg, end);



		// division
	lookup(root, "divlhsdisbeg", beg, 1);
	lookup(root, "divlhsdisend", end, 29);
	divlhsdis.create(beg, end);

	lookup(root, "divrhsdisbeg", beg, 1);
	lookup(root, "divrhsdisend", end, 9);
	divrhsdis.create(beg, end);



		// choice
	lookup(root, "type", strbuf, string("+"));
	init_choice(strbuf);

		// quescount & maxtime & writeresult
	lookup(root, "quescount", quescount, -1);
	lookup(root, "maxtime", maxtime, -1.0f);
	lookup(root, "writeresult", writeresult, true);


	return true;
}


bool write_config( std::string const &name )
{
	// preparation
	using namespace libconfig;
	Config conf;
	Setting &root = conf.getRoot();



	// addition
	root.add( "addlhsdisbeg", Setting::TypeInt ) = addlhsdis.a();
	root.add( "addlhsdisend", Setting::TypeInt ) = addlhsdis.b();
	root.add( "addrhsdisbeg", Setting::TypeInt ) = addrhsdis.a();
	root.add( "addrhsdisend", Setting::TypeInt ) = addrhsdis.b();

	// substraction
	root.add( "sublhsdisbeg", Setting::TypeInt ) = sublhsdis.a();
	root.add( "sublhsdisend", Setting::TypeInt ) = sublhsdis.b();
	root.add( "subrhsdisbeg", Setting::TypeInt ) = subrhsdis.a();
	root.add( "subrhsdisend", Setting::TypeInt ) = subrhsdis.b();

	// multiplication
	root.add( "multlhsdisbeg", Setting::TypeInt ) = multlhsdis.a();
	root.add( "multlhsdisend", Setting::TypeInt ) = multlhsdis.b();
	root.add( "multrhsdisbeg", Setting::TypeInt ) = multrhsdis.a();
	root.add( "multrhsdisend", Setting::TypeInt ) = multrhsdis.b();

	// addition
	root.add( "divlhsdisbeg", Setting::TypeInt ) = divlhsdis.a();
	root.add( "divlhsdisend", Setting::TypeInt ) = divlhsdis.b();
	root.add( "divrhsdisbeg", Setting::TypeInt ) = divrhsdis.a();
	root.add( "divrhsdisend", Setting::TypeInt ) = divrhsdis.b();

	// type
	string type;
	for(int i = 0; i <= choicedis.b(); ++i)
		type.push_back( choicearr[i] );
	root.add( "type", Setting::TypeString ) = type;

	// quescount & maxtime & writeresult
	root.add( "quescount", Setting::TypeInt ) = quescount;
	root.add( "maxtime", Setting::TypeFloat ) = maxtime;
	root.add( "writeresult", Setting::TypeBoolean ) = writeresult;


	// write in file
	conf.setTabWidth(0);
	try
	{
		conf.writeFile(name.c_str());
	}
	catch(...)
	{
		return false;
	}
	return true;
}





// end
