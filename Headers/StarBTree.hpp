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
    //void Eliminar(Type valor); // Elimina el primer elemento con este valor

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
        bool EsRaiz;
        Nodo* Padre;
        //Type claves[grado];
        Type* claves;
        //Nodo* hijo[grado + 1];
        Nodo** hijo;
        
        /*Nodo() : elemNodo(0) {
            for(int i = 0; i <= grado; ++i) {
                hijo[i] = nullptr;
            }
        }*/
        Nodo() : elemNodo(0), claves(nullptr), hijo(nullptr), EsRaiz(false), Padre(nullptr) {}
    } *raiz;

    // Métodos auxiliares privados
    Nodo* CopiarArbol(Nodo* subraiz);
    void Agregar(Type valor, Nodo* subraiz);
    //void Eliminar(Type valor, Nodo* subraiz);
    void Vaciar(Nodo* nodo);
    bool Buscar(Type valor, Nodo* subraiz) const;

    bool EsHoja(Nodo* nodo) const;

    // Métodos para impresión
    void ImprimirAsc(Nodo* nodo) const;
    void ImprimirDes(Nodo* nodo) const;
    
};

#include "../Templates/StarBTree.tpp"

#endif // STARBTREE_HPP_INCLUDED
