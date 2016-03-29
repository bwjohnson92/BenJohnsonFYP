#ifndef _LINKLIST_H
#define _LINKLIST_H

template <class T>
class Node{
public:
	T* data;
	Node<T>* next;
	Node<T>* previous;
	Node(T* newData){ data = newData; next = 0; previous = 0; }

	~Node(){ /*delete data;*/ }
};

template <class T>
class linkedList{
private:
	Node<T> *head;
	int listLength;
public:
	linkedList(){ head = 0; listLength = 0; }

	Node<T>* getHead() const
	{ 
		if(head)
			return head;
		else
			return 0;
	}

	T* getHeadData()
	{
		return head->data;
	}

	Node<T>* getTail()
	{
		return head->previous; 
	}

	T* getTailData()
	{
		return head->previous->data;
	}

	void addData(T& newData, bool newHead)
	{
		Node<T>* listEntry = new Node<T>(&newData);
		if(listLength == 0)
		{
			//add data to list, point to itself to ensure circularity
			listEntry->next = listEntry;
			listEntry->previous = listEntry;
			head = listEntry;

			//Increment size of list
			listLength++;
		}
		else
		{
			//set up tail of list
			head->previous->next = listEntry;
			listEntry->previous = head->previous;

			//set up head of list
			head->previous = listEntry;
			listEntry->next = head;

			//if addToHead function called, make new data into head
			if(newHead)
			{ 
				head = listEntry; 
			}

			//Increment size of list
			listLength++;
		}
	}

	void addToTail(T* newData)
	{
		addData(*newData, false);
	}

	void addToHead(T* newData)
	{
		addData(*newData, true);
	}

	T* getAtPosition(int position)
	{
		if(position > listLength || position < 0)
			return NULL;

		Node<T>* temp = head;

		for (int i=0; i<position;i++)
		{
			temp = temp->next;
		}
		return temp->data;
	}

	bool removeAtPosition(int position)
	{
		if(position > listLength || position < 0)
			return false;
		Node<T>* temp = head;
			
		for (int i=0; i<position;i++)
		{
			temp = temp->next;
		}
		temp->previous->next = temp->next;
		temp->next->previous = temp->previous;

		if(position == 0)
			head = head->next;
		delete temp;
		listLength--;
		return true;
	}

	void removeAtHead()
	{
		head->next->previous = head->previous;
		head->previous->next = head->next;
		Node<T>* temp = head;
		head = head->next;
		delete temp;
		listLength--;
	}

	void removeAtTail()
	{
		Node<T>* temp = head->previous;

		temp->previous->next = head;
		head->previous = temp->previous;
		delete temp;
		listLength--;
	}

	int listSize()
	{
		return listLength;
	}

	~linkedList()
	{
		while(head != 0)
		{
			Node<T>* temp = head;
			head = head->next;
			delete temp;
		}
	}
};

#endif