
#include "int128.h"

int128 int128::not()
{
	// ~this
	_0 = ~_0;
	_1 = ~_1;
	return *this;
}
int128 int128::l_shift(unsigned int s)
{
	if( s>= INT128_MAX_SHIFT)
	{
		_0 = INT64_ZERO;
		_1 = INT64_ZERO;
		return *this;
	}
	if(0==s)
		return *this;

	if( s< INT64_MAX_SHIFT)
	{
		_1 = (_1 << s) | (_0 >>(INT64_MAX_SHIFT-s));
		_0 =  _0 << s;
		return *this;
	}
	s -= INT64_MAX_SHIFT;
	_1 = _0;
	_1 = _0 << s;
	_0 = INT64_ZERO;
	return *this;
}

int128 int128::r_shift(unsigned int s)
{
	if( s>= INT128_MAX_SHIFT)
	{
		if(INT64_TOP_BIT & _1)
		{
			_0 = INT64_ALL_BIT;
			_1 = INT64_ALL_BIT;
		}
		else
		{
			_0 = INT64_ZERO;
			_1 = INT64_ZERO;
		}
		return *this;
	}
	if(0==s)
		return *this;

	if( s< INT64_MAX_SHIFT)
	{
		u64 f =(u64)_1;
		_0 = (_0 >> s) | (_1 <<(INT64_MAX_SHIFT-s));
		_1 = (_1 >> s);
		return *this;
	}
	s -= INT64_MAX_SHIFT;

	_1 = _1 >> s;
	_0 = (u64)_1;
	if(INT64_ZERO>_1)
		_1= INT64_MINUS_ONE;
	return *this;
}

int128 int128::netagive()						// negative = ~this + 1
{
	_0 = ~_0;
	_1 = ~_1;
	_0 += 1ll;
	if(_0 < 1ll)
		++_1;
	return *this;
}

int128 int128::add_to_this(const int128& rhs)	// +
{
	_0 += rhs._0;
	if(_0 < rhs._0)								// carry check: unsigned 두 수를 더해서 _0 또는 _0.r보다 작은 경우
		++_1;
	_1 += rhs._1;
	return *this;
}
int128 int128::sub_to_this(const int128& r)		// -: a-b => a + negative(b)
{
	int128 rhs{r};
	rhs.netagive();
	this->add_to_this(rhs);
	return *this;
}
int128 int128::mul_to_this(const int128& r)		// *
{
	return *this;
}
int128 int128::div_to_this(const int128& r)		// /
{
	return *this;
}
int128 int128::mod_to_this(const int128& r)		// %
{
	return *this;
}
int128 int128::increase()
{
	if((_0 + 1ll) < _0)
		++_1;
	++_0;
	return *this;
}
int128 int128::decrease()
{
	if((_0 - 1ll) > _0)
		--_1;
	--_0;
	return *this;
}



