/*
Descripción: Implementación de una lista ligada ordenada (Sorted Linked List)
que inserta los elementos automáticamente en orden según el operador <.
Equipo: Diego Sánchez Mancilla A01802999, Alexa Ávila Luna A01803526, Valeria Gissele Portilla Robles A01752371
*/

#ifndef SORTEDLINKEDLIST_H_INCLUDED
#define SORTEDLINKEDLIST_H_INCLUDED

#include <iostream>
#include "Node.h"

using std::cout;

// Declaración anticipada del iterador
template <typename T>
class ListIterator;

template <typename T>
class SortedLinkedList {
private:
    int listSize;          // Tamaño de la lista
    Node<T>* head;         // Nodo cabeza (sentinela)
    Node<T>* bottom;       // Nodo final (sentinela)

    friend class ListIterator<T>;       // Permite acceso al iterador

public:
    typedef ListIterator<T> iterator;      // Tipo de iterador

    // Constructor
    SortedLinkedList() : listSize{0}, head{new Node<T>{}}, bottom{new Node<T>{}} {              //Crea una lista de inicio vacía
        head->next = bottom;       //Conecta el inicio con el final
        bottom->prev = head;        //Conecta el final con el inicio
    }

    // Imprime todos los elementos que hay en la lista
    void print() {
        Node<T>* i = head->next;
        while (i != bottom) {
            cout << i->data << "\n";
            i = i->next;
        }
    }

    // Inserta un elemento en la posición ordenada
    void add(T &element) {
        Node<T>* nuevo = new Node<T>(element);
        Node<T>* actual = head->next;

        // Buscar la posición correcta (mantiene orden ascendente)
        while (actual != bottom && !(element < actual->data)) {
            actual = actual->next;
        }

        nuevo->next = actual;
        nuevo->prev = actual->prev;
        actual->prev->next = nuevo;
        actual->prev = nuevo;
        ++listSize;
    }

    // Elimina el primer elemento de la lista
    T remove_first() {
        Node<T>* r = head->next;
        head->next = r->next;
        r->next->prev = head;
        --listSize;
        T removed{r->data};
        delete r;
        return removed;
    }

    // Busca el primer elemento que cumpla con un predicado
    template <typename F>
    iterator find(F &fun) {
        Node<T>* actual = head->next;
        while (actual != bottom) {
            if (fun(actual->data)) {
                return iterator{actual};
            }
            actual = actual->next;
        }
        return end();
    }

    // Limpia la lista completamente
    void clear() {
        while (listSize > 0) {
            remove_first();
        }
    }

    // Retorna el tamaño actual de la lista
    int get_size() { return listSize; }

    // Verifica si la lista está vacía
    bool is_empty() { return listSize == 0; }

    // Retorna un iterador al primer elemento
    iterator begin() { return iterator{head->next}; }

    // Retorna un iterador al final (después del último)
    iterator end() { return iterator{bottom}; }

    // Destructor
    ~SortedLinkedList() {
        clear();
        delete head;
        delete bottom;
    }
};


// Clase iteradora para recorrer la lista ligada
template <typename T>
class ListIterator {
private:
    Node<T>* current;   // Puntero al nodo actual

public:
    // Constructor
    ListIterator(Node<T>* i) : current{i} {}

    // Comparaciones de igualdad
    bool operator==(const ListIterator<T>& rhs) const { return current == rhs.current; }
    bool operator!=(const ListIterator<T>& rhs) const { return current != rhs.current; }

    // Operador de acceso
    T& operator*() { return current->data; }

    // Avanza el iterador al siguiente nodo
    ListIterator& operator++() {
        current = current->next;
        return *this;
    }

    // Avanza y retorna el iterador previo
    ListIterator operator++(int) {
        ListIterator<T> clone(this->current);
        current = current->next;
        return clone;
    }
};

#endif
