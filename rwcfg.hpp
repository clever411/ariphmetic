/*
 * autor:   clever
 * date:    Nov 21 13:33:57 2018
 */

#ifndef RWCFG_HPP
#define RWCFG_HPP

#include <random>
#include <string>

#include "defs.hpp"





// extern objects
extern random_engine_type dre;
extern distribution_type
	addlhsdis, addrhsdis,
	sublhsdis, subrhsdis,
	multlhsdis, multrhsdis,
	divlhsdis, divrhsdis,
	choicedis;
extern char choicearr[4];
extern int quescount;
extern float maxtime;
extern bool writeresult;





// functions
void sort_choicearr();
void init_choice( std::string const &s );
void init_distributions();
bool read_config( std::string const &filename );
bool write_config( std::string const &filename );





#endif

