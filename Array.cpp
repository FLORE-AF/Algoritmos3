#include <iostream>
using namespace std;
int main() {
    int n=0;
    cout << "Ingrese la dimension del arreglo: ";
    cin >> n;
    // Crear un arreglo dinámico
    int* arreglo = new int[n];
    // Llenar el arreglo con valores
    for (int i = 0; i < n; ++i) {
        cout << "Ingrese valor para la posicion [" << i << "] ";
        cin >> arreglo[i];
    }
    // Mostrar los valores del arreglo
    cout << "Los valores del arreglo son: ";
    for (int i = 0; i < n; ++i) {
        cout << arreglo[i] << " ";
    }
    cout << endl;
    // Liberar la memoria asignada al arreglo
    delete[] arreglo;
    return 0;
}