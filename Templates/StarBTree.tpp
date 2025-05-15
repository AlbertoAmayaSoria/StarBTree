#include <iostream>
#include <queue>
#include <cmath>
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

    if (EsHoja(subraiz)) {
        // Inserción en hoja
        while (i >= 0 && valor < subraiz->claves[i]) {
            subraiz->claves[i + 1] = subraiz->claves[i];
            --i;
        }
        subraiz->claves[i + 1] = valor;
        subraiz->elemNodo++;
        cantElem++;
        ImprimirNiveles(); //Para pruebas

        // Si la hoja se llena, hay que reequilibrar
        if (subraiz->elemNodo == grado && subraiz == raiz) {
            std::cout << "El nodo esta lleno" << std::endl;
            OrdenarNodo(subraiz, i + 1);
        }
    } else {
        // Elegir hijo adecuado
        while (i >= 0 && valor < subraiz->claves[i]) --i;
        ++i;
        std::cout << "Bajando un nivel" << std::endl;
        Agregar(valor, subraiz->hijo[i]);

        // Tras bajar, si ese hijo se llenó, reequilibrar
        if (subraiz->hijo[i]->elemNodo == grado) {
            std::cout << "Se lleno el nodo visitado" << std::endl;
            OrdenarNodo(subraiz, i);
        }
    }
}

/**
 *
 */

template <typename Type, int grado>
void StarBTree<Type, grado>::OrdenarNodo(Nodo* subraiz, int indiceHijoHijo) {
    // caso raíz hoja
    if (subraiz == raiz && EsHoja(subraiz) && subraiz->elemNodo == grado) {
        std::cout << "Dividiendo el nodo raiz" << std::endl;
        int minClaves = std::ceil((2.0 * grado) / 3.0) - 1;
        int total = subraiz->elemNodo;
        int clavesI = minClaves;
        int clavesD = total - clavesI - 1;

        Nodo* izquierdo = new Nodo();
        Nodo* derecho = new Nodo();

        // L
        for (int i = 0; i < clavesI; ++i) {
            izquierdo->claves[i] = subraiz->claves[i];
            izquierdo->elemNodo++;
        }
        // R
        for (int i = 0; i < clavesD; ++i) {
            derecho->claves[i] = subraiz->claves[clavesI + 1 + i];
            derecho->elemNodo++;
        }
        // Nueva raíz
        Type mid = subraiz->claves[clavesI];
        Nodo* nueva = new Nodo();
        nueva->claves[0] = mid;
        nueva->elemNodo = 1;
        nueva->hijo[0] = izquierdo;
        nueva->hijo[1] = derecho;

        delete subraiz;
        raiz = nueva;
        return;
    }

    // caso general: reequilibrar al hijo indiceHijoHijo
    /*Nodo* h = subraiz->hijo[indiceHijoHijo];
    if (h->elemNodo < grado) return;  // nada que hacer*/

    // 1) intentar redistribuir
    std::cout << "Redistribuyendo" << std::endl;
    Redistribuir(subraiz, indiceHijoHijo);

    // 2) si sigue lleno, dividir
    if (subraiz->hijo[indiceHijoHijo]->elemNodo == grado) {
        std::cout << "Triple division" << std::endl;
        DividirTriple(subraiz, indiceHijoHijo);
    }
}




template <typename Type, int grado>
void StarBTree<Type, grado>::Redistribuir(Nodo* padre, int indiceHijo) {
    std::cout << "Redistribucion" << std::endl;
    Nodo* h = padre->hijo[indiceHijo];
    bool hijoHoja = EsHoja(h);

    // izquierdo
    if (indiceHijo > 0) {
        std::cout << "Redistribuyendo a la izquierda" << std::endl;
        Nodo* izquierdo = padre->hijo[indiceHijo - 1];
        if (izquierdo->elemNodo < grado) {
            // mover p->claves[indiceHijo-1] → L
            izquierdo->claves[izquierdo->elemNodo] = padre->claves[indiceHijo - 1];
            if (!hijoHoja)
                izquierdo->hijo[izquierdo->elemNodo + 1] = h->hijo[0];
            izquierdo->elemNodo++;

            // subir h->claves[0] → p
            padre->claves[indiceHijo - 1] = h->claves[0];

            // desplazar h
            for (int i = 0; i < h->elemNodo - 1; ++i) {
                h->claves[i] = h->claves[i + 1];
                if (!hijoHoja)
                    h->hijo[i] = h->hijo[i + 1];
            }
            if (!hijoHoja)
                h->hijo[h->elemNodo - 1] = h->hijo[h->elemNodo];
            h->elemNodo--;
            
            if(izquierdo->elemNodo == grado && h->elemNodo == grado){
                DividirTriple(h, indiceHijo);
            } return;
        }
    }

    // derecho
    if (indiceHijo < padre->elemNodo) {
        std::cout << "Redistribuyendo a la derecha" << std::endl;
        Nodo* derecho = padre->hijo[indiceHijo + 1];
        if (derecho->elemNodo < grado) {
            // desplazar en R
            for (int i = derecho->elemNodo; i > 0; --i) {
                derecho->claves[i] = derecho->claves[i - 1];
                if (!hijoHoja)
                    derecho->hijo[i + 1] = derecho->hijo[i];
            }
            if (!hijoHoja)
                derecho->hijo[1] = derecho->hijo[0];

            // mover p->claves[indiceHijo] → R[0]
            derecho->claves[0] = padre->claves[indiceHijo];
            if (!hijoHoja)
                derecho->hijo[0] = h->hijo[h->elemNodo];
            derecho->elemNodo++;

            // subir h->claves[last] → p
            padre->claves[indiceHijo] = h->claves[h->elemNodo - 1];
            h->elemNodo--;
            return;
        }
    }

    DividirTriple(padre, indiceHijo);
}


template <typename Type, int grado>
void StarBTree<Type, grado>::DividirTriple(Nodo* padre, int indiceHijo) {
    std::cout<< "Division triple" << std::endl;

    //Nodo* aux = 

    /*Nodo* h = padre->hijo[indiceHijo];
    int m = grado;
    // escoger hermano lleno (izq o der)
    bool canLeft = (indiceHijo > 0 && padre->hijo[indiceHijo - 1]->elemNodo == m);
    bool canRight = (indiceHijo < padre->elemNodo && padre->hijo[indiceHijo + 1]->elemNodo == m);

    int pivotParent = canLeft ? indiceHijo - 1 : indiceHijo;
    Nodo* S = canLeft ? padre->hijo[indiceHijo - 1] : h;
    Nodo* T = canLeft ? h : padre->hijo[indiceHijo + 1];

    // fusionar S + clavePadre + T
    std::vector<Type> vec;
    for (int i = 0; i < S->elemNodo; ++i) vec.push_back(S->claves[i]);
    vec.push_back(padre->claves[pivotParent]);
    for (int i = 0; i < T->elemNodo; ++i) vec.push_back(T->claves[i]);

    int total = vec.size();
    int one = total / 3;
    int two = total - one - 1;

    Nodo* A = new Nodo(); A->elemNodo = one;
    Nodo* B = new Nodo(); B->elemNodo = two;
    Type mid = vec[one];

    for (int i = 0; i < one; ++i) A->claves[i] = vec[i];
    for (int i = 0; i < two; ++i) B->claves[i] = vec[one + 1 + i];

    // insertar mid en p
    for (int j = padre->elemNodo; j > pivotParent; --j) {
        padre->claves[j]  = padre->claves[j - 1];
        padre->hijo[j + 1] = padre->hijo[j];
    }
    padre->claves[pivotParent] = mid;
    padre->hijo[pivotParent] = A;
    padre->hijo[pivotParent + 1] = B;
    padre->elemNodo++;

    // liberar viejos
    if (canLeft) {
        delete S; delete T;
    } else {
        delete h; delete T;
    }*/
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


