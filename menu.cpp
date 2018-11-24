/*
 * autor:   clever
 * date:    Nov 20 11:02:36 2018
 */

#include "menu.hpp"

#include <iostream>
#include <sstream>

#define CLEAR_CONSOLE system("clear");

using namespace std;





// functions
template<
	typename T,
	typename Functor
>
ReturnType scanvalue(T &val, Functor fun)
{
	string buf;
	if( !( cin >> buf ) )
	{
		cin.clear();
		return input_cancelled;
	}
	cin.get(); // clear from \n

	try
	{
		val = fun(buf);
	}
	catch(...)
	{
		return invalid_input;
	}
	return success;
}





// core
void print_configuration()
{
	for(int i = 0; i <= choicedis.b(); ++i)
	{
		switch(choicearr[i])
		{
		case '+':
			cout <<
				"[" << addlhsdis.a() << ", " <<
				addlhsdis.b() << "] + " <<
				"[" << addrhsdis.a() << ", " <<
				addrhsdis.b() << "]\n";
			break;
		case '-':
			cout <<
				"[" << sublhsdis.a() << ", " <<
				sublhsdis.b() << "] - " <<
				"[" << subrhsdis.a() << ", " <<
				subrhsdis.b() << "]\n";
			break;
		case '*':
			cout <<
				"[" << multlhsdis.a() << ", " <<
				multlhsdis.b() << "] * " <<
				"[" << multrhsdis.a() << ", " <<
				multrhsdis.b() << "]\n";
			break;
		case '/':
			cout <<
				"[" << divlhsdis.a() << ", " <<
				divlhsdis.b() << "] / " <<
				"[" << divrhsdis.a() << ", " <<
				divrhsdis.b() << "]\n";
			break;
		}
	}
	if( quescount > 0 )
		cout << "Questions count: " << quescount << '\n';
	if( maxtime > 0 )
		cout << "Max time: " << maxtime << '\n';
	cout << "Write result: ";
	if(writeresult)
		cout << "true\n";
	else
		cout << "false\n";
	return;
}

string set_type()
{
	string type;
	CLEAR_CONSOLE
	cout << "Enter type: ";
	if( !( cin >> type ) )
	{
		cin.clear();
		return "Cancelled";
	}
	cin.get(); // clear from \n
	init_choice(type);
	
	type = "";
	for(int i = 0; i <= choicedis.b(); ++i)
		type.push_back(choicearr[i]);

	return string("Type has been set: ") + type;
}

ReturnType scan_intervals(
	int &lb, int &le, int &rb, int &re
)
{
	CLEAR_CONSOLE
	cout << "Enter min and max for lhs: ";
	auto res = scanvalue<int>(
		lb, [](string const &s) { return stoi(s); }
	); 
	if(res != success)
		return res;

	res = scanvalue<int>(
		le, [](string const &s) { return stoi(s); }
	); 
	if(res != success)
		return res;



	// rhs
	cout << "Enter min and max for rhs: ";
	res = scanvalue<int>(
		rb, [](string const &s) { return stoi(s); }
	); 
	if(res != success)
		return res;

	res = scanvalue<int>(
		re, [](string const &s) { return stoi(s); }
	); 



	return res;
}

string set_questions_count(int &qc)
{
	CLEAR_CONSOLE
	cout << "Enter questions count: ";
	switch(scanvalue<int>(
		qc,
		[](string const &s) { return stoi(s); }
	))
	{
	case input_cancelled:
		return "Input cancelled";
	case invalid_input:
		return "Invalid input";
	case success:
		return string("Questions count set to ") +
			to_string(qc);
	default:
		throw "impossible(set_questions_count)";
	}
}

string set_maxtime(float &mt)
{
	CLEAR_CONSOLE
	cout << "Enter max time: ";
	switch(scanvalue<float>(
			mt,
			[](string const &s) { return stof(s); }
	))
	{
	case input_cancelled:
		return "Input cancelled";
	case invalid_input:
		return "Invalid input";
	case success: {
		stringstream ss;
		ss << mt;
		return string( "Max time is set to " ) + ss.str();
	}
	default:
		throw "Impossible(set_maxtime)";
	}
}

string set_writeresult(bool &writeresult)
{
	string input;
	CLEAR_CONSOLE
	cout << "Enter do write result: ";
	if( !(cin >> input) )
	{
		cin.clear();
		return "Cancelled";
	}
	switch(input.front())
	{
	case '1': case 't': case 'T':
		writeresult = true;
		return "Smart mode set to true";
	case '0': case 'f': case 'F':
		writeresult = false;
		return "Smart mode set to false";
	default:
		return "Invalid input";
	}
}

string menu_read_config()
{
	string input;
	CLEAR_CONSOLE
	cout << "Enter name file: ";
	if( !( cin >> input ) )
	{
		cin.clear();
		return "Reading configuration file is cancelled";
	}
	if(read_config(input))
		return "Success read config";
	return "Fail read config";
}

string menu_write_config()
{
	string input;
	CLEAR_CONSOLE
	cout << "Enter name file: ";
	if( !( cin >> input ) )
	{
		cin.clear();
		return "Writing configuration file is cancelled";
	}
	if(write_config(input))
		return "Success write config";
	return "Fail write config";
}





// end
