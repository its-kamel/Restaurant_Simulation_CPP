#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\QueueSorted.h"//M.K

#include "..\Generic_DS\LinkedList.h"
#include "..\Events\Event.h"
#include <fstream> //To input data-M.K


#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"

#include "Order.h"
#include "Cook.h"
#include <math.h>
// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	// TODO: Add More Data Members As Needed
	//
	int CurrentTS;
	int numinsameTimeStep;
	string filename;
	PROG_MODE sim_mode;     //simulation mode indicator

	//Queue for each equivalent Event
	Queue<ArrivalEvent*> ArrivalQueue;
	Queue<CancellationEvent*> CancelQueue;
	Queue<PromotionEvent*> PromoQueue;



	//Equivalent waiting type
	LinkedList<Order*> WaitingNormal;
	Queue<Order*> WaitingVegan;
	QueueSorted<Order*> WaitingVIP;

	//Equivalent Cook type
	Queue<Cook*> VeganCookQueue;
	Queue<Cook*> VIPCookQueue;
	Queue<Cook*> NormalCookQueue;
	//Status Lists
	LinkedList<Order*> InService;
	LinkedList<Order*> Finished;

	//AllTypes' Lists
	LinkedList<Order*> AllOrders;

	//AllCooks' Lists
	Queue<Cook*> AllCookQueue;
	//VIP Timesteps
	int AutoP;
	int VIP_WT;
public:
	Restaurant();
	~Restaurant();
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//

/// ===================    Simple Simulator functions for Phase 1  =================

	void fileLoading(ifstream& in);
	void fileExporting(ofstream &out);
	void Phase1_sim();

	//Simualtion Functions
	void ModesFunction();

	//Assigning
	void Assignment();
	//Injuries 
	void InjuryHandling();
	//finishing
	void FinishLogic();
	//Break Handling
	void BreakHandling();
	//Auto-Promotion
	void AutoPromotion();
	//Urgent Orders
	void UrgentOrders();

	//Update the Bottom of the Screen
	void BottomWindow();
	string array();

	void AddtoEveryWaitType(Order* ptr);  //Adds orders to phase1 queue


	void AddtoAllOrdersL(Order* Optr);   //Adds to all Orders List

	void setnuminsameTimeStep(int );
	void setAutoP(int );
	void setVIP_WT(int);

	void AddtoStatusLists(Order* ptr);     //Adds to the equivalent status list

	//getters
	Queue<Cook*>& getAllCookQueue();
	Queue<Cook*>& getVeganCookQueue();
	Queue<Cook*>& getVIPCookQueue();
	Queue<Cook*>& getNormalCookQueue();
	Queue<Event*>& getEventsQueue();
	LinkedList<Order*>& getWNormList();
	LinkedList<Order*>& getAllOrders();
	QueueSorted<Order*>& getWVIPList();
	Queue<Order*>& getWVeganList();
	LinkedList<Order*>& getServList();
	LinkedList<Order*>& getFinishList();
	int getnuminsameTimeStep();
	int getAutoP();
	int getVIP_WT();

	void setCurrentTS(int);
	int getCurrentTS();
/// ================================================================================================== 

};

#endif