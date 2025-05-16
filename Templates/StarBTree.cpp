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
 * @brief Agrega un elemento al árbol.
 * @param valor Valor a insertar.
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Agregar(Type valor){
    if (raiz == nullptr) raiz = new Nodo(/*true, true*/);  // raíz y hoja

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

    // INTENTAR REDISTRIBUIR EN CASCADA A LA IZQUIERDA
    for (int i = indiceHijo - 1; i >= 0; --i) {
        Nodo* izquierdo = padre->hijo[i];
        if (izquierdo->elemNodo < grado) {
            std::cout << "Redistribuyendo en cascada hacia la izquierda con el hijo[" << i << "]\n";

            // Mover la clave del padre hacia el final del izquierdo
            izquierdo->claves[izquierdo->elemNodo] = padre->claves[i];

            if (!hijoHoja)
                izquierdo->hijo[izquierdo->elemNodo + 1] = padre->hijo[i + 1]->hijo[0];
            izquierdo->elemNodo++;

            // Subir la primera clave del nodo h actual al padre
            padre->claves[i] = h->claves[0];

            // Desplazar a la izquierda todas las claves e hijos en h
            for (int j = 0; j < h->elemNodo - 1; ++j) {
                h->claves[j] = h->claves[j + 1];
                if (!hijoHoja)
                    h->hijo[j] = h->hijo[j + 1];
            }
            if (!hijoHoja)
                h->hijo[h->elemNodo - 1] = h->hijo[h->elemNodo];
            h->elemNodo--;

            return;
        }
    }

    // SI NO SE PUDO A LA IZQUIERDA, INTENTAR REDISTRIBUIR A LA DERECHA
    for (int i = indiceHijo + 1; i <= padre->elemNodo; ++i) {
        Nodo* derecho = padre->hijo[i];
        if (derecho->elemNodo < grado) {
            std::cout << "Redistribuyendo hacia la derecha con el hijo[" << i << "]\n";

            // Desplazar claves e hijos del derecho a la derecha
            for (int j = derecho->elemNodo; j > 0; --j) {
                derecho->claves[j] = derecho->claves[j - 1];
                if (!hijoHoja)
                    derecho->hijo[j + 1] = derecho->hijo[j];
            }
            if (!hijoHoja)
                derecho->hijo[1] = derecho->hijo[0];

            // Mover clave del padre a derecho[0]
            derecho->claves[0] = padre->claves[i - 1];
            if (!hijoHoja)
                derecho->hijo[0] = h->hijo[h->elemNodo];
            derecho->elemNodo++;

            // Subir la última clave de h al padre
            padre->claves[i - 1] = h->claves[h->elemNodo - 1];
            h->elemNodo--;

            return;
        }
    }

    // SI NO SE PUDO REDISTRIBUIR, HACER DIVISIÓN TRIPLE
    std::cout << "No se pudo redistribuir, dividiendo triple" << std::endl;
    DividirTriple(padre, indiceHijo);
}


template <typename Type, int grado>
void StarBTree<Type, grado>::DividirTriple(Nodo* padre, int indiceHijo) {
    std::cout << "=== DIVISIÓN TRIPLE ===" << std::endl;

    Nodo* medio = padre->hijo[indiceHijo];
    Nodo* izquierdo = (indiceHijo > 0) ? padre->hijo[indiceHijo - 1] : nullptr;
    Nodo* derecho = (indiceHijo < padre->elemNodo) ? padre->hijo[indiceHijo + 1] : nullptr;

    // Escoger hermano para fusionar
    bool usarIzquierdo = (izquierdo && izquierdo->elemNodo == grado);
    bool usarDerecho = (derecho && derecho->elemNodo == grado);

    int posFusion = -1;
    Nodo *A, *B, *C; // nuevos nodos
    std::vector<Type> fusion;
    std::vector<Nodo*> fusionHijos;

    if (usarIzquierdo) {
        posFusion = indiceHijo - 1;
        for (int i = 0; i < izquierdo->elemNodo; ++i)
            fusion.push_back(izquierdo->claves[i]);
        fusion.push_back(padre->claves[posFusion]);
        for (int i = 0; i < medio->elemNodo; ++i)
            fusion.push_back(medio->claves[i]);

        if (!EsHoja(medio)) {
            for (int i = 0; i <= izquierdo->elemNodo; ++i)
                fusionHijos.push_back(izquierdo->hijo[i]);
            for (int i = 0; i <= medio->elemNodo; ++i)
                fusionHijos.push_back(medio->hijo[i]);
        }
    } else if (usarDerecho) {
        posFusion = indiceHijo;
        for (int i = 0; i < medio->elemNodo; ++i)
            fusion.push_back(medio->claves[i]);
        fusion.push_back(padre->claves[posFusion]);
        for (int i = 0; i < derecho->elemNodo; ++i)
            fusion.push_back(derecho->claves[i]);

        if (!EsHoja(medio)) {
            for (int i = 0; i <= medio->elemNodo; ++i)
                fusionHijos.push_back(medio->hijo[i]);
            for (int i = 0; i <= derecho->elemNodo; ++i)
                fusionHijos.push_back(derecho->hijo[i]);
        }
    } else {
        std::cout << "No se puede dividir triple (no hay hermanos llenos)." << std::endl;
        return;
    }

    // Crear nodos
    A = new Nodo();
    B = new Nodo();
    C = new Nodo();

    int total = fusion.size();
    int porcion = total / 3;

    for (int i = 0; i < porcion; ++i)
        A->claves[A->elemNodo++] = fusion[i];

    padre->claves[posFusion] = fusion[porcion];

    for (int i = porcion + 1; i < 2 * porcion + 1; ++i)
        B->claves[B->elemNodo++] = fusion[i];

    padre->claves[posFusion + 1] = fusion[2 * porcion + 1];

    for (int i = 2 * porcion + 2; i < total; ++i)
        C->claves[C->elemNodo++] = fusion[i];

    // Si no son hojas, distribuir hijos
    if (!fusionHijos.empty()) {
        int idx = 0;
        for (int i = 0; i <= A->elemNodo; ++i)
            A->hijo[i] = fusionHijos[idx++];
        for (int i = 0; i <= B->elemNodo; ++i)
            B->hijo[i] = fusionHijos[idx++];
        for (int i = 0; i <= C->elemNodo; ++i)
            C->hijo[i] = fusionHijos[idx++];
    }

    // Desplazar claves y punteros en padre
    for (int i = padre->elemNodo; i > posFusion + 1; --i) {
        padre->claves[i] = padre->claves[i - 1];
        padre->hijo[i + 1] = padre->hijo[i];
    }
    padre->elemNodo++;

    padre->hijo[posFusion]     = A;
    padre->hijo[posFusion + 1] = B;
    padre->hijo[posFusion + 2] = C;

    if (usarIzquierdo) {
        delete izquierdo;
        delete medio;
    } else {
        delete medio;
        delete derecho;
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


template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar() {
    Vaciar(raiz);
    raiz = nullptr;  // Importante resetear la raíz
    cantElem = 0;    // Resetear el contador de elementos
}

/**
 * @brief Elimina todos los elementos del árbol de forma recursiva
 * @param nodo Nodo actual a eliminar
 */
template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar(Nodo* nodo) {
    if (nodo == nullptr) return;

    // Primero vaciamos recursivamente los hijos si no es hoja
    if (!EsHoja(nodo)) {  // Usamos el miembro directo en lugar de EsHoja()
        for (int i = 0; i <= nodo->elemNodo; ++i) {
            Vaciar(nodo->hijo[i]);
            nodo->hijo[i] = nullptr;  // Marcamos como nullptr después de liberar
        }
    }

    // Eliminamos el nodo actual
    delete nodo;
    // No necesitamos nodo = nullptr aquí porque es una copia local del puntero
    // Tampoco decrementamos cantElem aquí, se resetea en la función pública
}

/**
 * @brief Elimina recursivamente los nodos desde uno dado.
 * @param nodo Nodo raíz del subárbol a eliminar.
 */
/*template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar(Nodo* nodo) {
    if(nodo == nullptr) return;
    
    if(!EsHoja(nodo)) {
        for(int i = 0; i <= nodo->elemNodo; ++i) {
            Vaciar(nodo->hijo[i]);
        }
    }
    
    delete nodo;
}*/

/*template <typename Type, int grado>
void StarBTree<Type, grado>::Vaciar(){
    Vaciar(raiz);
}*/

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


