#ifndef _lctype_int128_H_
#define _lctype_int128_H_

struct int128
{
	#define INT64_ZERO       0x0ll
	#define INT64_MINUS_ONE  -1ll
	#define INT64_ALL_BIT    0xFFFFFFFFFFFFFFFFll
	#define INT64_TOP_BIT    0x8000000000000000ll
	#define INT64_MAX_SHIFT  64
	#define INT128_MAX_SHIFT 128

	typedef   signed char           s8;
	typedef unsigned char           u8;
	typedef   signed short          s16;
	typedef unsigned short          u16;
	typedef   signed int            s32;
	typedef unsigned int            u32;
	typedef   signed long long int  s64;
	typedef unsigned long long int  u64;
	typedef double                  f64;

	u64 _0; s64 _1;	// little endian

	int128() : _0(INT64_ZERO), _1(INT64_ZERO){}
	int128(u64 m0, s64 m1=INT64_ZERO) : _0(m0), _1(m1){}
	int128(s64 r) : _0(r), _1{ (r<0)? INT64_MINUS_ONE: INT64_ZERO} {}
	int128(u32 r) : int128((u64)r){}
	int128(s32 r) : int128((s64)r){}
	int128(f64 r) : int128((s64)r){}

	int128 not();
	int128 l_shift(unsigned int);
	int128 r_shift(unsigned int);
	int128 netagive();
	int128 add_to_this(const int128&);
	int128 sub_to_this(const int128&);
	int128 mul_to_this(const int128&);
	int128 div_to_this(const int128&);
	int128 mod_to_this(const int128&);
	int128 increase();
	int128 decrease();

	// compare
	int is_equal     (const int128& r){	return (_1 != r._1 || _0 != r._0)? 0: 1;	}
	int is_not_equal (const int128& r){	return (_1 != r._1 && _0 != r._0)? 1: 0;	}
	int is_less      (const int128& r){	if(_1 < r._1)return 1;	if(_1 > r._1)return 0;	if(_0 < r._0)return 1;	return 0;	}
	int is_greater   (const int128& r){	if(_1 > r._1)return 1;	if(_1 < r._1)return 0;	if(_0 > r._0)return 1;	return 0;	}

	// assignment
	int128 operator=(const int128& rhs){					_0=rhs._0; _1=rhs._1;	return *this; }
	int128 operator=(const u64     r)  {	int128 rhs(r);	_0=rhs._0; _1=rhs._1;	return *this; }
	int128 operator=(const s64     r)  {	int128 rhs(r);	_0=rhs._0; _1=rhs._1;	return *this; }
	int128 operator=(const u32     r)  {	int128 rhs(r);	_0=rhs._0; _1=rhs._1;	return *this; }
	int128 operator=(const s32     r)  {	int128 rhs(r);	_0=rhs._0; _1=rhs._1;	return *this; }

	// casting
	operator long long         () { return (s64)_0; }
	operator int               () { return (s32)_0; }
	operator unsigned long long() { return (u64)_0; }
	operator unsigned int      () { return (u32)_0; }
	operator double            () { return (u32)_0; }

	int128 add(const int128& r)	{	int128 ret = *this;	ret.add_to_this(r);	return ret;	}
	int128 sub(const int128& r) {	int128 ret = *this;	ret.sub_to_this(r);	return ret; }
	int128 mul(const int128& r)	{	int128 ret = *this;	ret.mul_to_this(r);	return ret;	}
	int128 div(const int128& r)	{	int128 ret = *this;	ret.div_to_this(r);	return ret;	}
	int128 mod(const int128& r)	{	int128 ret = *this;	ret.mod_to_this(r);	return ret;	}

	int128 add(const u64 r) { return this->add(int128{r}); }
	int128 sub(const u64 r) { return this->sub(int128{r}); }
	int128 mul(const u64 r) { return this->mul(int128{r}); }
	int128 div(const u64 r) { return this->div(int128{r}); }
	int128 mod(const u64 r) { return this->mod(int128{r}); }

	int128 add(const s64 r) { return this->add(int128{r}); }
	int128 sub(const s64 r) { return this->sub(int128{r}); }
	int128 mul(const s64 r) { return this->mul(int128{r}); }
	int128 div(const s64 r) { return this->div(int128{r}); }
	int128 mod(const s64 r) { return this->mod(int128{r}); }

	int128 add(const u32 r) { return this->add(int128{r}); }
	int128 sub(const u32 r) { return this->sub(int128{r}); }
	int128 mul(const u32 r) { return this->mul(int128{r}); }
	int128 div(const u32 r) { return this->div(int128{r}); }
	int128 mod(const u32 r) { return this->mod(int128{r}); }

	int128 add(const s32 r) { return this->add(int128{r}); }
	int128 sub(const s32 r) { return this->sub(int128{r}); }
	int128 mul(const s32 r) { return this->mul(int128{r}); }
	int128 div(const s32 r) { return this->div(int128{r}); }
	int128 mod(const s32 r) { return this->mod(int128{r}); }

	int128 add(const f64 r) { return this->add(int128{r}); }
	int128 sub(const f64 r) { return this->sub(int128{r}); }
	int128 mul(const f64 r) { return this->mul(int128{r}); }
	int128 div(const f64 r) { return this->div(int128{r}); }
	int128 mod(const f64 r) { return this->mod(int128{r}); }

	// arithmetic operator
	int128 operator++(   ) { return increase(); }
	int128 operator--(   ) { return decrease(); }
	int128 operator++(int) { int128 ret = *this; this->increase(); return ret; }
	int128 operator--(int) { int128 ret = *this; this->decrease(); return ret; }

	int128 operator+ (const int128& r) { return this->add(r); }
	int128 operator- (const int128& r) { return this->sub(r); }
	int128 operator* (const int128& r) { return this->mul(r); }
	int128 operator/ (const int128& r) { return this->div(r); }
	int128 operator% (const int128& r) { return this->mod(r); }

	int128 operator+ (const u64 r) { return this->add(r); }
	int128 operator- (const u64 r) { return this->sub(r); }
	int128 operator* (const u64 r) { return this->mul(r); }
	int128 operator/ (const u64 r) { return this->div(r); }
	int128 operator% (const u64 r) { return this->mod(r); }

	int128 operator+ (const s64 r) { return this->add(r); }
	int128 operator- (const s64 r) { return this->sub(r); }
	int128 operator* (const s64 r) { return this->mul(r); }
	int128 operator/ (const s64 r) { return this->div(r); }
	int128 operator% (const s64 r) { return this->mod(r); }

	int128 operator+ (const u32 r) { return this->add(r); }
	int128 operator- (const u32 r) { return this->sub(r); }
	int128 operator* (const u32 r) { return this->mul(r); }
	int128 operator/ (const u32 r) { return this->div(r); }
	int128 operator% (const u32 r) { return this->mod(r); }

	int128 operator+ (const s32 r) { return this->add(r); }
	int128 operator- (const s32 r) { return this->sub(r); }
	int128 operator* (const s32 r) { return this->mul(r); }
	int128 operator/ (const s32 r) { return this->div(r); }
	int128 operator% (const s32 r) { return this->mod(r); }

	int128 operator+ (const f64 r) { return this->add(r); }
	int128 operator- (const f64 r) { return this->sub(r); }
	int128 operator* (const f64 r) { return this->mul(r); }
	int128 operator/ (const f64 r) { return this->div(r); }
	int128 operator% (const f64 r) { return this->mod(r); }

	friend int128 operator+ (const int128& l, const int128& r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator- (const int128& l, const int128& r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator* (const int128& l, const int128& r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator/ (const int128& l, const int128& r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator% (const int128& l, const int128& r) { int128 ret = l;	ret += r;	return ret; }

	friend int128 operator+ (const int128& l, u64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator- (const int128& l, u64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator* (const int128& l, u64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator/ (const int128& l, u64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator% (const int128& l, u64 r) { int128 ret = l;	ret += r;	return ret; }

	friend int128 operator+ (const int128& l, s64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator- (const int128& l, s64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator* (const int128& l, s64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator/ (const int128& l, s64 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator% (const int128& l, s64 r) { int128 ret = l;	ret += r;	return ret; }

	friend int128 operator+ (const int128& l, u32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator- (const int128& l, u32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator* (const int128& l, u32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator/ (const int128& l, u32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator% (const int128& l, u32 r) { int128 ret = l;	ret += r;	return ret; }

	friend int128 operator+ (const int128& l, s32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator- (const int128& l, s32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator* (const int128& l, s32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator/ (const int128& l, s32 r) { int128 ret = l;	ret += r;	return ret; }
	friend int128 operator% (const int128& l, s32 r) { int128 ret = l;	ret += r;	return ret; }


	int128 operator+=(const int128& r) { return this->add_to_this(r); }
	int128 operator-=(const int128& r) { return this->sub_to_this(r); }
	int128 operator*=(const int128& r) { return this->mul_to_this(r); }
	int128 operator/=(const int128& r) { return this->div_to_this(r); }
	int128 operator%=(const int128& r) { return this->mod_to_this(r); }

	int128 operator+=(const u64 r) { return this->operator+=(int128{r}); }
	int128 operator-=(const u64 r) { return this->operator-=(int128{r}); }
	int128 operator*=(const u64 r) { return this->operator*=(int128{r}); }
	int128 operator/=(const u64 r) { return this->operator/=(int128{r}); }
	int128 operator%=(const u64 r) { return this->operator%=(int128{r}); }

	int128 operator+=(const s64 r) { return this->operator+=(int128{r}); }
	int128 operator-=(const s64 r) { return this->operator-=(int128{r}); }
	int128 operator*=(const s64 r) { return this->operator*=(int128{r}); }
	int128 operator/=(const s64 r) { return this->operator/=(int128{r}); }
	int128 operator%=(const s64 r) { return this->operator%=(int128{r}); }

	int128 operator+=(const u32 r) { return this->operator+=(int128{r}); }
	int128 operator-=(const u32 r) { return this->operator-=(int128{r}); }
	int128 operator*=(const u32 r) { return this->operator*=(int128{r}); }
	int128 operator/=(const u32 r) { return this->operator/=(int128{r}); }
	int128 operator%=(const u32 r) { return this->operator%=(int128{r}); }

	int128 operator+=(const s32 r) { return this->operator+=(int128{r}); }
	int128 operator-=(const s32 r) { return this->operator-=(int128{r}); }
	int128 operator*=(const s32 r) { return this->operator*=(int128{r}); }
	int128 operator/=(const s32 r) { return this->operator/=(int128{r}); }
	int128 operator%=(const s32 r) { return this->operator%=(int128{r}); }

	int128 operator+=(const f64 r) { return this->operator+=(int128{r}); }
	int128 operator-=(const f64 r) { return this->operator-=(int128{r}); }
	int128 operator*=(const f64 r) { return this->operator*=(int128{r}); }
	int128 operator/=(const f64 r) { return this->operator/=(int128{r}); }
	int128 operator%=(const f64 r) { return this->operator%=(int128{r}); }

	// compare operator
	int operator< (const int128& r){	return this->is_less(r);	}
	int operator> (const int128& r){	return this->is_greater(r);	}
	int operator<=(const int128& r){	if(this->is_equal(r))return 1;	if(this->is_less(r))return 1;	return 0;	}
	int operator>=(const int128& r){	if(this->is_equal(r))return 1;	if(this->is_greater(r))return 1;return 0;	}
	int operator==(const int128& r){	return this->is_equal(r);	}
	int operator!=(const int128& r){	return !this->is_not_equal(r);	}

	int operator< (const u64 r) { return this->operator< (int128{r}); }
	int operator> (const u64 r) { return this->operator> (int128{r}); }
	int operator<=(const u64 r) { return this->operator<=(int128{r}); }
	int operator>=(const u64 r) { return this->operator>=(int128{r}); }
	int operator==(const u64 r) { return this->operator==(int128{r}); }
	int operator!=(const u64 r) { return this->operator!=(int128{r}); }

	int operator< (const s64 r) { return this->operator< (int128{r}); }
	int operator> (const s64 r) { return this->operator> (int128{r}); }
	int operator<=(const s64 r) { return this->operator<=(int128{r}); }
	int operator>=(const s64 r) { return this->operator>=(int128{r}); }
	int operator==(const s64 r) { return this->operator==(int128{r}); }
	int operator!=(const s64 r) { return this->operator!=(int128{r}); }

	int operator< (const u32 r) { return this->operator< (int128{r}); }
	int operator> (const u32 r) { return this->operator> (int128{r}); }
	int operator<=(const u32 r) { return this->operator<=(int128{r}); }
	int operator>=(const u32 r) { return this->operator>=(int128{r}); }
	int operator==(const u32 r) { return this->operator==(int128{r}); }
	int operator!=(const u32 r) { return this->operator!=(int128{r}); }

	int operator< (const s32 r) { return this->operator< (int128{r}); }
	int operator> (const s32 r) { return this->operator> (int128{r}); }
	int operator<=(const s32 r) { return this->operator<=(int128{r}); }
	int operator>=(const s32 r) { return this->operator>=(int128{r}); }
	int operator==(const s32 r) { return this->operator==(int128{r}); }
	int operator!=(const s32 r) { return this->operator!=(int128{r}); }

	int operator< (const f64 r) { return this->operator< (int128{r}); }
	int operator> (const f64 r) { return this->operator> (int128{r}); }
	int operator<=(const f64 r) { return this->operator<=(int128{r}); }
	int operator>=(const f64 r) { return this->operator>=(int128{r}); }
	int operator==(const f64 r) { return this->operator==(int128{r}); }
	int operator!=(const f64 r) { return this->operator!=(int128{r}); }

	// shift and bit
	int128 operator>>(int s){	int128 ret = *this;	return ret.r_shift(s);	}
	int128 operator<<(int s){	int128 ret = *this;	return ret.l_shift(s);	}
	int128 operator~()		{	int128 ret = *this;	return ret.not();		}
	// bit
};
#endif
