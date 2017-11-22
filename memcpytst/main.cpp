// http://z3moon.com/%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D/%EB%A9%94%EB%AA%A8%EB%A6%AC%EB%B3%B5%EC%82%AC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <chrono>
#include <thread>

using namespace std::chrono;

system_clock::time_point g_timeBegin    = system_clock::now();

long long LcAux_TimeGetTime()
{
	microseconds elapsed ={};
	system_clock::time_point time_cur = system_clock::now();

	elapsed = duration_cast<microseconds>(time_cur - g_timeBegin);
	long long ticks = (long long)(elapsed.count());

	ticks /= 1000;
	return ticks;
}

void copy_byte1(char* dst, char* src, int count)
{
	while(count-- >0)
	*dst++ = *src++;
}


void copy_byte4(char* dst, char* src, int count)
{
	int* dst_4 = (int*)dst;
	int* src_4 = (int*)src;
	int repeat = count >> 2;

	while(repeat-- >0)
		*dst_4++ = *src_4++;

	char* dst1 = (char*)dst_4;
	char* src1 = (char*)src_4;

	repeat = count & 3;
	while(repeat-- >0)
		*dst++ = *src++;
}

void copy_byte8(char* dst, char* src, int count)
{
	long long* dst_8 = (long long*)dst;
	long long* src_8 = (long long*)src;
	int repeat = count >> 3;

	while(repeat-- >0)
		*dst_8++ = *src_8++;

	char* dst1 = (char*)dst_8;
	char* src1 = (char*)src_8;

	repeat = count & 7;
	while(repeat-- >0)
		*dst1++ = *src1++;
}


void copy_duff(char* dst, char* src, int count)
{
	int repeat = (count + 7) / 8;

	switch(count % 8)
	{
		case 0: do {
					*dst++ = *src++;
		case 7:		*dst++ = *src++;
		case 6:		*dst++ = *src++;
		case 5:		*dst++ = *src++;
		case 4:		*dst++ = *src++;
		case 3:		*dst++ = *src++;
		case 2:		*dst++ = *src++;
		case 1:		*dst++ = *src++;
					} while(repeat-- >0);
	}
}

void temporal_copy( char* outbuff, char* inbuff, unsigned int size )
{
    const unsigned int step = 64; // cache line size

    while ( size > step )
    {
        _mm_prefetch( inbuff + 640, _MM_HINT_NTA ); // non-temporal prefetch
        __m128i A = _mm_load_si128( (__m128i*)(inbuff +  0) );
        __m128i B = _mm_load_si128( (__m128i*)(inbuff + 16) );
        __m128i C = _mm_load_si128( (__m128i*)(inbuff + 32) );
        __m128i D = _mm_load_si128( (__m128i*)(inbuff + 48) );

        // destination address must be 16-byte aligned!
        _mm_store_si128( (__m128i*)(outbuff +  0), A );
        _mm_store_si128( (__m128i*)(outbuff + 16), B );
        _mm_store_si128( (__m128i*)(outbuff + 32), C );
        _mm_store_si128( (__m128i*)(outbuff + 48), D );

        inbuff  += step;
        outbuff += step;
        size -= step;
    }

    memcpy( inbuff, outbuff, size );  // 나머지부분은 간단하게 memcpy 로 복사

    //_mm_mfence(); // 멀티스레드 환경에서 사용
}

int main(int,char**)
{
	int size = 512 *1024 * 1024 ;
	char* src =(char*)malloc(size);
	char* dst =(char*)malloc(size);

	long long time_b = LcAux_TimeGetTime();
	long long time_e = time_b;
	long long time_t = 0;

	memcpy(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("memcpy: %lld\n",time_t);


	copy_byte1(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("copy_byte1: %lld\n",time_t);


	copy_byte4(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("copy_byte4: %lld\n",time_t);


	copy_byte8(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("copy_byte8: %lld\n",time_t);


	copy_duff(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("copy_duff: %lld\n",time_t);



	temporal_copy(dst, src, size);

	time_e = LcAux_TimeGetTime();
	time_t = time_e - time_b;
	time_e = time_b;

	printf("temporal_copy: %lld\n",time_t);


	return 0;
}
