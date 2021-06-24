#include "PromotionEvent.h"
#include"../Rest/Restaurant.h"
PromotionEvent::PromotionEvent(int eTime, int oID, double mon):Event(eTime,oID)
{
	ExMony = mon;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	int c1=0;
	Order** arr1 = pRest->getWNormList().toArray(c1);
	for (int i = 0; i < c1; i++)
	{
		if (arr1[i]->GetID() == getOrderID())
		{
			pRest->getWVIPList().enqueueSorted(arr1[i], 1);
			arr1[i]->SettotalMoney(arr1[i]->GettotalMoney() + ExMony);
			pRest->getWNormList().DeleteNode(arr1[i]);
		}
	}
}
