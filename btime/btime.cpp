// btime.cpp : Defines the exported functions for the DLL application.
//

#include <malloc.h>
#include "btime.h"
#include <time.h>
#include <sstream>

using namespace std;

#ifdef WIN32
#include <Windows.h>
#include <stdint.h>

int timeofday(struct timeval * tp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
	GetSystemTime( &system_time);
	tp->tv_sec = (int)system_time.wHour * 60 * 60 + (int)system_time.wMinute * 60 + (int)system_time.wSecond;
	tp->tv_usec = (int)system_time.wMilliseconds;
	return 0;
}
#else
#include <sys/time.h>
#include <string.h>

int timeofday(struct timeval * tp)
{
    gettimeofday(tp, NULL);
    tp->tv_sec = tp->tv_sec % 86400;
    return 0;
}
#endif

EXPORT char *byond_gettime(void)
{
	timeval *t;

	t = (timeval*) malloc(sizeof t);
	timeofday(t);
	
    std::stringstream ss;
	ss << t->tv_sec;
	ss << ".";
	ss << t->tv_usec;
	//ss << timeofday;
	char *p = new char[ss.str().size()+1];
	strcpy(p, ss.str().c_str());
	return p;
}

// C export
extern "C" EXPORT char *gettime(int argc, char *argv[]) 
{
	return byond_gettime();
}
