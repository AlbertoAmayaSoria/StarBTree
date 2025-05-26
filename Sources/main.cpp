#include <iostream>
#include "../Headers/StarBTree.hpp"

using namespace std;

int main() {
    // Creamos un árbol B* de tipo int con grado 4
    StarBTree<int, 4> arbol;

    //int opcion, valor;

    for(int i = 1 ; i <= 8 ; ++i){
        arbol.Agregar(i);
    }
    //arbol.ImprimirNiveles();
    /*do {
        cout << "\n===== Menú del Árbol B* =====" << endl;
        cout << "1. Agregar elemento" << endl;
        cout << "2. Eliminar elemento" << endl;
        cout << "3. Buscar elemento" << endl;
        cout << "4. Imprimir en orden ascendente" << endl;
        cout << "5. Imprimir en orden descendente" << endl;
        cout << "6. Imprimir por niveles" << endl;
        cout << "7. Vaciar árbol" << endl;
        cout << "8. Mostrar cantidad de elementos" << endl;
        cout << "9. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                cout << "Ingrese el elemento a agregar: ";
                cin >> valor;
                arbol.Agregar(valor);
                break;
            case 2:
                cout << "Se lo debemos para el el siguiente semestre" << endl;
                //cout << "Ingrese el elemento a eliminar: ";
                //cin >> valor;
                //arbol.Eliminar(valor);
                break;
            case 3:
                cout << "Ingrese el elemento a buscar: ";
                cin >> valor;
                if (arbol.Buscar(valor))
                    cout << "El elemento " << valor << " se encuentra en el árbol." << endl;
                else
                    cout << "El elemento " << valor << " no existe en el árbol." << endl;
                break;
            case 4:
                cout << "Árbol en orden ascendente:" << endl;
                arbol.ImprimirAsc();
                break;
            case 5:
                cout << "Árbol en orden descendente:" << endl;
                arbol.ImprimirDes();
                break;
            case 6:
                cout << "Impresión por niveles:" << endl;
                arbol.ImprimirNiveles();
                break;
            case 7:
                arbol.Vaciar();
                cout << "El árbol ha sido vaciado." << endl;
                break;
            case 8:
                cout << "Cantidad de elementos: " << arbol.CantElem() << endl;
                break;
            case 9:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
                break;
        }
    } while(opcion != 9);*/

    return 0;
}

