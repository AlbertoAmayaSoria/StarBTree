#include "../Headers/StarBTree.hpp"
#include <ostream>

using namespace std;

int main() {
    StarBTree<int, 4> arbol;

    for(int i = 0 ; i <= 8 ; ++i){
        cout << "Agregando el valor : " << i << endl;
        arbol.Agregar(i);
        //arbol.ImprimirNiveles();
        cout << "-------------------------" << endl;
        //i = i + 100;
    }

    arbol.ImprimirNiveles();

    return 0;
}

