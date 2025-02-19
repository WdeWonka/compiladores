#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

// Definicion de tokens
enum TokenType {
    IDENTIFICADOR, PALABRA_RESERVADA, NUMERO, OPERADOR, DELIMITADOR, DESCONOCIDO, ASIGNACION
};

struct Token {
    TokenType tipo;
    string valor;
};

vector<string> palabras_reservadas = {"if", "else", "while", "return", "int", "float"};
vector<string> operadores = {"+", "-", "*", "/", ">", "<", "<=", ">=", "==", "!="};
vector<char> delimitadores = {'(', ')', '{', '}', ';', ','};
map<string, TokenType> tablaSimbolos;

bool esPalabraReservada(const string& palabra) {
    return find(palabras_reservadas.begin(), palabras_reservadas.end(), palabra) != palabras_reservadas.end();
}

vector<Token> analisisLexico(const string& codigo) {
    vector<Token> tokens;
    string buffer;

    for (size_t i = 0; i < codigo.size(); i++) {
        char c = codigo[i];
        
        if (isalpha(c)) {
            buffer += c;
            while (isalnum(codigo[i + 1])) {
                buffer += codigo[++i];
            }
            tokens.push_back({esPalabraReservada(buffer) ? PALABRA_RESERVADA : IDENTIFICADOR, buffer});
            if (!esPalabraReservada(buffer)) {
                tablaSimbolos[buffer] = IDENTIFICADOR;
            }
            buffer.clear();
        } else if (isdigit(c)) {
            buffer += c;
            while (isdigit(codigo[i + 1])) {
                buffer += codigo[++i];
            }
            tokens.push_back({NUMERO, buffer});
            buffer.clear();
        } else if (c == '=') {
            tokens.push_back({codigo[i + 1] == '=' ? OPERADOR : ASIGNACION, codigo[i + 1] == '=' ? "==" : "="});
            if (codigo[i + 1] == '=') i++;
        } else {
            string op(1, c);
            if (find(operadores.begin(), operadores.end(), op) != operadores.end()) {
                tokens.push_back({OPERADOR, op});
            } else if (find(delimitadores.begin(), delimitadores.end(), c) != delimitadores.end()) {
                tokens.push_back({DELIMITADOR, string(1, c)});
            }
        }
    }
    return tokens;
}

void mostrarTokens(const vector<Token>& tokens) {
    cout << "\nTokens generados:" << endl;
    for (const Token& token : tokens) {
        string tipo;
        switch (token.tipo) {
            case PALABRA_RESERVADA: tipo = "PALABRA_RESERVADA"; break;
            case IDENTIFICADOR: tipo = "IDENTIFICADOR"; break;
            case NUMERO: tipo = "NUMERO"; break;
            case OPERADOR: tipo = "OPERADOR"; break;
            case DELIMITADOR: tipo = "DELIMITADOR"; break;
            case ASIGNACION: tipo = "ASIGNACION"; break;
            default: tipo = "DESCONOCIDO"; break;
        }
        cout << "Token: " << tipo << ", Valor: " << token.valor << endl;
    }
}

void guardarTablaSimbolos() {
    ofstream archivo("tabla_simbolos.txt");
    archivo << "====== Tabla de Simbolos ======" << endl;
    archivo << left << setw(20) << "Identificador" << "Tipo" << endl;
    archivo << "================================" << endl;
    for (const auto& par : tablaSimbolos) {
        archivo << left << setw(20) << par.first << "IDENTIFICADOR" << endl;
    }
    archivo.close();
    cout << "\nTabla de simbolos guardada en 'tabla_simbolos.txt'\n";
}

struct Nodo {
    string valor;
    vector<Nodo*> hijos;
    Nodo(string v) : valor(v) {}
};

void imprimirArbol(Nodo* nodo, int nivel = 0) {
    if (!nodo) return;
    cout << string(nivel * 4, ' ') << "|- " << nodo->valor << endl;
    for (Nodo* hijo : nodo->hijos) {
        imprimirArbol(hijo, nivel + 1);
    }
}

Nodo* construirArbolSintactico(const vector<Token>& tokens) {
    Nodo* raiz = new Nodo("Programa");
    Nodo* actual = raiz;
    
    for (const Token& token : tokens) {
        Nodo* nuevoNodo = new Nodo(token.valor);
        actual->hijos.push_back(nuevoNodo);
        if (token.tipo == DELIMITADOR && token.valor == "{") {
            actual = nuevoNodo;
        }
        if (token.tipo == DELIMITADOR && token.valor == "}") {
            actual = raiz;
        }
    }
    return raiz;
}

void analisisSintactico(const vector<Token>& tokens) {
    cout << "\nArbol Sintactico:" << endl;
    Nodo* arbol = construirArbolSintactico(tokens);
    imprimirArbol(arbol);
}

int main() {
    int opcion;
    string codigo;
    vector<Token> tokens;

    do {
        cout << "\nMENU PRINCIPAL\n";
        cout << "1. Ingreso de texto en el editor\n";
        cout << "2. Analisis lexico\n";
        cout << "3. Analisis sintactico\n";
        cout << "4. Tabla de simbolos\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Ingrese el codigo fuente: \n";
                getline(cin, codigo);
                break;
            case 2:
                tokens = analisisLexico(codigo);
                mostrarTokens(tokens);
                break;
            case 3:
                analisisSintactico(tokens);
                break;
            case 4:
                guardarTablaSimbolos();
                break;
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
    } while (opcion != 5);

    return 0;
}
