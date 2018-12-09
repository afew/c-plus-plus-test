

#include <stdio.h>

#include "Str.h"

int main(int, char**)
{
	Str s = "hey";
	s.setf("Hello world value: %d", 100);
	printf("%s\n", s.c_str());
}
