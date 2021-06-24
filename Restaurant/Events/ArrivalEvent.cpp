#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"



ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int OSize,double oMoney) : Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = oMoney;
	OrdSize=OSize;

}




void ArrivalEvent::Execute(Restaurant* pRest)
{
	
	//TYPE_NRM,	//normal order
	//TYPE_VGAN,	//vegan
	//TYPE_VIP,	//VIP

	//LOOP
	//create an Order
	Order* pOrd = new Order(OrderID,OrdType,OrdMoney,OrdSize,getEventTime());
	//pRest->AddtoStatusLists(pOrd);         //adds the order to waiting list
	//pRest->getWaitList().InsertBeg(pOrd);
	//pRest->AddtoStatusLists(pOrd); //adds the order to the Inservice list
	
	///////////Here in Phase 2: Cooks' assignment and availability check

	////Adding the order to its equivalent queue
	pRest->AddtoEveryWaitType(pOrd);	//adds the order to the equivalent according to its type (normal, vegan, VIP)
	pRest->AddtoAllOrdersL(pOrd);       //adds the order to the AllOrders list

}
