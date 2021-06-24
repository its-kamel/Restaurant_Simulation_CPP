#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, int money,int size, int ArrTime,int ServTime,int WaitTime)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	totalMoney=money;
	SIZE=size;
	status = WAIT;
	this->ArrTime=ArrTime;
	this->ServTime=ServTime;
	this->WaitTime=WaitTime;
	FinishTime=ArrTime+WaitTime+ServTime;
}

Order::Order()
{
}

Order::Order(int ID, ORD_TYPE r_Type, ORD_TYPE c_Type, int cID)
{
	this->ID = ID;
	type = r_Type;
	this->c_Type = c_Type;
	this->cID = cID;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}
void Order::setType(ORD_TYPE type)
{
	this->type=type;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::SetArrTime(int aTime)
{
	ArrTime = aTime;
}

int Order::GetArrTime()
{
	return ArrTime;
}

void Order::SetServTime(int sTime)
{
	ServTime = sTime;
}

int Order::GetServTime() {
	return ServTime;
}

void Order::SetFinishTime(int fTime)
{
	FinishTime = fTime;
}

int Order::GetFinishTime()
{
	return FinishTime;
}

void Order::SettotalMoney(double mon)
{
	totalMoney = mon;
}

double Order::GettotalMoney()
{
	return totalMoney;
}

void Order::SetWaitTime(int s)
{
	WaitTime = s;
}
int Order::getWaitTime()
{
	return WaitTime;
}
void Order:: SetSize(int siz)
{
	SIZE=siz;
}
int Order:: GetSize()
{
	return SIZE;
}

int Order::getcID()
{
	return cID;
}

ORD_TYPE Order::getCtype()
{
	return c_Type;
}