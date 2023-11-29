/*
   File Fila.h

   Header file for class list

   Class Fila stores Nodes (class Node) in a linked list.

   This file has the Fila's interface (header file).

   Class list is definition for a linked list, having the following operations:

*/
#pragma once
#include "Node.h"

class Fila {
  Node *head;

public:
  Fila();
  Fila(Dados);
  ~Fila();
  void insert(Dados dat);
  Dados read();
  Dados remove();
};