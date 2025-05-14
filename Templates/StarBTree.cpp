#include <iostream>
#include <queue>
#include <stdexcept>
#include "../Headers/StarBTree.hpp"

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

/**
 * @brief Agrega un elemento al árbol.
 * @param valor Valor a insertar.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Agregar(Type valor){
    if(raiz == nullptr) raiz = new Nodo();
    Agregar(valor, raiz);
}

/**
 * @brief Insertar recursivamente un valor en un subárbol.
 * @param valor Valor a insertar.
 * @param subraíz Nodo raíz del subárbol
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Agregar(Type valor, Nodo* subraiz){
    int i = subraiz->elemNodo - 1;

    if(EsHoja(subraiz)) {
        while(i >= 0 && valor < subraiz->claves[i]) {
            subraiz->claves[i+1] = subraiz->claves[i];
            --i;
        }
        
        subraiz->claves[i+1] = valor;
        subraiz->elemNodo++;
        ++cantElem;
        if(subraiz->elemNodo == grado){
            std::cout << "El nodo se lleno" << std::endl;
            OrdenarNodo(subraiz, i);
        }
    } else {
        while(i >= 0 && valor < subraiz->claves[i]) {
            --i;
        }
        ++i;
        
        if(subraiz->hijo[i] == nullptr) {
            subraiz->hijo[i] = new Nodo();
        }
        
        Agregar(valor, subraiz->hijo[i]);
        
        if(subraiz->hijo[i]->elemNodo == grado) {
            OrdenarNodo(subraiz, i);
            //std::cout << "El nodo se lleno" << std::endl;
        }
    }
}


/**
 *
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::OrdenarNodo(Nodo* subraiz, int hindiceHijo) {
    int minClaves = (grado * 2 ) / 3; // minimo de claves por nodo

    // Solo si el nodo a dividir es la raíz
    if (subraiz == raiz && raiz->elemNodo == grado) {
        Nodo* nuevaRaiz = new Nodo();
        Nodo* nuevoHijo = new Nodo();

        for(int i = 0 ; i < minClaves ; ++i){
            nuevoHijo->claves[i] = subraiz->claves[i];
            nuevoHijo->elemNodo++;
            nuevoHijo->hijo[i] = subraiz->hijo[i];
        }

        for(int i = minClaves ; i < grado ; ++i){
            nuevaRaiz->claves[i - minClaves] = subraiz->claves[i];
            nuevaRaiz->elemNodo++;
            nuevaRaiz->hijo[i - minClaves + 1] = subraiz->hijo[i];
        }

        nuevaRaiz->hijo[grado] = subraiz->hijo[grado];

        delete subraiz;
        nuevaRaiz->hijo[0] = nuevoHijo;
        raiz = nuevaRaiz;
        
    }
}















/**
 * @brief Verifica si un nodo es hoja.
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
 * @brief Elimina todos los elementos del árbol.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar() {
    Vaciar(raiz);
    raiz = nullptr;
    cantElem = 0;
}

/**
 * @brief Elimina recursivamente los nodos desde uno dado.
 * @param nodo Nodo raíz del subárbol a eliminar.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar(Nodo* nodo) {
    if(nodo == nullptr) return;
    
    if(!EsHoja(nodo)) {
        for(int i = 0; i <= nodo->elemNodo; ++i) {
            Vaciar(nodo->hijo[i]);
        }
    }
    
    delete nodo;
}

/**
 * @brief Imprime los elementos del árbol en orden ascendente.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirAsc() const {
    ImprimirAsc(raiz);
    std::cout << std::endl;
}

/**
 * @brief Imprime recursivamente en orden ascendente.
 * @param nodo Nodo desde donde imprimir.
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
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::ImprimirDes() const {
    ImprimirDes(raiz);
    std::cout << std::endl;
}

/**
 * @brief Imprime recursivamente en orden descendente.
 * @param nodo Nodo desde donde imprimir.
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

// Método para obtener cantidad de elementos
template <typename Type, int grado>
int StarBTree<Type, grado>::CantElem() const {
    return cantElem;
}

// Prueba commit2
