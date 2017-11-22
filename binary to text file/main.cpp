
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long  llong;

int main(int, char**)
{
	int   l_count = 0;
	llong* l_buf  = NULL;

	FILE* fp = fopen("NanumBarunGothicBold.otf", "rb");

	if(!fp)
		return 0;

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	l_count = int( (size+15)/8 );
	l_buf   = new llong[l_count]{};

	fread(l_buf, 1, size, fp);
	fclose(fp);

	FILE* fpo = fopen("NanumBarunGothicBold.txt", "wt");

	for(int i=0; i<l_count; ++i)
	{
		fprintf(fpo, "0x%016llX", l_buf[i]);

		if( (i+1)%16 )
			fprintf(fpo, ",");
		else
			fprintf(fpo, ",\n");
	}

	fclose(fpo);
	return 0;
}