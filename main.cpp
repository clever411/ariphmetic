/*
 * autor:   clever
 * date:    Nov 20 11:19:56 2018
 */


#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <tuple>

#include <clever/OstreamChrono.hpp>
#include <clever/Stopwatch.hpp>

// #include "CuteRandom.hpp"
#include "defs.hpp"
#include "menu.hpp"
#include "Menu.hpp"
#include "rwcfg.hpp"
#include "rwdata.hpp"
#include "Stat.hpp"
#include "ShuffleRandom.hpp"



using namespace clever;
using namespace std;
using namespace chrono;





// objects
random_engine_type dre( time(0) );
distribution_type
	addlhsdis(1, 99), addrhsdis(1, 99),
	sublhsdis(1, 99), subrhsdis(1, 99),
	multlhsdis(1, 20), multrhsdis(1, 20),
	divlhsdis(1, 99), divrhsdis(1, 9),
	choicedis(0, 0);
char choicearr[4] = { '+', '-', '*', '/' };
int quescount = -1;
float maxtime = -1.0f;
bool writeresult = true;

string shello("Math in your mind v0.5");





// process - using CuteRandom objects, choicearr, dre
// write Stat, write in cout, read from cin
bool ask( StatUnit *unit = nullptr, int *rightanswer = nullptr )
{
	// preparation
	string strinput;
	int lhs, rhs;
	int input, answer;
	char choice = choicearr[choicedis(dre)];
	Stopwatch<system_clock> watch;



	// output
	switch(choice)
	{
	case '+':
		lhs = addlhsdis(dre);
		rhs = addrhsdis(dre);
		answer = lhs+rhs;
		cout << lhs << " + " << rhs << " = ";
		if(unit)
			unit->type = StatUnit::addition;
		break;
	case '-':
		lhs = sublhsdis(dre);
		rhs = subrhsdis(dre);
		answer = lhs-rhs;
		cout << lhs << " - " << rhs << " = ";
		if(unit)
			unit->type = StatUnit::substraction;
		break;
	case '*':
		lhs = multlhsdis(dre);
		rhs = multrhsdis(dre);
		answer = lhs*rhs;
		cout << lhs << " * " << rhs << " = ";
		if(unit)
		unit->type = StatUnit::multiplication;
		break;
	case '/':
		lhs = divlhsdis(dre);
		rhs = divrhsdis(dre);
		answer = lhs/rhs;
		cout << lhs << " / " << rhs << " = ";
		if(unit)
			unit->type = StatUnit::division;
		break;
	default: 
		throw "impossible choice";
	}
	if(rightanswer)
		*rightanswer = answer;



	// input
read_again_label:
	watch.start();
	if(!(cin >> strinput))
	{
		cin.clear();
		return false;
	}
	watch.stop();
	cin.get(); // clear from \n
	try
	{
		input = stoi( strinput );
	}
	catch(...)
	{
		CLEAR_CONSOLE
		cout << "invalid input\n\n";
		switch(choice)
		{
		case '+':
			cout << lhs << " + " << rhs << " = ";
			break;
		case '-':
			cout << lhs << " - " << rhs << " = ";
			break;
		case '*':
			cout << lhs << " * " << rhs << " = ";
			break;
		case '/':
			cout << lhs << " / " << rhs << " = ";
			break;
		}
		goto read_again_label;
	}



	// write StatUnit
	if(unit)
	{
		unit->lhs = lhs; unit->rhs = rhs;
		unit->time = duration_cast<milliseconds>(
			watch.duration()
		).count()/1000.0f;
		unit->rightly = ( input == answer );
	}



	return true;
}

string process(
	Stat *stat = nullptr
)
{

	// preparation
	bool printwrong = false;
	int rightanswer;
	int count = 0, right = 0;
	int type = StatUnit::undefined;
	float time = 0.0f;
	if(stat)
	{
		stat->details.clear();
		stat->type = StatUnit::undefined;
		stat->count = stat->right = 0;
		stat->time = 0.0f;
	}
	StatUnit unit;



	// loop
	CLEAR_CONSOLE
	cout << "Tap ENTER to start"; cin.get();
	if(!cin)
	{
		cin.clear();
		return "Cancelled";
	}
	do
	{
		// for beauty
		CLEAR_CONSOLE
		if(printwrong)
		{
			cout << "wrong! Right answer: " << rightanswer << '\n';
			printwrong = false;
		}
		else
			cout.put('\n');
		cout.put('\n');


		// ask
		if( !ask( &unit, &rightanswer ) )
			break;


		// calc input
		type |= unit.type;
		++count;
		if(unit.rightly)
			++right;
		else
			printwrong = true;
		if(stat)
			stat->details.push_back(unit);
		time += unit.time;



		// print result, exit if need
		if( quescount > 0 && count == quescount )
		{
			CLEAR_CONSOLE
			if(printwrong)
				cout << "wrong\n";
			else
				cout << '\n';
			cout << "Questions is over! Tap ENTER";
			cin.get(); // waiting
			break;
		}
		if( maxtime > 0 && time >= maxtime )
		{
			CLEAR_CONSOLE
			if(printwrong)
				cout << "wrong\n";
			else
				cout << '\n';
			cout << "\nTime is out! Tap ENTER";
			cin.get(); // waiting
			break;
		}
	}
	while(true);


	if(stat)
	{
		// set scope
		stat->type = type;
		stat->count = count;
		stat->right = right;
		stat->time = time;
		static auto const f = [](
			Stat::Scope &s,
			distribution_type const &lhs,
			distribution_type const &rhs
		)->void
		{
			s.lhsbeg = lhs.a();
			s.lhsend = lhs.b();
			s.rhsbeg = rhs.a();
			s.rhsend = rhs.b();
			return;
		};
		f(stat->addscope, addlhsdis, addrhsdis);
		f(stat->subscope, sublhsdis, subrhsdis);
		f(stat->multscope, multlhsdis, multrhsdis);
		f(stat->divscope, divlhsdis, divrhsdis);

		CLEAR_CONSOLE
		cout << *stat << endl;
		std::cout << "-----------------------------------" << std::endl;
		cout << "Tap ENTER to back in main menu" << endl;
		cin.get(); // waiting
		return "";
	}



	cout << "Tap ENTER to back in main menu" << endl;
	cin.get(); // waiting
	return "";
}





// main
int main( int argc, char *argv[] )
{
	if(argc > 1)
		read_config( argv[1] );

	Menu mainmenu, intervalmenu;



	// interval menu
	intervalmenu.defheader = "Choose a type";
	intervalmenu.bars =
	{
		{
			"For addition",
			[]()->string
			{
				int lb, le, rb, re;
				auto res = scan_intervals(lb, le, rb, re);
				if(res == invalid_input)
					return "Invalid input";
				if(res == input_cancelled)
					return "Input cancelled";
				addlhsdis.create( lb, le );
				addrhsdis.create( rb, re );
				char buf[60];
				snprintf(
					buf, 60, "[%i, %i] + [%i, %i]",
					lb, le, rb, re
				);
				return string("Interval set: ") + string(buf);
			},
			"aA+"
		},
		{
			"For substraction",
			[]()->string
			{
				int lb, le, rb, re;
				auto res = scan_intervals(lb, le, rb, re);
				if(res == invalid_input)
					return "Invalid input";
				if(res == input_cancelled)
					return "Input cancelled";
				sublhsdis.create( lb, le );
				subrhsdis.create( rb, re );
				char buf[60];
				snprintf(
					buf, 60, "[%i, %i] - [%i, %i]",
					lb, le, rb, re
				);
				return string("Interval set: ") + string(buf);
			},
			"sS-"
		},
		{
			"For multiplication",
			[]()->string
			{
				int lb, le, rb, re;
				auto res = scan_intervals(lb, le, rb, re);
				if(res == invalid_input)
					return "Invalid input";
				if(res == input_cancelled)
					return "Input cancelled";
				multlhsdis.create( lb, le );
				multrhsdis.create( rb, re );
				char buf[60];
				snprintf(
					buf, 60, "[%i, %i] * [%i, %i]",
					lb, le, rb, re
				);
				return string("Interval set: ") + string(buf);
			},
			"mM*"
		},
		{
			"For division",
			[]()->string
			{
				int lb, le, rb, re;
				auto res = scan_intervals(lb, le, rb, re);
				if(res == invalid_input)
					return "Invalid input";
				if(res == input_cancelled)
					return "Input cancelled";
				divlhsdis.create( lb, le );
				divrhsdis.create( rb, re );
				char buf[60];
				snprintf(
					buf, 60, "[%i, %i] / [%i, %i]",
					lb, le, rb, re
				);
				return string("Interval set: ") + string(buf);
			},
			"dD/:"
		}
	};


	// main menu
	mainmenu.defheader = shello;
	mainmenu.bars =
	{
		{
			"Start",
			[]()->string
			{
				Stat stat;
				string str = process( &stat );
				if(stat.type != StatUnit::undefined)
					write_stat(stat);
				return str;
			},
			"sSgGbB"
		},
		{
			"Set type",
			set_type,
			"tT"
		},
		{
			"Set intervals",
			[&intervalmenu]()->string
			{
				intervalmenu();
				return "";
			},
			"iI"
		},
		{
			"Set questions count",
			[]()->string
			{
				return set_questions_count(quescount);
			},
			"cC"
		},
		{
			"Set max time",
			[]()->string
			{
				return set_maxtime(maxtime);
			},
			"mM"
		},
		{
			"Set wether to write result",
			[]() { return set_writeresult(writeresult); },
			"rR"
		},
		{
			"Print settings",
			[]()->string
			{
				CLEAR_CONSOLE
				cout << "The current configuration:\n\n";
				print_configuration();
				cout << "\nTap ENTER to back in main menu" << endl;
				cin.get();
				if(!cin)
					cin.clear();
				return "";
			},
			"pP"
		},
		{
			"Read config file",
			menu_read_config,
			"rR"
		},
		{
			"Write config file",
			menu_write_config,
			"wW"
		}
	};



	mainmenu();



	return 0;
}





// end
