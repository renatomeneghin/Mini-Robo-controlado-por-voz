/*
   File Node.cpp

   Class Node

   Class Node stores integer values for a linked list.
   
   This file has the implementation for the Node's interface.

   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica

   Data da criacao: Abril de 2006.
   Data da ultima alteracao: 8 de outubro de 2015.

*/

#include "Node.h"

// Constructor - initializes the node
//
Node::Node(Dados Novo_Log, Node* nxt){
       this->Log = Novo_Log;
       next = nxt;
}

// getVal returns the integer value stored in the node
//
Dados Node::getLog(){
     return Log;
}

// getNext returns a pointer for the next node in the linked list
//
Node* Node::getNext(){
     return next;
}

// setVal stores the integer value in the node
//
void Node::setLog(Dados NLog){
     this->Log = NLog;
}

// setNext stores the pointer to the next node in the list in the "next" field
//
void Node::setNext(Node* nxt){
       next = nxt;
}