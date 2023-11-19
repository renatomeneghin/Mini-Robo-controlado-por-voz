#include "../include/Node.h"

// @brief Constructor - initializes the node
Node::Node(Dados Novo_Log, Node* nxt){
     this->Log = Novo_Log;
     next = nxt;
}

// @brief getVal returns the integer value stored in the node
Dados Node::getLog(){return Log;}

// @brief getNext returns a pointer for the next node in the linked list
Node* Node::getNext(){return next;}

// @brief setVal stores the integer value in the node
void Node::setLog(Dados NLog){this->Log = NLog;}

// @brief setNext stores the pointer to the next node in the list in the "next" field
void Node::setNext(Node* nxt){next = nxt;}