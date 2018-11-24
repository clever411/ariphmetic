#ifndef STAT_UNIT_HPP
#define STAT_UNIT_HPP

#include <iostream>
#include <string>
#include <vector>





struct StatUnit
{
	enum Type
	{
		undefined = 0,
		addition = 1 << 0,
		substraction = 1 << 1,
		multiplication = 1 << 2,
		division = 1 << 3
	};

	Type type = undefined;
	int lhs = -1, rhs = -1;
	float time = -1.0f;
	bool rightly = false;

	
	
	
	
	// functions
	template<class Ostream>
	Ostream &write( Ostream &os ) const
	{
		os.
			write( (char const *)&type, sizeof type ).
			write( (char const *)&lhs, sizeof lhs).
			write( (char const *)&rhs, sizeof rhs).
			write( (char const *)&time, sizeof time).
			write( (char const *)&rightly, sizeof rightly);
		return os;
	}

	template<class Istream>
	Istream &read( Istream &is )
	{
		is.
			read( (char *)&type, sizeof type ).
			read( (char *)&lhs, sizeof lhs ).
			read( (char *)&rhs, sizeof rhs ).
			read( (char *)&time, sizeof time).
			read( (char *)&rightly, sizeof rightly);
		return is;
	}
	

	
	template<class Ostream>
	Ostream &print( Ostream &os ) const
	{
		os << "(";
		switch(type)
		{
		case addition:
			os << lhs << " + " << rhs;
			break;
		case substraction:
			os << lhs << " - " << rhs;
			break;
		case multiplication:
			os << lhs << " * " << rhs;
			break;
		case division:
			os << lhs << " / " << rhs;
			break;
		case undefined:
			os << lhs << " ? " << rhs;
			break;
		}
		os << ") ->\t" << time << "\t: ";
		if(rightly)
			os << "right";
		else
			os << "wrong";
		return os;
	}

	template<class Istream>
	Istream &scan( Istream &is )
	{
		std::string tp;
		is >> tp;
		switch(tp.front())
		{
		case 'a': case 'A': case '0': case '+':
			type = addition;
			break;
		case 's': case 'S': case '1': case '-':
			type = substraction;
			break;
		case 'm': case 'M': case '2': case '*':
			type = multiplication;
			break;
		case 'd': case 'D': case '3': case '/': case ':':
			type = division;
			break;
		default:
			type = undefined;
		}

		is >> lhs >> rhs >> time >> rightly;
		return is;
	}
	


};

template<class Ostream>
Ostream &operator<<( Ostream &os, StatUnit const &toprint )
{
	return toprint.print(os);
}

template<class Istream>
Istream &operator>>( Istream &is, StatUnit &toscan )
{
	return toscan.scan(is);
}





struct Stat
{
	struct Scope
	{
		int
			lhsbeg = -1, lhsend = -1,
			rhsbeg = -1, rhsend = -1;

		template<class Ostream>
		Ostream &write( Ostream &os ) const
		{
			os.
				write( (char const *)&lhsbeg, sizeof lhsbeg ).
				write( (char const *)&lhsend, sizeof lhsend ).
				write( (char const *)&rhsbeg, sizeof rhsbeg ).
				write( (char const *)&rhsend, sizeof rhsend );
			return os;
		}
		template<class Istream>
		Istream &read( Istream &is ) const
		{
			is.
				read( (char *)&lhsbeg, sizeof lhsbeg ).
				read( (char *)&lhsend, sizeof lhsend ).
				read( (char *)&rhsbeg, sizeof rhsbeg ).
				read( (char *)&rhsend, sizeof rhsend );
			return is;
		}
	};
	
	
	
	int type = StatUnit::undefined;
	Scope addscope, subscope, multscope, divscope;

	std::vector<StatUnit> details;
	int count, right;
	float time;
	
	
	
	
	
	// funtions
	template<class Ostream>
	Ostream &write( Ostream &os ) const
	{
		os.write( (char const *)&type, sizeof type );
		if( type & StatUnit::addition )
			addscope.write(os);
		if( type & StatUnit::substraction )
			subscope.write(os);
		if( type & StatUnit::multiplication )
			multscope.write(os);
		if( type & StatUnit::division )
			divscope.write(os);

		os.write( (char const *)&count, sizeof count );
		os.write( (char const *)&right, sizeof right );
		os.write( (char const *)&time, sizeof time );

		for(auto b = details.begin(), e = details.end(); b != e; ++b)
		{
			b->write(os);
		}
		
		return os;
	}

	template<class Istream>
	Istream &read( Istream &is ) 
	{
		is.read( (char *)&type, sizeof type );
		if( type & StatUnit::addition )
			addscope.read(is);
		if( type & StatUnit::substraction )
			subscope.read(is);
		if( type & StatUnit::multiplication )
			multscope.read(is);
		if( type & StatUnit::division )
			divscope.read(is);

		is.read( (char *)&count, sizeof count );
		is.read( (char *)&right, sizeof right );
		is.read( (char *)&time, sizeof time );

		details.resize(count);
		for(int i = 0; i < count; ++i)
		{
			details[i].read(is);
		}

		return is;
	}
	


	template<class Ostream>
	Ostream &print_type( Ostream &os, std::string const &tab = "" ) const
	{
		if(type == StatUnit::undefined)
			os << tab << "?";
		else
		{
			bool wasfirst = false;
			if( type & StatUnit::addition )
			{
				os << tab <<
					"[" << addscope.lhsbeg << ", " << addscope.lhsend << "] + " <<
					"[" << addscope.rhsbeg << ", " << addscope.rhsend << "]";
				wasfirst = true;
			}
			if( type & StatUnit::substraction )
			{
				if(wasfirst)
					os << "\n";
				else
					wasfirst = true;
				os << tab <<
					subscope.lhsbeg << ", " << subscope.lhsend << "] - " <<
					"[" << subscope.rhsbeg << ", " << subscope.rhsend << "]";
			}
			if( type & StatUnit::multiplication )
			{
				if(wasfirst)
					os << "\n";
				else
					wasfirst = true;
				os << tab <<
					"[" << multscope.lhsbeg << ", " << multscope.lhsend << "] * " <<
					"[" << multscope.rhsbeg << ", " << multscope.rhsend << "]";
			}
			if( type & StatUnit::division )
			{
				if(wasfirst)
					os << "\n";
				else
					wasfirst = true;
				os << tab <<
					"[" << divscope.lhsbeg << ", " << divscope.lhsend << "] / " <<
					"[" << divscope.rhsbeg << ", " << divscope.rhsend << "]";
			}
		}
		return os;
	}

	template<class Ostream>
	Ostream &print_details( Ostream &os, std::string const &tab = "" ) const
	{
		if(details.empty())
			return os << tab << "empty";
		os << tab << details.front();
		for(auto b = details.begin()+1, e = details.end(); b != e; ++b)
		{
			os << '\n' << tab << *b;
		}
		return os;
	}

	template<class Ostream>
	Ostream &print( Ostream &os, std::string const &tab = "" ) const
	{
		std::string newtab = tab+"\t";
		os << "type:\n";
		print_type( os, newtab );
		os << '\n' << right << '/' << count;
		if(count)
			os << ": " << right*1000/count/1000.0f;
		os << "\ntime: " << time;
		if(right)
		{
			os << "\ntime/right: " << int(time/right*1000)/1000.0f;
			os << "\ndetails:\n";
			print_details( os, newtab );
		}
		else if(count)
		{
			os << "\ntime/right: infinity";
			os << "\ndetails:\n";
			print_details( os, newtab );
		}
		
		return os;
	}
};

template<class Ostream>
Ostream &operator<<( Ostream &os, Stat const &toprint )
{
	return toprint.print(os);
}




// end



#endif
