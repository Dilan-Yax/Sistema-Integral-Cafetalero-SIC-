#include"encabezado.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include <conio.h> // Para _getch()
#include <windows.h>
#include <iomanip>


using namespace std;

// Función para mover el cursor a una posición específica en la pantalla
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

const int T = 2; // Grado mínimo del árbol B

struct Recibo {
    int idVenta;
    int idCliente;
    string tipoCafe;
    float precioPorKg;
    float cantidadKg;
    float descuento;
    float totalVenta;
    string formaPago;
};

struct BTreeNode {
    int n;
    bool hoja;
    Recibo claves[2 * T - 1];
    BTreeNode* hijos[2 * T];
};

BTreeNode* crearNodo(bool hoja);
void insertar(BTreeNode*& raiz, Recibo k);
void insertarNoLleno(BTreeNode* nodo, Recibo k);
void dividirHijo(BTreeNode* padre, int i, BTreeNode* hijo);
void mostrar(BTreeNode* nodo);
void mostrarRecibo(const Recibo& r);
void menu(BTreeNode*& raiz);
void buscar(BTreeNode* nodo, int idVenta);

void modificarConSFML(BTreeNode* nodo, int idVenta, BTreeNode* nodoRaiz);
void eliminarRecibo(BTreeNode*& nodo, int idVenta, BTreeNode* raiz);
void mostrarYEsperar(BTreeNode* raiz, BTreeNode* nodoResaltado, int indiceClaveResaltada);
void insertarReciboUI(BTreeNode*& raiz);
void MostrarArteComplejo();


void vaciarArbol(BTreeNode*& nodo);

// --- FUNCIONES DE ELIMINACIÓN ---
Recibo getPredecesor(BTreeNode* nodo, int idx);
Recibo getSucesor(BTreeNode* nodo, int idx);
void fusionar(BTreeNode* nodo, int idx);
void llenar(BTreeNode* nodo, int idx);
void pedirPrestadoIzq(BTreeNode* nodo, int idx);
void pedirPrestadoDer(BTreeNode* nodo, int idx);
void arbolABL();
void arbolABB();

// [El resto del código permanece igual hasta la función main()]

void mostrarMenuPrincipal(int seleccionado) {
    const int xMenu = 45;
    const int yInicio = 10;

    system("cls");
    MostrarArteComplejo();

    gotoxy(xMenu, yInicio - 2);
    cout << "MENU PRINCIPAL - ESTRUCTURAS DE DATOS";

    // Opción 1 - Árbol B
    gotoxy(xMenu, yInicio);
    cout << (seleccionado == 0 ? "> " : "  ") << "1. Arbol AVl ";

    // Opción 2 - Árbol AVL
    gotoxy(xMenu, yInicio + 1);
    cout << (seleccionado == 1 ? "> " : "  ") << "2. Arbol B (Recibos de Cafe)";

    // Opción 2 - Árbol AVL
    gotoxy(xMenu, yInicio + 2);
    cout << (seleccionado == 2 ? "> " : "  ") << "3. Arbol ABB";

    // Opción 2 - Árbol AVL
    gotoxy(xMenu, yInicio + 3);
    cout << (seleccionado == 3 ? "> " : "  ") << "4.  estructura lineales";

    // Opción 3 - Salir
    gotoxy(xMenu, yInicio + 4);
    cout << (seleccionado == 4 ? "> " : "  ") << "5. Salir";
}

int main() {
    int seleccionado = 0;
    const int totalOpciones = 5;

    while (true) {
        mostrarMenuPrincipal(seleccionado);

        int tecla = _getch();
        if (tecla == 224) { // Tecla especial (flechas)
            tecla = _getch();
            if (tecla == 72 && seleccionado > 0) { // Flecha arriba
                seleccionado--;
            }
            else if (tecla == 80 && seleccionado < totalOpciones - 1) { // Flecha abajo
                seleccionado++;
            }
        }
        else if (tecla == 13) { // Enter
            switch (seleccionado) {
            case 0: { // Árbol AVL
                arbolABL();
                break;
            }
            case 1: { // Árbol B
                BTreeNode* raiz = nullptr;
                menu(raiz);
                break;
            }
            case 2: {
                arbolABB();
                break;
            }
            case 3: {
                mostrarMenu();
                break;
            }
            case 4: // Salir
                
                return 0;
            }
        }
        else if (tecla == 27) { // ESC
            return 0;
        }
    }

    return 0;
}

// [El resto del código permanece igual]

struct NodoVisual {
    float centroX;
    float centroY;
};

int contarHojas(BTreeNode* nodo) {
    if (!nodo) return 0;
    if (nodo->hoja) return 1;

    int total = 0;
    for (int i = 0; i <= nodo->n; ++i) {
        total += contarHojas(nodo->hijos[i]);
    }
    return total;
}

float dibujarNodo(sf::RenderWindow& ventana, BTreeNode* nodo, float x, float y, float& nextX,
    std::map<BTreeNode*, NodoVisual>& posiciones, BTreeNode* nodoResaltado = nullptr, int indiceClaveResaltada = -1, float espacioY = 160) {

    if (!nodo) return 0;

    const float altoNodo = 100;
    const float anchoColumna = 80;
    const float espacioEntreColumnas = 5;

    static sf::Font font;
    static bool fontCargada = false;
    if (!fontCargada) {
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            std::cout << "Error al cargar la fuente desde el sistema\n";
        }
        fontCargada = true;
    }

    float anchoNodo = nodo->n > 0 ? nodo->n * anchoColumna + (nodo->n - 1) * espacioEntreColumnas : anchoColumna;

    float centroX;

    if (nodo->hoja) {
        centroX = nextX + anchoNodo / 2;
        nextX += anchoNodo + 30;
    }
    else {
        centroX = 0;
        for (int i = 0; i <= nodo->n; ++i) {
            centroX += dibujarNodo(ventana, nodo->hijos[i], x, y + espacioY, nextX, posiciones, nodoResaltado, indiceClaveResaltada, espacioY);
        }
        centroX /= (nodo->n + 1);
    }

    posiciones[nodo] = { centroX, y };

    // Dibuja fondo general del nodo (solo borde, sin relleno para no tapar columnas resaltadas)
    sf::RectangleShape rect(sf::Vector2f(anchoNodo, altoNodo));
    rect.setPosition(centroX - anchoNodo / 2, y);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    ventana.draw(rect);

    // Dibuja la dirección de memoria del nodo en la parte superior central
    {
        std::stringstream ss;
        ss << "Nodo: " << nodo;
        sf::Text direccionNodo(ss.str(), font, 8);
        direccionNodo.setPosition(centroX - (anchoNodo / 2), y - 15);
        direccionNodo.setFillColor(sf::Color::Blue);
        ventana.draw(direccionNodo);
    }

    // Dibuja cada columna (clave) con su propio rectángulo, para resaltar solo la deseada
    for (int i = 0; i < nodo->n; ++i) {
        const Recibo& r = nodo->claves[i];
        float baseX = centroX - anchoNodo / 2 + i * (anchoColumna + espacioEntreColumnas);
        float baseY = y;

        sf::RectangleShape columnaRect(sf::Vector2f(anchoColumna, altoNodo));
        columnaRect.setPosition(baseX, baseY);

        if (nodo == nodoResaltado && i == indiceClaveResaltada) {
            columnaRect.setFillColor(sf::Color(255, 230, 230));  // Rosado para clave resaltada
            columnaRect.setOutlineColor(sf::Color::Red);
            columnaRect.setOutlineThickness(3);
        }
        else {
            columnaRect.setFillColor(sf::Color(230, 250, 255));
            columnaRect.setOutlineColor(sf::Color::Black);
            columnaRect.setOutlineThickness(1);
        }

        ventana.draw(columnaRect);

        // Línea divisoria entre columnas (excepto la primera)
        if (i > 0) {
            sf::Vertex lineaSeparacion[] = {
                sf::Vertex(sf::Vector2f(baseX - espacioEntreColumnas / 2, y)),
                sf::Vertex(sf::Vector2f(baseX - espacioEntreColumnas / 2, y + altoNodo))
            };
            ventana.draw(lineaSeparacion, 2, sf::Lines);
        }

        // Dirección de memoria de la clave (columna) justo debajo de cada columna
        {
            std::stringstream ssDir;
            ssDir << nodo->claves + i;
            sf::Text direccionClave(ssDir.str(), font, 8);
            direccionClave.setPosition(baseX + 5, baseY + altoNodo + 2);
            direccionClave.setFillColor(sf::Color::Magenta);
            ventana.draw(direccionClave);
        }

        std::stringstream linea1, linea2, linea3, linea4, linea5;
        linea1 << "ID: " << r.idVenta;
        linea2 << "TIPO: " << r.tipoCafe;
        linea3 << "Q " << r.precioPorKg;
        linea4 << "Total: Q " << r.totalVenta;
        linea5 << "kg: " << r.cantidadKg;

        sf::Text text1(linea1.str(), font, 10); text1.setPosition(baseX + 5, baseY + 5);
        sf::Text text2(linea2.str(), font, 10); text2.setPosition(baseX + 5, baseY + 25);
        sf::Text text3(linea3.str(), font, 10); text3.setPosition(baseX + 5, baseY + 45);
        sf::Text text4(linea4.str(), font, 10); text4.setPosition(baseX + 5, baseY + 65);
        sf::Text text5(linea5.str(), font, 10); text5.setPosition(baseX + 5, baseY + 85);

        for (auto& t : { &text1, &text2, &text3, &text4, &text5 }) {
            t->setFillColor(sf::Color::Black);
            ventana.draw(*t);
        }
    }

    return centroX;
}


void dibujarRamas(sf::RenderWindow& ventana, BTreeNode* nodo, std::map<BTreeNode*, NodoVisual>& posiciones) {
    if (!nodo || nodo->hoja) return;

    const float altoNodo = 100;
    const float anchoColumna = 80;
    const float espacioEntreColumnas = 5;

    auto it = posiciones.find(nodo);
    if (it == posiciones.end()) return;

    float centroXPadre = it->second.centroX;
    float yPadre = it->second.centroY;

    float anchoNodo = nodo->n > 0 ? nodo->n * anchoColumna + (nodo->n - 1) * espacioEntreColumnas : anchoColumna;

    for (int i = 0; i <= nodo->n; ++i) {
        BTreeNode* hijo = nodo->hijos[i];
        if (!hijo) continue;

        auto hit = posiciones.find(hijo);
        if (hit == posiciones.end()) continue;

        float centroXHijo = hit->second.centroX;
        float yHijo = hit->second.centroY;

        float baseLineaX;
        if (i == 0) {
            baseLineaX = centroXPadre - anchoNodo / 2;
        }
        else if (i == nodo->n) {
            baseLineaX = centroXPadre + anchoNodo / 2;
        }
        else {
            baseLineaX = centroXPadre - anchoNodo / 2 + i * (anchoColumna + espacioEntreColumnas) - espacioEntreColumnas / 2;
        }

        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(baseLineaX, yPadre + altoNodo), sf::Color::Black),
            sf::Vertex(sf::Vector2f(centroXHijo, yHijo), sf::Color::Black)
        };
        ventana.draw(linea, 2, sf::Lines);

        dibujarRamas(ventana, hijo, posiciones);
    }
}

void mostrarConSFML(BTreeNode* raiz) {
    sf::RenderWindow ventana(sf::VideoMode(1400, 800), "Árbol B - Recibos de Café");

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear(sf::Color::White);

        float nextX = 50;
        std::map<BTreeNode*, NodoVisual> posiciones;

        dibujarNodo(ventana, raiz, 0, 50, nextX, posiciones);

        dibujarRamas(ventana, raiz, posiciones);

        ventana.display();
    }
}

BTreeNode* crearNodo(bool hoja) {
    BTreeNode* nuevo = new BTreeNode;
    nuevo->hoja = hoja;
    nuevo->n = 0;
    for (int i = 0; i < 2 * T; i++)
        nuevo->hijos[i] = nullptr;
    return nuevo;
}

BTreeNode* buscarNodoConId(BTreeNode* nodo, int idVenta) {
    if (!nodo) return nullptr;

    int i = 0;
    while (i < nodo->n && idVenta > nodo->claves[i].idVenta)
        i++;

    if (i < nodo->n && nodo->claves[i].idVenta == idVenta)
        return nodo;

    if (nodo->hoja)
        return nullptr;

    return buscarNodoConId(nodo->hijos[i], idVenta);
}

void mostrarConSFMLResaltado(BTreeNode* raiz, BTreeNode* nodoResaltado, int indiceClaveResaltada) {
    sf::RenderWindow ventana(sf::VideoMode(1400, 800), "Árbol B - Recibos de Café");

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear(sf::Color::White);

        float nextX = 50;
        std::map<BTreeNode*, NodoVisual> posiciones;

        dibujarNodo(ventana, raiz, 0, 50, nextX, posiciones, nodoResaltado, indiceClaveResaltada);

        dibujarRamas(ventana, raiz, posiciones);

        ventana.display();
    }
}

void mostrarRecibo(const Recibo& r) {
    cout << "\n--- RECIBO DE VENTA DE CAFÉ ---\n";
    cout << "ID Venta: " << r.idVenta << "\n";
    cout << "ID Cliente: " << r.idCliente << "\n";
    cout << "Tipo de Café: " << r.tipoCafe << "\n";
    cout << "Precio por Kg: " << r.precioPorKg << "\n";
    cout << "Cantidad Vendida (kg): " << r.cantidadKg << "\n";
    cout << "Descuento: " << r.descuento << "\n";
    cout << "Total de la Venta: " << r.totalVenta << "\n";
    cout << "Forma de Pago: " << r.formaPago << "\n";
}

void mostrar(BTreeNode* nodo) {
    if (nodo == nullptr) return;
    int i;
    for (i = 0; i < nodo->n; i++) {
        if (!nodo->hoja)
            mostrar(nodo->hijos[i]);
        mostrarRecibo(nodo->claves[i]);
    }
    if (!nodo->hoja)
        mostrar(nodo->hijos[i]);
}

bool existeIdVenta(BTreeNode* nodo, int idVenta) {
    if (!nodo) return false;

    int i = 0;
    while (i < nodo->n && idVenta > nodo->claves[i].idVenta)
        i++;

    if (i < nodo->n && nodo->claves[i].idVenta == idVenta)
        return true;

    if (nodo->hoja)
        return false;

    return existeIdVenta(nodo->hijos[i], idVenta);
}

// Función insertar modificada para evitar duplicados
void insertar(BTreeNode*& raiz, Recibo k) {
    if (existeIdVenta(raiz, k.idVenta)) {
        gotoxy(40, 23); cout << "Error: El idVenta " << k.idVenta << endl;
        gotoxy(40, 24); cout << "ya existe en el arbol. No se puede insertar.\n";
        system("pause");
        return;
    }

    if (raiz == nullptr) {
        raiz = crearNodo(true);
        raiz->claves[0] = k;
        raiz->n = 1;
    }
    else {
        if (raiz->n == 2 * T - 1) {
            BTreeNode* nuevaRaiz = crearNodo(false);
            nuevaRaiz->hijos[0] = raiz;
            dividirHijo(nuevaRaiz, 0, raiz);
            int i = (nuevaRaiz->claves[0].idVenta < k.idVenta) ? 1 : 0;
            insertarNoLleno(nuevaRaiz->hijos[i], k);
            raiz = nuevaRaiz;
        }
        else {
            insertarNoLleno(raiz, k);
        }
    }
}

void insertarNoLleno(BTreeNode* nodo, Recibo k) {
    int i = nodo->n - 1;
    if (nodo->hoja) {
        while (i >= 0 && nodo->claves[i].idVenta > k.idVenta) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = k;
        nodo->n++;
    }
    else {
        while (i >= 0 && nodo->claves[i].idVenta > k.idVenta)
            i--;
        i++;
        if (nodo->hijos[i]->n == 2 * T - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (nodo->claves[i].idVenta < k.idVenta)
                i++;
        }
        insertarNoLleno(nodo->hijos[i], k);
    }
}

void dividirHijo(BTreeNode* padre, int i, BTreeNode* hijo) {
    BTreeNode* nuevo = crearNodo(hijo->hoja);
    nuevo->n = T - 1;
    for (int j = 0; j < T - 1; j++)
        nuevo->claves[j] = hijo->claves[j + T];
    if (!hijo->hoja) {
        for (int j = 0; j < T; j++)
            nuevo->hijos[j] = hijo->hijos[j + T];
    }
    hijo->n = T - 1;
    for (int j = padre->n; j >= i + 1; j--)
        padre->hijos[j + 1] = padre->hijos[j];
    padre->hijos[i + 1] = nuevo;
    for (int j = padre->n - 1; j >= i; j--)
        padre->claves[j + 1] = padre->claves[j];
    padre->claves[i] = hijo->claves[T - 1];
    padre->n++;
}

void buscar(BTreeNode* raiz, int idVenta) {
    BTreeNode* nodo = buscarNodoConId(raiz, idVenta);
    if (!nodo) {
        cout << "Recibo no encontrado.\n";
        system("pause");
        return;
    }
    int indiceClaveResaltada = -1;
    for (int i = 0; i < nodo->n; ++i) {
        if (nodo->claves[i].idVenta == idVenta) {
            indiceClaveResaltada = i;
            break;
        }
    }
    mostrarConSFMLResaltado(raiz, nodo, indiceClaveResaltada);
}

void modificarConSFML(BTreeNode* nodo, int idVenta, BTreeNode* nodoRaiz) {
    if (!nodo) return;

    int i = 0;
    while (i < nodo->n && idVenta > nodo->claves[i].idVenta)
        i++;

    if (i < nodo->n && nodo->claves[i].idVenta == idVenta) {
        Recibo& r = nodo->claves[i];
        int seleccion = 0;
        const int opciones = 7;

        // Posición fija para el menú
        const int xMenu = 20;
        const int yInicio = 5;

        while (true) {
            system("cls");

            // Mostrar título en una posición fija, centrado horizontalmente para ancho 80
            string titulo = "=== MODIFICAR RECIBO - ID: " + to_string(idVenta) + " ===";
            gotoxy((80 - (int)titulo.length()) / 2, 2);
            cout << titulo;

            // Opciones del menú
            string textos[] = {
                "Tipo de Cafe: " + r.tipoCafe,
                "Precio por Kg: Q" + to_string(r.precioPorKg),
                "Cantidad Vendida (kg): " + to_string(r.cantidadKg),
                "Descuento: Q" + to_string(r.descuento),
                "Forma de Pago: " + r.formaPago,
                "Visualizar Cambios",
                "Guardar y salir"
            };

            // Mostrar menú con gotoxy en posición fija xMenu, desde yInicio hacia abajo
            for (int j = 0; j < opciones; j++) {
                gotoxy(xMenu, yInicio + j);
                if (seleccion == j) cout << "> ";
                else cout << "  ";
                cout << textos[j];
            }

            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72) seleccion = (seleccion - 1 + opciones) % opciones;  // Arriba
                else if (tecla == 80) seleccion = (seleccion + 1) % opciones;       // Abajo
            }
            else if (tecla == 13) { // Enter
                if (seleccion == 6) { // Guardar y salir
                    r.totalVenta = (r.precioPorKg * r.cantidadKg) - r.descuento;
                    system("cls");
                    string msg = "Modificaciones guardadas.";
                    gotoxy((80 - (int)msg.length()) / 2, 10);
                    cout << msg;
                    system("pause");
                    break;
                }
                else if (seleccion == 5) { // Visualizar cambios
                    r.totalVenta = (r.precioPorKg * r.cantidadKg) - r.descuento;
                    mostrarConSFML(nodoRaiz);
                }
                else {
                    system("cls");
                    int yInput = 10;
                    switch (seleccion) {
                    case 0:
                        gotoxy(xMenu, yInput);
                        cout << "Nuevo Tipo de Café: ";
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        getline(cin, r.tipoCafe);
                        break;
                    case 1:
                        gotoxy(xMenu, yInput);
                        cout << "Nuevo Precio por Kg: ";
                        cin >> r.precioPorKg;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    case 2:
                        gotoxy(xMenu, yInput);
                        cout << "Nueva Cantidad Vendida (kg): ";
                        cin >> r.cantidadKg;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    case 3:
                        gotoxy(xMenu, yInput);
                        cout << "Nuevo Descuento: ";
                        cin >> r.descuento;
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    case 4:
                        gotoxy(xMenu, yInput);
                        cout << "Nueva Forma de Pago: ";
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        getline(cin, r.formaPago);
                        break;
                    }
                    r.totalVenta = (r.precioPorKg * r.cantidadKg) - r.descuento;

                    system("cls");
                    string msg = "Campo modificado correctamente.";
                    gotoxy((80 - (int)msg.length()) / 2, 10);
                    cout << msg;
                    system("pause");
                }
            }
        }
        return;
    }

    if (nodo->hoja) {
        system("cls");
        string msg = "Recibo no encontrado.";
        gotoxy((80 - (int)msg.length()) / 2, 10);
        cout << msg;
        system("pause");
        return;
    }

    modificarConSFML(nodo->hijos[i], idVenta, nodoRaiz);
}

void vaciarArbol(BTreeNode*& nodo) {
    if (!nodo) return;
    if (!nodo->hoja)
        for (int i = 0; i <= nodo->n; i++)
            vaciarArbol(nodo->hijos[i]);
    delete nodo;
    nodo = nullptr;
}

// --- FUNCIONES DE ELIMINACIÓN COMPLETA ---
Recibo getPredecesor(BTreeNode* nodo, int idx) {
    BTreeNode* actual = nodo->hijos[idx];
    while (!actual->hoja)
        actual = actual->hijos[actual->n];
    return actual->claves[actual->n - 1];
}

Recibo getSucesor(BTreeNode* nodo, int idx) {
    BTreeNode* actual = nodo->hijos[idx + 1];
    while (!actual->hoja)
        actual = actual->hijos[0];
    return actual->claves[0];
}

void fusionar(BTreeNode* nodo, int idx) {
    BTreeNode* hijo = nodo->hijos[idx];
    BTreeNode* hermano = nodo->hijos[idx + 1];
    hijo->claves[T - 1] = nodo->claves[idx];
    for (int i = 0; i < hermano->n; ++i)
        hijo->claves[i + T] = hermano->claves[i];
    if (!hijo->hoja)
        for (int i = 0; i <= hermano->n; ++i)
            hijo->hijos[i + T] = hermano->hijos[i];
    for (int i = idx + 1; i < nodo->n; ++i)
        nodo->claves[i - 1] = nodo->claves[i];
    for (int i = idx + 2; i <= nodo->n; ++i)
        nodo->hijos[i - 1] = nodo->hijos[i];
    hijo->n += hermano->n + 1;
    nodo->n--;
    delete hermano;
}

void llenar(BTreeNode* nodo, int idx) {
    if (idx != 0 && nodo->hijos[idx - 1]->n >= T)
        pedirPrestadoIzq(nodo, idx);
    else if (idx != nodo->n && nodo->hijos[idx + 1]->n >= T)
        pedirPrestadoDer(nodo, idx);
    else {
        if (idx != nodo->n)
            fusionar(nodo, idx);
        else
            fusionar(nodo, idx - 1);
    }
}

void pedirPrestadoIzq(BTreeNode* nodo, int idx) {
    BTreeNode* hijo = nodo->hijos[idx];
    BTreeNode* hermano = nodo->hijos[idx - 1];
    for (int i = hijo->n - 1; i >= 0; --i)
        hijo->claves[i + 1] = hijo->claves[i];
    if (!hijo->hoja)
        for (int i = hijo->n; i >= 0; --i)
            hijo->hijos[i + 1] = hijo->hijos[i];
    hijo->claves[0] = nodo->claves[idx - 1];
    if (!hijo->hoja)
        hijo->hijos[0] = hermano->hijos[hermano->n];
    nodo->claves[idx - 1] = hermano->claves[hermano->n - 1];
    hijo->n += 1;
    hermano->n -= 1;
}

void pedirPrestadoDer(BTreeNode* nodo, int idx) {
    BTreeNode* hijo = nodo->hijos[idx];
    BTreeNode* hermano = nodo->hijos[idx + 1];
    hijo->claves[hijo->n] = nodo->claves[idx];
    if (!hijo->hoja)
        hijo->hijos[hijo->n + 1] = hermano->hijos[0];
    nodo->claves[idx] = hermano->claves[0];
    for (int i = 1; i < hermano->n; ++i)
        hermano->claves[i - 1] = hermano->claves[i];
    if (!hermano->hoja)
        for (int i = 1; i <= hermano->n; ++i)
            hermano->hijos[i - 1] = hermano->hijos[i];
    hijo->n += 1;
    hermano->n -= 1;
}

void eliminarRecibo(BTreeNode*& nodo, int idVenta, BTreeNode* raiz) {
    if (!nodo) return;
    int idx = 0;
    while (idx < nodo->n && nodo->claves[idx].idVenta < idVenta) idx++;

    if (idx < nodo->n && nodo->claves[idx].idVenta == idVenta) {
        // Mostrar antes de eliminar resaltando este nodo y clave
        mostrarYEsperar(raiz, nodo, idx);

        if (nodo->hoja) {
            // Eliminar clave desplazando
            for (int i = idx + 1; i < nodo->n; ++i)
                nodo->claves[i - 1] = nodo->claves[i];
            nodo->n--;

            // Mostrar después de eliminar
            mostrarYEsperar(raiz, nodo, -1);
        }
        else {
            if (nodo->hijos[idx]->n >= T) {
                Recibo pred = getPredecesor(nodo, idx);
                nodo->claves[idx] = pred;

                mostrarYEsperar(raiz, nodo, idx);

                eliminarRecibo(nodo->hijos[idx], pred.idVenta, raiz);
            }
            else if (nodo->hijos[idx + 1]->n >= T) {
                Recibo succ = getSucesor(nodo, idx);
                nodo->claves[idx] = succ;

                mostrarYEsperar(raiz, nodo, idx);

                eliminarRecibo(nodo->hijos[idx + 1], succ.idVenta, raiz);
            }
            else {
                fusionar(nodo, idx);

                mostrarYEsperar(raiz, nodo, -1);

                eliminarRecibo(nodo->hijos[idx], idVenta, raiz);
            }
        }
    }
    else {
        if (nodo->hoja) {
            cout << "Recibo no encontrado.\n";
            return;
        }
        bool ultima = (idx == nodo->n);
        if (nodo->hijos[idx]->n < T)
            llenar(nodo, idx);

        mostrarYEsperar(raiz, nodo->hijos[idx], -1);

        if (ultima && idx > nodo->n)
            eliminarRecibo(nodo->hijos[idx - 1], idVenta, raiz);
        else
            eliminarRecibo(nodo->hijos[idx], idVenta, raiz);
    }

    if (nodo->n == 0) {
        BTreeNode* temp = nodo;
        if (nodo->hoja)
            nodo = nullptr;
        else
            nodo = nodo->hijos[0];
        delete temp;

        mostrarYEsperar(raiz, nullptr, -1);
    }
}

void mostrarYEsperar(BTreeNode* raiz, BTreeNode* nodoResaltado, int indiceClaveResaltada) {
    mostrarConSFMLResaltado(raiz, nodoResaltado, indiceClaveResaltada);
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void menu(BTreeNode*& raiz) {
    const int opciones = 7;
    string textos[opciones] = {
        "Insertar Recibo", "Mostrar Recibos", "Buscar Recibo",
        "Modificar Recibo", "Eliminar Recibo", "Vaciar Arbol", "Salir"
    };
    int seleccionado = 0;
    const int xMenu = 45;
    const int yInicio = 10;

    while (true) {
        system("cls");

        MostrarArteComplejo();

        gotoxy(xMenu, yInicio - 2);
        cout << "MENU B-TREE";

        for (int i = 0; i < opciones; i++) {
            gotoxy(xMenu, yInicio + i);
            if (i == seleccionado)
                cout << "* " << textos[i];
            else
                cout << "  " << textos[i];
        }

        char tecla = _getch();
        if (tecla == 72 && seleccionado > 0) seleccionado--;
        else if (tecla == 80 && seleccionado < opciones - 1) seleccionado++;
        else if (tecla == 13) {
            system("cls");

            // Función para verificar si el árbol está vacío
            bool arbolVacio = (raiz == nullptr);

            if (seleccionado == 0) {
                // Insertar siempre está permitido
                insertarReciboUI(raiz);
            }
            else if (seleccionado == 1) {
                if (arbolVacio) {
                    gotoxy(xMenu, yInicio); cout << "El árbol está vacío. No hay recibos para mostrar.";
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
                else {
                    mostrarConSFML(raiz);
                }
            }
            else if (seleccionado == 2) {
                if (arbolVacio) {
                    gotoxy(xMenu, yInicio); cout << "El arbol esta vacio. No se puede buscar.";
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
                else {
                    int id;
                    gotoxy(xMenu, yInicio); cout << "ID de Venta a buscar: "; cin >> id;
                    buscar(raiz, id);
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
            }
            else if (seleccionado == 3) {
                if (arbolVacio) {
                    gotoxy(xMenu, yInicio); cout << "El arbol está vacio. No se puede modificar.";
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
                else {
                    int id;
                    gotoxy(xMenu, yInicio); cout << "ID de Venta a modificar: "; cin >> id;
                    modificarConSFML(raiz, id, raiz);
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
            }
            else if (seleccionado == 4) {
                if (arbolVacio) {
                    gotoxy(xMenu, yInicio); cout << "El arbol esta vacio. No se puede eliminar.";
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
                else {
                    int id;
                    gotoxy(xMenu, yInicio); cout << "ID de Venta a eliminar: "; cin >> id;
                    eliminarRecibo(raiz, id, raiz);
                    gotoxy(xMenu, yInicio + 2); cout << "Proceso de eliminación finalizado.";
                    gotoxy(xMenu, yInicio + 3); system("pause");
                }
            }
            else if (seleccionado == 5) {
                if (arbolVacio) {
                    gotoxy(xMenu, yInicio); cout << "El arbol ya esta vacio.";
                    gotoxy(xMenu, yInicio + 2); system("pause");
                }
                else {
                    vaciarArbol(raiz);
                    gotoxy(xMenu, yInicio); cout << "Arbol vaciado.";
                    gotoxy(xMenu, yInicio + 1); system("pause");
                }
            }
            else if (seleccionado == 6) {
                break;
            }
        }
    }
}

void dibujarCuadro(int x1, int y1, int x2, int y2) {
    for (int x = x1 + 1; x < x2; ++x) {
        gotoxy(x, y1); cout << char(205);
        gotoxy(x, y2); cout << char(205);
    }
    for (int y = y1 + 1; y < y2; ++y) {
        gotoxy(x1, y); cout << char(186);
        gotoxy(x2, y); cout << char(186);
    }
    gotoxy(x1, y1); cout << char(201);
    gotoxy(x2, y1); cout << char(187);
    gotoxy(x1, y2); cout << char(200);
    gotoxy(x2, y2); cout << char(188);
}

void insertarReciboUI(BTreeNode*& raiz) {
    Recibo r;
    const int x1 = 30, y1 = 4, x2 = 110, y2 = 22;

    system("cls");
    dibujarCuadro(x1, y1, x2, y2);

    for (int x = x1 + 1; x < x2; ++x) {
        gotoxy(x, y1 + 2); cout << char(205);
    }
    gotoxy(x1, y1 + 2); cout << char(204);
    gotoxy(x2, y1 + 2); cout << char(185);

    int tituloX = (x1 + x2) / 2 - 13;
    gotoxy(tituloX, y1 + 1); cout << "INGRESO DE RECIBO DE VENTA";

    int y = y1 + 4;
    gotoxy(x1 + 2, y);   cout << "ID Venta:";                gotoxy(x1 + 20, y++); cin >> r.idVenta;
    gotoxy(x1 + 2, y);   cout << "ID Cliente:";              gotoxy(x1 + 20, y++); cin >> r.idCliente;
    cin.ignore();

    gotoxy(x1 + 2, y);   cout << "Tipo de Cafe:";            gotoxy(x1 + 20, y++); getline(cin, r.tipoCafe);
    gotoxy(x1 + 2, y);   cout << "Precio por Kg:";           gotoxy(x1 + 20, y++); cin >> r.precioPorKg;
    gotoxy(x1 + 2, y);   cout << "Cantidad Vendida (kg):";   gotoxy(x1 + 28, y++); cin >> r.cantidadKg;
    gotoxy(x1 + 2, y);   cout << "Descuento:";               gotoxy(x1 + 20, y++); cin >> r.descuento;
    r.totalVenta = (r.precioPorKg * r.cantidadKg) - r.descuento;
    cin.ignore();

    gotoxy(x1 + 2, y);   cout << "Forma de Pago:";           gotoxy(x1 + 20, y++); getline(cin, r.formaPago);

    dibujarCuadro(x1 + 1, y + 1, x2 - 1, y + 4);
    gotoxy(x1 + 4, y + 2); cout << "Total calculado: Q " << fixed << setprecision(2) << r.totalVenta;

    insertar(raiz, r);
    gotoxy(x1 + 4, y + 5); cout << "\xFE Recibo insertado correctamente.";
    gotoxy(x1 + 4, y + 6); system("pause");
}

void MostrarArteComplejo() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 1); // Verde claro
    cout << ".                                            . . . ." << endl;
    SetConsoleTextAttribute(hConsole, 2); // Verde claro
    cout << "     *   .                  .              .        .   *          .  ,   ..   .  .  ." << endl;
    cout << "  .         .                     .       .           .      .        ." << endl;
    cout << "        o                             .                   .               . ,. ,. ,. ," << endl;
    cout << "         .              .                  .           .   . .  . .. . .  . . , , .  ,." << endl;
    cout << "          0     ." << endl;
    cout << "                 .          .                 ,                ,     ,    .   ,  . .  .   ," << endl;
    cout << " .          \\          .                         .                               " << endl;
    cout << "      .      \\   ," << endl;
    cout << "   .          o     .                 .                   .          .   .               " << endl;
    cout << "     .         \\                 ,             .                .         .     " << endl;
    cout << "               #\\##\\#      .                              .        .       .    " << endl;
    cout << "             #  #O##\\###                .                        .            .   " << endl;
    cout << "   .        #*#  #\\##\\###                       .                               . ," << endl;
    cout << "        .   ##*#  #\\##\\##               .                     .        . " << endl;
    cout << "      .      ##*#  #o##\\#         .                             ,  .  .     .." << endl;
    cout << "          .     *#  #\\#     .                    .             .          ," << endl;
    cout << "                      \\          .                         .       .      .  .´" << endl;
    cout << "____^/\\___^--____/\\____O______________/\\/\\---/\\___________---______________" << endl;
    cout << "   /\\^   ^  ^    ^                  ^^ ^  '\\ ^          ^       ---" << endl;
    cout << "         --           -            --  -      -         ---  __       ^" << endl;
    cout << "   --  __                      ___--  ^  ^                         --  __" << endl;
}





using namespace std;
// ==== ESTRUCTURA DE CAFÉ Y AVL ====

struct Cafe {
    string nombre;
    int id;
    string tipo;
    string fechaCompra;
    float precioPorKg, cantidadComprada;
    bool esSocio;
    float total;

};

struct NodoAVL {
    Cafe cafe;
    NodoAVL* izquierda = nullptr;
    NodoAVL* derecha = nullptr;
    int altura = 1;
};

// ==== FUNCIONES AVL ====

int altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int contarNodos(NodoAVL* nodo) {
    if (!nodo) return 0;
    return 1 + contarNodos(nodo->izquierda) + contarNodos(nodo->derecha);
}

int balanceFactor(NodoAVL* nodo) {
    return nodo ? altura(nodo->izquierda) - altura(nodo->derecha) : 0;
}

void actualizarAltura(NodoAVL* nodo) {
    nodo->altura = 1 + max(altura(nodo->izquierda), altura(nodo->derecha));
}

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

NodoAVL* insertar(NodoAVL* nodo, Cafe cafe) {
    if (!nodo) {
        NodoAVL* nuevo = new NodoAVL{ cafe };
        return nuevo;
    }

    if (cafe.id < nodo->cafe.id)
        nodo->izquierda = insertar(nodo->izquierda, cafe);
    else if (cafe.id > nodo->cafe.id)
        nodo->derecha = insertar(nodo->derecha, cafe);
    else
        return nodo; // ID duplicado no permitido

    actualizarAltura(nodo);

    int balance = balanceFactor(nodo);

    if (balance > 1 && cafe.id < nodo->izquierda->cafe.id)
        return rotarDerecha(nodo);
    if (balance < -1 && cafe.id > nodo->derecha->cafe.id)
        return rotarIzquierda(nodo);
    if (balance > 1 && cafe.id > nodo->izquierda->cafe.id) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && cafe.id < nodo->derecha->cafe.id) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// ==== RECORRIDOS ====

void preorden(NodoAVL* nodo, vector<Cafe>& resultado) {
    if (!nodo) return;
    resultado.push_back(nodo->cafe);
    preorden(nodo->izquierda, resultado);
    preorden(nodo->derecha, resultado);
}

void inorden(NodoAVL* nodo, vector<Cafe>& resultado) {
    if (!nodo) return;
    inorden(nodo->izquierda, resultado);
    resultado.push_back(nodo->cafe);
    inorden(nodo->derecha, resultado);
}

void postorden(NodoAVL* nodo, vector<Cafe>& resultado) {
    if (!nodo) return;
    postorden(nodo->izquierda, resultado);
    postorden(nodo->derecha, resultado);
    resultado.push_back(nodo->cafe);
}

// ==== DIBUJAR ÁRBOL ====

void dibujarNodo(sf::RenderWindow& window, NodoAVL* nodo, sf::Font& font, float x, float y, float offsetX, float offsetY, int totalNodos, int idResaltado = -1) {
    if (!nodo) return;

    float ancho = 180;
    float alto = 80;

    // Cambiar el color del nodo si es el que se debe resaltar
    sf::Color fillColor = (nodo->cafe.id == idResaltado) ? sf::Color(255, 140, 0) : sf::Color(100, 149, 237);

    sf::RectangleShape rect(sf::Vector2f(ancho, alto));
    rect.setFillColor(fillColor);
    rect.setOutlineThickness(2.0f);
    rect.setOutlineColor(sf::Color::Black);
    rect.setPosition(x - ancho / 2, y);

    stringstream ss;
    ss << nodo;
    string direccionMemoria = ss.str();

    string textoMostrar = "Nombre: " + nodo->cafe.nombre.substr(0, 8) +
        "\nID: " + to_string(nodo->cafe.id) +
        "\nDir: " + direccionMemoria.substr(direccionMemoria.size() - 4);

    sf::Text texto(textoMostrar, font, 12);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(x - ancho / 2 + 5, y + 5);

    window.draw(rect);
    window.draw(texto);

    float espaciadoHorizontal = offsetX;
    float espaciadoVertical = offsetY;

    if (nodo->izquierda) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(x, y + alto)),
            sf::Vertex(sf::Vector2f(x - espaciadoHorizontal + ancho / 2, y + espaciadoVertical))
        };
        window.draw(linea, 2, sf::Lines);
        dibujarNodo(window, nodo->izquierda, font, x - espaciadoHorizontal, y + espaciadoVertical,
            offsetX / 1.8f, offsetY, totalNodos, idResaltado);
    }

    if (nodo->derecha) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(x, y + alto)),
            sf::Vertex(sf::Vector2f(x + espaciadoHorizontal - ancho / 2, y + espaciadoVertical))
        };
        window.draw(linea, 2, sf::Lines);
        dibujarNodo(window, nodo->derecha, font, x + espaciadoHorizontal, y + espaciadoVertical,
            offsetX / 1.8f, offsetY, totalNodos, idResaltado);
    }
}


// ==== DIBUJAR RECORRIDOS ====

void dibujarRecorrido(sf::RenderWindow& window, const vector<Cafe>& recorrido, sf::Font& font, float startX, float startY, const std::string& tituloStr) {
    float posX = startX;
    float posY = startY;

    sf::Text titulo(tituloStr, font, 20);
    titulo.setFillColor(sf::Color::Black);
    titulo.setPosition(posX, posY);
    window.draw(titulo);

    posY += 30;

    for (const auto& cafe : recorrido) {
        // Simular dirección de memoria para el recorrido
        std::stringstream ss;
        ss << "0x" << std::hex << (cafe.id * 12345);
        std::string direccionMemoria = ss.str().substr(0, 6) + "...";

        sf::RectangleShape rect(sf::Vector2f(180, 60));
        rect.setFillColor(sf::Color(100, 149, 237));
        rect.setOutlineThickness(1.0f);
        rect.setOutlineColor(sf::Color::Black);
        rect.setPosition(posX, posY);

        std::string textoStr = "Nombre: " + cafe.nombre.substr(0, 8) +
            "\nID: " + std::to_string(cafe.id) +
            "\nDir: " + direccionMemoria;
        sf::Text texto(textoStr, font, 12);
        texto.setFillColor(sf::Color::White);
        texto.setPosition(posX + 5, posY + 5);

        window.draw(rect);
        window.draw(texto);

        posX += 190;

        if (posX > 1500) {
            posX = startX;
            posY += 70;
        }
    }
}

// ==== INTERFAZ GRÁFICA ====

struct Boton {
    sf::RectangleShape rect;
    sf::Text texto;
    bool activo = false;
};

Cafe* buscar(NodoAVL* nodo, int id) {
    if (!nodo) return nullptr; // Si el nodo es nulo, no se encontró
    if (nodo->cafe.id == id) return &nodo->cafe; // Si se encuentra el café, devolverlo
    if (id < nodo->cafe.id) return buscar(nodo->izquierda, id); // Buscar en el subárbol izquierdo
    return buscar(nodo->derecha, id); // Buscar en el subárbol derecho
}

void buscarVisual(sf::RenderWindow& window, NodoAVL* raiz, sf::Font& font) {
    std::string input;
    int idBuscado = -1;
    Cafe* cafeEncontrado = nullptr;
    bool mostrandoResultado = false;
    bool busquedaInvalida = false;
    bool inputActivo = true;

    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 560);

    sf::Text etiqueta("Ingrese ID a buscar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 530);

    sf::Text textoError("", font, 16);
    textoError.setFillColor(sf::Color::Red);
    textoError.setPosition(30, 590);

    sf::RectangleShape botonBuscar(sf::Vector2f(120, 40));
    botonBuscar.setFillColor(sf::Color(50, 150, 50));
    botonBuscar.setPosition(200, 560);

    sf::Text textoBuscar("Buscar", font, 18);
    textoBuscar.setFillColor(sf::Color::White);
    textoBuscar.setPosition(220, 565);

    sf::RectangleShape panelResultado(sf::Vector2f(500, 300));
    panelResultado.setFillColor(sf::Color(240, 240, 240));
    panelResultado.setOutlineThickness(2);
    panelResultado.setOutlineColor(sf::Color(100, 149, 237));
    panelResultado.setPosition(50, 200);

    sf::RectangleShape botonRegresar(sf::Vector2f(120, 40));
    botonRegresar.setFillColor(sf::Color(200, 50, 50));
    botonRegresar.setPosition(250, 480);

    sf::Text textoRegresar("Regresar", font, 18);
    textoRegresar.setFillColor(sf::Color::White);
    textoRegresar.setPosition(270, 485);

    // NUEVO: Botón de regreso al menú principal
    sf::RectangleShape botonMenu(sf::Vector2f(160, 40));
    botonMenu.setFillColor(sf::Color(100, 100, 100));
    botonMenu.setPosition(420, 560);

    sf::Text textoMenu("Menú Principal", font, 18);
    textoMenu.setFillColor(sf::Color::White);
    textoMenu.setPosition(430, 565);

    sf::Clock cursorClock;
    bool showCursor = true;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;

                    if (!mostrandoResultado) {
                        if (textoInput.getGlobalBounds().contains(mx, my)) {
                            inputActivo = true;
                        }
                        else {
                            inputActivo = false;
                        }

                        if (botonBuscar.getGlobalBounds().contains(mx, my)) {
                            if (!input.empty()) {
                                try {
                                    idBuscado = std::stoi(input);
                                    cafeEncontrado = buscar(raiz, idBuscado);
                                    mostrandoResultado = true;
                                    busquedaInvalida = false;
                                }
                                catch (...) {
                                    textoError.setString("ID inválido. Solo números enteros.");
                                    busquedaInvalida = true;
                                }
                            }
                            else {
                                textoError.setString("Ingrese un ID para buscar");
                                busquedaInvalida = true;
                            }
                        }

                        if (botonMenu.getGlobalBounds().contains(mx, my)) {
                            return; // Volver al menú principal
                        }
                    }
                    else {
                        if (botonRegresar.getGlobalBounds().contains(mx, my)) {
                            mostrandoResultado = false;
                            input.clear();
                            textoInput.setString(input);
                            idBuscado = -1;
                            cafeEncontrado = nullptr;
                            inputActivo = true;
                            busquedaInvalida = false;
                            cursorClock.restart();
                        }
                    }
                }
            }
            else if (e.type == sf::Event::TextEntered && inputActivo && !mostrandoResultado) {
                if (e.text.unicode == 8 && !input.empty()) { // Backspace
                    input.pop_back();
                    textoInput.setString(input);
                    busquedaInvalida = false;
                }
                else if (e.text.unicode >= 48 && e.text.unicode <= 57) { // Digitos
                    input += static_cast<char>(e.text.unicode);
                    textoInput.setString(input);
                    busquedaInvalida = false;
                }
                textoError.setString("");
            }
        }

        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }

        window.clear(sf::Color(220, 220, 220));

        int totalNodos = contarNodos(raiz);
        dibujarNodo(window, raiz, font, 1000, 100, 250, 100, totalNodos, (cafeEncontrado ? cafeEncontrado->id : -1));

        if (!mostrandoResultado) {
            window.draw(etiqueta);

            sf::RectangleShape campoInput(sf::Vector2f(150, 30));
            campoInput.setFillColor(inputActivo ? sf::Color(200, 230, 255) : sf::Color::White);
            campoInput.setOutlineThickness(inputActivo ? 2 : 1);
            campoInput.setOutlineColor(inputActivo ? sf::Color::Blue : sf::Color(150, 150, 150));
            campoInput.setPosition(30, 555);
            window.draw(campoInput);

            window.draw(textoInput);
            window.draw(botonBuscar);
            window.draw(textoBuscar);
            window.draw(botonMenu);
            window.draw(textoMenu);

            if (showCursor && inputActivo) {
                sf::RectangleShape cursor(sf::Vector2f(2, 20));
                cursor.setFillColor(sf::Color::Black);
                cursor.setPosition(35 + textoInput.getLocalBounds().width, 560);
                window.draw(cursor);
            }

            if (busquedaInvalida) {
                window.draw(textoError);
            }
        }
        else {
            window.draw(panelResultado);

            if (cafeEncontrado) {
                sf::Text tituloResultado("Café Encontrado", font, 24);
                tituloResultado.setFillColor(sf::Color(100, 149, 237));
                tituloResultado.setPosition(70, 210);
                window.draw(tituloResultado);

                std::stringstream precioStream, totalStream;
                precioStream << std::fixed << std::setprecision(2) << cafeEncontrado->precioPorKg;
                totalStream << std::fixed << std::setprecision(2) << cafeEncontrado->total;

                std::string detalles =
                    "Nombre: " + cafeEncontrado->nombre + "\n" +
                    "ID: " + std::to_string(cafeEncontrado->id) + "\n" +
                    "Tipo: " + cafeEncontrado->tipo + "\n" +
                    "Fecha: " + cafeEncontrado->fechaCompra + "\n" +
                    "Precio/kg: $" + precioStream.str() + "\n" +
                    "Cantidad: " + std::to_string(cafeEncontrado->cantidadComprada) + " kg\n" +
                    "Socio: " + (cafeEncontrado->esSocio ? "Sí" : "No") + "\n" +
                    "Total: $" + totalStream.str();

                sf::Text textoDetalles(detalles, font, 18);
                textoDetalles.setFillColor(sf::Color::Black);
                textoDetalles.setPosition(70, 250);
                window.draw(textoDetalles);
            }
            else {
                sf::Text textoNoEncontrado("No se encontró un café con ID: " + std::to_string(idBuscado), font, 20);
                textoNoEncontrado.setFillColor(sf::Color::Red);
                textoNoEncontrado.setPosition(70, 250);
                window.draw(textoNoEncontrado);
            }

            window.draw(botonRegresar);
            window.draw(textoRegresar);
        }

        window.display();
    }
}

NodoAVL* minimoNodo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual->izquierda != nullptr)
        actual = actual->izquierda;
    return actual;
}

NodoAVL* eliminar(NodoAVL* nodo, int id) {
    if (!nodo) return nodo;
    if (id < nodo->cafe.id)
        nodo->izquierda = eliminar(nodo->izquierda, id);
    else if (id > nodo->cafe.id)
        nodo->derecha = eliminar(nodo->derecha, id);
    else {
        // Nodo con un hijo o sin hijos
        if (nodo->izquierda == nullptr) {
            NodoAVL* temp = nodo->derecha;
            delete nodo;
            return temp;
        }
        else if (nodo->derecha == nullptr) {
            NodoAVL* temp = nodo->izquierda;
            delete nodo;
            return temp;
        }
        // Nodo con dos hijos, obtener el sucesor inorder (menor en el subárbol derecho)
        NodoAVL* temp = minimoNodo(nodo->derecha);
        // Copiar los datos del sucesor al nodo actual
        nodo->cafe = temp->cafe;
        // Eliminar el sucesor
        nodo->derecha = eliminar(nodo->derecha, temp->cafe.id);
    }
    actualizarAltura(nodo);
    int balance = balanceFactor(nodo);
    // Balancear el nodo
    if (balance > 1 && balanceFactor(nodo->izquierda) >= 0)
        return rotarDerecha(nodo);
    if (balance > 1 && balanceFactor(nodo->izquierda) < 0) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && balanceFactor(nodo->derecha) <= 0)
        return rotarIzquierda(nodo);
    if (balance < -1 && balanceFactor(nodo->derecha) > 0) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

void eliminarVisual(sf::RenderWindow& window, NodoAVL*& raiz, sf::Font& font) {
    std::string input;
    int idBuscado = -1;
    Cafe* cafeEncontrado = nullptr;
    bool mostrandoResultado = false;
    bool busquedaInvalida = false;
    bool inputActivo = true;

    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 560);

    sf::Text etiqueta("Ingrese ID a eliminar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 530);

    sf::Text textoError("", font, 16);
    textoError.setFillColor(sf::Color::Red);
    textoError.setPosition(30, 590);

    sf::RectangleShape botonBuscar(sf::Vector2f(120, 40));
    botonBuscar.setFillColor(sf::Color(50, 150, 50));
    botonBuscar.setPosition(200, 560);

    sf::Text textoBuscar("Buscar", font, 18);
    textoBuscar.setFillColor(sf::Color::White);
    textoBuscar.setPosition(220, 565);

    sf::RectangleShape panelResultado(sf::Vector2f(500, 300));
    panelResultado.setFillColor(sf::Color(240, 240, 240));
    panelResultado.setOutlineThickness(2);
    panelResultado.setOutlineColor(sf::Color(100, 149, 237));
    panelResultado.setPosition(50, 200);

    sf::RectangleShape botonEliminar(sf::Vector2f(120, 40));
    botonEliminar.setFillColor(sf::Color(178, 34, 34));
    botonEliminar.setPosition(50, 510);

    sf::Text textoEliminar("Eliminar", font, 18);
    textoEliminar.setFillColor(sf::Color::White);
    textoEliminar.setPosition(85, 515);

    sf::RectangleShape botonRegresar(sf::Vector2f(120, 40));
    botonRegresar.setFillColor(sf::Color(200, 50, 50));
    botonRegresar.setPosition(250, 510);

    sf::Text textoRegresar("Regresar", font, 18);
    textoRegresar.setFillColor(sf::Color::White);
    textoRegresar.setPosition(275, 515);

    sf::RectangleShape botonMenu(sf::Vector2f(160, 40));
    botonMenu.setFillColor(sf::Color(100, 100, 100));
    botonMenu.setPosition(420, 510);

    sf::Text textoMenu("Menú Principal", font, 18);
    textoMenu.setFillColor(sf::Color::White);
    textoMenu.setPosition(435, 515);

    sf::Clock cursorClock;
    bool showCursor = true;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;

                    if (!mostrandoResultado) {
                        if (textoInput.getGlobalBounds().contains(mx, my)) {
                            inputActivo = true;
                        }
                        else {
                            inputActivo = false;
                        }

                        if (botonBuscar.getGlobalBounds().contains(mx, my)) {
                            if (!input.empty()) {
                                try {
                                    idBuscado = std::stoi(input);
                                    cafeEncontrado = buscar(raiz, idBuscado);
                                    mostrandoResultado = true;
                                    busquedaInvalida = false;
                                }
                                catch (...) {
                                    textoError.setString("ID inválido. Solo números enteros.");
                                    busquedaInvalida = true;
                                }
                            }
                            else {
                                textoError.setString("Ingrese un ID para buscar");
                                busquedaInvalida = true;
                            }
                        }

                        if (botonMenu.getGlobalBounds().contains(mx, my)) {
                            return; // Volver al menú principal
                        }
                    }
                    else {
                        if (botonEliminar.getGlobalBounds().contains(mx, my) && cafeEncontrado != nullptr) {
                            // Eliminar nodo encontrado y actualizar raíz
                            raiz = eliminar(raiz, cafeEncontrado->id);
                            cafeEncontrado = nullptr;
                            mostrandoResultado = false;
                            input.clear();
                            textoInput.setString("");
                            busquedaInvalida = false;
                            inputActivo = true;
                        }
                        if (botonRegresar.getGlobalBounds().contains(mx, my)) {
                            // Volver a la búsqueda, limpiar resultados
                            mostrandoResultado = false;
                            input.clear();
                            textoInput.setString("");
                            cafeEncontrado = nullptr;
                            busquedaInvalida = false;
                            inputActivo = true;
                        }
                    }
                }
            }
            else if (e.type == sf::Event::TextEntered && inputActivo && !mostrandoResultado) {
                if (e.text.unicode == 8 && !input.empty()) { // Backspace
                    input.pop_back();
                    textoInput.setString(input);
                    busquedaInvalida = false;
                }
                else if (e.text.unicode >= 48 && e.text.unicode <= 57) { // Dígitos 0-9
                    input += static_cast<char>(e.text.unicode);
                    textoInput.setString(input);
                    busquedaInvalida = false;
                }
                textoError.setString("");
            }
        }

        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }

        window.clear(sf::Color(220, 220, 220));

        int totalNodos = contarNodos(raiz);
        dibujarNodo(window, raiz, font, 1000, 100, 250, 100, totalNodos, (cafeEncontrado ? cafeEncontrado->id : -1));

        if (!mostrandoResultado) {
            window.draw(etiqueta);

            sf::RectangleShape campoInput(sf::Vector2f(150, 30));
            campoInput.setFillColor(inputActivo ? sf::Color(200, 230, 255) : sf::Color::White);
            campoInput.setOutlineThickness(inputActivo ? 2 : 1);
            campoInput.setOutlineColor(inputActivo ? sf::Color::Blue : sf::Color(150, 150, 150));
            campoInput.setPosition(30, 555);
            window.draw(campoInput);

            window.draw(textoInput);
            window.draw(botonBuscar);
            window.draw(textoBuscar);
            window.draw(botonMenu);
            window.draw(textoMenu);

            if (showCursor && inputActivo) {
                sf::RectangleShape cursor(sf::Vector2f(2, 20));
                cursor.setFillColor(sf::Color::Black);
                cursor.setPosition(35 + textoInput.getLocalBounds().width, 560);
                window.draw(cursor);
            }

            if (busquedaInvalida) {
                window.draw(textoError);
            }
        }
        else {
            window.draw(panelResultado);

            if (cafeEncontrado) {
                sf::Text tituloResultado("Café Encontrado", font, 24);
                tituloResultado.setFillColor(sf::Color(100, 149, 237));
                tituloResultado.setPosition(70, 210);
                window.draw(tituloResultado);

                std::stringstream precioStream, totalStream;
                precioStream << std::fixed << std::setprecision(2) << cafeEncontrado->precioPorKg;
                totalStream << std::fixed << std::setprecision(2) << cafeEncontrado->total;

                std::string detalles =
                    "Nombre: " + cafeEncontrado->nombre + "\n" +
                    "ID: " + std::to_string(cafeEncontrado->id) + "\n" +
                    "Tipo: " + cafeEncontrado->tipo + "\n" +
                    "Fecha: " + cafeEncontrado->fechaCompra + "\n" +
                    "Precio/kg: $" + precioStream.str() + "\n" +
                    "Cantidad: " + std::to_string(cafeEncontrado->cantidadComprada) + " kg\n" +
                    "Socio: " + (cafeEncontrado->esSocio ? "Sí" : "No") + "\n" +
                    "Total: $" + totalStream.str();

                sf::Text textoDetalles(detalles, font, 18);
                textoDetalles.setFillColor(sf::Color::Black);
                textoDetalles.setPosition(70, 250);
                window.draw(textoDetalles);
            }
            else {
                sf::Text textoNoEncontrado("No se encontró un café con ID: " + std::to_string(idBuscado),
                    font, 20);
                textoNoEncontrado.setFillColor(sf::Color::Red);
                textoNoEncontrado.setPosition(70, 250);
                window.draw(textoNoEncontrado);
            }

            window.draw(botonEliminar);
            window.draw(textoEliminar);
            window.draw(botonRegresar);
            window.draw(textoRegresar);
            window.draw(botonMenu);
            window.draw(textoMenu);
        }

        window.display();
    }
}



void modificarVisual(sf::RenderWindow& window, NodoAVL* raiz, sf::Font& font) {
    std::string input;
    int idBuscado = -1;
    Cafe* cafeAModificar = nullptr;
    bool mostrandoFormulario = false;
    bool busquedaInvalida = false;

    // Variables para los valores modificables
    float totalCalculado = 0.0f;

    // Botones para la primera pantalla (búsqueda)
    sf::RectangleShape botonBuscar(sf::Vector2f(120, 40));
    botonBuscar.setFillColor(sf::Color(50, 150, 50));
    botonBuscar.setPosition(200, 560);

    sf::Text textoBuscar("Buscar", font, 18);
    textoBuscar.setFillColor(sf::Color::White);
    textoBuscar.setPosition(220, 565);

    // Botón de regresar en pantalla de búsqueda
    sf::RectangleShape botonRegresarBusqueda(sf::Vector2f(120, 40));
    botonRegresarBusqueda.setFillColor(sf::Color(100, 100, 100));
    botonRegresarBusqueda.setPosition(350, 560);

    sf::Text textoRegresarBusqueda("Regresar", font, 18);
    textoRegresarBusqueda.setFillColor(sf::Color::White);
    textoRegresarBusqueda.setPosition(370, 565);

    // Botones para la segunda pantalla (modificación)
    sf::RectangleShape botonGuardar(sf::Vector2f(120, 40));
    botonGuardar.setFillColor(sf::Color(50, 150, 50));
    botonGuardar.setPosition(250, 650);

    sf::Text textoGuardar("Guardar", font, 18);
    textoGuardar.setFillColor(sf::Color::White);
    textoGuardar.setPosition(270, 655);

    sf::RectangleShape botonCancelar(sf::Vector2f(120, 40));
    botonCancelar.setFillColor(sf::Color(200, 50, 50));
    botonCancelar.setPosition(400, 650);

    sf::Text textoCancelar("Cancelar", font, 18);
    textoCancelar.setFillColor(sf::Color::White);
    textoCancelar.setPosition(420, 655);

    // Botón de regresar en pantalla de modificación
    sf::RectangleShape botonRegresarMod(sf::Vector2f(120, 40));
    botonRegresarMod.setFillColor(sf::Color(100, 100, 100));
    botonRegresarMod.setPosition(550, 650);

    sf::Text textoRegresarMod("Regresar", font, 18);
    textoRegresarMod.setFillColor(sf::Color::White);
    textoRegresarMod.setPosition(570, 655);

    // Campos de texto
    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 560);

    sf::Text etiqueta("ID a modificar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 530);

    // Mensaje de error
    sf::Text textoError("", font, 16);
    textoError.setFillColor(sf::Color::Red);
    textoError.setPosition(30, 590);

    // Campos del formulario
    struct CampoFormulario {
        sf::Text etiqueta;
        sf::Text valor;
        sf::RectangleShape fondo;
        bool esNumerico;
        bool esModificable;
    };

    std::vector<CampoFormulario> campos;
    std::vector<std::string> nombresCampos = {
        "ID:", "Nombre:", "Tipo:", "Fecha compra:",
        "Precio por kg:", "Cantidad:", "Es socio:", "Total:"
    };

    // Inicializar campos del formulario
    for (size_t i = 0; i < nombresCampos.size(); i++) {
        CampoFormulario campo;

        // Configurar etiqueta
        campo.etiqueta.setFont(font);
        campo.etiqueta.setString(nombresCampos[i]);
        campo.etiqueta.setCharacterSize(16);
        campo.etiqueta.setFillColor(sf::Color::Black);
        campo.etiqueta.setPosition(50, 150 + i * 40);

        // Configurar valor
        campo.valor.setFont(font);
        campo.valor.setString("");
        campo.valor.setCharacterSize(16);
        campo.valor.setFillColor(sf::Color::Black);
        campo.valor.setPosition(250, 150 + i * 40);

        // Configurar fondo (para selección)
        campo.fondo.setSize(sf::Vector2f(300, 25));
        campo.fondo.setFillColor(sf::Color::Transparent);
        campo.fondo.setOutlineThickness(0);
        campo.fondo.setPosition(245, 150 + i * 40);

        // Configurar propiedades del campo
        campo.esNumerico = (i == 0 || i == 4 || i == 5); // ID, Precio, Cantidad
        campo.esModificable = (i != 0 && i != 7); // ID y Total no son modificables

        campos.push_back(campo);
    }

    // Variable para controlar qué campo está seleccionado
    int campoSeleccionado = -1;
    std::string inputModificacion;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::TextEntered) {
                if (!mostrandoFormulario) {
                    // Primera pantalla: búsqueda de café
                    if (e.text.unicode == 8 && !input.empty()) { // Backspace
                        input.pop_back();
                        busquedaInvalida = false;
                    }
                    else if (e.text.unicode >= 48 && e.text.unicode <= 57) { // Dígitos
                        input += static_cast<char>(e.text.unicode);
                        busquedaInvalida = false;
                    }
                    textoInput.setString(input);
                    textoError.setString("");
                }
                else {
                    // Segunda pantalla: modificación de campos
                    if (campoSeleccionado >= 0) {
                        if (e.text.unicode == 8 && !inputModificacion.empty()) { // Backspace
                            inputModificacion.pop_back();
                        }
                        else if (e.text.unicode == 13) { // Enter - confirmar cambios
                            try {
                                switch (campoSeleccionado) {
                                case 1: cafeAModificar->nombre = inputModificacion; break;
                                case 2: cafeAModificar->tipo = inputModificacion; break;
                                case 3: cafeAModificar->fechaCompra = inputModificacion; break;
                                case 4: cafeAModificar->precioPorKg = std::stof(inputModificacion); break;
                                case 5: cafeAModificar->cantidadComprada = std::stof(inputModificacion); break;
                                case 6:
                                    cafeAModificar->esSocio = (inputModificacion == "s" || inputModificacion == "S");
                                    campos[6].valor.setString(cafeAModificar->esSocio ? "Sí" : "No");
                                    break;
                                }

                                // Recalcular total si cambió precio, cantidad o socio
                                if (campoSeleccionado == 4 || campoSeleccionado == 5 || campoSeleccionado == 6) {
                                    totalCalculado = cafeAModificar->precioPorKg * cafeAModificar->cantidadComprada;
                                    if (cafeAModificar->esSocio) totalCalculado *= 1.1f;
                                    campos[7].valor.setString(std::to_string(totalCalculado));
                                }

                                if (campoSeleccionado != 6) {
                                    campos[campoSeleccionado].valor.setString(inputModificacion);
                                }
                            }
                            catch (...) {
                                campos[campoSeleccionado].valor.setString("INVALIDO");
                            }

                            campoSeleccionado = -1;
                            inputModificacion = "";
                        }
                        else if (e.text.unicode >= 32) { // Caracteres imprimibles
                            if (campos[campoSeleccionado].esNumerico) {
                                if ((e.text.unicode >= '0' && e.text.unicode <= '9') || e.text.unicode == '.') {
                                    inputModificacion += static_cast<char>(e.text.unicode);
                                }
                            }
                            else {
                                inputModificacion += static_cast<char>(e.text.unicode);
                            }
                        }
                    }
                }
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;

                    if (!mostrandoFormulario) {
                        if (botonBuscar.getGlobalBounds().contains(mx, my)) {
                            if (!input.empty()) {
                                try {
                                    idBuscado = std::stoi(input);
                                    cafeAModificar = buscar(raiz, idBuscado);

                                    if (cafeAModificar) {
                                        // Preparar datos para el formulario
                                        campos[0].valor.setString(std::to_string(cafeAModificar->id));
                                        campos[1].valor.setString(cafeAModificar->nombre);
                                        campos[2].valor.setString(cafeAModificar->tipo);
                                        campos[3].valor.setString(cafeAModificar->fechaCompra);
                                        campos[4].valor.setString(std::to_string(cafeAModificar->precioPorKg));
                                        campos[5].valor.setString(std::to_string(cafeAModificar->cantidadComprada));
                                        campos[6].valor.setString(cafeAModificar->esSocio ? "Sí" : "No");

                                        totalCalculado = cafeAModificar->precioPorKg * cafeAModificar->cantidadComprada;
                                        if (cafeAModificar->esSocio) totalCalculado *= 1.1f;
                                        campos[7].valor.setString(std::to_string(totalCalculado));

                                        mostrandoFormulario = true;
                                        busquedaInvalida = false;
                                    }
                                    else {
                                        textoError.setString("Café no encontrado");
                                        busquedaInvalida = true;
                                    }
                                }
                                catch (...) {
                                    textoError.setString("ID inválido");
                                    busquedaInvalida = true;
                                }
                            }
                            else {
                                textoError.setString("Ingrese un ID");
                                busquedaInvalida = true;
                            }
                        }
                        else if (botonRegresarBusqueda.getGlobalBounds().contains(mx, my)) {
                            return; // Regresar al menú principal
                        }
                    }
                    else {
                        // Segunda pantalla: modificación de campos
                        for (size_t i = 0; i < campos.size(); i++) {
                            if (campos[i].esModificable && campos[i].fondo.getGlobalBounds().contains(mx, my)) {
                                campoSeleccionado = i;
                                inputModificacion = campos[i].valor.getString();
                                break;
                            }
                        }

                        if (botonGuardar.getGlobalBounds().contains(mx, my)) {
                            mostrandoFormulario = false;
                            input.clear();
                            textoInput.setString("");
                        }
                        else if (botonCancelar.getGlobalBounds().contains(mx, my)) {
                            mostrandoFormulario = false;
                            input.clear();
                            textoInput.setString("");
                        }
                        else if (botonRegresarMod.getGlobalBounds().contains(mx, my)) {
                            return; // Regresar al menú principal
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(220, 220, 220));

        // Dibuja el árbol
        int totalNodos = contarNodos(raiz);
        dibujarNodo(window, raiz, font, 1000, 100, 250, 100, totalNodos);

        if (!mostrandoFormulario) {
            // Primera pantalla: búsqueda de café
            window.draw(etiqueta);

            // Dibujar campo de entrada
            sf::RectangleShape campoInput(sf::Vector2f(150, 30));
            campoInput.setFillColor(sf::Color::White);
            campoInput.setOutlineThickness(1);
            campoInput.setOutlineColor(sf::Color(150, 150, 150));
            campoInput.setPosition(30, 555);
            window.draw(campoInput);

            window.draw(textoInput);
            window.draw(botonBuscar);
            window.draw(textoBuscar);
            window.draw(botonRegresarBusqueda);
            window.draw(textoRegresarBusqueda);

            if (busquedaInvalida) {
                window.draw(textoError);
            }
        }
        else {
            // Segunda pantalla: formulario de modificación
            sf::RectangleShape fondoFormulario(sf::Vector2f(600, 550));
            fondoFormulario.setFillColor(sf::Color(240, 240, 240));
            fondoFormulario.setOutlineThickness(2);
            fondoFormulario.setOutlineColor(sf::Color(100, 149, 237));
            fondoFormulario.setPosition(50, 100);
            window.draw(fondoFormulario);

            sf::Text titulo("Modificando Café ID: " + std::to_string(cafeAModificar->id), font, 20);
            titulo.setFillColor(sf::Color(100, 149, 237));
            titulo.setPosition(70, 110);
            window.draw(titulo);

            // Dibujar campos del formulario en orden correcto
            for (size_t i = 0; i < campos.size(); i++) {
                // 1. Dibujar fondo primero
                if (static_cast<int>(i) == campoSeleccionado) {
                    campos[i].fondo.setFillColor(sf::Color(200, 230, 255));
                    campos[i].fondo.setOutlineThickness(1);
                    campos[i].fondo.setOutlineColor(sf::Color::Blue);
                }
                else if (campos[i].esModificable) {
                    campos[i].fondo.setFillColor(sf::Color(230, 230, 230));
                    campos[i].fondo.setOutlineThickness(0);
                }
                else {
                    campos[i].fondo.setFillColor(sf::Color(210, 210, 210));
                    campos[i].fondo.setOutlineThickness(0);
                }
                window.draw(campos[i].fondo);

                // 2. Dibujar etiqueta
                window.draw(campos[i].etiqueta);

                // 3. Dibujar valor o texto de edición
                if (static_cast<int>(i) == campoSeleccionado) {
                    sf::Text textoEditando(inputModificacion, font, 16);
                    textoEditando.setFillColor(sf::Color::Black);
                    textoEditando.setPosition(250, 150 + i * 40);
                    window.draw(textoEditando);
                }
                else {
                    window.draw(campos[i].valor);
                }
            }

            // Dibujar botones
            window.draw(botonGuardar);
            window.draw(textoGuardar);
            window.draw(botonCancelar);
            window.draw(textoCancelar);
            window.draw(botonRegresarMod);
            window.draw(textoRegresarMod);
        }

        window.display();
    }
}


void arbolABL() {
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Visualizador de Árbol AVL");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error cargando la fuente\n";
        
    }

    sf::Color fondo(200, 200, 200);

    // Menú principal
    vector<string> opciones = {
        "1. Ingresar nuevo café",
        "2. Recorrer estructura",
        "3. Buscar café por ID",  // Nueva opción
        "4. Eliminar Cafe por ID",
        "4. Modificar Cafe por ID"
    };

    vector<Boton> botones;
    float startY = 100;
    float spacing = 70;

    for (size_t i = 0; i < opciones.size(); ++i) {
        Boton boton;
        boton.rect.setSize(sf::Vector2f(300, 50));         // Ancho: 300px, Alto: 50px
        boton.rect.setFillColor(sf::Color(100, 149, 237)); // Color azul (Cornflower Blue)
        boton.rect.setPosition(50, startY + i * spacing);  // Posición X: 50px, Y: calculada
        //Configuración del texto:
        boton.texto.setFont(font);                         // Fuente previamente cargada
        boton.texto.setString(opciones[i]);                // Texto tomado del vector opciones
        boton.texto.setCharacterSize(16);                  // Tamaño de letra
        boton.texto.setFillColor(sf::Color::White);        // Color blanco
        //Centrado del texto:
        sf::FloatRect bounds = boton.texto.getLocalBounds(); //getLocalBounds() obtiene las dimensiones del texto
        boton.texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f); //setOrigin() establece el punto de origen (centro) del texto
        boton.texto.setPosition(50 + 150, startY + i * spacing + 25);   //setPosition() coloca el texto :

        botones.push_back(boton); ///Almacenamiento del botón:
    }

    NodoAVL* raiz = nullptr;
    vector<Cafe> recorridoPreorden;
    vector<Cafe> recorridoInorden;
    vector<Cafe> recorridoPostorden;
    int recorridoActivo = 0;

    // Submenú para recorridos
    bool mostrarSubmenu = false;
    vector<std::string> opcionesSubmenu = { "Preorden", "Inorden", "Postorden" };
    vector<Boton> botonesSubmenu;
    float startYSubmenu = startY + 410;

    for (size_t i = 0; i < opcionesSubmenu.size(); ++i) {
        Boton boton;
        boton.rect.setSize(sf::Vector2f(300, 50));
        boton.rect.setFillColor(sf::Color(100, 149, 237));
        boton.rect.setPosition(50, startYSubmenu + i * spacing);

        boton.texto.setFont(font);
        boton.texto.setString(opcionesSubmenu[i]);
        boton.texto.setCharacterSize(16);
        boton.texto.setFillColor(sf::Color::White);
        sf::FloatRect bounds = boton.texto.getLocalBounds();
        boton.texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        boton.texto.setPosition(50 + 150, startYSubmenu + i * spacing + 25);

        botonesSubmenu.push_back(boton);
    }

    // Botón de regresar
    Boton botonRegresar;
    botonRegresar.rect.setSize(sf::Vector2f(200, 50));
    botonRegresar.rect.setFillColor(sf::Color(255, 99, 71));
    botonRegresar.rect.setPosition(50, startYSubmenu + 3 * spacing);

    botonRegresar.texto.setFont(font);
    botonRegresar.texto.setString("Regresar");
    botonRegresar.texto.setCharacterSize(16);
    botonRegresar.texto.setFillColor(sf::Color::White);
    sf::FloatRect bounds = botonRegresar.texto.getLocalBounds();
    botonRegresar.texto.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    botonRegresar.texto.setPosition(50 + 100, startYSubmenu + 3 * spacing + 25);

    // Ventana de ingreso de datos
    bool mostrandoIngreso = false;
    string nombreCafe, idCafe, tipoCafe, fechaCompra, precioPorKg, cantidadComprada, esSocioInput;
    enum CampoIngreso { NOMBRE, ID, TIPO, FECHA, PRECIO, CANTIDAD, SOCIO, NINGUNO } campoActivo = NINGUNO;

    // Botones para ingreso de datos
    Boton botonNombre, botonID, botonTipo, botonFecha, botonPrecioKg, botonCantidad, botonSocio, botonGuardar, botonCancelar;

    botonNombre.rect.setSize(sf::Vector2f(400, 40));   //Establece el tamaño del área rectangular del botón
    botonNombre.rect.setFillColor(sf::Color::White);   //Define el color de relleno del rectángulo
    botonNombre.rect.setPosition(100, 150);            //Posiciona el rectángulo en la ventana
    botonNombre.texto.setFont(font);                   //Asigna la fuente previamente cargada (font) al texto
    botonNombre.texto.setString("Nombre: ");           //Establece el texto de etiqueta que se mostrará
    botonNombre.texto.setCharacterSize(16);            //Define el tamaño de la fuente en píxeles
    botonNombre.texto.setFillColor(sf::Color::Black);  //Color del texto(contrasta con el fondo blanco)
    botonNombre.texto.setPosition(110, 160);           // Posiciona el texto dentro del rectángulo

    botonID.rect.setSize(sf::Vector2f(400, 40));       //Establece el tamaño del área rectangular del botón
    botonID.rect.setFillColor(sf::Color::White);
    botonID.rect.setPosition(100, 220);
    botonID.texto.setFont(font);
    botonID.texto.setString("ID: ");
    botonID.texto.setCharacterSize(16);
    botonID.texto.setFillColor(sf::Color::Black);
    botonID.texto.setPosition(110, 230);

    botonTipo.rect.setSize(sf::Vector2f(400, 40));
    botonTipo.rect.setFillColor(sf::Color::White);
    botonTipo.rect.setPosition(100, 290);
    botonTipo.texto.setFont(font);
    botonTipo.texto.setString("Tipo: ");
    botonTipo.texto.setCharacterSize(16);
    botonTipo.texto.setFillColor(sf::Color::Black);
    botonTipo.texto.setPosition(110, 300);

    botonFecha.rect.setSize(sf::Vector2f(400, 40));            //Establece el tamaño del área rectangular del botón
    botonFecha.rect.setFillColor(sf::Color::White);            //Define el color de relleno del rectángulo
    botonFecha.rect.setPosition(100, 360);                    //Posiciona el rectángulo en la ventana
    botonFecha.texto.setFont(font);                           //Asigna la fuente previamente cargada (font) al texto   
    botonFecha.texto.setString("Fecha (DD/MM/AAAA): ");       //Establece el texto de etiqueta que se mostrará
    botonFecha.texto.setCharacterSize(16);                    //Define el tamaño de la fuente en píxeles
    botonFecha.texto.setFillColor(sf::Color::Black);          //Color del texto(contrasta con el fondo blanco)
    botonFecha.texto.setPosition(110, 370);                   // Posiciona el texto dentro del rectángulo

    botonPrecioKg.rect.setSize(sf::Vector2f(400, 40));
    botonPrecioKg.rect.setFillColor(sf::Color::White);
    botonPrecioKg.rect.setPosition(100, 430);
    botonPrecioKg.texto.setFont(font);
    botonPrecioKg.texto.setString("Precio por kg: ");
    botonPrecioKg.texto.setCharacterSize(16);
    botonPrecioKg.texto.setFillColor(sf::Color::Black);
    botonPrecioKg.texto.setPosition(110, 440);

    botonCantidad.rect.setSize(sf::Vector2f(400, 40));
    botonCantidad.rect.setFillColor(sf::Color::White);
    botonCantidad.rect.setPosition(100, 500);
    botonCantidad.texto.setFont(font);
    botonCantidad.texto.setString("Cantidad comprada: ");
    botonCantidad.texto.setCharacterSize(16);
    botonCantidad.texto.setFillColor(sf::Color::Black);
    botonCantidad.texto.setPosition(110, 510);

    botonSocio.rect.setSize(sf::Vector2f(400, 40));             //Establece el tamaño del área rectangular del botón
    botonSocio.rect.setFillColor(sf::Color::White);              //Define el color de relleno del rectángulo
    botonSocio.rect.setPosition(100, 570);                       //Posiciona el rectángulo en la ventana
    botonSocio.texto.setFont(font);                              //Asigna la fuente previamente cargada (font) al texto
    botonSocio.texto.setString("Es socio [s/n]: ");              //Establece el texto de etiqueta que se mostrará
    botonSocio.texto.setCharacterSize(16);                       //Define el tamaño de la fuente en píxeles
    botonSocio.texto.setFillColor(sf::Color::Black);             //Color del texto(contrasta con el fondo blanco)
    botonSocio.texto.setPosition(110, 580);                     //Posiciona el texto dentro del rectángulo

    botonGuardar.rect.setSize(sf::Vector2f(150, 40));
    botonGuardar.rect.setFillColor(sf::Color(50, 205, 50));
    botonGuardar.rect.setPosition(150, 650);
    botonGuardar.texto.setFont(font);
    botonGuardar.texto.setString("Guardar");
    botonGuardar.texto.setCharacterSize(16);
    botonGuardar.texto.setFillColor(sf::Color::White);
    //getLocalBounds(): Obtiene un rectángulo (FloatRect) que representa el área ocupada por el texto
    sf::FloatRect boundsGuardar = botonGuardar.texto.getLocalBounds();
    //: Define el punto de referencia del texto (su "centro geométrico")
    botonGuardar.texto.setOrigin(boundsGuardar.left + boundsGuardar.width / 2.f, boundsGuardar.top + boundsGuardar.height / 2.f);
    botonGuardar.texto.setPosition(150 + 75, 650 + 20); // Posicionar el texto:

    botonCancelar.rect.setSize(sf::Vector2f(150, 40));
    botonCancelar.rect.setFillColor(sf::Color(220, 20, 60));
    botonCancelar.rect.setPosition(350, 650);
    botonCancelar.texto.setFont(font);
    botonCancelar.texto.setString("Cancelar");
    botonCancelar.texto.setCharacterSize(16);
    botonCancelar.texto.setFillColor(sf::Color::White);
    sf::FloatRect boundsCancelar = botonCancelar.texto.getLocalBounds();
    botonCancelar.texto.setOrigin(boundsCancelar.left + boundsCancelar.width / 2.f, boundsCancelar.top + boundsCancelar.height / 2.f);
    botonCancelar.texto.setPosition(350 + 75, 650 + 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            //Verifica si el evento actual es un clic del mouse(botón presionado), boton izquierdo del mause
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Crea un vector 2D con coordenadas flotantes (x,y),, almacena la posición exacta del clic en píxeles
                sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                if (mostrandoIngreso) {
                    if (botonNombre.rect.getGlobalBounds().contains(mouse)) {  // Código que se ejecuta cuando hay clic en el botón
                        campoActivo = NOMBRE;
                        botonNombre.activo = true;
                        botonID.activo = false;
                        botonTipo.activo = false;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = false;
                        botonSocio.activo = false;
                    }
                    else if (botonID.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = ID;
                        botonNombre.activo = false;
                        botonID.activo = true;
                        botonTipo.activo = false;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = false;
                        botonSocio.activo = false;
                    }
                    else if (botonTipo.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = TIPO;
                        botonNombre.activo = false;
                        botonID.activo = false;
                        botonTipo.activo = true;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = false;
                        botonSocio.activo = false;
                    }
                    else if (botonFecha.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = FECHA;
                        botonNombre.activo = false;
                        botonID.activo = false;
                        botonTipo.activo = false;
                        botonFecha.activo = true;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = false;
                        botonSocio.activo = false;
                    }
                    else if (botonPrecioKg.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = PRECIO;
                        botonNombre.activo = false;
                        botonID.activo = false;
                        botonTipo.activo = false;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = true;
                        botonCantidad.activo = false;
                        botonSocio.activo = false;
                    }
                    else if (botonCantidad.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = CANTIDAD;
                        botonNombre.activo = false;
                        botonID.activo = false;
                        botonTipo.activo = false;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = true;
                        botonSocio.activo = false;
                    }
                    else if (botonSocio.rect.getGlobalBounds().contains(mouse)) {
                        campoActivo = SOCIO;
                        botonNombre.activo = false;
                        botonID.activo = false;
                        botonTipo.activo = false;
                        botonFecha.activo = false;
                        botonPrecioKg.activo = false;
                        botonCantidad.activo = false;
                        botonSocio.activo = true;
                    }
                    else if (botonGuardar.rect.getGlobalBounds().contains(mouse)) {
                        // Código que se ejecuta si TODOS los campos están llenos
                        if (!nombreCafe.empty() && !idCafe.empty() && !tipoCafe.empty() &&
                            !fechaCompra.empty() && !precioPorKg.empty() && !cantidadComprada.empty() && !esSocioInput.empty()) {
                            try {
                                Cafe nuevoCafe;
                                // 2. Asignación de valores desde los campos del formulario
                                nuevoCafe.nombre = nombreCafe;
                                nuevoCafe.id = std::stoi(idCafe);  // Conversión string ? int
                                nuevoCafe.tipo = tipoCafe;
                                nuevoCafe.fechaCompra = fechaCompra;
                                nuevoCafe.precioPorKg = std::stof(precioPorKg);   // Conversión string ? float
                                nuevoCafe.cantidadComprada = std::stof(cantidadComprada);// Conversión string ? float
                                // 3. Procesamiento de condición de socio
                                nuevoCafe.esSocio = (esSocioInput == "s"); // Conversión a booleano

                                // Calcular total con bonificación
                                float subtotal = nuevoCafe.precioPorKg * nuevoCafe.cantidadComprada;
                                nuevoCafe.total = nuevoCafe.esSocio ? subtotal * 1.1f : subtotal;  // +10% si es socio

                                raiz = insertar(raiz, nuevoCafe);
                                //5. Inserción en el árbol AVL

                                // Limpiar campos
                                nombreCafe.clear(); idCafe.clear(); tipoCafe.clear();
                                fechaCompra.clear(); precioPorKg.clear();
                                cantidadComprada.clear(); esSocioInput.clear();
                                campoActivo = NINGUNO;
                                mostrandoIngreso = false;
                            }
                            catch (...) {
                                idCafe = "INVALIDO";  // Marca el ID como inválido para mostrar error
                            }
                        }
                    }
                    else if (botonCancelar.rect.getGlobalBounds().contains(mouse)) {
                        nombreCafe.clear();
                        idCafe.clear();
                        tipoCafe.clear();
                        fechaCompra.clear();
                        precioPorKg.clear();
                        cantidadComprada.clear();
                        esSocioInput.clear();
                        campoActivo = NINGUNO;
                        mostrandoIngreso = false;
                    }
                }
                else {
                    for (size_t i = 0; i < botones.size(); ++i) {
                        if (botones[i].rect.getGlobalBounds().contains(mouse)) {
                            if (i == 0) {
                                mostrandoIngreso = true;
                                campoActivo = NOMBRE;
                                botonNombre.activo = true;
                                botonID.activo = false;
                                botonTipo.activo = false;
                                botonFecha.activo = false;
                                botonPrecioKg.activo = false;
                                botonCantidad.activo = false;
                                botonSocio.activo = false;
                            }
                            else if (i == 1) {
                                mostrarSubmenu = !mostrarSubmenu;
                                if (!mostrarSubmenu) {
                                    recorridoActivo = 0;
                                }
                            }
                            else if (i == 2) {
                                buscarVisual(window, raiz, font); // Llama a la función de búsqueda
                            }
                            else if (i == 3) {
                                eliminarVisual(window, raiz, font); // Llama a tu función de eliminar
                            }
                            else if (i == 4) {
                                modificarVisual(window, raiz, font);
                            }
                        }
                    }

                    if (mostrarSubmenu && botonRegresar.rect.getGlobalBounds().contains(mouse)) {
                        mostrarSubmenu = false;
                        recorridoActivo = 0;
                    }

                    if (mostrarSubmenu) {
                        for (size_t i = 0; i < botonesSubmenu.size(); ++i) {
                            if (botonesSubmenu[i].rect.getGlobalBounds().contains(mouse)) {
                                if (i == 0) {
                                    recorridoPreorden.clear();
                                    preorden(raiz, recorridoPreorden);
                                    recorridoActivo = 1;
                                }
                                else if (i == 1) {
                                    recorridoInorden.clear();
                                    inorden(raiz, recorridoInorden);
                                    recorridoActivo = 2;
                                }
                                else if (i == 2) {
                                    recorridoPostorden.clear();
                                    postorden(raiz, recorridoPostorden);
                                    recorridoActivo = 3;
                                }
                            }
                        }
                    }
                }
            }
            // formulario visible y  texto valido 
            if (mostrandoIngreso && event.type == sf::Event::TextEntered) {
                if (campoActivo == NOMBRE) {
                    //se preciona backspace y hay texto para borrar
                    if (event.text.unicode == '\b' && !nombreCafe.empty()) {
                        nombreCafe.pop_back();
                    }
                    // ase agrega texto
                    else if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != '\r') {
                        nombreCafe += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == ID) {
                    if (event.text.unicode == '\b' && !idCafe.empty()) {
                        idCafe.pop_back();
                    }
                    else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                        idCafe += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == TIPO) {
                    if (event.text.unicode == '\b' && !tipoCafe.empty()) {
                        tipoCafe.pop_back();
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\b' && event.text.unicode != '\r') {
                        tipoCafe += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == FECHA) {
                    if (event.text.unicode == '\b' && !fechaCompra.empty()) {
                        fechaCompra.pop_back();
                    }
                    else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '/') {
                        fechaCompra += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == PRECIO) {
                    if (event.text.unicode == '\b' && !precioPorKg.empty()) {
                        precioPorKg.pop_back();
                    }
                    else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                        precioPorKg += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == CANTIDAD) {
                    if (event.text.unicode == '\b' && !cantidadComprada.empty()) {
                        cantidadComprada.pop_back();
                    }
                    else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                        cantidadComprada += static_cast<char>(event.text.unicode);
                    }
                }
                else if (campoActivo == SOCIO) {
                    if (event.text.unicode == '\b' && !esSocioInput.empty()) {
                        esSocioInput.pop_back();
                    }
                    else if (event.text.unicode == 's' || event.text.unicode == 'n' ||
                        event.text.unicode == 'S' || event.text.unicode == 'N') {
                        esSocioInput = static_cast<char>(tolower(event.text.unicode));
                    }
                }
            }
        }

        window.clear(fondo);

        if (!mostrandoIngreso) {  // Si NO estamos mostrando el formulario de ingreso
            for (const Boton& b : botones) {
                window.draw(b.rect);
                window.draw(b.texto);
            }

            if (mostrarSubmenu) {
                for (const Boton& b : botonesSubmenu) {
                    window.draw(b.rect);
                    window.draw(b.texto);
                }
                window.draw(botonRegresar.rect);
                window.draw(botonRegresar.texto);
            }
        }
        else {
            sf::RectangleShape fondoIngreso(sf::Vector2f(600, 700));// panel visual del fomulario 
            fondoIngreso.setFillColor(sf::Color(240, 240, 240));    // fondo, gris claro
            fondoIngreso.setPosition(50, 50);                        // Posiciona el rectángulo
            window.draw(fondoIngreso);                               //Dibuja el rectángulo en la ventana (window).

            sf::Text titulo("Ingreso de Datos del Cafe", font, 24);  // Crea un objeto de texto 
            titulo.setFillColor(sf::Color::Black);                   // Establece el color del texto a negro.
            titulo.setPosition(150, 80);                             // Posiciona el texto 
            window.draw(titulo);                                     //Renderiza el texto en la ventana.

            botonNombre.rect.setFillColor(botonNombre.activo ? sf::Color(200, 200, 255) : sf::Color::White); //cambiar el color segun el estado 
            window.draw(botonNombre.rect);                               //Renderiza el fondo del campo en la ventana.
            sf::Text textoNombre("Nombre: " + nombreCafe, font, 16);     //Concatena el texto ingresado por el usuario
            textoNombre.setFillColor(sf::Color::Black);                  //Color negro para máximo contraste con el fondo(blanco / azul claro).
            textoNombre.setPosition(110, 160);                           // Coordenadas:
            window.draw(textoNombre);                                    //Dibujado del texto:

            botonID.rect.setFillColor(botonID.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonID.rect);
            sf::Text textoID("ID: " + idCafe, font, 16);
            textoID.setFillColor(sf::Color::Black);
            textoID.setPosition(110, 230);
            window.draw(textoID);

            botonTipo.rect.setFillColor(botonTipo.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonTipo.rect);
            sf::Text textoTipo("Tipo: " + tipoCafe, font, 16);
            textoTipo.setFillColor(sf::Color::Black);
            textoTipo.setPosition(110, 300);
            window.draw(textoTipo);

            botonFecha.rect.setFillColor(botonFecha.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonFecha.rect);
            sf::Text textoFecha("Fecha (DD/MM/AAAA): " + fechaCompra, font, 16);
            textoFecha.setFillColor(sf::Color::Black);
            textoFecha.setPosition(110, 370);
            window.draw(textoFecha);

            botonPrecioKg.rect.setFillColor(botonPrecioKg.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonPrecioKg.rect);
            sf::Text textPrecio("Precio por kg: " + precioPorKg, font, 16);
            textPrecio.setPosition(110, 440);
            textPrecio.setFillColor(sf::Color::Black);
            window.draw(textPrecio);

            botonCantidad.rect.setFillColor(botonCantidad.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonCantidad.rect);
            sf::Text textCantidad("Cantidad comprada: " + cantidadComprada, font, 16);
            textCantidad.setPosition(110, 510);
            textCantidad.setFillColor(sf::Color::Black);
            window.draw(textCantidad);

            botonSocio.rect.setFillColor(botonSocio.activo ? sf::Color(200, 200, 255) : sf::Color::White);
            window.draw(botonSocio.rect);
            sf::Text textoSocio("Es socio [s/n]: " + esSocioInput, font, 16);
            textoSocio.setPosition(110, 580);
            textoSocio.setFillColor(sf::Color::Black);
            window.draw(textoSocio);

            // Mostrar total calculado
            if (!precioPorKg.empty() && !cantidadComprada.empty() && !esSocioInput.empty()) {
                try {
                    float precio = std::stof(precioPorKg);
                    float cantidad = std::stof(cantidadComprada);
                    bool socio = (esSocioInput == "s");
                    float subtotal = precio * cantidad;
                    float total = socio ? subtotal * 1.1f : subtotal;

                    // Formatear el total a 2 decimales
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(2) << total;
                    std::string totalStr = stream.str();

                    sf::Text textoTotal("Total: $" + totalStr, font, 20);
                    textoTotal.setPosition(110, 620);
                    textoTotal.setFillColor(sf::Color::Black);
                    window.draw(textoTotal);
                }
                catch (...) {
                    // No hacer nada si hay error en los datos
                }
            }

            window.draw(botonGuardar.rect);
            window.draw(botonGuardar.texto);
            window.draw(botonCancelar.rect);
            window.draw(botonCancelar.texto);
        }

        // Dibujar el árbol
        int totalNodos = contarNodos(raiz);
        dibujarNodo(window, raiz, font, 1000, 100, 250, 100, totalNodos);

        // Dibujar recorrido activo
        if (recorridoActivo == 1) {
            dibujarRecorrido(window, recorridoPreorden, font, 100, 800, "Recorrido Preorden:");
        }
        else if (recorridoActivo == 2) {
            dibujarRecorrido(window, recorridoInorden, font, 100, 800, "Recorrido Inorden:");
        }
        else if (recorridoActivo == 3) {
            dibujarRecorrido(window, recorridoPostorden, font, 100, 800, "Recorrido Postorden:");
        }

        window.display();
    }

  
}


#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <limits>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <sstream>
#include <string>
#include <vector>
using namespace std;



struct Nodo {
    int id;
    string nombre;
    string codigo;
    string descripcion;
    int cantidad;
    int precioc;
    int preciov;
    string fecha;
    string proveedor;

    Nodo* izq;
    Nodo* der;
    float x, y;
};

Nodo* arbol = NULL;

Nodo* crearNodo(int, string, string, string, int, int, int, string, string);
void insertar(Nodo*&, int, string, string, string, int, int, int, string, string);
void vaciarArbol(Nodo*&);
void calcularPosiciones(Nodo* nodo, int x, int y, int nivel, int alturaTotal, int& offsetX);
void dibujarArbol(sf::RenderWindow&, Nodo*, sf::Font&);
void dibujarNodo(sf::RenderWindow&, Nodo*, Nodo*, sf::Font&);
void menu();
void menu_orden();
void DibujarRecuadro2();
Nodo* buscarNodo(Nodo*, int);
Nodo* encontrarMinimo(Nodo*);
Nodo* eliminarNodo(Nodo* nodo, int, Nodo* padre = nullptr);
void modificarNodo(Nodo* arbol, int id);
void mostrarNodo(Nodo* nodo, Nodo* padre);
void mostrarAsciiArt();
void mostrarAsciiArt2();
void dibujarArbolPreorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font);
void dibujarArbolInorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font);
void dibujarArbolPostorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font);
void eliminarVisual(sf::RenderWindow& window, Nodo*& raiz, sf::Font& font);
void dibujarNodoConSeleccion(sf::RenderWindow& window, Nodo* nodo, Nodo* padre, sf::Font& font, int idSeleccionado);
void modificarVisual(sf::RenderWindow& window, Nodo*& raiz, sf::Font& font);
void buscarVisual(sf::RenderWindow& window, Nodo* raiz, sf::Font& font);



Nodo* crearNodo(int a, string b, string c, string d, int e, int f, int g, string h, string i) {
    Nodo* nuevo_nodo = new Nodo();
    nuevo_nodo->id = a;
    nuevo_nodo->nombre = b;
    nuevo_nodo->codigo = c;
    nuevo_nodo->descripcion = d;
    nuevo_nodo->cantidad = e;
    nuevo_nodo->precioc = f;
    nuevo_nodo->preciov = g;
    nuevo_nodo->fecha = h;
    nuevo_nodo->proveedor = i;
    nuevo_nodo->izq = nuevo_nodo->der = nullptr;
    nuevo_nodo->x = nuevo_nodo->y = 0;
    return nuevo_nodo;
}

void insertar(Nodo*& arbol, int a, string b, string c, string d, int e, int f, int g, string h, string i) {
    if (arbol == nullptr) {
        arbol = crearNodo(a, b, c, d, e, f, g, h, i);
    }
    else if (a < arbol->id) insertar(arbol->izq, a, b, c, d, e, f, g, h, i);
    else if (a > arbol->id) insertar(arbol->der, a, b, c, d, e, f, g, h, i);
    else {
        gotoxy(10, 28); cout << "[ADVERTENCIA] El valor " << a << " ya existe en el arbol. no se tomaran en cuenta los datos ingresados";
    }
}

void vaciarArbol(Nodo*& nodo) {
    if (!nodo) return;
    vaciarArbol(nodo->izq);
    vaciarArbol(nodo->der);
    delete nodo;
    nodo = nullptr;
}

int alturaArbol(Nodo* nodo) {
    if (!nodo) return 0;
    return 1 + max(alturaArbol(nodo->izq), alturaArbol(nodo->der));
}

void calcularPosiciones(Nodo* nodo, int x, int y, int nivel, int alturaTotal, int& offsetX) {
    if (!nodo) return;
    int espacioVertical = min(120, 500 / alturaTotal);
    calcularPosiciones(nodo->izq, x, y + espacioVertical, nivel + 1, alturaTotal, offsetX);
    nodo->x = offsetX * 60 + 100;
    nodo->y = y;
    offsetX++;
    calcularPosiciones(nodo->der, x, y + espacioVertical, nivel + 1, alturaTotal, offsetX);
}

void dibujarNodo(sf::RenderWindow& window, Nodo* nodo, Nodo* padre, sf::Font& font) {
    if (!nodo) return;

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setPosition(nodo->x - 50, nodo->y - 50);
    window.draw(circle);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(9);
    text.setFillColor(sf::Color::Black);

    float textoY = nodo->y - 40;
    float espaciado = 12;

    text.setString("ID: " + std::to_string(nodo->id));
    text.setPosition(nodo->x - 20, textoY);
    window.draw(text);
    textoY += espaciado;

    text.setString("Cant: " + std::to_string(nodo->cantidad));
    text.setPosition(nodo->x - 35, textoY);
    window.draw(text);
    textoY += espaciado;

    text.setString("Fecha: " + nodo->fecha);
    text.setPosition(nodo->x - 35, textoY);
    window.draw(text);
    textoY += espaciado;

    text.setString("Nombre: " + nodo->nombre);
    text.setPosition(nodo->x - 50, textoY);
    window.draw(text);
    textoY += espaciado;

    text.setString("Código: " + nodo->codigo);
    text.setPosition(nodo->x - 35, textoY);
    window.draw(text);
    textoY += espaciado;

    std::stringstream ss;
    ss << "  " << nodo;
    text.setString(ss.str());
    text.setPosition(nodo->x - 45, textoY);
    window.draw(text);

    if (padre) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(padre->x, padre->y)),
            sf::Vertex(sf::Vector2f(nodo->x, nodo->y))
        };
        window.draw(line, 2, sf::Lines);
    }

    dibujarNodo(window, nodo->izq, nodo, font);
    dibujarNodo(window, nodo->der, nodo, font);
}

void dibujarArbol(sf::RenderWindow& window, Nodo* raiz, sf::Font& font) {
    int altura = alturaArbol(raiz);
    int offsetX = 0;
    calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);

    sf::RectangleShape botonCerrar(sf::Vector2f(100, 40));
    botonCerrar.setFillColor(sf::Color(200, 50, 50));
    botonCerrar.setPosition(680, 540);

    sf::Text textoCerrar("Cerrar", font, 18);
    textoCerrar.setFillColor(sf::Color::White);
    textoCerrar.setPosition(700, 545);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float mouseX = event.mouseButton.x;
                    float mouseY = event.mouseButton.y;
                    if (botonCerrar.getGlobalBounds().contains(mouseX, mouseY)) {
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color(220, 220, 220));
        dibujarNodo(window, raiz, nullptr, font);
        window.draw(botonCerrar);
        window.draw(textoCerrar);
        window.display();
    }
}

Nodo* buscarNodo(Nodo* nodo, int id) {
    if (!nodo) return nullptr;
    if (id == nodo->id) return nodo;
    return (id < nodo->id) ? buscarNodo(nodo->izq, id) : buscarNodo(nodo->der, id);
}

Nodo* encontrarMinimo(Nodo* nodo) { while (nodo && nodo->izq) nodo = nodo->izq; return nodo; }

Nodo* eliminarNodo(Nodo* nodo, int id, Nodo* padre) {
    if (!nodo) {
        std::cout << "[INFO] Nodo con ID " << id << " no encontrado.\n";
        return nullptr;
    }

    if (id < nodo->id) {
        nodo->izq = eliminarNodo(nodo->izq, id, nodo);
    }
    else if (id > nodo->id) {
        nodo->der = eliminarNodo(nodo->der, id, nodo);
    }
    else {
        std::cout << "[INFO] Eliminando nodo con ID: " << id << "\n";

        if (!nodo->izq && !nodo->der) {
            delete nodo;
            std::cout << "[ELIMINADO] Nodo sin hijos eliminado.\n";
            return nullptr;
        }

        if (!nodo->izq) {
            Nodo* temp = nodo->der;
            delete nodo;
            std::cout << "[ELIMINADO] Nodo con hijo derecho eliminado.\n";
            return temp;
        }
        if (!nodo->der) {
            Nodo* temp = nodo->izq;
            delete nodo;
            std::cout << "[ELIMINADO] Nodo con hijo izquierdo eliminado.\n";
            return temp;
        }

        Nodo* sucesor = encontrarMinimo(nodo->der);
        nodo->id = sucesor->id;
        nodo->nombre = sucesor->nombre;
        nodo->codigo = sucesor->codigo;
        nodo->descripcion = sucesor->descripcion;
        nodo->cantidad = sucesor->cantidad;
        nodo->precioc = sucesor->precioc;
        nodo->preciov = sucesor->preciov;
        nodo->fecha = sucesor->fecha;
        nodo->proveedor = sucesor->proveedor;

        nodo->der = eliminarNodo(nodo->der, sucesor->id, nodo);
        std::cout << "[ELIMINADO] Nodo reemplazado con su sucesor.\n";
    }

    return nodo;
}

void mostrarNodo(Nodo* nodo, Nodo* padre) {
    if (padre == nullptr) {
        cout << " [RAIZ]" << endl << endl;
    }
    else if (padre->izq == nodo) {
        cout << " [HIJO IZQUIERDO de Nodo con ID: " << padre->id << "]" << endl << endl;
    }
    else if (padre->der == nodo) {
        cout << " [HIJO DERECHO de Nodo con ID: " << padre->id << "]" << endl << endl;
    }

    cout << " Direccion del Nodo: " << nodo << endl;

    cout << "  ID: " << nodo->id << "              \xfe Direccion: " << &nodo->id << endl;
    cout << "  Nombre: " << nodo->nombre << "              \xfe Direccion: " << &nodo->nombre << endl;
    cout << "  Codigo: " << nodo->codigo << "              \xfe Direccion: " << &nodo->codigo << endl;
    cout << "  Descripcion: " << nodo->descripcion << "           \xfe Direccion: " << &nodo->descripcion << endl;
    cout << "  Cantidad: " << nodo->cantidad << "              \xfe Dirección: " << &nodo->cantidad << endl;
    cout << "  Precio Compra: " << nodo->precioc << "              \xfe Direccion: " << &nodo->precioc << endl;
    cout << "  Precio Venta: " << nodo->preciov << "              \xfe Direccion: " << &nodo->preciov << endl;
    cout << "  Fecha: " << nodo->fecha << "              \xfe Direccion: " << &nodo->fecha << endl;
    cout << "  Proveedor: " << nodo->proveedor << "             \xfe Direccion: " << &nodo->proveedor << endl;
    cout << "----------------------------------------" << endl;
}

Nodo* buscarPadre(Nodo* raiz, Nodo* hijo) {
    if (!raiz || raiz == hijo) return nullptr;
    if (raiz->izq == hijo || raiz->der == hijo) return raiz;

    if (hijo->id < raiz->id) return buscarPadre(raiz->izq, hijo);
    else return buscarPadre(raiz->der, hijo);
}

void modificarNodo(Nodo* arbol, int id) {
    Nodo* nodo = buscarNodo(arbol, id);
    if (!nodo) {
        gotoxy(40, 12); cout << "No se encontró un nodo con el ID " << id << "." << endl;
        return;
    }

    Nodo* padre = buscarPadre(arbol, nodo);
    int opcion = 1;
    char tecla;

    do {
        system("cls");
        gotoxy(45, 10); cout << "=== Modificar Nodo (ID: " << id << ") ===";
        gotoxy(45, 12); cout << (opcion == 1 ? "--> " : "    ") << "1. Nombre: " << nodo->nombre;
        gotoxy(45, 13); cout << (opcion == 2 ? "--> " : "    ") << "2. Codigo: " << nodo->codigo;
        gotoxy(45, 14); cout << (opcion == 3 ? "--> " : "    ") << "3. Descripcion: " << nodo->descripcion;
        gotoxy(45, 15); cout << (opcion == 4 ? "--> " : "    ") << "4. Cantidad: " << nodo->cantidad;
        gotoxy(45, 16); cout << (opcion == 5 ? "--> " : "    ") << "5. Precio compra: " << nodo->precioc;
        gotoxy(45, 17); cout << (opcion == 6 ? "--> " : "    ") << "6. Precio venta: " << nodo->preciov;
        gotoxy(45, 18); cout << (opcion == 7 ? "--> " : "    ") << "7. Fecha: " << nodo->fecha;
        gotoxy(45, 19); cout << (opcion == 8 ? "--> " : "    ") << "8. Proveedor: " << nodo->proveedor;
        gotoxy(45, 20); cout << (opcion == 9 ? "--> " : "    ") << "9. Mostrar datos actuales";
        gotoxy(45, 21); cout << (opcion == 10 ? "--> " : "    ") << "10. Salir";

        tecla = _getch();
        if (tecla == 72 && opcion > 1) opcion--;
        if (tecla == 80 && opcion < 10) opcion++;

        if (tecla == 13) {
            system("cls");
            if (opcion == 10) break;

            if (opcion == 9) {
                cout << "\n-- Datos actuales del Nodo --\n\n";
                mostrarNodo(nodo, padre);
                system("pause");
            }
            else {
                gotoxy(45, 10); cout << "Ingrese el nuevo valor:";
                switch (opcion) {
                case 1: gotoxy(20, 15); cout << "nomnre del producto ";  gotoxy(45, 15); cin >> nodo->nombre; break;
                case 2: gotoxy(20, 15); cout << "codiog del producto "; gotoxy(45, 15); cin >> nodo->codigo; break;
                case 3: gotoxy(20, 15); cout << "descripcion del producto "; gotoxy(45, 15); cin >> nodo->descripcion; break;
                case 4: gotoxy(20, 15); cout << "cantidad del producto "; gotoxy(45, 15); cin >> nodo->cantidad; break;
                case 5: gotoxy(20, 15); cout << "precio de venta del compra "; gotoxy(50, 15); cin >> nodo->precioc; break;
                case 6: gotoxy(20, 15); cout << "precio de venta del producto "; gotoxy(50, 15); cin >> nodo->preciov; break;
                case 7: gotoxy(20, 15); cout << "fecha del producto "; gotoxy(45, 15); cin >> nodo->fecha; break;
                case 8: gotoxy(20, 15); cout << "proveedor del producto "; gotoxy(45, 15); cin >> nodo->proveedor; break;
                }
            }
        }

    } while (true);

    gotoxy(40, 7); cout << "Nodo con ID " << id << " modificado correctamente." << endl;
}

void menu()
{
    int opcion = 1;
    char tecla;

    do
    {
        system("cls");
        mostrarAsciiArt();
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, 1);
        gotoxy(45, 10); cout << "==== MENU ABB ====";
        SetConsoleTextAttribute(hConsole, 2);
        gotoxy(45, 12); cout << (opcion == 1 ? " --> " : "     ") << "1. Insertar producto\n";
        SetConsoleTextAttribute(hConsole, 3);
        gotoxy(45, 13); cout << (opcion == 2 ? " --> " : "     ") << "2. Mostrar producto\n";
        SetConsoleTextAttribute(hConsole, 4);
        gotoxy(45, 14); cout << (opcion == 3 ? " --> " : "     ") << "3. Vaciar inventario\n";
        SetConsoleTextAttribute(hConsole, 5);
        gotoxy(45, 15); cout << (opcion == 4 ? " --> " : "     ") << "4. Recorrer estructura\n";
        SetConsoleTextAttribute(hConsole, 6);
        gotoxy(45, 16); cout << (opcion == 5 ? " --> " : "     ") << "5. Eliminar producto\n";
        SetConsoleTextAttribute(hConsole, 7);
        gotoxy(45, 17); cout << (opcion == 6 ? " --> " : "     ") << "6. Modificar producto\n";
        SetConsoleTextAttribute(hConsole, 8);
        gotoxy(45, 18); cout << (opcion == 7 ? " --> " : "     ") << "7. Buscar producto\n";
        SetConsoleTextAttribute(hConsole, 9);
        gotoxy(45, 19); cout << (opcion == 8 ? " --> " : "     ") << "8. Salir\n";

        tecla = _getch();

        if (tecla == 72 && opcion > 1)
        {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC);
            opcion--;
        }
        else if (tecla == 80 && opcion < 8)
        {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC);
            opcion++;
        }
        else if (tecla == 13) {
            system("cls");
            switch (opcion) {
            case 1: {
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                char s;
                do {
                    int id, cant, pc, pv;
                    string nom, cod, desc, fec, prov;

                    DibujarRecuadro2();
                    gotoxy(45, 6); cin >> id;
                    gotoxy(45, 7); cin >> nom;
                    gotoxy(45, 8); cin >> cod;
                    gotoxy(45, 13); cin >> desc;
                    gotoxy(45, 14); cin >> cant;
                    gotoxy(45, 15); cin >> pc;
                    gotoxy(45, 16); cin >> pv;
                    gotoxy(45, 21); cin >> fec;
                    gotoxy(45, 22); cin >> prov;

                    insertar(arbol, id, nom, cod, desc, cant, pc, pv, fec, prov);

                    gotoxy(45, 27); cout << "Otro (s/n): "; cin >> s;
                } while (s == 's' || s == 'S');
                break;
            }
            case 2: {
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow wnd(sf::VideoMode(800, 600), "Arbol ABB");
                    while (wnd.isOpen()) {
                        sf::Event e;
                        while (wnd.pollEvent(e)) if (e.type == sf::Event::Closed) wnd.close();
                        wnd.clear(sf::Color(220, 220, 220));
                        dibujarArbol(wnd, arbol, font);
                        wnd.display();
                    }
                }
                system("pause");
                break;
            }
            case 3:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    vaciarArbol(arbol);
                    cout << "Arbol vaciado.\n";
                }
                system("pause");
                break;
            case 4:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    menu_orden();
                }
                system("pause");
                break;
            case 5: {
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow window(sf::VideoMode(950, 700), "Eliminar Nodo del Arbol");
                    eliminarVisual(window, arbol, font);
                }
                break;
            }
            case 6: {
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow window(sf::VideoMode(800, 700), "Modificar Nodo del Arbol");
                    modificarVisual(window, arbol, font);
                }
                break;
            }
            case 7: {
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow window(sf::VideoMode(800, 700), "Buscar Nodo en el Arbol");
                    buscarVisual(window, arbol, font);
                }
                break;
            }
            case 8:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                cout << "Saliendo...\n";
                return;
            }
        }
    } while (true);
}

void menu_orden() {
    int opcion = 1;
    char tecla;

    do {
        system("cls");
        mostrarAsciiArt2();
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, 1);
        gotoxy(45, 10); cout << "=== Recorridos ===\n\n";
        SetConsoleTextAttribute(hConsole, 2);
        gotoxy(45, 12); cout << (opcion == 1 ? " --> " : "     ") << "1. Preorden\n";
        SetConsoleTextAttribute(hConsole, 3);
        gotoxy(45, 13); cout << (opcion == 2 ? " --> " : "     ") << "2. Inorden\n";
        SetConsoleTextAttribute(hConsole, 4);
        gotoxy(45, 14); cout << (opcion == 3 ? " --> " : "     ") << "3. Postorden\n";
        SetConsoleTextAttribute(hConsole, 5);
        gotoxy(45, 15); cout << (opcion == 4 ? " --> " : "     ") << "4. Volver\n";

        tecla = _getch();

        if (tecla == 72 && opcion > 1) {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC);
            opcion--;
        }
        else if (tecla == 80 && opcion < 4) {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC);
            opcion++;
        }
        else if (tecla == 13) {
            system("cls");
            switch (opcion) {
            case 1:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow wnd(sf::VideoMode(800, 600), "Preorden del Arbol ABB");
                    dibujarArbolPreorden(wnd, arbol, font);
                }
                system("pause");
                break;
            case 2:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow wnd(sf::VideoMode(800, 600), "Preorden del Arbol ABB");
                    dibujarArbolInorden(wnd, arbol, font);
                }
                system("pause");
                break;
            case 3:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                if (!arbol) {
                    gotoxy(45, 15); cout << "El arbol esta vacio.\n";
                }
                else {
                    sf::Font font;
                    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) break;
                    sf::RenderWindow wnd(sf::VideoMode(800, 600), "Preorden del Arbol ABB");
                    dibujarArbolPostorden(wnd, arbol, font);
                }
                system("pause");
                break;
            case 4:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcialArbol\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC);
                return;
            }
        }
    } while (true);
}

void dibujarCuadro2(int x1, int y1, int x2, int y2) {
    int i;
    for (i = x1; i < x2; i++) {
        gotoxy(i, y1); cout << (char)220;
        gotoxy(i, y2); cout << (char)220;
    }
    for (i = y1; i < y2; i++) {
        gotoxy(x1, i); cout << (char)219;
        gotoxy(x2, i); cout << (char)219;
    }
    gotoxy(x1, y1); cout << (char)220;
    gotoxy(x1, y2); cout << (char)220;
    gotoxy(x2, y1); cout << (char)220;
    gotoxy(x2, y2); cout << (char)220;
}

void DibujarRecuadro2() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    SetConsoleTextAttribute(hConsole, 15);

    dibujarCuadro2(10, 0, 100, 4);
    dibujarCuadro2(10, 4, 100, 25);
    dibujarCuadro2(10, 10, 100, 10);
    dibujarCuadro2(10, 18, 100, 18);

    gotoxy(100, 4); cout << "\xDB";
    gotoxy(10, 4); cout << "\xDB";
    gotoxy(10, 10); cout << "\xDB";
    gotoxy(10, 18); cout << "\xDB";
    gotoxy(100, 18); cout << "\xDB";
    gotoxy(100, 10); cout << "\xDB";
    gotoxy(100, 25); cout << "\xDB";
    gotoxy(10, 25); cout << "\xDB";

    gotoxy(40, 2); cout << "INVENTARIO";
    gotoxy(40, 6); cout << "ID";
    gotoxy(20, 7); cout << "Nombre del producto:";
    gotoxy(20, 8); cout << "Codigo del producto:";
    gotoxy(40, 12); cout << "DETALLES DEL PRODUCTO";
    gotoxy(20, 13); cout << "Descipcion:";
    gotoxy(20, 14); cout << "Cantidad";
    gotoxy(20, 15); cout << "precio de veta";
    gotoxy(20, 16); cout << "precio de compra: ";
    gotoxy(40, 20); cout << "Características y Origen ";
    gotoxy(20, 21); cout << "fecha de entrada: ";
    gotoxy(20, 22); cout << "proveedor: ";
}





void mostrarAsciiArt() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);
    cout << R"(
  ` : | | | |:  ||  :     `  :  |  |+|: | : : :|   .        `              .
      ` : | :|  ||  |:  :    `  |  | :| : | : |:   |  .                    :
         .' ':  ||  |:  |  '       ` || | : | |: : |   .  `           .   :.
                `'  ||  |  ' |   *    ` : | | :| |*|  :   :               :|
        *    *       `  |  : :  |  .      ` ' :| | :| . : :         *   :.||
             .`            | |  |  : .:|       ` | || | : |: |          | ||
      '          .         + `  |  :  .: .         '| | : :| :    .   |:| ||
         .                 .    ` *|  || :       `    | | :| | :      |:| |
 .                .          .        || |.: *          | || : :     :|||
        .            .   . *    .   .  ` |||.  +        + '| |||  .  ||`
     .             *              .     +:`|!             . ||||  :.||`
 +                      .                ..!|*          . | :`||+ |||`
     .                         +      : |||`        .| :| | | |.| ||`     .
       *     +   '               +  :|| |`     :.+. || || | |:`|| `
                            .      .||` .    ..|| | |: '` `| | |`  +
  .       +++                      ||        !|!: `       :| |
              +         .      .    | .      `|||.:      .||    .      .    `
          '                           `|.   .  `:|||   + ||'     `
  __    +      *                         `'       `'|.    `:
"'  `---"""----....____,..^---`^``----.,.___          `.    `.  .    ____,.,-
    ___,--'""`---"'   ^  ^ ^        ^       """'---,..___ __,..---""'
--"'                           ^                         ``--..,__
    )" << "\n\n";
}

void mostrarAsciiArt2() {
    cout << R"(
                             ^^                   @@@@@@@@@
                        ^^       ^^            @@@@@@@@@@@@@@@
                                             @@@@@@@@@@@@@@@@@@              ^^
                                            @@@@@@@@@@@@@@@@@@@@
                  ~~~~ ~~ ~~~~~ ~~~~~~~~ ~~ &&&&&&&&&&&&&&&&&&&& ~~~~~~~ ~~~~~~~~~~~ ~~~
                  ~         ~~   ~  ~       ~~~~~~~~~~~~~~~~~~~~ ~       ~~     ~~ ~
                    ~      ~~      ~~ ~~ ~~  ~~~~~~~~~~~~~ ~~~~  ~     ~~~    ~ ~~~  ~ ~~ 
                    ~  ~~     ~         ~      ~~~~~~  ~~ ~~~       ~~ ~ ~~  ~~ ~ 
                  ~  ~       ~ ~      ~           ~~ ~~~~~~  ~      ~~  ~             ~~
                        ~             ~        ~      ~      ~~   ~             ~
    )" << "\n";
}

void recorrerPreorden(Nodo* nodo, std::vector<Nodo*>& lista) {
    if (!nodo) return;
    lista.push_back(nodo);
    recorrerPreorden(nodo->izq, lista);
    recorrerPreorden(nodo->der, lista); //  RID
}

void dibujarArbolPreorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font) {
    if (!raiz) return;

    std::vector<Nodo*> preorden;
    recorrerPreorden(raiz, preorden);

    sf::RectangleShape botonCerrar(sf::Vector2f(100, 40));
    botonCerrar.setFillColor(sf::Color(200, 50, 50));
    botonCerrar.setPosition(900, 740);

    sf::Text textoCerrar("Cerrar", font, 18);
    textoCerrar.setFillColor(sf::Color::White);
    textoCerrar.setPosition(920, 745);

    int altura = alturaArbol(raiz);                        // Calcula la altura total del árbol (número de niveles).
    int offsetX = 0;                                       // ayuda a calcular la posición horizontal (X)
    calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);   // asigna coordenadas x e y a cada nodo del árbol para poder dibujarlos.

    sf::RenderWindow wnd(sf::VideoMode(1000, 800), "Arbol ABB");   //ventana grafica 

    while (wnd.isOpen()) {
        sf::Event event;
        while (wnd.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                wnd.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float mouseX = event.mouseButton.x;
                    float mouseY = event.mouseButton.y;
                    if (botonCerrar.getGlobalBounds().contains(mouseX, mouseY)) {
                        wnd.close();
                    }
                }
            }
        }

        wnd.clear(sf::Color(220, 220, 220));
        dibujarNodo(wnd, raiz, nullptr, font);


        int startX = 50;
        int y = 500;
        int spacingX = 130;

        for (size_t i = 0; i < preorden.size(); ++i) {
            Nodo* nodo = preorden[i];
            int x = startX + i * spacingX;

            sf::CircleShape circle(50);
            circle.setFillColor(sf::Color(255, 255, 255));
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(2);
            circle.setPosition(x, y);
            wnd.draw(circle);

            std::vector<std::string> lineas = {
                "ID: " + std::to_string(nodo->id),
                "Nom: " + nodo->nombre
            };

            std::stringstream ss;
            ss << nodo;
            lineas.push_back(ss.str());

            float centroX = x + 50;
            float textoY = y + 10;

            for (const std::string& linea : lineas) {
                sf::Text texto;
                texto.setFont(font);
                texto.setCharacterSize(11);
                texto.setString(linea);
                texto.setFillColor(sf::Color::Black);
                sf::FloatRect bounds = texto.getLocalBounds();
                texto.setOrigin(bounds.left + bounds.width / 2.f, 0);
                texto.setPosition(centroX, textoY);
                wnd.draw(texto);
                textoY += 14;
            }
        }

        wnd.draw(botonCerrar);
        wnd.draw(textoCerrar);
        wnd.display();
    }
}

void recorrerInorden(Nodo* nodo, std::vector<Nodo*>& nodosInorden) {
    if (nodo == nullptr) return;
    recorrerInorden(nodo->izq, nodosInorden);
    nodosInorden.push_back(nodo);
    recorrerInorden(nodo->der, nodosInorden);
}

void dibujarArbolInorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font) {
    if (!raiz) return;

    std::vector<Nodo*> inorden;
    recorrerInorden(raiz, inorden);

    sf::RectangleShape botonCerrar(sf::Vector2f(100, 40));
    botonCerrar.setFillColor(sf::Color(200, 50, 50));
    botonCerrar.setPosition(900, 740);

    sf::Text textoCerrar("Cerrar", font, 18);
    textoCerrar.setFillColor(sf::Color::White);
    textoCerrar.setPosition(920, 745);

    int altura = alturaArbol(raiz);
    int offsetX = 0;
    calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);

    sf::RenderWindow wnd(sf::VideoMode(1000, 800), "Arbol ABB");

    while (wnd.isOpen()) {
        sf::Event event;
        while (wnd.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                wnd.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float mouseX = event.mouseButton.x;
                    float mouseY = event.mouseButton.y;
                    if (botonCerrar.getGlobalBounds().contains(mouseX, mouseY)) {
                        wnd.close();
                    }
                }
            }
        }

        wnd.clear(sf::Color(220, 220, 220));
        dibujarNodo(wnd, raiz, nullptr, font);

        int startX = 50;
        int y = 500;
        int spacingX = 130;

        for (size_t i = 0; i < inorden.size(); ++i) {
            Nodo* nodo = inorden[i];
            int x = startX + i * spacingX;

            sf::CircleShape circle(50);
            circle.setFillColor(sf::Color(255, 255, 255));
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(2);
            circle.setPosition(x, y);
            wnd.draw(circle);

            std::vector<std::string> lineas = {
                "ID: " + std::to_string(nodo->id),
                "Nom: " + nodo->nombre
            };

            std::stringstream ss;
            ss << nodo;
            lineas.push_back(ss.str());

            float centroX = x + 50;
            float textoY = y + 10;

            for (const std::string& linea : lineas) {
                sf::Text texto;
                texto.setFont(font);
                texto.setCharacterSize(11);
                texto.setString(linea);
                texto.setFillColor(sf::Color::Black);
                sf::FloatRect bounds = texto.getLocalBounds();
                texto.setOrigin(bounds.left + bounds.width / 2.f, 0);
                texto.setPosition(centroX, textoY);
                wnd.draw(texto);
                textoY += 14;
            }
        }

        wnd.draw(botonCerrar);
        wnd.draw(textoCerrar);
        wnd.display();
    }
}

void recorrerPostorden(Nodo* nodo, std::vector<Nodo*>& nodosPostorden) {
    if (nodo == nullptr) return;
    recorrerPostorden(nodo->izq, nodosPostorden);
    recorrerPostorden(nodo->der, nodosPostorden);
    nodosPostorden.push_back(nodo);
}

void dibujarArbolPostorden(sf::RenderWindow& window, Nodo* raiz, sf::Font& font) {
    if (!raiz) return;

    std::vector<Nodo*> postorden;
    recorrerPostorden(raiz, postorden);

    sf::RectangleShape botonCerrar(sf::Vector2f(100, 40));
    botonCerrar.setFillColor(sf::Color(200, 50, 50));
    botonCerrar.setPosition(900, 740);

    sf::Text textoCerrar("Cerrar", font, 18);
    textoCerrar.setFillColor(sf::Color::White);
    textoCerrar.setPosition(920, 745);

    int altura = alturaArbol(raiz);
    int offsetX = 0;
    calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);

    sf::RenderWindow wnd(sf::VideoMode(1000, 800), "Arbol ABB");

    while (wnd.isOpen()) {
        sf::Event event;
        while (wnd.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                wnd.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float mouseX = event.mouseButton.x;
                    float mouseY = event.mouseButton.y;
                    if (botonCerrar.getGlobalBounds().contains(mouseX, mouseY)) {
                        wnd.close();
                    }
                }
            }
        }

        wnd.clear(sf::Color(220, 220, 220));
        dibujarNodo(wnd, raiz, nullptr, font);

        int startX = 50;
        int y = 500;
        int spacingX = 130;

        for (size_t i = 0; i < postorden.size(); ++i) {
            Nodo* nodo = postorden[i];
            int x = startX + i * spacingX;

            sf::CircleShape circle(50);
            circle.setFillColor(sf::Color(255, 255, 255));
            circle.setOutlineColor(sf::Color::Black);
            circle.setOutlineThickness(2);
            circle.setPosition(x, y);
            wnd.draw(circle);

            std::vector<std::string> lineas = {
                "ID: " + std::to_string(nodo->id),
                "Nom: " + nodo->nombre
            };

            std::stringstream ss;
            ss << nodo;
            lineas.push_back(ss.str());

            float centroX = x + 50;
            float textoY = y + 10;

            for (const std::string& linea : lineas) {
                sf::Text texto;
                texto.setFont(font);
                texto.setCharacterSize(11);
                texto.setString(linea);
                texto.setFillColor(sf::Color::Black);
                sf::FloatRect bounds = texto.getLocalBounds();
                texto.setOrigin(bounds.left + bounds.width / 2.f, 0);
                texto.setPosition(centroX, textoY);
                wnd.draw(texto);
                textoY += 14;
            }
        }

        wnd.draw(botonCerrar);
        wnd.draw(textoCerrar);
        wnd.display();
    }
}

void eliminarVisual(sf::RenderWindow& window, Nodo*& raiz, sf::Font& font) {
    std::string input;
    int nodoSeleccionado = -1;
    bool nodoEncontrado = false;
    std::string infoNodo = "";
    std::string infoReemplazo = "";

    sf::RectangleShape botonEliminar(sf::Vector2f(120, 40));
    botonEliminar.setFillColor(sf::Color(200, 50, 50));
    botonEliminar.setPosition(740, 590);

    sf::Text textoEliminar("Eliminar", font, 18);
    textoEliminar.setFillColor(sf::Color::White);
    textoEliminar.setPosition(760, 595);

    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 590);

    sf::Text etiqueta("ID a eliminar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 560);

    sf::Text textoInfoNodo(infoNodo, font, 16);
    textoInfoNodo.setFillColor(sf::Color::Black);
    textoInfoNodo.setPosition(30, 630);

    sf::Text textoInfoReemplazo(infoReemplazo, font, 16);
    textoInfoReemplazo.setFillColor(sf::Color::Black);
    textoInfoReemplazo.setPosition(30, 660);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::TextEntered) {
                if (e.text.unicode == 8 && !input.empty()) {
                    input.pop_back();
                }
                else if (e.text.unicode == 13) { // Enter
                    nodoSeleccionado = std::stoi(input);
                    Nodo* nodo = buscarNodo(raiz, nodoSeleccionado);
                    nodoEncontrado = nodo != nullptr;

                    if (nodoEncontrado) {
                        // Determinar tipo de nodo
                        if (!nodo->izq && !nodo->der) {
                            infoNodo = "Nodo hoja (sin hijos)";
                            infoReemplazo = "Se eliminara directamente";
                        }
                        else if (!nodo->izq || !nodo->der) {
                            infoNodo = "Nodo con un hijo";
                            infoReemplazo = "Se reemplazara por su hijo";
                        }
                        else {
                            Nodo* sucesor = encontrarMinimo(nodo->der);
                            infoNodo = "Nodo con dos hijos";
                            infoReemplazo = "Sucesor: ID " + std::to_string(sucesor->id) + " (" + sucesor->nombre + ")";
                        }
                    }
                    else {
                        infoNodo = "Nodo no encontrado";
                        infoReemplazo = "";
                    }
                    textoInfoNodo.setString(infoNodo);
                    textoInfoReemplazo.setString(infoReemplazo);
                }
                else if (e.text.unicode >= 48 && e.text.unicode <= 57) {
                    input += static_cast<char>(e.text.unicode);
                }
                textoInput.setString(input);
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;
                    if (botonEliminar.getGlobalBounds().contains(mx, my)) {
                        if (nodoSeleccionado != -1 && nodoEncontrado) {
                            raiz = eliminarNodo(raiz, nodoSeleccionado);
                            nodoSeleccionado = -1;
                            input = "";
                            textoInput.setString("");
                            nodoEncontrado = false;
                            infoNodo = "";
                            infoReemplazo = "";
                            textoInfoNodo.setString(infoNodo);
                            textoInfoReemplazo.setString(infoReemplazo);
                        }
                    }
                }
            }
        }
        window.clear(sf::Color(220, 220, 220));

        int altura = alturaArbol(raiz);
        int offsetX = 0;
        calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);
        dibujarNodoConSeleccion(window, raiz, nullptr, font, nodoSeleccionado);

        window.draw(botonEliminar);
        window.draw(textoEliminar);
        window.draw(textoInput);
        window.draw(etiqueta);
        window.draw(textoInfoNodo);
        window.draw(textoInfoReemplazo);
        window.display();
    }
}

void dibujarNodoConSeleccion(sf::RenderWindow& window, Nodo* nodo, Nodo* padre, sf::Font& font, int idSeleccionado) {
    if (!nodo) return;

    // Color diferente según el tipo de nodo
    sf::Color colorNodo = sf::Color::White;
    if (nodo->id == idSeleccionado) {
        if (!nodo->izq && !nodo->der) {
            colorNodo = sf::Color(255, 200, 200); // Rosa claro para nodo hoja
        }
        else if (!nodo->izq || !nodo->der) {
            colorNodo = sf::Color(200, 255, 200); // Verde claro para nodo con un hijo
        }
        else {
            colorNodo = sf::Color(200, 200, 255); // Azul claro para nodo con dos hijos
        }
    }

    sf::CircleShape circle(50);
    circle.setFillColor(colorNodo);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setPosition(nodo->x - 50, nodo->y - 50);
    window.draw(circle);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::Black);

    float textoY = nodo->y - 20;
    text.setString("ID: " + std::to_string(nodo->id));
    text.setPosition(nodo->x - 20, textoY);
    window.draw(text);
    textoY += 14;

    text.setString("Nombre: " + nodo->nombre);
    text.setPosition(nodo->x - 35, textoY);
    window.draw(text);
    textoY += 14;

    std::stringstream ss;
    ss << nodo;
    text.setString(ss.str());
    text.setPosition(nodo->x - 30, textoY);
    window.draw(text);

    if (padre) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(padre->x, padre->y)),
            sf::Vertex(sf::Vector2f(nodo->x, nodo->y))
        };
        window.draw(line, 2, sf::Lines);
    }

    dibujarNodoConSeleccion(window, nodo->izq, nodo, font, idSeleccionado);
    dibujarNodoConSeleccion(window, nodo->der, nodo, font, idSeleccionado);
}


void modificarVisual(sf::RenderWindow& window, Nodo*& raiz, sf::Font& font) {
    std::string input;
    int nodoSeleccionado = -1;
    bool nodoEncontrado = false;
    Nodo* nodoAModificar = nullptr;
    bool mostrandoFormulario = false;

    // Variables para almacenar los strings de los valores numéricos
    std::string idStr, cantidadStr, precioCompraStr, precioVentaStr;

    // Botones para la primera pantalla (selección)
    sf::RectangleShape botonSeleccionar(sf::Vector2f(120, 40));
    botonSeleccionar.setFillColor(sf::Color(50, 150, 50));
    botonSeleccionar.setPosition(740, 590);

    sf::Text textoSeleccionar("Seleccionar", font, 18);
    textoSeleccionar.setFillColor(sf::Color::White);
    textoSeleccionar.setPosition(750, 595);

    // Botones para la segunda pantalla (modificación)
    sf::RectangleShape botonGuardar(sf::Vector2f(120, 40));
    botonGuardar.setFillColor(sf::Color(50, 150, 50));
    botonGuardar.setPosition(350, 400);

    sf::Text textoGuardar("Guardar", font, 18);
    textoGuardar.setFillColor(sf::Color::White);
    textoGuardar.setPosition(370, 405);

    sf::RectangleShape botonCancelar(sf::Vector2f(120, 40));
    botonCancelar.setFillColor(sf::Color(200, 50, 50));
    botonCancelar.setPosition(500, 400);

    sf::Text textoCancelar("Cancelar", font, 18);
    textoCancelar.setFillColor(sf::Color::White);
    textoCancelar.setPosition(520, 405);

    // Campos de texto
    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 590);

    sf::Text etiqueta("ID a modificar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 560);

    // Campos del formulario
    std::vector<sf::Text> etiquetasCampos;
    std::vector<sf::Text> valoresCampos;
    std::vector<std::string> nombresCampos = { "ID:", "Nombre:", "Codigo:", "Descripcion:", "Cantidad:",
                                             "Precio Compra:", "Precio Venta:", "Fecha:", "Proveedor:" };
    std::vector<std::string*> valoresPtr = { &idStr, nullptr, nullptr, nullptr, &cantidadStr,
                                          &precioCompraStr, &precioVentaStr, nullptr, nullptr };

    for (int i = 0; i < nombresCampos.size(); i++) {
        sf::Text etiqueta(nombresCampos[i], font, 16);
        etiqueta.setFillColor(sf::Color::Black);
        etiqueta.setPosition(50, 50 + i * 30);
        etiquetasCampos.push_back(etiqueta);

        sf::Text valor("", font, 16);
        valor.setFillColor(sf::Color::Black);
        valor.setPosition(250, 50 + i * 30);
        valoresCampos.push_back(valor);
    }

    // Variable para controlar qué campo está seleccionado
    int campoSeleccionado = -1;
    std::string inputModificacion;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::TextEntered) {
                if (!mostrandoFormulario) {
                    // Primera pantalla: selección de nodo
                    if (e.text.unicode == 8 && !input.empty()) {
                        input.pop_back();
                    }
                    else if (e.text.unicode == 13) { // Enter
                        nodoSeleccionado = std::stoi(input);
                        nodoAModificar = buscarNodo(raiz, nodoSeleccionado);
                        nodoEncontrado = nodoAModificar != nullptr;

                        if (nodoEncontrado) {
                            // Preparar datos para el formulario
                            idStr = std::to_string(nodoAModificar->id);
                            cantidadStr = std::to_string(nodoAModificar->cantidad);
                            precioCompraStr = std::to_string(nodoAModificar->precioc);
                            precioVentaStr = std::to_string(nodoAModificar->preciov);

                            valoresPtr[1] = &nodoAModificar->nombre;
                            valoresPtr[2] = &nodoAModificar->codigo;
                            valoresPtr[3] = &nodoAModificar->descripcion;
                            valoresPtr[7] = &nodoAModificar->fecha;
                            valoresPtr[8] = &nodoAModificar->proveedor;

                            for (int i = 0; i < valoresCampos.size(); i++) {
                                valoresCampos[i].setString(*valoresPtr[i]);
                            }

                            mostrandoFormulario = true;
                        }
                    }
                    else if (e.text.unicode >= 48 && e.text.unicode <= 57) {
                        input += static_cast<char>(e.text.unicode);
                    }
                    textoInput.setString(input);
                }
                else {
                    // Segunda pantalla: modificación de campos
                    if (campoSeleccionado >= 0) {
                        if (e.text.unicode == 8 && !inputModificacion.empty()) {
                            inputModificacion.pop_back();
                        }
                        else if (e.text.unicode == 13) { // Enter - guardar cambios
                            try {
                                if (campoSeleccionado == 0) {
                                    nodoAModificar->id = std::stoi(inputModificacion);
                                    idStr = inputModificacion;
                                }
                                else if (campoSeleccionado == 1) nodoAModificar->nombre = inputModificacion;
                                else if (campoSeleccionado == 2) nodoAModificar->codigo = inputModificacion;
                                else if (campoSeleccionado == 3) nodoAModificar->descripcion = inputModificacion;
                                else if (campoSeleccionado == 4) {
                                    nodoAModificar->cantidad = std::stoi(inputModificacion);
                                    cantidadStr = inputModificacion;
                                }
                                else if (campoSeleccionado == 5) {
                                    nodoAModificar->precioc = std::stoi(inputModificacion);
                                    precioCompraStr = inputModificacion;
                                }
                                else if (campoSeleccionado == 6) {
                                    nodoAModificar->preciov = std::stoi(inputModificacion);
                                    precioVentaStr = inputModificacion;
                                }
                                else if (campoSeleccionado == 7) nodoAModificar->fecha = inputModificacion;
                                else if (campoSeleccionado == 8) nodoAModificar->proveedor = inputModificacion;

                                valoresCampos[campoSeleccionado].setString(inputModificacion);
                            }
                            catch (...) {
                                // Manejar error de conversión numérica
                                valoresCampos[campoSeleccionado].setString("ERROR");
                            }
                            campoSeleccionado = -1;
                            inputModificacion = "";
                        }
                        else if (e.text.unicode >= 32) {
                            inputModificacion += static_cast<char>(e.text.unicode);
                        }
                    }
                }
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;

                    if (!mostrandoFormulario) {
                        // Primera pantalla: selección de nodo
                        if (botonSeleccionar.getGlobalBounds().contains(mx, my) && !input.empty()) {
                            nodoSeleccionado = std::stoi(input);
                            nodoAModificar = buscarNodo(raiz, nodoSeleccionado);
                            nodoEncontrado = nodoAModificar != nullptr;

                            if (nodoEncontrado) {
                                // Preparar datos para el formulario
                                idStr = std::to_string(nodoAModificar->id);
                                cantidadStr = std::to_string(nodoAModificar->cantidad);
                                precioCompraStr = std::to_string(nodoAModificar->precioc);
                                precioVentaStr = std::to_string(nodoAModificar->preciov);

                                valoresPtr[1] = &nodoAModificar->nombre;
                                valoresPtr[2] = &nodoAModificar->codigo;
                                valoresPtr[3] = &nodoAModificar->descripcion;
                                valoresPtr[7] = &nodoAModificar->fecha;
                                valoresPtr[8] = &nodoAModificar->proveedor;

                                for (int i = 0; i < valoresCampos.size(); i++) {
                                    valoresCampos[i].setString(*valoresPtr[i]);
                                }

                                mostrandoFormulario = true;
                            }
                        }
                    }
                    else {
                        // Segunda pantalla: modificación de campos
                        // Verificar si se hizo clic en algún campo
                        for (int i = 0; i < valoresCampos.size(); i++) {
                            sf::FloatRect bounds = valoresCampos[i].getGlobalBounds();
                            if (bounds.contains(mx, my)) {
                                campoSeleccionado = i;
                                inputModificacion = valoresCampos[i].getString();
                                break;
                            }
                        }

                        // Verificar botones
                        if (botonGuardar.getGlobalBounds().contains(mx, my)) {
                            // Guardar cambios y volver al árbol
                            mostrandoFormulario = false;
                            input = "";
                            textoInput.setString("");
                            nodoSeleccionado = -1;
                            campoSeleccionado = -1;
                        }
                        else if (botonCancelar.getGlobalBounds().contains(mx, my)) {
                            // Cancelar y volver al árbol sin guardar
                            mostrandoFormulario = false;
                            input = "";
                            textoInput.setString("");
                            nodoSeleccionado = -1;
                            campoSeleccionado = -1;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(220, 220, 220));

        if (!mostrandoFormulario) {
            // Primera pantalla: mostrar árbol y seleccionar nodo
            int altura = alturaArbol(raiz);
            int offsetX = 0;
            calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);
            dibujarNodoConSeleccion(window, raiz, nullptr, font, nodoSeleccionado);

            window.draw(botonSeleccionar);
            window.draw(textoSeleccionar);
            window.draw(textoInput);
            window.draw(etiqueta);
        }
        else {
            // Segunda pantalla: formulario de modificación
            sf::Text titulo("Modificando Nodo ID: " + std::to_string(nodoAModificar->id), font, 20);
            titulo.setFillColor(sf::Color::Black);
            titulo.setPosition(300, 10);
            window.draw(titulo);

            // Dibujar etiquetas y valores
            for (int i = 0; i < etiquetasCampos.size(); i++) {
                // Resaltar campo seleccionado
                if (i == campoSeleccionado) {
                    sf::RectangleShape fondo(sf::Vector2f(300, 25));
                    fondo.setFillColor(sf::Color(200, 230, 255));
                    fondo.setPosition(245, 45 + i * 30);
                    window.draw(fondo);

                    // Mostrar input actual
                    sf::Text textoInput(inputModificacion, font, 16);
                    textoInput.setFillColor(sf::Color::Black);
                    textoInput.setPosition(250, 50 + i * 30);
                    window.draw(textoInput);
                }
                else {
                    window.draw(valoresCampos[i]);
                }
                window.draw(etiquetasCampos[i]);
            }

            window.draw(botonGuardar);
            window.draw(textoGuardar);
            window.draw(botonCancelar);
            window.draw(textoCancelar);
        }

        window.display();
    }
}


void buscarVisual(sf::RenderWindow& window, Nodo* raiz, sf::Font& font) {
    std::string input;
    int nodoBuscado = -1;
    Nodo* nodoEncontrado = nullptr;
    bool mostrandoResultado = false;

    // Variables para almacenar los strings de los valores
    std::string idStr, cantidadStr, precioCompraStr, precioVentaStr;

    // Botón de búsqueda (ahora oculto cuando se muestra el resultado)
    sf::RectangleShape botonBuscar(sf::Vector2f(120, 40));
    botonBuscar.setFillColor(sf::Color(50, 150, 50));
    botonBuscar.setPosition(740, 590);

    sf::Text textoBuscar("Buscar", font, 18);
    textoBuscar.setFillColor(sf::Color::White);
    textoBuscar.setPosition(760, 595);

    // Botón de regreso
    sf::RectangleShape botonRegresar(sf::Vector2f(120, 40));
    botonRegresar.setFillColor(sf::Color(200, 50, 50));
    botonRegresar.setPosition(350, 400);

    sf::Text textoRegresar("Regresar", font, 18);
    textoRegresar.setFillColor(sf::Color::White);
    textoRegresar.setPosition(370, 405);

    // Campos de texto
    sf::Text textoInput("", font, 18);
    textoInput.setFillColor(sf::Color::Black);
    textoInput.setPosition(30, 590);

    sf::Text etiqueta("ID a buscar:", font, 18);
    etiqueta.setFillColor(sf::Color::Black);
    etiqueta.setPosition(30, 560);

    // Campos del resultado
    std::vector<sf::Text> etiquetasCampos;
    std::vector<sf::Text> valoresCampos;
    std::vector<std::string> nombresCampos = { "ID:", "Nombre:", "Codigo:", "Descripcion:", "Cantidad:",
                                             "Precio Compra:", "Precio Venta:", "Fecha:", "Proveedor:" };

    for (int i = 0; i < nombresCampos.size(); i++) {
        sf::Text etiqueta(nombresCampos[i], font, 16);
        etiqueta.setFillColor(sf::Color::Black);
        etiqueta.setPosition(50, 50 + i * 30);
        etiquetasCampos.push_back(etiqueta);

        sf::Text valor("", font, 16);
        valor.setFillColor(sf::Color::Black);
        valor.setPosition(250, 50 + i * 30);
        valoresCampos.push_back(valor);
    }

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::TextEntered) {
                if (!mostrandoResultado) {
                    // Fase de entrada del ID a buscar
                    if (e.text.unicode == 8 && !input.empty()) { // Backspace
                        input.pop_back();
                    }
                    else if (e.text.unicode == 13 && !input.empty()) { // Enter - realizar búsqueda
                        nodoBuscado = std::stoi(input);
                        nodoEncontrado = buscarNodo(raiz, nodoBuscado);

                        if (nodoEncontrado != nullptr) {
                            // Mostrar los datos del nodo encontrado
                            idStr = std::to_string(nodoEncontrado->id);
                            cantidadStr = std::to_string(nodoEncontrado->cantidad);
                            precioCompraStr = std::to_string(nodoEncontrado->precioc);
                            precioVentaStr = std::to_string(nodoEncontrado->preciov);

                            valoresCampos[0].setString(idStr);
                            valoresCampos[1].setString(nodoEncontrado->nombre);
                            valoresCampos[2].setString(nodoEncontrado->codigo);
                            valoresCampos[3].setString(nodoEncontrado->descripcion);
                            valoresCampos[4].setString(cantidadStr);
                            valoresCampos[5].setString(precioCompraStr);
                            valoresCampos[6].setString(precioVentaStr);
                            valoresCampos[7].setString(nodoEncontrado->fecha);
                            valoresCampos[8].setString(nodoEncontrado->proveedor);

                            mostrandoResultado = true;
                        }
                    }
                    else if (e.text.unicode >= 48 && e.text.unicode <= 57) { // Dígitos
                        input += static_cast<char>(e.text.unicode);
                    }
                    textoInput.setString(input);
                }
            }
            else if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    float mx = e.mouseButton.x, my = e.mouseButton.y;

                    if (!mostrandoResultado) {
                        // Fase de búsqueda (también funciona con clic en botón Buscar)
                        if (botonBuscar.getGlobalBounds().contains(mx, my) && !input.empty()) {
                            nodoBuscado = std::stoi(input);
                            nodoEncontrado = buscarNodo(raiz, nodoBuscado);

                            if (nodoEncontrado != nullptr) {
                                // Mostrar los datos del nodo encontrado
                                idStr = std::to_string(nodoEncontrado->id);
                                cantidadStr = std::to_string(nodoEncontrado->cantidad);
                                precioCompraStr = std::to_string(nodoEncontrado->precioc);
                                precioVentaStr = std::to_string(nodoEncontrado->preciov);

                                valoresCampos[0].setString(idStr);
                                valoresCampos[1].setString(nodoEncontrado->nombre);
                                valoresCampos[2].setString(nodoEncontrado->codigo);
                                valoresCampos[3].setString(nodoEncontrado->descripcion);
                                valoresCampos[4].setString(cantidadStr);
                                valoresCampos[5].setString(precioCompraStr);
                                valoresCampos[6].setString(precioVentaStr);
                                valoresCampos[7].setString(nodoEncontrado->fecha);
                                valoresCampos[8].setString(nodoEncontrado->proveedor);

                                mostrandoResultado = true;
                            }
                        }
                    }
                    else {
                        // Fase de visualización de resultados
                        if (botonRegresar.getGlobalBounds().contains(mx, my)) {
                            // Volver a la pantalla de búsqueda
                            mostrandoResultado = false;
                            input = "";
                            textoInput.setString("");
                            nodoBuscado = -1;
                            nodoEncontrado = nullptr;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(220, 220, 220));

        if (!mostrandoResultado) {
            // Pantalla de búsqueda
            int altura = alturaArbol(raiz);
            int offsetX = 0;
            calcularPosiciones(raiz, 0, 50, 1, altura, offsetX);
            dibujarNodoConSeleccion(window, raiz, nullptr, font, (nodoEncontrado ? nodoEncontrado->id : -1));

            window.draw(botonBuscar);
            window.draw(textoBuscar);
            window.draw(textoInput);
            window.draw(etiqueta);

            // Mostrar mensaje si no se encontró el nodo (después de una búsqueda fallida)
            if (nodoBuscado != -1 && nodoEncontrado == nullptr) {
                sf::Text textoNoEncontrado("Nodo no encontrado!", font, 20);
                textoNoEncontrado.setFillColor(sf::Color::Red);
                textoNoEncontrado.setPosition(300, 630);
                window.draw(textoNoEncontrado);
            }
        }
        else {
            // Pantalla de resultados
            sf::Text titulo("Resultado de Busqueda - ID: " + idStr, font, 20);
            titulo.setFillColor(sf::Color::Black);
            titulo.setPosition(250, 10);
            window.draw(titulo);

            // Dibujar todos los campos
            for (int i = 0; i < etiquetasCampos.size(); i++) {
                window.draw(etiquetasCampos[i]);
                window.draw(valoresCampos[i]);
            }

            window.draw(botonRegresar);
            window.draw(textoRegresar);
        }

        window.display();
    }
}


void arbolABB() {

    menu();
}
