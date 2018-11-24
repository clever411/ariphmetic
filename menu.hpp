/*
 * autor:   clever
 * date:    Nov 21 13:43:49 2018
 */

#ifndef MENUFUN_HPP
#define MENUFUN_HPP

#include <string>

#include "rwcfg.hpp"




// enum
enum ReturnType
{
	success,
	input_cancelled,
	invalid_input
};





// functions
void print_configuration();
std::string set_type();
ReturnType scan_intervals(
	int &lb, int &le, int &rb, int &re
);
std::string set_questions_count(int &qc);
std::string set_maxtime(float &mt);
std::string set_writeresult(bool &writeresult);
std::string menu_read_config();
std::string menu_write_config();





#endif
