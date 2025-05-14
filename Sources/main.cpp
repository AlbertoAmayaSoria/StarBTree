#include "../Headers/StarBTree.hpp"
#include <ostream>

int main() {
    StarBTree<int, 5> arbol;

    for(int i = 500 ; i > 0 ;){
        std::cout << "Agregando el valor : " << i << std::endl;
        arbol.Agregar(i);
        arbol.ImprimirNiveles();
        i = i - 100;
    }

    //arbol.ImprimirNiveles();

    return 0;
}

