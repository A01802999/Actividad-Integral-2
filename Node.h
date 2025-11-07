/*
Descripción: Definición de la clase genérica Node utilizada en la lista ligada.
Equipo: Diego Sánchez Mancilla A01802999, Alexa Ávila Luna A01803526, Valeria Gissele Portilla Robles A01752371  
*/

#ifndef NODE_H
#define NODE_H

// Clase que representa un nodo doblemente ligado
template <typename T>

class Node {
public:
    T data;          // Dato almacenado en el nodo
    Node<T>* next;   // Puntero al siguiente nodo
    Node<T>* prev;   // Puntero al nodo anterior

    // Constructores
    Node() : data{}, next{nullptr}, prev{nullptr} {}        // Nodo vacío
    Node(T data) : data{data}, next{nullptr}, prev{nullptr} {}          // Nodo con dato
    Node(T data, Node<T>* next, Node<T>* prev) : data{data}, next{next}, prev{prev} {}      // Nodo con dato y punteros
};

#endif
