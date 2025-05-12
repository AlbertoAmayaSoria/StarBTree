#ifndef STARBTREE_HPP_INCLUDED
#define STARBTREE_HPP_INCLUDED

template <typename Type, int grado>
class StarBTree {
public:
    explicit StarBTree(); // Constructor por defecto
    StarBTree(const StarBTree &c); // Constructor de copia
    ~StarBTree(); // Destructor
    StarBTree& operator=(const StarBTree &c); // Operador asignación

    void Agregar(Type valor); // Agrega un nuevo elemento
    void Eliminar(Type valor); // Elimina el primer elemento con este valor

    bool Buscar(Type valor) const; // Busca un elemento en el árbol
    int CantElem() const; // Devuelve la cantidad de elementos actuales

    void Vaciar(); // Vacía el árbol

    // Métodos de impresión
    void ImprimirAsc() const;
    void ImprimirDes() const;
    void ImprimirNiveles() const;

private:
    int cantElem;

    struct Nodo {
        int elemNodo;
        Type claves[grado];
        Nodo* hijo[grado + 1];
        
        Nodo() : elemNodo(0) {
            for(int i = 0; i <= grado; ++i) {
                hijo[i] = nullptr;
            }
        }
    } *raiz;

    // Métodos auxiliares privados
    Nodo* CopiarArbol(Nodo* subraiz);
    void Agregar(Type valor, Nodo* subraiz);


    // Métodos para impresión
    void ImprimirAsc(Nodo* nodo) const;
    void ImprimirDes(Nodo* nodo) const;
    
};

#include "../Templates/StarBTree.tpp"

#endif // StarBTree_HPP_INCLUDED
