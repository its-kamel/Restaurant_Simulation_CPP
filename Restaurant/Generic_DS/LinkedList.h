#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T> *Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:

LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll(); 
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{		
		cout<<"\nprinting list contents:\n\n";
		Node<T> *p = Head;

		while(p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	* 
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T &data)
	{
		Node<T> *R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T> *P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T &data)
	{
		Node<T>*newNodePtr=new Node<T>(data);
		Node<T>*ptr=Head;
		if(Head==nullptr)
		{
			newNodePtr->setNext(Head);
			Head=newNodePtr;
		}
		else
		{
			while(ptr->getNext())
			{
				ptr=ptr->getNext();
			}
			ptr->setNext(newNodePtr);
		}
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key)
	{
		Node<T>*ptr=Head;
		while(ptr)
		{
			if(ptr->getItem()==Key)
				return 1;
			ptr=ptr->getNext();
		}
		return 0;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T &value)
	{
		int count=0;
		Node<T>* ptr=Head;
		while(ptr)
		{
			if(ptr->getItem()==value)
				count++;
			ptr=ptr->getNext();
		}
		return count;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst( T &p)
	{
		Node<T>* ptr=Head;
		if(Head) //incase list is empty
		{
			p=Head->getItem();
			Head=Head->getNext();
			ptr->setNext(nullptr);
			delete ptr;
			ptr=nullptr;
		}
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast()
	{
		Node<T>* curPtr=Head;
		Node<T>*prevPtr;
		if(Head) //incase list is empty
		{
			if(Head->getNext()) 
			{
				while(curPtr->getNext())
				{
					prevPtr=curPtr;
					curPtr=curPtr->getNext();
				}
				prevPtr->setNext(nullptr);
				delete curPtr;
				curPtr=nullptr;
			}
			else{ //incase list has only one element
				Head=Head->getNext();
				delete curPtr;
				curPtr=nullptr;
			}
		}
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T &value)
	{
		Node<T>*curPtr=Head;
		Node<T>*prevPtr;
			while(curPtr) //also checks if list is empty
			{
			if(curPtr->getItem()==value)    
			{
				if(curPtr==Head)    //in case value is in the first place
				{
					Head=Head->getNext();
					curPtr->setNext(nullptr);
					delete curPtr;
					curPtr=Head;
                     return 1;
				}
				else
				{
				prevPtr->setNext(curPtr->getNext());
				curPtr->setNext(nullptr);
				delete curPtr;
				curPtr=prevPtr;
				return 1;
				}
			}
			prevPtr=curPtr;
			curPtr=curPtr->getNext();
			}
		return 0;
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T &value)
		{
			bool found=0;
		Node<T>*curPtr=Head;
		Node<T>*prevPtr;
			while(curPtr) //also checks if list is empty
			{
			if(curPtr->getItem()==value)    
			{
				if(curPtr==Head)    //in case value is in the first place
				{
					Head=Head->getNext();
					curPtr->setNext(nullptr);
					delete curPtr;
					curPtr=Head;
					found= 1;
				}
				else
				{
				prevPtr->setNext(curPtr->getNext());
				curPtr->setNext(nullptr);
				delete curPtr;
				curPtr=prevPtr;
				found=1;
				}
			}
			prevPtr=curPtr;
			curPtr=curPtr->getNext();
			}
		return found;
	}


	bool IsEmpty()
	{
		if (!Head)
			return true;
		return false;

	}



	/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if LinkedList is empty)
returns: The array of T. (nullptr if LinkedList is empty)
*/

	T* toArray(int& count)
	{
		count = 0;

		if (!Head)
			return nullptr;
		//counting the no. of items in the List
		Node<T>* p = Head;
		while (p)
		{
			count++;
			p = p->getNext();
		}

		T* Arr = new T[count];
		p = Head;

		for (int i = 0; i < count; i++)
		{
			Arr[i] = p->getItem();
			p = p->getNext();
		}
		return Arr;
	}

};

#endif	
