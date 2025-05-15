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
        
        /*if(subraiz->hijo[i] == nullptr) {
            subraiz->hijo[i] = new Nodo();
        }*/
        
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
void StarBTree<Type, grado>::OrdenarNodo(Nodo* subraiz, int indiceHijo) {
    int minClaves = std::ceil((2.0 * grado) / 3.0) - 1; //ceil funciona para redondear hacia arriba

    // Caso especial: La raíz es hoja y esta llena
    if (subraiz == raiz && EsHoja(subraiz) && subraiz->elemNodo == grado) {
        int totalClaves = subraiz->elemNodo;

        // Crear nueva raíz
        Nodo* nuevaRaiz = new Nodo();
        Nodo* hijoIzquierdo = new Nodo();
        Nodo* hijoDerecho = new Nodo();

        // Determinar punto medio para cumplir mínimo
        int clavesIzq = minClaves;
        int clavesDer = totalClaves - clavesIzq - 1; // 1 es la clave que subiremos

        // Copiar claves al hijo izquierdo
        for (int i = 0; i < clavesIzq; ++i) {
            hijoIzquierdo->claves[i] = subraiz->claves[i];
            hijoIzquierdo->elemNodo++;
        }

        // Clave que sube a la raíz
        Type claveMediana = subraiz->claves[clavesIzq];
        nuevaRaiz->claves[0] = claveMediana;
        nuevaRaiz->elemNodo = 1;

        // Copiar claves al hijo derecho
        for (int i = 0; i < clavesDer; ++i) {
            hijoDerecho->claves[i] = subraiz->claves[clavesIzq + 1 + i];
            hijoDerecho->elemNodo++;
        }

        // Asignar hijos a la nueva raíz
        nuevaRaiz->hijo[0] = hijoIzquierdo;
        nuevaRaiz->hijo[1] = hijoDerecho;

        // Actualizar raíz
        delete subraiz;
        raiz = nuevaRaiz;
    } else {
        Nodo* hijo = subraiz->hijo[indiceHijo];
        if (hijo->elemNodo == grado) {
            // Try redistribution first
            Redistribuir(subraiz, indiceHijo);
            
            // If still full, split
            if (subraiz->hijo[indiceHijo]->elemNodo == grado) {
                // Special case: parent is root and has only one key
                if (subraiz == raiz && subraiz->elemNodo == 1) {
                    // Create new root
                    Nodo* nuevaRaiz = new Nodo();
                    // Perform the split and update nuevaRaiz
                    // ...
                    raiz = nuevaRaiz;
                } else {
                    DividirTriple(subraiz, indiceHijo);
                }
            }
        }
    }}




template <typename Type, int grado>
void StarBTree<Type, grado>::Redistribuir(Nodo* subraiz, int indiceHijo) {
    Nodo* hijo = subraiz->hijo[indiceHijo];
    bool hoja = EsHoja(hijo);

    // Redistribución con hermano izquierdo
    if (indiceHijo > 0) {
        Nodo* hermanoIzq = subraiz->hijo[indiceHijo - 1];
        if (hermanoIzq->elemNodo < grado) {
            Type clavePadre = subraiz->claves[indiceHijo - 1];

            hermanoIzq->claves[hermanoIzq->elemNodo] = clavePadre;
            if (!hoja)
                hermanoIzq->hijo[hermanoIzq->elemNodo + 1] = hijo->hijo[0];
            hermanoIzq->elemNodo++;

            subraiz->claves[indiceHijo - 1] = hijo->claves[0];

            for (int i = 0; i < hijo->elemNodo - 1; ++i) {
                hijo->claves[i] = hijo->claves[i + 1];
                if (!hoja)
                    hijo->hijo[i] = hijo->hijo[i + 1];
            }
            if (!hoja)
                hijo->hijo[hijo->elemNodo - 1] = hijo->hijo[hijo->elemNodo];

            hijo->elemNodo--;
            return;
        }
    }

    // Redistribución con hermano derecho
    if (indiceHijo < subraiz->elemNodo) {
        Nodo* hermanoDer = subraiz->hijo[indiceHijo + 1];
        if (hermanoDer->elemNodo < grado) {
            Type clavePadre = subraiz->claves[indiceHijo];

            for (int i = hermanoDer->elemNodo; i > 0; --i) {
                hermanoDer->claves[i] = hermanoDer->claves[i - 1];
                if (!hoja)
                    hermanoDer->hijo[i + 1] = hermanoDer->hijo[i];
            }
            if (!hoja)
                hermanoDer->hijo[1] = hermanoDer->hijo[0];

            subraiz->claves[indiceHijo] = hijo->claves[hijo->elemNodo - 1];
            hermanoDer->claves[0] = clavePadre;
            if (!hoja)
                hermanoDer->hijo[0] = hijo->hijo[hijo->elemNodo];

            hermanoDer->elemNodo++;  // corregido (antes estaba mal decrementado)
            hijo->elemNodo--;
            return;
        }
    }

    // Si no se pudo redistribuir, se hará una división triple más adelante
}



template <typename Type, int grado>
void StarBTree<Type, grado>::DividirTriple(Nodo* subraiz, int indiceHijo) {
    if (!subraiz || indiceHijo < 0 || indiceHijo > subraiz->elemNodo) {
        std::cerr << "Invalid parameters to DividirTriple\n";
        return;
    }

    Nodo* hijo = subraiz->hijo[indiceHijo];
    Nodo* hermanoIzq = (indiceHijo > 0) ? subraiz->hijo[indiceHijo - 1] : nullptr;
    Nodo* hermanoDer = (indiceHijo < subraiz->elemNodo) ? subraiz->hijo[indiceHijo + 1] : nullptr;

    std::vector<Type> clavesFusion;
    
    // Fusión con hermano izquierdo
    if (hermanoIzq && hermanoIzq->elemNodo == grado) {
        for (int i = 0; i < hermanoIzq->elemNodo; ++i)
            clavesFusion.push_back(hermanoIzq->claves[i]);

        clavesFusion.push_back(subraiz->claves[indiceHijo - 1]);

        for (int i = 0; i < hijo->elemNodo; ++i)
            clavesFusion.push_back(hijo->claves[i]);

        int total = clavesFusion.size();
        int tercio = total / 3;

        Nodo* nuevo1 = new Nodo();
        Nodo* nuevo2 = new Nodo();

        for (int i = 0; i < tercio; ++i)
            nuevo1->claves[i] = clavesFusion[i];
        nuevo1->elemNodo = tercio;

        for (int i = 0; i < total - tercio - 1; ++i)
            nuevo2->claves[i] = clavesFusion[i + tercio + 1];
        nuevo2->elemNodo = total - tercio - 1;

        Type claveCentral = clavesFusion[tercio];

        for (int i = subraiz->elemNodo; i > indiceHijo - 1; --i) {
            subraiz->claves[i] = subraiz->claves[i - 1];
            subraiz->hijo[i + 1] = subraiz->hijo[i];
        }

        subraiz->claves[indiceHijo - 1] = claveCentral;
        subraiz->hijo[indiceHijo - 1] = nuevo1;
        subraiz->hijo[indiceHijo] = nuevo2;
        subraiz->elemNodo++;

        delete hijo;
        delete hermanoIzq;
        return;
    }

    // Fusión con hermano derecho
    if (hermanoDer && hermanoDer->elemNodo == grado) {
        for (int i = 0; i < hijo->elemNodo; ++i)
            clavesFusion.push_back(hijo->claves[i]);

        clavesFusion.push_back(subraiz->claves[indiceHijo]);

        for (int i = 0; i < hermanoDer->elemNodo; ++i)
            clavesFusion.push_back(hermanoDer->claves[i]);

        int total = clavesFusion.size();
        int tercio = total / 3;

        Nodo* nuevo1 = new Nodo();
        Nodo* nuevo2 = new Nodo();

        for (int i = 0; i < tercio; ++i)
            nuevo1->claves[i] = clavesFusion[i];
        nuevo1->elemNodo = tercio;

        for (int i = 0; i < total - tercio - 1; ++i)
            nuevo2->claves[i] = clavesFusion[i + tercio + 1];
        nuevo2->elemNodo = total - tercio - 1;

        Type claveCentral = clavesFusion[tercio];

        for (int i = subraiz->elemNodo; i > indiceHijo; --i) {
            subraiz->claves[i] = subraiz->claves[i - 1];
            subraiz->hijo[i + 1] = subraiz->hijo[i];
        }

        subraiz->claves[indiceHijo] = claveCentral;
        subraiz->hijo[indiceHijo] = nuevo1;
        subraiz->hijo[indiceHijo + 1] = nuevo2;
        subraiz->elemNodo++;

        delete hijo;
        delete hermanoDer;
        return;
    }

    std::cerr << "División triple falló: no hay hermanos válidos.\n";
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


