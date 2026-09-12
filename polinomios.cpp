#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
struct Termino {
    float coeficiente;
    int   exponente;
    Termino* siguiente;
};
Termino* crearNodo(float coef, int exp) {
    Termino* nuevo = new Termino();
    nuevo->coeficiente = coef;
    nuevo->exponente   = exp;
    nuevo->siguiente   = nullptr;
    return nuevo;
}
Termino* insertarTermino(Termino* cabeza, float coef, int exp) {
    if (coef == 0.0f) return cabeza;
    if (cabeza == nullptr || exp > cabeza->exponente) {
        Termino* nuevo   = crearNodo(coef, exp);
        nuevo->siguiente = cabeza;
        return nuevo;
    }   
   if (cabeza->exponente == exp) {
        cabeza->coeficiente += coef;
        if (cabeza->coeficiente == 0.0f) {
            Termino* temp = cabeza->siguiente;
            delete cabeza;
            return temp;
        }
        return cabeza;
    }
    Termino* actual = cabeza;
    while (actual->siguiente != nullptr &&
           actual->siguiente->exponente > exp) {
        actual = actual->siguiente;
    }

     if (actual->siguiente != nullptr &&
        actual->siguiente->exponente == exp) {
        actual->siguiente->coeficiente += coef;
        if (actual->siguiente->coeficiente == 0.0f) {
            Termino* temp    = actual->siguiente->siguiente;
            delete actual->siguiente;
            actual->siguiente = temp;
        }
    } else {
         Termino* nuevo       = crearNodo(coef, exp);
        nuevo->siguiente     = actual->siguiente;
        actual->siguiente    = nuevo;
    }
    return cabeza;
}
void imprimirPolinomio(Termino* cabeza) {
    if (cabeza == nullptr) {
        cout << "0";
        return;
    }
    Termino* actual  = cabeza;
    bool     primero = true;

    while (actual != nullptr) {
        float coef = actual->coeficiente;
        int   exp  = actual->exponente;

        if (primero) {
            if (coef < 0) cout << "-";
            primero = false;
        } else {
            cout << (coef < 0 ? " - " : " + ");
        }

        float absCoef = fabs(coef);

        if (exp == 0) {
            cout << absCoef;
        } else if (absCoef != 1.0f) {
            cout << absCoef;
        }


        if (exp == 1)       cout << "x";
        else if (exp > 1)   cout << "x^" << exp;

        actual = actual->siguiente;
    }
    cout << endl;
}

int destruirPolinomio(Termino*& cabeza) {
    int contador = 0;
    while (cabeza != nullptr) {
        Termino* temp = cabeza;
        cabeza        = cabeza->siguiente;
        delete temp;
        contador++;
    }
    return contador;
}
Termino* sumarPolinomios(Termino* p1, Termino* p2) {
    Termino* resultado = nullptr;

    // Copiar todos los términos de P1
    Termino* actual = p1;
    while (actual != nullptr) {
        resultado = insertarTermino(resultado,
                                    actual->coeficiente,
                                    actual->exponente);
        actual = actual->siguiente;
    }

    // Sumar (insertar) todos los términos de P2
    actual = p2;
    while (actual != nullptr) {
        resultado = insertarTermino(resultado,
                                    actual->coeficiente,
                                    actual->exponente);
        actual = actual->siguiente;
    }

    return resultado;
}

Termino* multiplicarPolinomios(Termino* p1, Termino* p2) {
    Termino* resultado = nullptr;
    Termino* t1        = p1;

    while (t1 != nullptr) {
        Termino* t2 = p2;
        while (t2 != nullptr) {
            float nuevoCoef = t1->coeficiente * t2->coeficiente;
            int   nuevoExp  = t1->exponente   + t2->exponente;
            resultado = insertarTermino(resultado, nuevoCoef, nuevoExp);
            t2 = t2->siguiente;
        }
        t1 = t1->siguiente;
    }
    return resultado;
}

float evaluarPolinomio(Termino* cabeza, float x) {
    float    resultado = 0.0f;
    Termino* actual    = cabeza;

    while (actual != nullptr) {
        resultado += actual->coeficiente * pow(x, actual->exponente);
        actual = actual->siguiente;
    }
    return resultado;
}

Termino* derivarPolinomio(Termino* cabeza) {
    Termino* resultado = nullptr;
    Termino* actual    = cabeza;

    while (actual != nullptr) {
        if (actual->exponente != 0) {
            float nuevoCoef = actual->coeficiente * actual->exponente;
            int   nuevoExp  = actual->exponente - 1;
            resultado = insertarTermino(resultado, nuevoCoef, nuevoExp);
        }
        actual = actual->siguiente;
    }
    return resultado;
}

int contarTerminos(Termino* cabeza) {
    int      contador = 0;
    Termino* actual   = cabeza;
    while (actual != nullptr) {
        contador++;
        actual = actual->siguiente;
    }
    return contador;
}

int gradoPolinomio(Termino* cabeza) {
    if (cabeza == nullptr) return -1;  // polinomio vacío
    return cabeza->exponente;
}

void separador(const string& titulo) {
    cout << "\n" << string(55, '=') << endl;
    cout << "  " << titulo << endl;
    cout << string(55, '=') << endl;
}