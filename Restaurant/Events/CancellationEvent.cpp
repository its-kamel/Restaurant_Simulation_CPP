#include "CancellationEvent.h"
#include"../Rest/Restaurant.h"

CancellationEvent::CancellationEvent(int eTime, int oID): Event(eTime,oID)
{

}

void CancellationEvent::Execute(Restaurant* pRest)
{
	int c;
	Order ** arr=pRest->getWNormList().toArray(c);
	for (int i = 0; i < c; i++)
	{
		if(arr[i]->GetID() == getOrderID())
		{
			pRest->getWNormList().DeleteNode(arr[i]);
		}
	}
	int count2;
	Order** arrO = pRest->getAllOrders().toArray(count2);

	for (int i = 1; i < count2; i++)
	{
		if(arrO[i]->GetID()== getOrderID())
		{
			if(arrO[i]->getStatus() != SRV && arrO[i]->getStatus() != DONE)
			{
			pRest->getAllOrders().DeleteNode(arrO[i]);
			}
		}
	}
	



}