#ifndef __QUEUESORTED_H_
#define __QUEUESORTED_H_

/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers, 
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

The Node: item of type T and a "next" pointer
------------- 
| item| next | --->
-------------
General Queue case:

frontPtr																backPtr
\											   						/		
\											  					   /		
------------- 	  ------------- 	  ------------- 	  ------------- 	  	  
| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
------------- 	  ------------- 	  ------------- 	  -------------	  

Empty Case:

frontptr	 backptr
\	 /				
\	/				
---- NULL ------


Single Node Case:
frontPtr	 backPtr
\		/	
\	   /			
-------- 	
|	|nxt -->NULL
--------	

*/

#include "Node.h"

template <typename T>
class QueueSorted
{
private :

	Node<T>* backPtr;
	Node<T>* frontPtr;
public :
	QueueSorted();	
	bool isEmpty() const ;
	bool enqueueSorted(const T& newEntry,int PRT);
	bool dequeue(T& frntEntry);  
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array if items)
	~QueueSorted();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
QueueSorted<T>::QueueSorted()
{
	backPtr=nullptr;
	frontPtr=nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool QueueSorted<T>::isEmpty() const
{
	if(frontPtr==nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool QueueSorted<T>::enqueueSorted( const T& newEntry,int PRT)
{



	Node<T>* newNodePtr = new Node<T>(newEntry,PRT);
	// Insert the new node
	if (isEmpty() || PRT <frontPtr->getPrt())
	{
		newNodePtr->setNext(frontPtr);
		frontPtr=newNodePtr;
		if(frontPtr->getNext()==nullptr)
		{
			backPtr=frontPtr;
		}
		// The queue is empty
	}
	else
	{
		//Node<T>* nxtPtr=curPtr->getNext();
		Node<T>* curPtr=frontPtr;
		
		if(curPtr->getNext())
		{
			while(PRT >= curPtr->getNext()->getPrt()  && curPtr->getNext())
			{
				curPtr=curPtr->getNext();
				if(curPtr->getNext()==nullptr)
					break;
			}
		}
		
		newNodePtr->setNext(curPtr->getNext());
		curPtr->setNext(newNodePtr);
		while(curPtr->getNext())
		{
			curPtr=curPtr->getNext();
		}
		backPtr = curPtr;
	}

	// New node is at back

	return true ;
} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool QueueSorted<T>:: dequeue(T& frntEntry)  
{
	if(isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr ;	

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;


	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool QueueSorted<T>:: peekFront(T& frntEntry) const 
{
	if(isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
QueueSorted<T>::~QueueSorted()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T" 
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/

template <typename T>
T* QueueSorted<T>::toArray(int& count)
{
	count=0;

	if(!frontPtr)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = frontPtr;
	while(p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr= new T[count];
	p = frontPtr;
	for(int i=0; i<count;i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

#endif
