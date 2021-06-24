#include "Cook.h"

Cook::Cook()
{}
Cook::Cook(int ID,ORD_TYPE type,int speed,int BO,int BN,double InjProp,int RstPrd ,COOK_STATUS status)
{
	NumAvCook++;
	this->ID=ID;
	this->BO=BO;
	this->BN=BN;
	this->InjProp=InjProp;
	this->RstPrd=RstPrd;
	this->type=type;
	if(this->type == TYPE_NRM)
		NumAvNCook++;
	else if(this->type == TYPE_VGAN)
		NumAvVegCook++;
	else NumAvVIPCook++;
	this->speed=speed;
	this->status = status;
	change = 0;
	numberofFinishedOrders=0;
	R = 0; ///bagrb el part beta3yy lesa 
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}
int Cook::getBO() const
{
	return BO;
}
int Cook::getBN() const
{
	return BN;
}
double Cook::getInjProp() const
{
	return InjProp;
}
int Cook::getRstPrd() const
{
	return RstPrd;
}
ORD_TYPE Cook::GetType() const
{
	return type;
}
int Cook::getnumberofFinishedOrders() const
{
	return numberofFinishedOrders;
}

void Cook::setID(int id)
{
	ID = id;
}
void Cook::setBO(int bo)
{
	BO=bo;
}
void Cook::setRstPrd(int rst)
{
	RstPrd=rst;
}
void Cook::setInjProp(double prp)
{
	InjProp=prp;
}
void Cook::setBN(int bn)
{
	BN=bn;
}
void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setStatus(COOK_STATUS status)
{
	this->status = status;
}

void Cook::setChange(int change)
{
	this->change = change;
}

int Cook::getChange()
{
	return change;
}

int Cook::getSpeed() const
{
	return speed;
}
COOK_STATUS Cook::getStatus()
{
	return status;
}
int Cook::getNumAvCook()
{
	return NumAvCook;
}
int Cook::getNumAvNCook()
{
	return NumAvNCook;
}
int Cook::getNumAvVegCook()
{
	return NumAvVegCook;
}
int Cook::getNumAvVIPCook()
{
	return NumAvVIPCook;
}
void Cook::setNumAvCook(int numofavcook)
{
	NumAvCook=numofavcook;
}
void Cook::setNumAvNCook(int numofavNcook)
{
	NumAvNCook=numofavNcook;
}
void Cook::setNumAvVegCook(int numofavVegcook)
{
	NumAvVegCook=numofavVegcook;
}
void Cook::setNumAvVIPCook(int numofavVIPcook)
{
	NumAvVIPCook=numofavVIPcook;
}
int Cook::incNumAvCook()
{ 
	return NumAvCook++;
}
int Cook::incNumAvNCook()
{
	return NumAvNCook++;
}
int Cook::incNumAvVegCook()
{
	return NumAvVegCook++;
}
int Cook::incNumAvVIPCook()
{
	return NumAvVIPCook++;
}
int Cook::decNumAvNCook()
{
	return NumAvNCook--;
}
int Cook::decNumAvVegCook()
{
	return NumAvVegCook--;
}
int Cook::decNumAvVIPCook()
{
	return NumAvVIPCook--;
}
int Cook::decNumAvCook()
{return NumAvCook--;}
int Cook::incnumberofFinishedOrders()
{
	return numberofFinishedOrders++;
}
int Cook::decnumberofFinishedOrders()
{
	return numberofFinishedOrders--;
}
float Cook::GetR()
{
	return R;
}
void Cook::setR(float r)
{
	R = r;
}
void Cook::setSpeed(float s)
{
	speed = s;
}

int Cook::NumAvCook=0;
int Cook::NumAvNCook=0;
int Cook::NumAvVegCook=0;
int Cook::NumAvVIPCook=0;