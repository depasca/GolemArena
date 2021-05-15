#ifndef	__GATIMER_H__
#define	__GATIMER_H__	

#include <sys/time.h>
//#include "gautils.h"



class gaTimer
{
	long time, oldtime;

  public:
	
	gaTimer()
	{
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=t.tv_sec+(t.tv_usec/1000000);
		oldtime=time;
	}
	
	~gaTimer() {}
	
	void ResetTimer()
	{
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=t.tv_sec+(t.tv_usec/1000000);
		oldtime=time;
	}
	
	void SetTimer(long _time)
	{
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=t.tv_sec+(t.tv_usec/1000000);
		oldtime=time -_time;
	}
	
	long ReadTimer()
	{
		UpdateTimer();
		return (time-oldtime);
	}
	
	void UpdateTimer()
	{
//		if(GAPAUSE==false)
	 	{
	 		struct timeval t;
			struct timezone tz;
			gettimeofday(&t, &tz);
			time=t.tv_sec+(t.tv_usec/1000000);		
		}
	}
	
};


#endif

