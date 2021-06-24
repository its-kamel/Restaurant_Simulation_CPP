#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 
	int SIZE;               
	double totalMoney;	//Total order money

	int ArrTime, ServTime, WaitTime,FinishTime;	//arrival, service start, and finish times
	
	
	//
	// TODO: Add More Data Members As Needed
	//for Interface Purposes
	ORD_TYPE c_Type;
	int cID;

public:
	Order(int ID, ORD_TYPE r_Type,int mony,int size,int ArrTime,int ServTime=5,int WaitTime=0);
	Order();
	Order(int ID, ORD_TYPE r_Type, ORD_TYPE c_Type, int cID);
	virtual ~Order();


	int GetID();

	ORD_TYPE GetType() const;
	void setType(ORD_TYPE);
	void SetDistance(int d);
	int GetDistance() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	//
	// TODO: Add More Member Functions As Needed
	//

	void SetArrTime(int aTime);
	int GetArrTime();

	void SetServTime(int sTime);
	int GetServTime();

	void SetFinishTime(int fTime);
	int GetFinishTime();

	void SettotalMoney(double mon);
	double GettotalMoney();

	void SetSize(int siz);
	int GetSize();

	void SetWaitTime(int s);
	int getWaitTime();

	int getcID();
	ORD_TYPE getCtype();

};

#endif