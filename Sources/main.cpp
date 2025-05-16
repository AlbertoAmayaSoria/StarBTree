#include "../Headers/StarBTree.hpp"
#include <ostream>

using namespace std;

int main() {
    StarBTree<int, 4> arbol;

    for(int i = 1 ; i <= 23 ; ++i){
        cout << "Agregando el valor : " << i << endl;
        arbol.Agregar(i);
        //arbol.ImprimirNiveles();
        cout << "-------------------------" << endl;
        //i = i + 100;
    }

    arbol.ImprimirNiveles();

    std::cout << "---------------------------" << std::endl;
    std::cout << "Hay elemento 20? ";
    if(arbol.Buscar(20)){
        std::cout << "Si se encontro el elemento 20" << std::endl;
    } else std::cout << "No se encontro el elemento 20" << std::endl;

    arbol.Vaciar();

    std::cout << "Vaciando.. " << std::endl;
    arbol.ImprimirNiveles();
    std::cout << "Elementos en el arbol despues de vaciar " << arbol.CantElem();

    return 0;
}

