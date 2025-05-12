/**
 * @file StarBTree.hpp
 * @brief Implementación de un Árbol B genérico con operaciones básicas.
 * @tparam Type Tipo de datos almacenados.
 * @tparam grado Grado máximo del árbol (máximo de claves por nodo).
 */

/**
 * @brief Constructor por defecto del Árbol B.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::StarBTree() : cantElem(0), raiz(nullptr) {}

/**
 * @brief Constructor por copia.
 * @param c Árbol B a copiar.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::StarBTree(const StarBTree &c) : raiz(CopiarArbol(c.raiz)), cantElem(c.cantElem) {}

/**
 * @brief Operador de asignación.
 * @param c Árbol B a asignar.
 * @return Referencia al árbol actual.
 */
template <typename Type, int grado>
StarBTree<Type, grado>& StarBTree<Type, grado>::operator=(const StarBTree &c) {
    if(this != &c) {
        Vaciar();
        raiz = CopiarArbol(c.raiz);
        cantElem = c.cantElem;
    }
    return *this;
}

/**
 * @brief Destructor del Árbol B.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::~StarBTree() {
    Vaciar();
}

/**
 * @brief Copia un subárbol.
 * @param subraiz Puntero al nodo raíz del subárbol a copiar.
 * @return Puntero al nuevo subárbol copiado.
 */
template <typename Type, int grado>
typename StarBTree<Type, grado>::Nodo* StarBTree<Type, grado>::CopiarArbol(Nodo* subraiz) {
    if(subraiz == nullptr) return nullptr;
    
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->elemNodo = subraiz->elemNodo;
    
    for(int i = 0; i < subraiz->elemNodo; ++i) {
        nuevoNodo->claves[i] = subraiz->claves[i];
    }
    
    for(int i = 0; i <= subraiz->elemNodo; ++i) {
        nuevoNodo->hijo[i] = CopiarArbol(subraiz->hijo[i]);
    }
    
    return nuevoNodo;
}

