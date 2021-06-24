
#ifndef __NODE_H_
#define __NODE_H_

template < typename T>
class Node
{
private :
	T item; // A data item
	Node<T>* next; // Pointer to next node
	int PRT; //Priority
public :
	Node();
	Node( const T & r_Item);	//passing by const ref.
	Node( const T & r_Item,int PRT);	//passing by const ref.
	Node( const T & r_Item, Node<T>* nextNodePtr);
	void setItem( const T & r_Item);
	void setPrt( int num);
	int getPrt();
	void setNext(Node<T>* nextNodePtr);
	void setPrtNext(int PRT);
	int getPrtNext();
	T getItem() const ;
	Node<T>* getNext() const ;
}; // end Node


template < typename T>
Node<T>::Node() 
{
	next = nullptr;
} 

template < typename T>
Node<T>::Node( const T& r_Item)
{
	item = r_Item;
	next = nullptr;
} 
template < typename T>
Node<T>::Node( const T& r_Item,int PRT)
{
	item = r_Item;
	this->PRT=PRT;
	next = nullptr;
}

template < typename T>
Node<T>::Node( const T& r_Item, Node<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void Node<T>::setItem( const T& r_Item)
{
	item = r_Item;
} 

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
} 

template < typename T>
T Node<T>::getItem() const
{
	return item;
} 

template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
} 
template < typename T>
void Node<T>:: setPrt( int num)
{ PRT=num; }
template < typename T>
int Node<T>::getPrt()
{
	return PRT;
}
template < typename T>
void Node<T>::setPrtNext(int PRT)
{
	next->setPrt(PRT);
}
template < typename T>
int Node<T>::getPrtNext()
{
	return next->getPrt();
}
#endif