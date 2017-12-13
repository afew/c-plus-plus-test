#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declaration
old_c_style_func(a, b);

void test_old_c_function_call()
{
	int d = old_c_style_func(100, 200);
}


// definition
int old_c_style_func(a, b)
	int a;
	int b;
{
	printf("c style function call(): %d %d\n", a, b);
	return a + b;
}
