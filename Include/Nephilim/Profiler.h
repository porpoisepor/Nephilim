#ifndef Profiler_h__
#define Profiler_h__

#include "Platform.h"
#include "Strings.h"
#include "Clock.h"

#include <map>

#include <iostream>
using namespace std;

NEPHILIM_NS_BEGIN

class ProfileStatistics
{
public:
	
	

	struct fnTrack
	{
		fnTrack()
		{
			counter = 0;
			m_total = 0.f;
		}

		void add()
		{
			counter++;
			if(counter >= 1000)
			{
				cout << "Profiler: Fn ran 1000 times with a total time of: "<<m_total <<endl;
				counter = 0;
			}
		}

		float m_total;
		int counter;
	};

	static Clock clock;
	static std::map<String, fnTrack> m_functions;

	static void trackFn(const String& fn, float time)
	{
		//cout<< "Track: "<<fn<<endl;
		m_functions[fn].add();
		m_functions[fn].m_total += time;
	} 
};

class AutoFnProfiler
{
public:
	AutoFnProfiler(const char* name, void* obj)
	{
		m_funcName = name;
		this->obj = obj;
	}

	~AutoFnProfiler()
	{
		ProfileStatistics::trackFn(m_funcName, clock.getElapsedTime().asSeconds());
	}

	String m_funcName;
	void* obj;
	Clock clock;
};

/// -- Fully disable all macros for release and global disable
#if defined NEPHILIM_NOPROFILER || defined NEPHILIM_RELEASE
#define PROFILE_FN
#define PROFILE_MFN
#else
#define PROFILE_FN AutoFnProfiler _profilerAuto(__FUNCTION__, NULL);
#define PROFILE_MFN AutoFnProfiler _profilerAuto(__FUNCTION__, this);
#endif


NEPHILIM_NS_END
#endif // Profiler_h__
