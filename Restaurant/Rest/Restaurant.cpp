#include <cstdlib>
#include <time.h>
#include <iostream>
#include <sstream>
using namespace std;

#include "Restaurant.h"




Restaurant::Restaurant() 
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;

	int n = pGUI->getFileName();  //Assigns the required file name

	switch (n)
	{
	case 0:
		filename = "Sample1.txt";
		break;
	case 1:
		filename = "Sample2.txt";
		break;
	case 2:
		filename = "Sample3.txt";
		break;
	};

	PROG_MODE	mode = pGUI->getGUIMode();
	sim_mode = mode;

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		ModesFunction();
		break;
	case MODE_STEP:
		ModesFunction();
		break;
	case MODE_SLNT:
		ModesFunction();
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime()>CurrentTimeStep )//no more events at current timestep
		{

			return;
		}

		pE->Execute(this);

		numinsameTimeStep++;
		EventsQueue.dequeue(pE);//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//Drawing all cooks
	int count1 = 0;
	Cook** arrC = AllCookQueue.toArray(count1);

	for (int i = 0; i < count1; i++)
	{
		if (arrC[i]->getStatus() == AVAILABLE)
		{
			pGUI->AddToDrawingList(arrC[i]);
		}
		//else if (arrC[i]->getStatus() == INBREAK)
		//{
		//	pGUI->PrintMessage5("InBreak Cooks' IDs: " + to_string(arrC[i]->GetID()) + "");
		//}
	}


	//Drawing Orders
	int count2 = 0;
	Order** arrO = AllOrders.toArray(count2);
	for (int i = 0; i < count2; i++)
	{
		if(arrO[i]->getStatus()==WAIT)
			pGUI->AddToDrawingList(arrO[i]);
	}

	int count3 = 0;
	Order** ServArr = InService.toArray(count3);
	for (int i = 0; i < count3; i++)
	{
		pGUI->AddToDrawingList(ServArr[i]);
	}

	int count4 = 0;
	Order** FinishedArr = Finished.toArray(count4);
	for (int i = 0; i < count4; i++)
	{
		pGUI->AddToDrawingList(FinishedArr[i]);
	}


	pGUI->UpdateInterface();
	pGUI->ResetDrawingList();

}





//////////////////////////////////////////////////////////////////////////////////////////////
/// ==>  Begining of Phase 2 functions

//M.K
//File Loading Function Implementation  
//Note that before calling this function anywhere you should add the following satetment before:
//ifstream inp("Sample1.txt");
void Restaurant::fileLoading(ifstream &inp)
{
	Event* pEv;
	Cook* pCo;
	int N,G,V,SNmin,SNmax,SGmin,SGmax,SVmin,SVmax,BO,BNmin,BNmax,BGmin,BGmax,BVmin,BVmax,AutoP,M,TS,IDE,IDC,IDP,SIZE,MONY,ExMony,RstPrd,VIP_WT;
	char TYP,eventType;
	double InjProp;
	int IDVIP=1,IDN=1,IDVE=1;
	if(inp.is_open())
	{
		//To be changed later to generate random speeds for every cook
		inp>>N>>G>>V; //line 1
		inp>>SNmin>>SNmax>>SGmin>>SGmax>>SVmin>>SVmax;//line 2
		inp>>BO>>BNmin>>BNmax>>BGmin>>BGmax>>BVmin>>BVmax;//line 3
		inp>>InjProp>>RstPrd;//line 4
		for(int i=0;i<N;i++)
		{
			//To be changed later to generate random speeds for every cook
			pCo=new Cook(IDN++,(ORD_TYPE)0,SNmin,BO,BNmin,InjProp,RstPrd);
			getAllCookQueue().enqueue(pCo);
			getNormalCookQueue().enqueue(pCo);

		}
		for(int i=0;i<G;i++)
		{
			//To be changed later to generate random speeds for every cook
			pCo=new Cook(IDVE++,(ORD_TYPE)1,SGmin,BO,BGmin,InjProp,RstPrd);
			getAllCookQueue().enqueue(pCo);
			getVeganCookQueue().enqueue(pCo);

		}
		for(int i=0;i<V;i++)
		{
			//To be changed later to generate random speeds for every cook
			pCo=new Cook(IDVIP++,(ORD_TYPE)2,SVmin,BO,BVmin,InjProp,RstPrd);
			getAllCookQueue().enqueue(pCo);
			getVIPCookQueue().enqueue(pCo);

		}
		inp>>AutoP>>VIP_WT;//line 5
		setAutoP(AutoP);
		setVIP_WT(VIP_WT);
		inp>>M;//line 6
		for(int i=0;i<M;i++)
		{
			inp>>eventType;
			if(eventType=='R')
			{
				inp>>TYP>>TS>>IDE>>SIZE>>MONY;
				switch(TYP)
				{
				case 78:
					pEv = new ArrivalEvent(TS,IDE,(ORD_TYPE)0,SIZE,MONY);
					EventsQueue.enqueue(pEv);
					break;
				case 71:
					pEv = new ArrivalEvent(TS,IDE,(ORD_TYPE)1,SIZE,MONY);
					EventsQueue.enqueue(pEv);
					break;
				case 86:
					pEv = new ArrivalEvent(TS,IDE,(ORD_TYPE)2,SIZE,MONY);
					EventsQueue.enqueue(pEv);
					break;
				}

			}
			else if(eventType=='X')
			{
				inp>>TS>>IDC;
				pEv=new CancellationEvent(TS,IDC);
				EventsQueue.enqueue(pEv);
			}
			else
			{
				inp>>TS>>IDP>>ExMony;
				pEv=new PromotionEvent(TS,IDP,ExMony);
				EventsQueue.enqueue(pEv);
			}
		}

		inp.close();
	}
}
void Restaurant::fileExporting(ofstream &out)
{
	int countN=0;
	int countVeg=0;
	int countVIP=0;
	int count = 0;
	float TotalWtime=0;
	float TotalStime=0;
	float AvgStime;
	float AvgWtime;
	Order** ServArr = Finished.toArray(count);
	Cook* pCo;

	if(out.is_open())
	{
		out<<"FT  "<<"ID  "<<"AT  "<<"WT  "<<"ST  "<<endl;
		for (int i = 0; i < count; i++)
		{

			out<<ServArr[i]->GetFinishTime()<<"   "<<ServArr[i]->GetID()<<"   "<<ServArr[i]->GetArrTime()<<"   "<<ServArr[i]->getWaitTime()<<"   "<<ServArr[i]->GetServTime()<<"   "<<endl;
			TotalWtime+=ServArr[i]->getWaitTime();
			TotalStime+=ServArr[i]->GetServTime();
			if(ServArr[i]->GetType() == TYPE_NRM)
				countN++;
			else if(ServArr[i]->GetType() == TYPE_VGAN)
				countVeg++;
			else countVIP++;

		}
		AvgWtime = floorf((TotalWtime/count) * 100) / 100;
		AvgStime = floorf((TotalStime/count) * 100) / 100;
		out<<"...................."<<endl;
		out<<"Orders:"<<count<<"   "<<"[Norm:"<<countN<<", "<<"Veg:"<<countVeg<<", "<<"VIP:"<<countVIP<<"]"<<endl;
		out<<"Cooks:"<<pCo->getNumAvCook()<<"    "<<"[Norm:"<<pCo->getNumAvNCook()<<", "<<"Veg:"<<pCo->getNumAvVegCook()<<", "<<"VIP:"<<pCo->getNumAvVIPCook()<<", "<<"Injured:"<<"bla bla"<<"]"<<endl;
		out<<"Avg Wait:"<<AvgWtime<<"   "<<"Avg Serv:"<<AvgStime<<endl;
		out<<"Urgent Orders:"<<"bla blah"<<"  "<<"Auto-promoted:"<<"blaaah%"<<endl;
		out.close();
	}

}



/////////////////////////////////////////////////////////////////////////////////
////Required setters, getters, and some functions///

LinkedList<Order*>& Restaurant::getWNormList()
{
	return WaitingNormal;
}
QueueSorted<Order*>& Restaurant::getWVIPList()
{
	return WaitingVIP;
}
Queue<Order*>& Restaurant::getWVeganList()
{
	return WaitingVegan;
}
Queue<Event*>& Restaurant::getEventsQueue()
{
	return EventsQueue;
}
Queue<Cook*>& Restaurant::getAllCookQueue()
{
	return AllCookQueue; 
}
Queue<Cook*>& Restaurant::getVeganCookQueue()
{
	return VeganCookQueue;
}
Queue<Cook*>& Restaurant::getVIPCookQueue()
{
	return VIPCookQueue;
}
Queue<Cook*>& Restaurant::getNormalCookQueue()
{
	return NormalCookQueue;
}

LinkedList<Order*>& Restaurant::getServList()
{
	return InService;
}
LinkedList<Order*>& Restaurant::getFinishList()
{
	return Finished;
}

int Restaurant::getnuminsameTimeStep()
{
	return numinsameTimeStep;
}

void Restaurant::setnuminsameTimeStep(int a)
{
	this->numinsameTimeStep = a;
}
void Restaurant::setAutoP(int au)
{
	AutoP=au;
}
void Restaurant::setVIP_WT(int vi)
{
	VIP_WT=vi;
}

void Restaurant::setCurrentTS(int t)
{
	CurrentTS = t;
}

int Restaurant::getCurrentTS()
{
	return CurrentTS;
}

LinkedList<Order*>& Restaurant::getAllOrders()
{
	return AllOrders;
}
int Restaurant::getAutoP()
{
	return AutoP;
}
int Restaurant::getVIP_WT()
{
	return VIP_WT;
}
//Adds Order to the equivalent Queue.
void Restaurant::AddtoEveryWaitType(Order* ptr) {

	if (ptr->GetType() == TYPE_NRM) {

		WaitingNormal.InsertEnd(ptr);

	}
	else if (ptr->GetType() == TYPE_VIP) {
		//priorty equation
		WaitingVIP.enqueueSorted(ptr, 1);

	}
	else if (ptr->GetType() == TYPE_VGAN) {

		WaitingVegan.enqueue(ptr);


	}

}

void Restaurant::AddtoStatusLists(Order* ptr)
{

	if (ptr->getStatus() == SRV) {

		InService.InsertEnd(ptr);
		//Waiting.DeleteFirst();
	}
	else if (ptr->getStatus() == DONE)
	{

		Finished.InsertEnd(ptr);
		Order* p;
		int c = 0;
		Order** ArrSERV = InService.toArray(c);
		for (int i = 0; i < c; i++)
		{
			if (ArrSERV[i]->GetID() == ptr->GetID())
				InService.DeleteNode(ptr);
		}

	}
}

void Restaurant::AddtoAllOrdersL(Order* Optr)
{
	AllOrders.InsertEnd(Optr);
}

//////////////////////////////////////////////////////////////////////////////////////////////

//Assignment Criteria Function
void Restaurant::Assignment()
{
	Cook* pCo;
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);
	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);

	if (pCo->getNumAvCook() != 0)
	{
		int cVIP = 0;
		Order** VIPOrderArr = WaitingVIP.toArray(cVIP);
		for (int i = 0; i < cVIP; i++)
		{
			if (pCo->getNumAvVIPCook() != 0 /*&& VIPOrderArr[i]->getStatus()==WAIT*/)
			{
				for (int j = 0; j < c1; j++)
				{
					if (arrVIPCook[j]->getStatus() == AVAILABLE)			 //check cook availability
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingVIP.dequeue(pO);
						VIPOrderArr[i]->setStatus(SRV);			//assigning the order and add it to the INService List 
						AddtoStatusLists(VIPOrderArr[i]);
						int duration = ceil((double)VIPOrderArr[i]->GetSize() / (double)arrVIPCook[j]->getSpeed());
						VIPOrderArr[i]->SetFinishTime(duration + CurrentTS);   
						VIPOrderArr[i]->SetServTime(duration);
						VIPOrderArr[i]->SetWaitTime(CurrentTS - VIPOrderArr[i]->GetArrTime());
						//Assign ORD Effect on Cook
						arrVIPCook[j]->setStatus(BUSY);		//Change Cook status to BUSY after assignment
						arrVIPCook[j]->setChange(VIPOrderArr[i]->GetFinishTime());	//The timestep at which the COOK Status will change
						pCo->decNumAvCook();		//Update the stats of Available Cooks
						pCo->decNumAvVIPCook();
						break;
					}
				}

			}
			else if (pCo->getNumAvVIPCook() == 0 && pCo->getNumAvNCook() != 0 /*&& VIPOrderArr[i]->getStatus() == WAIT*/)
			{
				for (int j = 0; j < c2; j++)
				{
					if (arrNCook[j]->getStatus() == AVAILABLE)
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingVIP.dequeue(pO);
						VIPOrderArr[i]->setStatus(SRV);
						AddtoStatusLists(VIPOrderArr[i]);
						int duration = ceil((double)VIPOrderArr[i]->GetSize() / (double)arrNCook[j]->getSpeed());
						VIPOrderArr[i]->SetFinishTime(duration + CurrentTS);
						VIPOrderArr[i]->SetServTime(duration);
						VIPOrderArr[i]->SetWaitTime(CurrentTS - VIPOrderArr[i]->GetArrTime());
						//Assign ORD Effect on Cook
						arrNCook[j]->setStatus(BUSY);
						arrNCook[j]->setChange(VIPOrderArr[i]->GetFinishTime());
						pCo->decNumAvCook();
						pCo->decNumAvNCook();
						break;
					}
				}

			}
			else if (pCo->getNumAvVIPCook() == 0 && pCo->getNumAvNCook() == 0 && pCo->getNumAvVegCook() != 0 /*&& VIPOrderArr[i]->getStatus() == WAIT*/)
			{
				for (int j = 0; j < c3; j++)
				{
					if (arrVegCook[j]->getStatus() == AVAILABLE)
					{
						//Assign Order Effect on Order
						Order* pO;
						WaitingVIP.dequeue(pO);
						VIPOrderArr[i]->setStatus(SRV);
						AddtoStatusLists(VIPOrderArr[i]);
						int duration = ceil((double)VIPOrderArr[i]->GetSize() / (double)arrVegCook[j]->getSpeed());
						VIPOrderArr[i]->SetFinishTime(duration + CurrentTS);
						VIPOrderArr[i]->SetServTime(duration);
						VIPOrderArr[i]->SetWaitTime(CurrentTS - VIPOrderArr[i]->GetArrTime());
						//Assign Order Effect on Cook
						arrVegCook[j]->setStatus(BUSY);
						arrVegCook[j]->setChange(VIPOrderArr[i]->GetFinishTime());
						pCo->decNumAvCook();
						pCo->decNumAvVegCook();
						break;
					}
				}
			}
		}

		int cNorm = 0;
		Order** NOrderArr = WaitingNormal.toArray(cNorm);
		for (int i = 0; i < cNorm; i++)
		{
			if (pCo->getNumAvNCook() != 0 /*&& NOrderArr[i]->getStatus() == WAIT*/)
			{
				for (int j = 0; j < c2; j++)
				{
					if (arrNCook[j]->getStatus() == AVAILABLE)
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingNormal.DeleteFirst(pO);
						NOrderArr[i]->setStatus(SRV);
						AddtoStatusLists(NOrderArr[i]);
						int duration = ceil((double)NOrderArr[i]->GetSize() / (double)arrNCook[j]->getSpeed());
						NOrderArr[i]->SetFinishTime(duration + CurrentTS);
						NOrderArr[i]->SetServTime(duration);
						NOrderArr[i]->SetWaitTime(CurrentTS - NOrderArr[i]->GetArrTime());
						//Assign ORD Effcet on Cook
						arrNCook[j]->setStatus(BUSY);
						arrNCook[j]->setChange(NOrderArr[i]->GetFinishTime());
						pCo->decNumAvCook();
						pCo->decNumAvNCook();
						break;
					}

				}
			}
			else if (pCo->getNumAvNCook() == 0 && pCo->getNumAvVIPCook() != 0 /*&& NOrderArr[i]->getStatus() == WAIT*/)
			{
				for (int j = 0; j < c1; j++)
				{
					if (arrVIPCook[j]->getStatus() == AVAILABLE)
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingNormal.DeleteFirst(pO);
						NOrderArr[i]->setStatus(SRV);
						AddtoStatusLists(NOrderArr[i]);
						int duration = ceil((double)NOrderArr[i]->GetSize() / (double)arrVIPCook[j]->getSpeed());
						NOrderArr[i]->SetFinishTime(duration + CurrentTS);
						NOrderArr[i]->SetServTime(duration);
						NOrderArr[i]->SetWaitTime(CurrentTS - NOrderArr[i]->GetArrTime());
						//Assign ORD Effcet on Cook
						arrVIPCook[j]->setStatus(BUSY);
						arrVIPCook[j]->setChange(NOrderArr[i]->GetFinishTime());
						pCo->decNumAvCook();
						pCo->decNumAvVIPCook();
						break;
					}
				}
			}
			else /*if(NOrderArr[i]->getStatus() == WAIT)*/
			{
				break;
			}
		}

		int cVeg = 0;
		Order** VEGOrderArr = WaitingVegan.toArray(cVeg);
		for (int i = 0; i < cVeg; i++)
		{
			if (pCo->getNumAvVegCook() != 0 /*&& VEGOrderArr[i]->getStatus() == WAIT*/)
			{
				for (int j = 0; j < c3; j++)
				{
					if (arrVegCook[j]->getStatus() == AVAILABLE)
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingVegan.dequeue(pO);
						VEGOrderArr[i]->setStatus(SRV);
						AddtoStatusLists(VEGOrderArr[i]);
						int duration = ceil((double)VEGOrderArr[i]->GetSize() / (double)arrVegCook[j]->getSpeed());
						VEGOrderArr[i]->SetFinishTime(duration + CurrentTS);
						VEGOrderArr[i]->SetServTime(duration);
						VEGOrderArr[i]->SetWaitTime(CurrentTS - VEGOrderArr[i]->GetArrTime());
						//Assign ORD Effcet on Cook
						arrVegCook[j]->setStatus(BUSY);
						arrVegCook[j]->setChange(VEGOrderArr[i]->GetFinishTime());
						pCo->decNumAvCook();
						pCo->decNumAvVegCook();
						break;
					}
				}
			}
			else 
			{
				break;
			}
		}
	}
}
void Restaurant::InjuryHandling() 
{
	//VIP Injuries
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);

	for (int j = 0; j < c1; j++)
	{
		if (arrVIPCook[j]->getStatus() == BUSY && arrVIPCook[j]->GetR() <= arrVIPCook[j]->getInjProp() && arrVIPCook[j]->GetR() != 0)
		{
			arrVIPCook[j]->setStatus(INJURED);
			arrVIPCook[j]->setSpeed(arrVIPCook[j]->getSpeed()/2);


			int ServC = 0;
			Order** ServArr = InService.toArray(ServC);
			for (int i = 0; i < ServC; i++)
			{
				if(ServArr[i]->GetFinishTime() == arrVIPCook[j]->getChange())
				{
					int remainingDishes = ServArr[i]->GetSize() - (arrVIPCook[j]->getSpeed() * 2 * (CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime()));
					int duration = ceil((double)remainingDishes / (double)arrVIPCook[j]->getSpeed());				 //new decreased speed
					ServArr[i]->SetServTime(duration + CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime());
					ServArr[i]->SetFinishTime(duration + CurrentTS);
					arrVIPCook[j]->setChange(ServArr[i]->GetFinishTime());
				}
			}
		}
		//rest period
		if (arrVIPCook[j]->getChange() == CurrentTS && arrVIPCook[j]->getStatus() == INJURED)
		{
			arrVIPCook[j]->setStatus(INREST);
			arrVIPCook[j]->setChange(CurrentTS + arrVIPCook[j]->getRstPrd());
			arrVIPCook[j]->setR(0); //nshelha sa3et l randomization
		}
		//back from rest
		if (arrVIPCook[j]->getChange() == CurrentTS && arrVIPCook[j]->getStatus() == INREST)
		{
			arrVIPCook[j]->setStatus(AVAILABLE);
			arrVIPCook[j]->setSpeed(arrVIPCook[j]->getSpeed() * 2);			//return to the original speed
			arrVIPCook[j]->setChange(0);
			arrVIPCook[j]->incNumAvCook();
			arrVIPCook[j]->incNumAvVIPCook();
		}
	}

	//Normal Injuries
	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	for (int j = 0; j < c2; j++)
	{
		if (arrNCook[j]->getStatus() == BUSY && arrNCook[j]->GetR() <= arrNCook[j]->getInjProp() && arrNCook[j]->GetR() != 0)
		{
			arrNCook[j]->setStatus(INJURED);
			arrNCook[j]->setSpeed(arrNCook[j]->getSpeed() / 2);

			int ServC = 0;
			Order** ServArr = InService.toArray(ServC);
			for (int i = 0; i < ServC; i++)
			{
				if (ServArr[i]->GetFinishTime() == arrNCook[j]->getChange())
				{
					int remainingDishes = ServArr[i]->GetSize() - (arrNCook[j]->getSpeed() * 2 * (CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime()));
					int duration = ceil((double)remainingDishes / (double)arrNCook[j]->getSpeed());				 //new decreased speed
					ServArr[i]->SetServTime(duration + CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime());
					ServArr[i]->SetFinishTime(duration + CurrentTS);
					arrNCook[j]->setChange(ServArr[i]->GetFinishTime());
				}

			}
		}
		//rest period
		if (arrNCook[j]->getChange() == CurrentTS && arrNCook[j]->getStatus() == INJURED)
		{
			arrNCook[j]->setStatus(INREST);
			arrNCook[j]->setChange(CurrentTS + arrNCook[j]->getRstPrd());
			arrNCook[j]->setR(0); //nshelha sa3et l randomization
		}
		//back from rest
		if (arrNCook[j]->getChange() == CurrentTS && arrNCook[j]->getStatus() == INREST)
		{
			arrNCook[j]->setStatus(AVAILABLE);
			arrNCook[j]->setSpeed(arrNCook[j]->getSpeed() * 2);			//return to the original speed
			arrNCook[j]->setChange(0);
			arrNCook[j]->incNumAvCook();
			arrNCook[j]->incNumAvNCook();
		}
	}

	//Vegan Injuries
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);
	for (int j = 0; j < c3; j++)
	{
		if (arrVegCook[j]->getStatus() == BUSY && arrVegCook[j]->GetR() <= arrVegCook[j]->getInjProp() && arrVegCook[j]->GetR() != 0)
		{
			arrVegCook[j]->setStatus(INJURED);
			arrVegCook[j]->setSpeed(arrVegCook[j]->getSpeed() / 2);

			int ServC = 0;
			Order** ServArr = InService.toArray(ServC);
			for (int i = 0; i < ServC; i++)
			{
				if (ServArr[i]->GetFinishTime() == arrVegCook[j]->getChange())
				{
					int remainingDishes = ServArr[i]->GetSize() - (arrVegCook[j]->getSpeed() * 2 * (CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime()));
					int duration = ceil((double)remainingDishes / (double)arrVegCook[j]->getSpeed());				 //new decreased speed
					ServArr[i]->SetServTime(duration + CurrentTS - ServArr[i]->GetArrTime() + ServArr[i]->getWaitTime());
					ServArr[i]->SetFinishTime(duration + CurrentTS);
					arrVegCook[j]->setChange(ServArr[i]->GetFinishTime());
				}
			}
		}
		//rest period
		if (arrVegCook[j]->getChange() == CurrentTS && arrVegCook[j]->getStatus() == INJURED)
		{
			arrVegCook[j]->setStatus(INREST);
			arrVegCook[j]->setChange(CurrentTS + arrVegCook[j]->getRstPrd());
			arrVegCook[j]->setR(0); //nshelha sa3et l randomization
		}
		//back from rest
		if (arrVegCook[j]->getChange() == CurrentTS && arrVegCook[j]->getStatus() == INREST)
		{
			arrVegCook[j]->setStatus(AVAILABLE);
			arrVegCook[j]->setSpeed(arrVegCook[j]->getSpeed() * 2);			//return to the original speed
			arrVegCook[j]->setChange(0);
			arrVegCook[j]->incNumAvCook();
			arrVegCook[j]->incNumAvVegCook();
		}
	}
}

//Urgent Orders
void Restaurant::UrgentOrders()
{
	Cook* pCo;
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);
	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);

	if (pCo->getNumAvCook() == 0)
	{
		int cVIP = 0;
		Order** VIPOrderArr = WaitingVIP.toArray(cVIP);
		for (int i = 0; i < cVIP; i++)
		{
			if (CurrentTS - VIPOrderArr[i]->GetArrTime() > getVIP_WT())
			{
				//VIP Cooks
				for (int j = 0; j < c1; j++)
				{
					if (arrVIPCook[j]->getStatus() == INJURED || arrVIPCook[j]->getStatus() == INREST)
					{
						//Assign ORD Effect on Order
						Order* pO;
						WaitingVIP.dequeue(pO);
						VIPOrderArr[i]->setStatus(SRV);			
						AddtoStatusLists(VIPOrderArr[i]);
						arrVIPCook[j]->setSpeed(arrVIPCook[j]->getSpeed() / 2);      //decrease its Speed
						int duration = ceil((double)VIPOrderArr[i]->GetSize() / (double)arrVIPCook[j]->getSpeed());
						VIPOrderArr[i]->SetFinishTime(duration + CurrentTS);
						VIPOrderArr[i]->SetServTime(duration);
						VIPOrderArr[i]->SetWaitTime(CurrentTS - VIPOrderArr[i]->GetArrTime());
						//Assign ORD Effect on Cook
						arrVIPCook[j]->setStatus(URGENT);		//Change Cook status to BUSY after assignment
						arrVIPCook[j]->setChange(VIPOrderArr[i]->GetFinishTime());	//The timestep at which the COOK Status will change
						break;
					}
				}
			}
		}
	}
	//Return from Urgent
	for (int j = 0; j < c1; j++)
	{
		if (arrVIPCook[j]->getStatus() == URGENT && arrVIPCook[j]->getChange() == CurrentTS)
		{
			arrVIPCook[j]->setStatus(AVAILABLE);
			arrVIPCook[j]->setSpeed(arrVIPCook[j]->getSpeed() * 2);
			arrVIPCook[j]->setChange(0);
			arrVIPCook[j]->incNumAvCook();
			arrVIPCook[j]->incNumAvVIPCook();
		}
	}
}

//Break Handling
void Restaurant::BreakHandling()
{
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);

	for (int j = 0; j < c1; j++)
	{
		if ((arrVIPCook[j]->getnumberofFinishedOrders() % arrVIPCook[j]->getBO()) == 0  && arrVIPCook[j]->getnumberofFinishedOrders()!=0)
		{
			arrVIPCook[j]->setStatus(INBREAK);
			arrVIPCook[j]->setChange(arrVIPCook[j]->getBN() + CurrentTS);
			arrVIPCook[j]->decNumAvCook();
			arrVIPCook[j]->decNumAvVIPCook();

		}

	}

	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	for (int j = 0; j < c2; j++)
	{
		if ((arrNCook[j]->getnumberofFinishedOrders() % arrNCook[j]->getBO()) == 0 && arrNCook[j]->getnumberofFinishedOrders()!=0 && arrNCook[j]->getChange() == 0)
		{
			int duration = arrNCook[j]->getBN();

			arrNCook[j]->setStatus(INBREAK);
			arrNCook[j]->setChange(arrNCook[j]->getBN() + CurrentTS);
			arrNCook[j]->decNumAvCook();
			arrNCook[j]->decNumAvNCook();
		}
	}
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);
	for (int j = 0; j < c3; j++)
	{
		if ((arrVegCook[j]->getnumberofFinishedOrders() % arrVegCook[j]->getBO()) == 0 && arrVegCook[j]->getnumberofFinishedOrders()!=0)
		{
			arrVegCook[j]->setStatus(INBREAK);
			arrVegCook[j]->setChange(arrVegCook[j]->getBN() + CurrentTS);
			arrVegCook[j]->decNumAvCook();
			arrVegCook[j]->decNumAvVegCook();
		}
	}

	//return from Break;
	for (int j = 0; j < c1; j++)
	{
		if (arrVIPCook[j]->getChange() == CurrentTS && arrVIPCook[j]->getStatus() == INBREAK)
		{
			arrVIPCook[j]->setStatus(AVAILABLE);
			arrVIPCook[j]->setChange(0);
			arrVIPCook[j]->incNumAvCook();
			arrVIPCook[j]->incNumAvVIPCook();
		}

	}
	for (int j = 0; j < c2; j++)
	{
		if (arrNCook[j]->getChange() == CurrentTS && arrNCook[j]->getStatus() == INBREAK)
		{
			arrNCook[j]->setStatus(AVAILABLE);
			arrNCook[j]->setChange(0);
			arrNCook[j]->incNumAvCook();
			arrNCook[j]->incNumAvNCook();

		}
	}
	for (int j = 0; j < c3; j++)
	{
		if (arrVegCook[j]->getChange() == CurrentTS && arrVegCook[j]->getStatus() == INBREAK)
		{
			arrVegCook[j]->setStatus(AVAILABLE);
			arrVegCook[j]->setChange(0);
			arrVegCook[j]->incNumAvCook();
			arrVegCook[j]->incNumAvVegCook();
		}
	}

}
void Restaurant::AutoPromotion() {

	int c1 = 0;
	Order** arr1 = getWNormList().toArray(c1);
	for (int i = 0; i < c1; i++)
	{
		if ((CurrentTS - arr1[i]->GetArrTime()) == getAutoP())
		{
			getWVIPList().enqueueSorted(arr1[i], 1);
			getWNormList().DeleteNode(arr1[i]);
		}
	}

}
//Move to Finished list
//Finish Logic
void Restaurant::FinishLogic()
{
	int ServC = 0;
	Order** ServArr = InService.toArray(ServC);
	for (int i = 0; i < ServC; i++)
	{
		if (ServArr[i]->GetFinishTime() == CurrentTS )      //change Order status
		{
			ServArr[i]->setStatus(DONE);
			AddtoStatusLists(ServArr[i]);
		}
	}

	//Change Cooks' Status
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);
	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);
	for (int j = 0; j < c1; j++)
	{
		if (arrVIPCook[j]->getChange() == CurrentTS )
		{
			if (arrVIPCook[j]->getStatus() == BUSY)      //return to the available status
			{
				arrVIPCook[j]->setStatus(AVAILABLE);
				arrVIPCook[j]->setChange(0);
				arrVIPCook[j]->incnumberofFinishedOrders();
				arrVIPCook[j]->incNumAvCook();
				arrVIPCook[j]->incNumAvVIPCook();
			}

		}

	}
	for (int j = 0; j < c2; j++)
	{
		if (arrNCook[j]->getChange() == CurrentTS )
		{
			if (arrNCook[j]->getStatus() == BUSY)
			{
				arrNCook[j]->setStatus(AVAILABLE);
				arrNCook[j]->setChange(0);
				arrNCook[j]->incnumberofFinishedOrders();
				arrNCook[j]->incNumAvCook();
				arrNCook[j]->incNumAvNCook();
			}
		}
	}
	for (int j = 0; j < c3; j++)
	{
		if (arrVegCook[j]->getChange() == CurrentTS)
		{
			if (arrVegCook[j]->getStatus() == BUSY)
			{
				arrVegCook[j]->setStatus(AVAILABLE);
				arrVegCook[j]->setChange(0);
				arrVegCook[j]->incnumberofFinishedOrders();
				arrVegCook[j]->incNumAvCook();
				arrVegCook[j]->incNumAvVegCook();
			}
		}
	}
}

//Information line to be printed
string Restaurant::array()
{
	int count = 0;
	int f = 0;
	Order** arrF = InService.toArray(f);
	for (int j = 0; j < f; j++)
	{
		if ((arrF[j]->getWaitTime() + arrF[j]->GetArrTime()) == CurrentTS)
			count++;
	}

	//Cooks' Queues
	int c1 = 0;
	Cook** arrVIPCook = VIPCookQueue.toArray(c1);
	int c2 = 0;
	Cook** arrNCook = NormalCookQueue.toArray(c2);
	int c3 = 0;
	Cook** arrVegCook = VeganCookQueue.toArray(c3);

	//Data to be filled
	ORD_TYPE OrderType = TYPE_NRM;
	int ORDID = 0;
	ORD_TYPE CookType = TYPE_NRM;
	int COOKID = 0;

	//Array of Orders Finished in the last TS
	Order* ORDarray = new Order[count];
	int index = 0;
	for (int j = 0; j < f; j++)
	{
		if ((arrF[j]->getWaitTime() + arrF[j]->GetArrTime()) == CurrentTS)
		{
			OrderType = arrF[j]->GetType();
			ORDID = arrF[j]->GetID();
			if (OrderType == TYPE_NRM)
			{
				for (int k = 0; k < c2; k++)			//Normal Cooks
				{
					if (arrNCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrNCook[k]->GetType();
						COOKID = arrNCook[k]->GetID();

					}
				}

				for (int k = 0; k < c1; k++)			//VIP Cooks
				{
					if (arrVIPCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrVIPCook[k]->GetType();
						COOKID = arrVIPCook[k]->GetID();
					}
				}
			}
			else if (OrderType == TYPE_VIP)
			{
				for (int k = 0; k < c1; k++)			//VIP Cooks
				{
					if (arrVIPCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrVIPCook[k]->GetType();
						COOKID = arrVIPCook[k]->GetID();
					}
				}

				for (int k = 0; k < c2; k++)			//Normal Cooks
				{
					if (arrNCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrNCook[k]->GetType();
						COOKID = arrNCook[k]->GetID();

					}
				}

				for (int k = 0; k < c3; k++)			//Vegan Cooks
				{
					if (arrVegCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrVegCook[k]->GetType();
						COOKID = arrVegCook[k]->GetID();
					}
				}
			}
			else if (OrderType == TYPE_VGAN)
			{
				for (int k = 0; k < c3; k++)			//Vegan Cooks
				{
					if (arrVegCook[k]->getChange() == arrF[j]->GetFinishTime())
					{
						CookType = arrVegCook[k]->GetType();
						COOKID = arrVegCook[k]->GetID();
					}
				}
			}
			//Create an Order with the collected Data
			Order OR(ORDID, OrderType, CookType, COOKID);
			
			//Add it to the Array of Orders Finished in the last TS
			ORDarray[index] = OR;
			index++;

		}
	}

	//Data to be Printed
	string Otype = "N";
	string Ctype = "N";

	string* array = new string[count];
	for (int i = 0; i < count; i++)
	{
		OrderType = ORDarray[i].GetType();
		if (OrderType == TYPE_NRM)
			Otype = "N";
		else if (OrderType == TYPE_VIP)
			Otype = "V";
		else if (OrderType == TYPE_VGAN)
			Otype = "G";

		ORDID = ORDarray[i].GetID();

		CookType = ORDarray[i].getCtype();
		if (CookType == TYPE_NRM)
			Ctype = "N";
		else if (CookType == TYPE_VIP)
			Ctype = "V";
		else if (CookType == TYPE_VGAN)
			Ctype = "G";

		COOKID = ORDarray[i].getcID();

		string b1 = "(";
		string b2 = ")";
		string s = Ctype + to_string(COOKID) + b1 + Otype + to_string(ORDID) + b2+"	,	";
		
		array[i] = s;
	}

	//convert the array into a single string
	string output = "";
	for (int i = 0; i < count; i++)
	{
		output = output + array[i];
	}

	return output;
}

//Update the Bottom of the Screen
void Restaurant::BottomWindow()
{
	Cook* pCo;
	//Number of waiting orders of each order type
	int cVIP = 0;
	Order** VIPOrderArr = WaitingVIP.toArray(cVIP);
	int cNorm = 0;
	Order** NOrderArr = WaitingNormal.toArray(cNorm);
	int cVeg = 0;
	Order** VEGOrderArr = WaitingVegan.toArray(cVeg);

	//Number of Ordered Served of each Type
	int VIPf = 0;
	int Nf = 0;
	int Vegf = 0;
	int c = 0;
	Order** arrF = Finished.toArray(c);
	for (int i = 0; i < c; i++)
	{
		if (arrF[i]->GetType() == TYPE_NRM)
			Nf++;
		else if (arrF[i]->GetType() == TYPE_VIP)
			VIPf++;
		else
			Vegf++;
	}

	pGUI->PrintMessage2("Waiting VIP Orders:  " + to_string(cVIP) + "                Number of Available VIP Cooks:   " + to_string(pCo->getNumAvVIPCook()) + "                    No. of Served VIP Orders:   "+ to_string(VIPf)+" ");
	pGUI->PrintMessage3("Waiting Normal Orders:  " + to_string(cNorm) + "         Number of Available Normal Cooks:   " + to_string(pCo->getNumAvNCook()) + "              No. of Served Normal Orders:   " + to_string(Nf) + " ");
	pGUI->PrintMessage4("Waiting Vegan Orders:  " + to_string(cVeg) + "           Number of Available Vegan Cooks:   " + to_string(pCo->getNumAvVegCook()) + "               No. of Served Vegan Orders:   " + to_string(Vegf) + " ");

	string s = array();
	pGUI->PrintMessage5("Assigned In TS "+ to_string(CurrentTS)+":  "+s);
}
////////////Simulation function//////////////////

void Restaurant::ModesFunction()
{
	if (sim_mode == 0)
	{
		pGUI->PrintMessage("Interactive Mode:  you can go to the next timestep by mouse click...CLICK to continue ");
		pGUI->waitForClick();  ///First timestep


		//Call file loading function
		ifstream inp(filename);

		fileLoading(inp);

		int CurrentTimeStep = 1;
		Cook* pCo;
		//as long as events queue is not empty yet  and no active orders in the system
		while (!EventsQueue.isEmpty() || !(InService.IsEmpty()) || !(WaitingNormal.IsEmpty()) || !(WaitingVIP.isEmpty()) || !(WaitingVegan.isEmpty()))
		{
			setCurrentTS(CurrentTimeStep);
			//print current timestep
			char timestep[10];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep);

			setnuminsameTimeStep(0);    //update the timestep data member

			ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

			//UrgentOrders();
			BreakHandling();
			Assignment();
			InjuryHandling();
			FinishLogic();
			AutoPromotion();


			BottomWindow();
			FillDrawingList();  //Update the Interface

			pGUI->waitForClick();
			CurrentTimeStep++;	//advance timestep

		}
		ofstream out("OUTPUT.txt");
		fileExporting(out);
		pGUI->PrintMessage("generation done, click to END program");
		pGUI->waitForClick();

	}
	else if (sim_mode == 1)
	{
		pGUI->PrintMessage("Step_by_step Mode:  the program waits for one second then resumes automatically");

		//Call file loading function
		ifstream inp(filename);
		fileLoading(inp);

		int CurrentTimeStep = 1;


		//as long as events queue is not empty yet  and no active orders in the system
		while (!EventsQueue.isEmpty() || !(InService.IsEmpty()))
		{
			setCurrentTS(CurrentTimeStep);
			//print current timestep
			char timestep[10];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep);

			setnuminsameTimeStep(0);    //update the timestep data member

			ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

			BreakHandling();
			Assignment();
			FinishLogic();
			AutoPromotion();


			BottomWindow();
			FillDrawingList();  //Update the Interface


			Sleep(1000);         //wait for 1 second
			CurrentTimeStep++;	//advance timestep

		}
		ofstream out("OUTPUT.txt");
		fileExporting(out);
		pGUI->PrintMessage("generation done, click to END program");
		pGUI->waitForClick();
	}
	else if (sim_mode == 2)
	{
		pGUI->PrintMessage("Silent Mode: The program produces only an output file");

		//Call file loading function
		ifstream inp(filename);
		fileLoading(inp);

		int CurrentTimeStep = 1;

		//as long as events queue is not empty yet  and no active orders in the system
		while (!EventsQueue.isEmpty() || !(InService.IsEmpty()))
		{
			setCurrentTS(CurrentTimeStep);
			//print current timestep
			char timestep[10];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep);

			setnuminsameTimeStep(0);    //update the timestep data member

			ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

			BreakHandling();
			Assignment();
			FinishLogic();
			AutoPromotion();

			Sleep(1000);         //wait for 1 second
			CurrentTimeStep++;	//advance timestep

		}
		ofstream out("OUTPUT.txt");
		fileExporting(out);
		pGUI->PrintMessage("generation done, click to END program");
		pGUI->waitForClick();
	}
}
