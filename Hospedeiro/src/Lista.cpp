/*
   File List.cpp

   Class List stores Nodes (class Node) in a linked list.
   
   This file has the implementation for class List

   Class list is definition for a linked list, having the following operations:

   1. Initialize the list to an empty list.
   2. Free the nodes of a list.
   3. Determine whether a list is empty.
   4. Add a node with a given value into the list following
      the first node with another given value.
   5. Add a node with a given value to the front of the list.
   6. Add a node with a given value to the end of the list.
   7. Delete the first node from the list.

   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica

   Data da criacao: Abril de 2006.
   Data da ultima alteracao: 8 de outubro de 2015.

*/

#include "../include/Lista.h"

List::List() {
   head = 0;
}

List::~List() {
  Node* cursor = head;
  while(head) {
    cursor = cursor->getNext();
    delete head;
    head = cursor;
  }
  head = 0; // Officially empty
}

void List::insertBeforeFirst(Dados dat) {
  head = new Node(dat, head);
}

void List::insertAfterLast(Dados dat) {
  Node* p = head;
  Node* q = head;

  if (head == 0)
     head = new Node(dat, head);
  else {
     while (q != 0) {
        p = q;
        q = p->getNext();
     }
     p->setNext(new Node(dat,0));
  }
}

Dados List::readFirst() {
   return head->getLog(); 
}

Dados List::removeFirst() {
  Dados retval;
  if (head){
     retval = head->getLog();
     Node* oldHead = head;
     head = head->getNext();
     delete oldHead;
  }
  return retval;
}


void List::insertionSort(Dados value) {
  Node* p = head;
  Node* q = head;

  if (head == 0)
  {
     head = new Node(value,head);
	 Dados i;
	 i = head->getLog();
  }
  else
  {
	  Dados pint;
	  Dados auxint;
	  pint = q->getLog();
	  auxint = pint;
	  while ((q != 0) && (auxint.Data_Hora < value.Data_Hora))
	  {
		  p = q;
		  q = p->getNext();
		  if (q != 0)
		  {
			pint = q->getLog();
			auxint = pint;
		  }
	  }
	  if (p == q)
		  head = new Node(value,head);
	  else
			p->setNext(new Node(value,q));
  }
}


Dados List::removeNode(Dados dat) {
  Node* p = head;
  Node* q = head;
  Dados result;

  if (head == 0)
     throw 101;
  else {
	  while ((q != 0) && (q->getLog() != dat)){  // Error!! the addresses will always be different!
		  p = q;
		  q = p->getNext();
	  }
	  if (q != 0) {
		  if (q == head){	// it is the first node
			  result = q->getLog();
			  head = q->getNext();
			  delete q;
		  }
		  else{				// the node is in the middle
			  result = q->getLog();
			  p->setNext(q->getNext());
			  delete q;
		  }
	  }
	  else
		  result = Dados("",ClockCalendar());		// node not found!
  }

  return result;
}
