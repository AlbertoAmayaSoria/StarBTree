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
        bool esRaiz;
        Type* claves;
        Nodo** hijo;
    
        Nodo(bool esRaiz = false) : elemNodo(0), esRaiz(esRaiz){
            int capacidad = esRaiz ? grado * 2 : grado;
            claves = new Type[capacidad];
            hijo = new Nodo*[capacidad + 1];
            for(int i = 0; i <= capacidad; ++i) {
                hijo[i] = nullptr;
            }

        ~Nodo() {
        delete[] claves;
        delete[] hijo;

        }
        /*Type claves[grado];
        Nodo* hijo[grado + 1];
        
        Nodo() : elemNodo(0) {
            for(int i = 0; i <= grado; ++i) {
                hijo[i] = nullptr;
            }
        }*/
    } *raiz;

    // Métodos auxiliares privados
    Nodo* CopiarArbol(Nodo* subraiz);
    void Agregar(Type valor, Nodo* subraiz);
    void Eliminar(Type valor, Nodo* subraiz);
    void Vaciar(Nodo* nodo);

    // Complementos para Agregar y Eliminar
    bool EsHoja(Nodo* nodo) const;
    void OrdenarNodo(Nodo* subraiz, int indiceHijo);
    void Redistribuir(Nodo* subraiz, int indiceHijo);
    void DividirTriple(Nodo* subraiz, int indiceHijo);

    // Métodos para impresión
    void ImprimirAsc(Nodo* nodo) const;
    void ImprimirDes(Nodo* nodo) const;
    
};

#include "../Templates/StarBTree.tpp"

#endif // STARBTREE_HPP_INCLUDED
