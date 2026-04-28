#include"encabezado.h"

struct NodoL
{
    int ID;
    string nombre;
    string tipo;
    int cantidad;
    int precio;
    string fecha;
    string proveedor;
    string codigo;
    string origen;
    NodoL* siguiente;
} PODUCTO;


//prototipos de funciones
void Menu_Lista();
void Insertar_Lista(NodoL*&, int, string, string, int, int, string, string, string, string);
NodoL* Buscar_Lista(NodoL*, int);// parametros
void Eliminar_Lista(NodoL*&, int);
void Modificar_Lista(NodoL*&, int);
void Vaciar_Lista(NodoL*&);
void Mostrar_Lista(NodoL*);
void MostrarArteComplejo2();
void DibujarRecuadro4();



void Insertar_Lista(NodoL*& lista, int i, string n, string t, int ca, int pre, string f, string pro, string co, string o)
{
    string fecha = obtenerFechaActual();// Obtiene la fecha actual
    NodoL* nuevo_nodo = new NodoL();// Crea un nuevo nodo
    nuevo_nodo->ID = i;// Asigna el ID al nuevo nodo
    nuevo_nodo->nombre = n;// Asigna el nombre al nuevo nodo
    nuevo_nodo->tipo = t;
    nuevo_nodo->cantidad = ca;
    nuevo_nodo->precio = pre;
    nuevo_nodo->fecha = f;
    nuevo_nodo->proveedor = pro;
    nuevo_nodo->codigo = co;
    nuevo_nodo->origen = o;

    NodoL* aux1 = lista;// Puntero auxiliar para recorrer la lista
    NodoL* aux2 = nullptr; // Puntero auxiliar para mantener el nodo anterior

    // Ordenar por cantidad: busca la posición correcta para insertar el nuevo nodo
    while ((aux1 != nullptr) && (aux1->cantidad < ca))
    {
        aux2 = aux1; // aux2 ahora apunta al nodo actual
        aux1 = aux1->siguiente; // aux1 avanza al siguiente nodo
    }

    // Insertar el nuevo nodo en la posición correcta
    if (aux2 == nullptr)    // Si aux2 es nullptr, el nuevo nodo debe ser el primero
    {
        // Si aux2 es nullptr, el nuevo nodo debe ser el primero
        nuevo_nodo->siguiente = lista;
        lista = nuevo_nodo;
    }
    else
    {
        // Insertar el nuevo nodo después de aux2
        nuevo_nodo->siguiente = aux1;
        aux2->siguiente = nuevo_nodo;
    }
}


void Mostrar_Lista(NodoL* lista) {
    // Verificar si la lista está vacía
    if (lista == nullptr) {
        gotoxy(40, 10); cout << "La lista esta vacia. No hay productos para mostrar." << endl << endl;
        return;
    }

    NodoL* actual = lista; // Puntero para recorrer la lista
    NodoL* ultimo = lista; // Puntero para encontrar el último nodo

    // Encontrar el último nodo
    while (ultimo->siguiente != nullptr) {
        ultimo = ultimo->siguiente;
    }

    // Recorrer la lista y mostrar los detalles de cada producto
    while (actual != nullptr) {

        if (actual == lista) { // Si es el primer nodo
            cout << "    \xDB\xDB\xDB* Este es el PRIMER nodo de la lista *\xDB\xDB\xDB" << endl;
        }
        cout << "    | === Detalles del Producto ===" << endl;
        cout << "    | Direccion de memoria del nodo: " << "   \xDB " << actual << " \xDB" << endl;
        cout << "    | ID: " << actual->ID << "   \xDB " << &actual->ID << " \xDB" << endl;
        cout << "    | Nombre: " << actual->nombre << "    \xDB" << &actual->nombre << "\xDB" << endl;
        cout << "    | Tipo: " << actual->tipo << "    \xDB " << &actual->tipo << " \xDB" << endl;
        cout << "    | Cantidad: " << actual->cantidad << "    \xDB" << &actual->cantidad << "\xDB" << endl;
        cout << "    | Precio: " << actual->precio << "    \xDB " << &actual->precio << " \xDB" << endl;
        cout << "    | Fecha: " << actual->fecha << "    \xDB " << &actual->fecha << " \xDB" << endl;
        cout << "    | Proveedor: " << actual->proveedor << "   \xDB " << &actual->proveedor << " \xDB" << endl;
        cout << "    | Codigo: " << actual->codigo << "    \xDB " << &actual->codigo << " \xDB" << endl;
        cout << "    | Origen: " << actual->origen << "    \xDB " << &actual->origen << " \xDB" << endl;
        cout << "    | ============================" << endl << endl;
        if (actual == ultimo) { // Si es el último nodo
            cout << "    \xDB\xDB\xDB*Este es el ULTIMO nodo de la lista *\xDB\xDB\xDB" << endl;
        }



        actual = actual->siguiente; // Avanzar al siguiente nodo
    }
}

void Eliminar_Lista(NodoL*& lista, int id) {
    // Verificar si la lista está vacía
    if (lista == nullptr) {
        gotoxy(38, 11); cout << "La lista está vacía. No hay nada que eliminar." << endl << endl;
        return;
    }

    // Buscar el nodo con el ID proporcionado
    NodoL* nodo_a_eliminar = Buscar_Lista(lista, id);

    // Si no se encontró el nodo
    if (nodo_a_eliminar == nullptr) {
        gotoxy(40, 12); cout << "No se encontró un producto con el ID " << id << "." << endl << endl;
        return;
    }

    // Mostrar el nodo antes de eliminarlo con todos sus datos
    gotoxy(40, 14); cout << "Se eliminará el siguiente nodo:" << endl;
    gotoxy(40, 15); cout << "ID: " << nodo_a_eliminar->ID << endl;
    gotoxy(40, 16); cout << "Nombre: " << nodo_a_eliminar->nombre << endl;
    gotoxy(40, 17); cout << "Tipo: " << nodo_a_eliminar->tipo << endl;
    gotoxy(40, 18); cout << "Cantidad: " << nodo_a_eliminar->cantidad << endl;
    gotoxy(40, 19); cout << "Precio: " << nodo_a_eliminar->precio << endl;
    gotoxy(40, 20); cout << "Fecha: " << nodo_a_eliminar->fecha << endl;
    gotoxy(40, 21); cout << "Proveedor: " << nodo_a_eliminar->proveedor << endl;
    gotoxy(40, 22); cout << "Código: " << nodo_a_eliminar->codigo << endl;
    gotoxy(40, 23); cout << "Origen: " << nodo_a_eliminar->origen << endl;
    gotoxy(40, 24); cout << "------------------------------------" << endl;

    // Si el nodo a eliminar es el primero de la lista
    if (nodo_a_eliminar == lista) {
        lista = lista->siguiente; // El segundo nodo ahora es el primero
        delete nodo_a_eliminar; // Liberar memoria del nodo eliminado
        gotoxy(40, 26); cout << "Producto con ID " << id << " eliminado correctamente." << endl << endl;
    }
    else {
        // Buscar el nodo anterior al que se va a eliminar
        NodoL* anterior = lista;
        while (anterior->siguiente != nodo_a_eliminar) {
            anterior = anterior->siguiente;
        }

        // Saltar el nodo a eliminar
        anterior->siguiente = nodo_a_eliminar->siguiente;
        delete nodo_a_eliminar; // Liberar memoria del nodo eliminado
        gotoxy(40, 26); cout << "Producto con ID " << id << " eliminado correctamente." << endl;
    }
}


void Modificar_Lista(NodoL*& lista, int id) {
    // Buscar el nodo con el ID proporcionado
    NodoL* nodo_a_modificar = Buscar_Lista(lista, id);

    // Si no se encontró el nodo
    if (nodo_a_modificar == nullptr) {
        gotoxy(40, 12); cout << "No se encontro un producto con el ID " << id << "." << endl << endl;
        return;
    }

    int opcion = 1;
    char tecla;
    do {
        system("cls");

        MostrarArteComplejo2();
        gotoxy(45, 10); cout << "=== Modificar Producto (ID: " << id << ") ===\n";
        gotoxy(45, 12); cout << (opcion == 1 ? "--> " : "    ") << "1. Nombre: " << nodo_a_modificar->nombre << "\n";
        gotoxy(45, 13); cout << (opcion == 2 ? "--> " : "    ") << "2. Tipo: " << nodo_a_modificar->tipo << "\n";
        gotoxy(45, 14); cout << (opcion == 3 ? "--> " : "    ") << "3. Cantidad: " << nodo_a_modificar->cantidad << "\n";
        gotoxy(45, 15); cout << (opcion == 4 ? "--> " : "    ") << "4. Precio: " << nodo_a_modificar->precio << "\n";
        gotoxy(45, 16); cout << (opcion == 5 ? "--> " : "    ") << "5. Fecha: " << nodo_a_modificar->fecha << "\n";
        gotoxy(45, 17); cout << (opcion == 6 ? "--> " : "    ") << "6. Proveedor: " << nodo_a_modificar->proveedor << "\n";
        gotoxy(45, 18); cout << (opcion == 7 ? "--> " : "    ") << "7. Codigo: " << nodo_a_modificar->codigo << "\n";
        gotoxy(45, 19); cout << (opcion == 8 ? "--> " : "    ") << "8. Origen: " << nodo_a_modificar->origen << "\n";
        gotoxy(45, 20); cout << (opcion == 9 ? "--> " : "    ") << "9. Salir\n";
        gotoxy(45, 21); cout << "\nUse las flechas para navegar y Enter para seleccionar.";

        tecla = _getch();
        if (tecla == 72 && opcion > 1) opcion--; // Flecha arriba
        if (tecla == 80 && opcion < 9) opcion++; // Flecha abajo

        if (tecla == 13) { // Enter
            system("cls");
            if (opcion == 9) break;
            cout << "Ingrese el nuevo valor: ";
            switch (opcion) {
            case 1: cin >> nodo_a_modificar->nombre; break;
            case 2: cin >> nodo_a_modificar->tipo; break;
            case 3: cin >> nodo_a_modificar->cantidad; break;
            case 4: cin >> nodo_a_modificar->precio; break;
            case 5: cin >> nodo_a_modificar->fecha; break;
            case 6: cin >> nodo_a_modificar->proveedor; break;
            case 7: cin >> nodo_a_modificar->codigo; break;
            case 8: cin >> nodo_a_modificar->origen; break;
            }
        }
    } while (true);

    gotoxy(40, 12); cout << "Producto con ID " << id << " modificado correctamente." << endl;
}

void Vaciar_Lista(NodoL*& lista) {
    // Verificar si la lista está vacía
    if (lista == nullptr) {
        gotoxy(45, 10); cout << "La lista ya esta vacia." << endl << endl;
        return;
    }

    NodoL* actual = lista; // Puntero para recorrer la lista
    NodoL* siguiente = nullptr; // Puntero auxiliar para guardar el siguiente nodo

    // Recorrer la lista y liberar la memoria de cada nodo
    while (actual != nullptr) {
        siguiente = actual->siguiente; // Guardar el siguiente nodo
        delete actual; // Liberar la memoria del nodo actual
        actual = siguiente; // Mover al siguiente nodo
    }

    lista = nullptr; // La lista ahora está vacía
    gotoxy(45, 10); cout << "La lista ha sido vaciada correctamente." << endl << endl;
}

NodoL* Buscar_Lista(NodoL* lista, int id) {
    // Verifica si la lista está vacía
    if (lista == nullptr) {
        gotoxy(40, 12); cout << "La Lista Esta Vacia" << endl << endl;
        return nullptr; // Devuelve nullptr si la lista está vacía
    }

    NodoL* actual = lista; // Puntero para recorrer la lista

    // Busca el nodo con el ID proporcionado
    while (actual != nullptr && actual->ID != id) {
        actual = actual->siguiente;
    }

    // Si se encontró el nodo
    if (actual != nullptr) {
        gotoxy(40, 10); cout << "=== Producto Encontrado ===" << endl;
        gotoxy(40, 11); cout << "Dirección de memoria del nodo: " << actual << endl; // M
        gotoxy(40, 12); cout << "ID: " << actual->ID << endl;
        gotoxy(40, 13); cout << "Nombre: " << actual->nombre << endl;
        gotoxy(40, 14); cout << "Tipo: " << actual->tipo << endl;
        gotoxy(40, 15);  cout << "Cantidad: " << actual->cantidad << endl;
        gotoxy(40, 16);  cout << "Precio: " << actual->precio << endl;
        gotoxy(40, 17); cout << "Fecha: " << actual->fecha << endl;
        gotoxy(40, 18); cout << "Proveedor: " << actual->proveedor << endl;
        gotoxy(40, 19); cout << "Codigo: " << actual->codigo << endl;
        gotoxy(40, 20); cout << "Origen: " << actual->origen << endl;
        gotoxy(40, 21); cout << "==========================" << endl;
    }
    // Si no se encontró el nodo
    else {
        gotoxy(40, 12); cout << "No se encontro un producto con el ID " << id << "." << endl;
    }

    return actual; // Devuelve el nodo encontrado o nullptr si no se encuentra
}

void Menu_Lista() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    NodoL* lista = nullptr;
    int opcion = 1, id, cantidad, precio;
    string nombre, tipo, fecha, proveedor, codigo, origen;
    char tecla;
    char continuar;

    do {
        system("cls");
        int numero = generarNumeroAleatorio();
        grafica(numero);

        // Dibujo decorativo
        SetConsoleTextAttribute(hConsole, 3);
        gotoxy(90, 10); cout << "    (  )   (   )  )\n";
        SetConsoleTextAttribute(hConsole, 3);
        gotoxy(90, 11); cout << "     ) (   )  (  (\n";
        SetConsoleTextAttribute(hConsole, 3);
        gotoxy(90, 12); cout << "     ( )  (    ) )\n";
        SetConsoleTextAttribute(hConsole, 10);
        gotoxy(90, 13); cout << "     _____________\n";
        gotoxy(90, 14); cout << "    <_____________> ___\n";
        gotoxy(90, 15); cout << "    |             |/ _ \\\n";
        gotoxy(90, 16); cout << "    |              | | |\n";
        gotoxy(90, 17); cout << "    |              |_| |\n";
        gotoxy(90, 18);  cout << " ___|             |\___/\n";
        gotoxy(90, 19); cout << "/    \\___________/    \\\n";
        gotoxy(90, 20); cout << "\\_____________________/\n\n";


        gotoxy(3, 10); cout << "                 *       +";
        gotoxy(3, 11); cout << "           '                  |";
        gotoxy(3, 12); cout << "       ()    .-.,=\"``\"=.    - o -";
        gotoxy(3, 13); cout << "             '=/_       \\     |";
        gotoxy(3, 14); cout << "          *   |  '=._    |";
        gotoxy(3, 15); cout << "               \\     `=./`,        '";
        gotoxy(3, 16); cout << "            .   '=.__.=' `='      *";
        gotoxy(3, 17); cout << "   +                         +";
        gotoxy(3, 18); cout << "        O      *        '       .";

        // Menú
        SetConsoleTextAttribute(hConsole, 2);
        gotoxy(45, 10); cout << "=== Menu de Productos de Cafe(LISTA SIMPLE) orden ascendente ===\n";
        SetConsoleTextAttribute(hConsole, 3);

        gotoxy(45, 12); cout << (opcion == 1 ? "--> " : "    ") << "1. Agregar Producto\n";
        SetConsoleTextAttribute(hConsole, 4);
        gotoxy(45, 13); cout << (opcion == 2 ? "--> " : "    ") << "2. Mostrar Productos\n";
        SetConsoleTextAttribute(hConsole, 4);
        gotoxy(45, 14); cout << (opcion == 3 ? "--> " : "    ") << "3. Eliminar Producto\n";
        SetConsoleTextAttribute(hConsole, 5);
        gotoxy(45, 15); cout << (opcion == 4 ? "--> " : "    ") << "4. Modificar Producto\n";
        SetConsoleTextAttribute(hConsole, 6);
        gotoxy(45, 16); cout << (opcion == 5 ? "--> " : "    ") << "5. Buscar Producto\n";
        SetConsoleTextAttribute(hConsole, 7);
        gotoxy(45, 17); cout << (opcion == 6 ? "--> " : "    ") << "6. Vaciar Lista\n";
        SetConsoleTextAttribute(hConsole, 10);
        gotoxy(45, 18); cout << (opcion == 7 ? "--> " : "    ") << "7. Salir\n";

        tecla = _getch(); // Captura la tecla presionada

        if (tecla == 72 && opcion > 1) { // Flecha arriba
            opcion--;
            PlaySound(L"..\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC);
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

        }
        else if (tecla == 80 && opcion < 7) { // Flecha abajo
            opcion++;
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        }
        else if (tecla == 13) { // Enter
            system("cls");
            if (opcion == 7) {
                cout << "Saliendo..." << endl;
                break;
            }
            switch (opcion) {
            case 1:

                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

                do {
                    DibujarRecuadro4();  // Dibuja el recuadro
                    fecha = obtenerFechaActual(); // Asignar la fecha actual

                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 7); cin >> id;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 8); cin >> nombre;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 13); cin >> tipo;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 14); cin >> cantidad;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 15); cin >> precio;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 16); cout << fecha;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 21); cin >> proveedor;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 22); cin >> codigo;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    gotoxy(45, 23); cin >> origen;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                    Insertar_Lista(lista, id, nombre, tipo, cantidad, precio, fecha, proveedor, codigo, origen);
                    cout << endl;
                    cout << endl;
                    gotoxy(45, 27); cout << "Desea agregar otro producto? (S/N): ";
                    cin >> continuar;
                } while (continuar == 'S' || continuar == 's'); // Repetir si el usuario ingresa 
                break;
            case 2:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                Mostrar_Lista(lista);
                break;
            case 3:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                gotoxy(40, 8);  cout << "Ingrese el ID del producto a eliminar: "; cin >> id;
                Eliminar_Lista(lista, id);
                break;
            case 4:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                gotoxy(40, 10); cout << "Ingrese el ID del producto a modificar: "; cin >> id;
                Modificar_Lista(lista, id);
                break;
            case 5:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                gotoxy(40, 10); cout << "Ingrese el ID del producto a buscar: "; cin >> id;
                Buscar_Lista(lista, id);
                break;
            case 6:
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                Vaciar_Lista(lista);
                break;
            }
            system("pause");
        }
    } while (true);
}

void MostrarArteComplejo2() {

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

void dibujarCuadro4(int x1, int y1, int x2, int y2) {
    int i;

    for (i = x1; i < x2; i++) {
        gotoxy(i, y1); cout << (char)220;// Línea horizontal superior (\304 - 196 decimal: ?)
        gotoxy(i, y2); cout << (char)220; // Línea horizontal superior (\304 - 196 decimal: ?)
    }

    for (i = y1; i < y2; i++) {
        gotoxy(x1, i); cout << (char)219; // Línea vertical izquierda (\263 - 179 decimal: ?)
        gotoxy(x2, i); cout << (char)219; // Línea vertical izquierda (\263 - 179 decimal: ?)
    }

    gotoxy(x1, y1); cout << (char)220;// Esquina superior izquierda (\332 - 218 decimal: ?)
    gotoxy(x1, y2); cout << (char)220;// Esquina inferior izquierda (\300 - 192 decimal: ?)
    gotoxy(x2, y1); cout << (char)220;;// Esquina superior derecha (\277 - 191 decimal: ?)
    gotoxy(x2, y2); cout << (char)220; // Esquina inferior derecha (\331 - 217 decimal: ?)
}

void DibujarRecuadro4() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls"); // Limpiar la pantalla
    SetConsoleTextAttribute(hConsole, 15);

    dibujarCuadro4(10, 0, 100, 4); // Dibujar el cuadro
    dibujarCuadro4(10, 4, 100, 25); // Dibujar el cuadro

    dibujarCuadro4(10, 10, 100, 10); // Dibujar el cuadro
    dibujarCuadro4(10, 18, 100, 18); // Dibujar el cuadro

    gotoxy(100, 4); cout << "\xDB";
    gotoxy(10, 4); cout << "\xDB";
    gotoxy(10, 10); cout << "\xDB";
    gotoxy(10, 18); cout << "\xDB";
    gotoxy(100, 18); cout << "\xDB";
    gotoxy(100, 10); cout << "\xDB";
    gotoxy(100, 25); cout << "\xDB";
    gotoxy(10, 25); cout << "\xDB";

    gotoxy(40, 2); cout << "INVENTARIO DE PRODUCTO";

    gotoxy(40, 6); cout << "INFORMACION DEL PRODUCTO";
    gotoxy(20, 7); cout << "ID:";
    gotoxy(20, 8); cout << "Nombre del producto:";

    gotoxy(40, 12); cout << "DETALLES DEL PRODUCTO";
    gotoxy(20, 13); cout << "Tipo de prodecto:";
    gotoxy(20, 14); cout << "Cantidad";
    gotoxy(20, 15); cout << "Precio";
    gotoxy(20, 16); cout << "Fecha: ";

    gotoxy(40, 20); cout << "Características y Origen del Producto";
    gotoxy(20, 21); cout << "Proveedor: ";
    gotoxy(20, 22); cout << "Codigo: ";
    gotoxy(20, 23); cout << "Origen:    ";
}

