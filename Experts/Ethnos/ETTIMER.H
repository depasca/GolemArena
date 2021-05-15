#ifndef	__ETTIMER_H__
#define	__ETTIMER_H__	

#include <sys/time.h>
//#include "gautils.h"



class ETTimer
{
	double time, oldtime;

  public:
	
	ETTimer()
	{/*
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=(double)t.tv_sec+((double)t.tv_usec/ 1000000.0);
		oldtime=time;*/
	}
	
	~ETTimer() {}
	
	void ResetTimer()
	{/*
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=(double)t.tv_sec+((double)t.tv_usec/1000000.0);
		oldtime=time;*/
	}
	
	void SetTimer(double _time)
	{/*
		struct timeval t;
		struct timezone tz;
		gettimeofday(&t, &tz);
		time=(double)t.tv_sec+((double)t.tv_usec/1000000.0);
		oldtime=time-_time;*/
	}
	
	double ReadTimer()
	{
		UpdateTimer();
		return (time-oldtime);
	}
	
	void UpdateTimer()
	{
//		if(GAPAUSE==false)
	 	{/*
	 		struct timeval t;
			struct timezone tz;
			gettimeofday(&t, &tz);
			time=(double)t.tv_sec+((double)t.tv_usec/1000000.0);	*/	
		}
	}
	
};


#endif

