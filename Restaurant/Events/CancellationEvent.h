#ifndef  __CANCELLATION_EVENT_
#define   __CANCELLATION_EVENT_

#include "Event.h"

//class for the cancellation event
class CancellationEvent: public Event
{
	
public:
	CancellationEvent(int eTime, int oID);

	virtual void Execute(Restaurant* pRest);
};

#endif 

