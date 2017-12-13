#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <array>
#include <memory>

using namespace std;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "static_member.h"

#define DECLARE_CLZZ_FUNC(f)       \
void MyFunc ## f ##(int a, int b)

#define CALL_FUNC(f)       \
MyFunc ## f

class clzzFinal final
{
	virtual void TstFunc(int arg1, int arg2)
	{
		printf("%d %d\n", arg1, arg2);
	}

	//mutable int c = 100;
};

DECLARE_CLZZ_FUNC(clzzFinal)
{
	printf("Hello world\n");
}

int tstF()
{
	int ret = 12345678;
	return ret;
}

int func_rval(int& c)
{
	printf("func_rval:reference: %d\n", c);
	return 0;
}

int func_rval(int&& c)
{
	printf("func_rval:rvalue: %d\n", c);
	return 0;
}


typedef std::vector<int >	lsINT;


struct MyStr
{
	int a, b, c;
};

extern "C" void test_old_c_function_call();

int main(int, char**)
{
	// call c function
	test_old_c_function_call();

	// macro test
	CALL_FUNC(clzzFinal)(10,20);

	// reference static member
	auto tt = TStaticMember::tst_array;

	// unique ptr
	unique_ptr<int > c(new int[10]);

	// decltype and auto
	auto arr  = new int[1]{0x12};
	decltype(arr) cc = arr;

	// r-value test
	func_rval(tstF() );


	// vector test
	lsINT* vc = new lsINT;
	for(int i=0; i<20; ++i)
		vc->push_back((i+1)*100);

	int& ref = (*vc)[10];
	printf("%d\n", (*vc)[10]);
	printf("%d\n", vc->operator[](10) );


	int i = 10;
	int v = 100;

	// mutable lambda function
	auto fl = [&v]()mutable -> int
    {
        ++v;
        return 0;
    };

	fl();
	printf("v lambda : %d \n", v);


	[](int i, int a)mutable -> int
	{
		++i;
		printf("lambda funciton call at once: %d %d\n", i, a);
		return 10000;
	}(i, 100);

	return 0;
}


