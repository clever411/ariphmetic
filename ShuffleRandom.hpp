#ifndef CLEVER_SHUFFLE_RANDOM_HPP
#define CLEVER_SHUFFLE_RANDOM_HPP

#include <algorithm>
#include <random>





namespace clever
{




template< class RandomEngine = std::default_random_engine >
class ShuffleRandom
{
public:
	typedef RandomEngine random_enginb_type;


	// init
	ShuffleRandom() {}

	ShuffleRandom( int a, int b )
	{
		create( a, b );
		return;
	}

	ShuffleRandom &create( int a, int b )
	{
		// init data
		a_ = a;
		b_ = b;
		size_ = b_-a_+1;
		if(size_ <= 0)
			throw "invalid interval";

		if(arr_)
			delete[] arr_;
		arr_ = new int[size_];
		pos_ = size_;


		// fulling data
		for(int i = 0; i < size_; ++i)
		{
			arr_[i] = i+a_;
		}

		return *this;
	}

	~ShuffleRandom()
	{
		if(arr_)
			delete[] arr_;
	}



	// main
	int operator()( random_enginb_type &re )
	{
		if(!arr_)
			throw "use not-init object";
		if( pos_ == size_ )
		{
			std::shuffle( arr_, arr_+size_, re );
			pos_ = 0;
		}
		++pos_;
		return arr_[ pos_-1 ];
	}



	// member acess
	inline int a() const
	{
		return a_;
	}
	inline int b() const
	{
		return b_;
	}



private:

	int a_, b_;
	int size_ = 0;
	int *arr_ = nullptr;
	int pos_ = 0;



};





}





#endif
