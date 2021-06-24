#ifndef __PROMOTIONEVENT_H_
#define __PROMOTIONEVENT_H_ 

#include "Event.h"

class PromotionEvent :public Event
{
	double ExMony;
public:

	PromotionEvent(int eTime, int oID, double mon);

	virtual void Execute(Restaurant* pRest);
};

#endif 