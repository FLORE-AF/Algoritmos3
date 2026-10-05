#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;


class PolF1;
class PolF2;

class Termino {
private:
    int coeficiente;
    int exponente;

public:
    Termino(int coef = 0, int exp = 0) {
        this->coeficiente = coef;
        this->exponente = exp;
    }

    int getCoef() const { return coeficiente; }
    int getExp() const { return exponente; }
    void setCoef(int c) { coeficiente = c; }
    void setExp(int e) { exponente = e; }
};


class Nodo {
private:
    int coeficiente;
    int exponente;
    Nodo* siguiente;

public:
    Nodo(int c = 0, int e = 0) : coeficiente(c), exponente(e), siguiente(nullptr) {}

    int getCoef() const { return coeficiente; }
    int getExp() const { return exponente; }
    Nodo* getLiga() const { return siguiente; }
    
    void setCoef(int c) { coeficiente = c; }
    void setExp(int e) { exponente = e; }
    void setLiga(Nodo* n) { siguiente = n; }
};



class PolF1 {
private:
    int* datos;
    int deg;

    void ajustarGrado() {
        int primerIndiceValido = -1;
        for (int k = 1; k <= deg + 1; ++k) {
            if (datos[k] != 0) {
                primerIndiceValido = k;
                break;
            }
        }

        if (primerIndiceValido == -1) {
            delete[] datos;
            datos = new int[2]{0, 0};
            deg = 0;
            return;
        }

        int nuevoGrado = deg - primerIndiceValido + 1;
        if (nuevoGrado != deg) {
            int* nuevoVector = new int[nuevoGrado + 2];
            nuevoVector[0] = nuevoGrado;
            for (int k = 0; k <= nuevoGrado; ++k) {
                nuevoVector[k + 1] = datos[primerIndiceValido + k];
            }
            delete[] datos;
            datos = nuevoVector;
            deg = nuevoGrado;
        }
    }

    void expandir(int coef, int exp) {
        if (exp > deg) {
            int* buffer = new int[exp + 2]();
            buffer[0] = exp;
            buffer[1] = coef;

            for (int idx = 1; idx <= deg + 1; ++idx) {
                int expActual = deg - idx + 1;
                if (expActual >= 0) {
                    buffer[exp - expActual + 1] = datos[idx];
                }
            }
            delete[] datos;
            datos = buffer;
            deg = exp;
        }
    }

public:
    PolF1() {
        deg = 0;
        datos = new int[2]{0, 0};
    }

    PolF1(int coef, int exp) {
        if (coef == 0) {
            deg = 0;
            datos = new int[2]{0, 0};
        } else {
            deg = exp;
            datos = new int[exp + 2]();
            datos[0] = exp;
            datos[1] = coef;
        }
    }

   
    PolF1& operator=(const PolF1& fuente) {
        if (this != &fuente) {
            delete[] datos;
            deg = fuente.deg;
            datos = new int[deg + 2];
            for (int i = 0; i < deg + 2; ++i) {
                datos[i] = fuente.datos[i];
            }
        }
        return *this;
    }

    int getGrado() const { return deg; }

    int getCoeficiente(int exp) const {
        if (exp < 0 || exp > deg) return 0;
        return datos[deg - exp + 1];
    }

    int getCoeficienteLider() const {
        return getCoeficiente(deg);
    }

    bool esCero() const {
        for (int i = 1; i <= deg + 1; ++i) {
            if (datos[i] != 0) return false;
        }
        return true;
    }

    void insertar(int exp, double coe) {
        int c = static_cast<int>(coe);
        if (c == 0) return;

        if (exp > deg) {
            expandir(c, exp);
        } else {
            int posicion = deg - exp + 1;
            datos[posicion] += c;
            ajustarGrado();
        }
    }

    void insertar(int coef, int exp) {
        insertar(exp, static_cast<double>(coef));
    }

    PolF1 multiplicar(const PolF1& b) const {
        if (this->esCero() || b.esCero()) {
            return PolF1(0, 0);
        }

        PolF1 res;
        for (int e1 = deg; e1 >= 0; --e1) {
            int c1 = getCoeficiente(e1);
            if (c1 == 0) continue;

            for (int e2 = b.deg; e2 >= 0; --e2) {
                int c2 = b.getCoeficiente(e2);
                if (c2 == 0) continue;

                res.insertar(e1 + e2, static_cast<double>(c1 * c2));
            }
        }
        return res;
    }

    PolF1 restar(const PolF1& b) const {
        PolF1 diferencia = *this;
        for (int e = b.deg; e >= 0; --e) {
            int c = b.getCoeficiente(e);
            if (c != 0) {
                diferencia.insertar(e, static_cast<double>(-c));
            }
        }
        return diferencia;
    }

    PolF1 dividir(const PolF1& b) const {
        if (b.esCero() || b.getGrado() > this->getGrado()) {
            return PolF1();
        }

        PolF1 q;
        PolF1 r = *this;

        while (!r.esCero() && r.getGrado() >= b.getGrado()) {
            int cRem = r.getCoeficienteLider();
            int cDiv = b.getCoeficienteLider();

            if (cDiv == 0) break;

            int cTermino = cRem / cDiv;
            int eTermino = r.getGrado() - b.getGrado();

            if (cTermino == 0) break;

            q.insertar(eTermino, static_cast<double>(cTermino));

            PolF1 t(cTermino, eTermino);
            PolF1 prod = b.multiplicar(t);
            r = r.restar(prod);
        }

        return q;
    }

    bool sonIguales(const PolF2& b) const;

    string mostrar() const {
        if (esCero()) return "|0|";
        stringstream ss;
        ss << "|";
        for (int idx = 0; idx < deg + 2; ++idx) {
            ss << datos[idx] << "|";
        }
        return ss.str();
    }

    bool esPalindromo() const {
        int izq = 1;
        int der = deg + 1;
        while (izq < der) {
            if (datos[izq] != datos[der]) return false;
            izq++;
            der--;
        }
        return true;
    }
};




class PolF2 {
private:
    Termino* arreglo;
    int cantidad;

public:
    PolF2() : arreglo(nullptr), cantidad(0) {}

    ~PolF2() {
        delete[] arreglo;
    }

    PolF2(const PolF2& orig) {
        cantidad = orig.cantidad;
        arreglo = new Termino[cantidad];
        for (int i = 0; i < cantidad; ++i) {
            arreglo[i] = orig.arreglo[i];
        }
    }

    PolF2& operator=(const PolF2& orig) {
        if (this != &orig) {
            delete[] arreglo;
            cantidad = orig.cantidad;
            arreglo = new Termino[cantidad];
            for (int i = 0; i < cantidad; ++i) {
                arreglo[i] = orig.arreglo[i];
            }
        }
        return *this;
    }

    int getGrado() const {
        if (cantidad == 0) return 0;
        return arreglo[0].getExp();
    }

    int getCoeficiente(int exp) const {
        for (int i = 0; i < cantidad; ++i) {
            if (arreglo[i].getExp() == exp) return arreglo[i].getCoef();
        }
        return 0;
    }

    Termino getTerminoLider() const {
        return (cantidad > 0) ? arreglo[0] : Termino(0, 0);
    }

    int buscarUbicacion(int exp) const {
        for (int i = 0; i < cantidad; ++i) {
            if (arreglo[i].getExp() <= exp) {
                return i;
            }
        }
        return cantidad;
    }

    void remover(int indice) {
        if (indice < 0 || indice >= cantidad) return;
        Termino* temp = new Termino[cantidad - 1];
        int k = 0;
        for (int i = 0; i < cantidad; ++i) {
            if (i != indice) {
                temp[k++] = arreglo[i];
            }
        }
        delete[] arreglo;
        arreglo = temp;
        cantidad--;
    }

    void insertar(const Termino& nuevoTermino) {
        if (nuevoTermino.getCoef() == 0) return;

        int pos = buscarUbicacion(nuevoTermino.getExp());

        if (pos < cantidad && arreglo[pos].getExp() == nuevoTermino.getExp()) {
            arreglo[pos].setCoef(arreglo[pos].getCoef() + nuevoTermino.getCoef());
            if (arreglo[pos].getCoef() == 0) {
                remover(pos);
            }
        } else {
            Termino* nuevoArreglo = new Termino[cantidad + 1];
            for (int i = 0; i < pos; ++i) nuevoArreglo[i] = arreglo[i];
            nuevoArreglo[pos] = nuevoTermino;
            for (int i = pos; i < cantidad; ++i) nuevoArreglo[i + 1] = arreglo[i];

            delete[] arreglo;
            arreglo = nuevoArreglo;
            cantidad++;
        }
    }

    PolF2 multiplicar(const PolF2& b) const {
        PolF2 producto;
        for (int i = 0; i < cantidad; ++i) {
            for (int j = 0; j < b.cantidad; ++j) {
                int c = arreglo[i].getCoef() * b.arreglo[j].getCoef();
                int e = arreglo[i].getExp() + b.arreglo[j].getExp();
                producto.insertar(Termino(c, e));
            }
        }
        return producto;
    }

    PolF2 restar(const PolF2& b) const {
        PolF2 res = *this;
        for (int i = 0; i < b.cantidad; ++i) {
            res.insertar(Termino(-b.arreglo[i].getCoef(), b.arreglo[i].getExp()));
        }
        return res;
    }

    PolF2 dividir(const PolF2& b) const {
        PolF2 q;
        if (b.cantidad == 0 || cantidad == 0) return q;
        if (b.getGrado() > getGrado()) return q;

        PolF2 r = *this;

        while (r.cantidad > 0 && r.getGrado() >= b.getGrado()) {
            Termino tResiduo = r.getTerminoLider();
            Termino tDivisor = b.getTerminoLider();

            if (tDivisor.getCoef() == 0) break;

            int cQ = tResiduo.getCoef() / tDivisor.getCoef();
            int eQ = tResiduo.getExp() - tDivisor.getExp();

            if (cQ == 0) break;

            Termino tActual(cQ, eQ);
            q.insertar(tActual);

            PolF2 f;
            f.insertar(tActual);

            PolF2 p = b.multiplicar(f);
            r = r.restar(p);
        }

        return q;
    }

    bool sonIguales(const PolF2& b) const {
        if (cantidad != b.cantidad) return false;
        for (int i = 0; i < cantidad; ++i) {
            if (arreglo[i].getCoef() != b.arreglo[i].getCoef() ||
                arreglo[i].getExp() != b.arreglo[i].getExp()) {
                return false;
            }
        }
        return true;
    }

    PolF1 multiplicarAForma1(const PolF2& b) const;

    string mostrar() const {
        if (cantidad == 0) return "0";
        stringstream ss;
        ss << "|";
        for (int i = 0; i < cantidad; ++i) {
            ss << "|Coef: " << arreglo[i].getCoef() << " Exp: " << arreglo[i].getExp() << "| ";
        }
        return ss.str();
    }
};


bool PolF1::sonIguales(const PolF2& b) const {
    for (int e = deg; e >= 0; --e) {
        if (getCoeficiente(e) != b.getCoeficiente(e)) {
            return false;
        }
    }
    if (b.getGrado() > deg) {
        for (int e = b.getGrado(); e > deg; --e) {
            if (b.getCoeficiente(e) != 0) return false;
        }
    }
    return true;
}

PolF1 PolF2::multiplicarAForma1(const PolF2& b) const {
    PolF2 auxProd = multiplicar(b);
    PolF1 resPol;
    for (int i = 0; i < auxProd.cantidad; ++i) {
        resPol.insertar(auxProd.arreglo[i].getExp(), static_cast<double>(auxProd.arreglo[i].getCoef()));
    }
    return resPol;
}

int main() {
    cout << "\FORMA 1 " << endl;
    PolF1 p1_a;
    p1_a.insertar(2, 1.0); 
    p1_a.insertar(1, 6.0); 
    p1_a.insertar(0, 3.0); 

    PolF1 p1_b;
    p1_b.insertar(5, 2.0);
    p1_b.insertar(1, 2.0); 

    cout << "Polinomio A: " << p1_a.mostrar() << endl;
    cout << "Polinomio B: " << p1_b.mostrar() << endl;

    PolF1 resMult1 = p1_a.multiplicar(p1_b);
    cout << "Multiplicacion (A * B): " << resMult1.mostrar() << endl;

    PolF1 resDiv1 = p1_a.dividir(p1_b);
    cout << "Division (A / B): " << resDiv1.mostrar() << endl;

    cout << "\n=== PRUEBAS DE FORMA 2 (PolF2) ===" << endl;
    PolF2 p2_a;
    p2_a.insertar(Termino(7, 2));
    p2_a.insertar(Termino(4, 3));
    p2_a.insertar(Termino(2, 5));

    PolF2 p2_b;
    p2_b.insertar(Termino(1, 2));
    p2_b.insertar(Termino(4, 0));

    cout << "Polinomio A: " << p2_a.mostrar() << endl;
    cout << "Polinomio B: " << p2_b.mostrar() << endl;

    PolF2 resMult2 = p2_a.multiplicar(p2_b);
    cout << "Multiplicacion PolF2 (A * B): " << resMult2.mostrar() << endl;

    PolF2 resDiv2 = p2_a.dividir(p2_b);
    cout << "Division PolF2 (A / B): " << resDiv2.mostrar() << endl;

    cout << "¿PolF2 A es igual a PolF2 B?: " << (p2_a.sonIguales(p2_b) ? "Si" : "No") << endl;
    cout << "¿PolF1 A es equivalente a PolF2 A?: " << (p1_a.sonIguales(p2_a) ? "Si" : "No") << endl;

    PolF1 resMultForma1 = p2_a.multiplicarAForma1(p2_b);
    cout << "Multiplicacion de PolF2 -> PolF1: " << resMultForma1.mostrar() << endl;

    return 0;
}