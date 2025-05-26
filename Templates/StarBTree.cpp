#include <iostream>
#include <queue>
#include <cmath>
#include <stdexcept>
#include "../Headers/StarBTree.hpp"

/**
 * @file StarBTree.tpp
 * @brief Implementación de las operaciones de un Árbol B* genérico (StarBTree).
 * @details Soporta inserción con redistribución, división triple y lógica especial para la raíz.
 * @tparam Type Tipo de los elementos almacenados en el árbol.
 * @tparam grado Grado del árbol (número máximo de claves por nodo excepto la raíz).
 */

/**
 * @brief Constructor por defecto del Árbol B*.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::StarBTree() : cantElem(0), raiz(nullptr) {}



/**
 * @brief Constructor por copia.
 * @param c Árbol B* a copiar.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::StarBTree(const StarBTree &c) : raiz(CopiarArbol(c.raiz)), cantElem(c.cantElem) {}

/**
 * @brief Operador de asignación por copia.
 * @param c Árbol B* a asignar.
 * @return Referencia al objeto actual.
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
 * @brief Destructor del Árbol B*.
 */
template <typename Type, int grado>
StarBTree<Type, grado>::~StarBTree() {
    Vaciar(raiz);
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


/**
 *
 *
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Agregar(Type valor) {
    if(raiz == nullptr){
        raiz = new Nodo();
        raiz->EsRaiz = true;
        //delete raiz->claves;
        //delete raiz->hijo;
        
        raiz->claves = new Type[grado * 2];
        raiz->hijo = new Nodo*[grado * 2 + 1];
        
        for(int i = 0 ; i < grado * 2 + 1 ; ++i){
            raiz->hijo[i] = nullptr;
        }
    
        //raiz->claves[0] = valor;
        //std::cout << "Insertando el valor: " << valor << std::endl;
    }

    Agregar(valor, raiz);
}

/**
 *
 *
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Agregar(Type valor, Nodo* subraiz) {
    if(EsHoja(subraiz)) {
        //Implementar el ordenar mientras se agrega
        subraiz->claves[subraiz->elemNodo] = valor;
        subraiz->elemNodo++;
    }
}















//****************************************************************************


/**
 * @brief Verifica si un nodo es hoja.
 * 
 * Un nodo es hoja si todos sus hijos son nulos.
 * 
 * @tparam Type Tipo de dato almacenado en el árbol.
 * @tparam grado Grado del árbol B*.
 * @param nodo Nodo a verificar.
 * @return true si es hoja, false en caso contrario.
 */
template <typename Type, int grado>
bool StarBTree<Type, grado>::EsHoja(Nodo* nodo) const {
    if(nodo == nullptr) return true;
    for(int i = 0; i <= nodo->elemNodo; ++i) {
        if(nodo->hijo[i] != nullptr) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Busca un valor en el árbol.
 * 
 * @tparam Type Tipo de dato almacenado en el árbol.
 * @tparam grado Grado del árbol B*.
 * @param valor Valor a buscar.
 * @return true si el valor se encuentra en el árbol, false en caso contrario.
 */
template <typename Type, int grado>
bool StarBTree<Type, grado>::Buscar(Type valor) const {
    return Buscar(valor, raiz);
}

/**
 * @brief Función auxiliar recursiva para buscar un valor.
 * 
 * @param valor Valor a buscar.
 * @param subraiz Subárbol en el que se realiza la búsqueda.
 * @return true si el valor se encuentra, false en caso contrario.
 */
template <typename Type, int grado>
bool StarBTree<Type, grado>::Buscar(Type valor, Nodo* subraiz) const {
    if(subraiz == nullptr) return false;
    
    int i = 0;
    while(i < subraiz->elemNodo && valor > subraiz->claves[i]) {
        ++i;
    }
    
    if(i < subraiz->elemNodo && valor == subraiz->claves[i]) {
        return true;
    }
    
    return Buscar(valor, subraiz->hijo[i]);
}

/**
 * @brief Elimina todos los elementos del árbol.
 * 
 * Libera toda la memoria dinámica y reinicia el árbol.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar() {
    Vaciar(raiz);
    raiz = nullptr;  ///< Importante resetear la raíz
    cantElem = 0;    ///< Resetear el contador de elementos
}

/**
 * @brief Elimina todos los nodos del subárbol de forma recursiva.
 * 
 * @param nodo Nodo raíz del subárbol a eliminar.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar(Nodo* nodo) {
    if (nodo == nullptr) return;

    if (!EsHoja(nodo)) {
        for (int i = 0; i <= nodo->elemNodo; ++i) {
            Vaciar(nodo->hijo[i]);
            nodo->hijo[i] = nullptr;
        }
    }

    delete[] nodo->claves;
    delete[] nodo->hijo;
    delete nodo;
}

/**
 * @brief Imprime los elementos del árbol en orden ascendente.
 * 
 * Utiliza recorrido en orden (in-order).
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirAsc() const {
    ImprimirAsc(raiz);
    std::cout << std::endl;
}

/**
 * @brief Función auxiliar recursiva para imprimir en orden ascendente.
 * 
 * @param nodo Nodo desde donde se inicia la impresión.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirAsc(Nodo* nodo) const {
    if(nodo == nullptr) return;
    
    for(int i = 0; i < nodo->elemNodo; ++i) {
        ImprimirAsc(nodo->hijo[i]);
        std::cout << nodo->claves[i] << " ";
    }
    ImprimirAsc(nodo->hijo[nodo->elemNodo]);
}

/**
 * @brief Imprime los elementos del árbol en orden descendente.
 * 
 * Utiliza recorrido inverso (reverse in-order).
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirDes() const {
    ImprimirDes(raiz);
    std::cout << std::endl;
}

/**
 * @brief Función auxiliar recursiva para imprimir en orden descendente.
 * 
 * @param nodo Nodo desde donde se inicia la impresión.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirDes(Nodo* nodo) const {
    if(nodo == nullptr) return;
    
    ImprimirDes(nodo->hijo[nodo->elemNodo]);
    for(int i = nodo->elemNodo-1; i >= 0; --i) {
        std::cout << nodo->claves[i] << " ";
        ImprimirDes(nodo->hijo[i]);
    }
}

/**
 * @brief Imprime el árbol por niveles (recorrido por amplitud).
 * 
 * Imprime cada nivel del árbol en una línea, útil para ver la estructura.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirNiveles() const {
    if(raiz == nullptr) return;
    
    std::queue<Nodo*> cola;
    cola.push(raiz);
    
    while(!cola.empty()) {
        int tamanoNivel = cola.size();
        
        for(int i = 0; i < tamanoNivel; ++i) {
            Nodo* actual = cola.front();
            cola.pop();
            
            std::cout << "[";
            for(int j = 0; j < actual->elemNodo; ++j) {
                std::cout << actual->claves[j];
                if(j < actual->elemNodo-1) std::cout << " ";
            }
            std::cout << "] ";
            
            if(!EsHoja(actual)) {
                for(int j = 0; j <= actual->elemNodo; ++j) {
                    if(actual->hijo[j] != nullptr) {
                        cola.push(actual->hijo[j]);
                    }
                }
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Devuelve la cantidad total de elementos en el árbol.
 * 
 * @return Número de elementos insertados actualmente en el árbol.
 */
template <typename Type, int grado>
int StarBTree<Type, grado>::CantElem() const {
    return cantElem;
}
