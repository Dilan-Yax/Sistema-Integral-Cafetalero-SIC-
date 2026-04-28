#include "encabezado.h"


int color2 = 0;
// Estructura para almacenar información sobre un trabajador  
struct Trabajador {
    string nombre;
    int id;
    string puesto;
    string fechaContratacion;
    string telefono;
    string direccion;
    string correoElectronico;
    double salario;
};

// Nodo de la lista doblemente enlazada
struct Nodo {
    Trabajador trabajador;
    Nodo* anterior;
    Nodo* siguiente;
    Nodo(const Trabajador& t) : trabajador(t), anterior(nullptr), siguiente(nullptr) {}
};

struct ListaDobleTrabajadores {
    Nodo* cabeza;
    Nodo* cola;

    ListaDobleTrabajadores() : cabeza(nullptr), cola(nullptr) {}

    void agregarTrabajador(const Trabajador& nuevoTrabajador) {
        Nodo* nuevoNodo = new Nodo(nuevoTrabajador);
        if (!cabeza) {
            cabeza = cola = nuevoNodo;
        }
        else {
            Nodo* actual = cabeza;
            while (actual && actual->trabajador.id > nuevoTrabajador.id) {
                actual = actual->siguiente;
            }
            if (!actual) { // Insertar al final
                cola->siguiente = nuevoNodo;
                nuevoNodo->anterior = cola;
                cola = nuevoNodo;
            }
            else if (!actual->anterior) { // Insertar al inicio
                nuevoNodo->siguiente = cabeza;
                cabeza->anterior = nuevoNodo;
                cabeza = nuevoNodo;
            }
            else { // Insertar en medio
                nuevoNodo->siguiente = actual;
                nuevoNodo->anterior = actual->anterior;
                actual->anterior->siguiente = nuevoNodo;
                actual->anterior = nuevoNodo;
            }
        }
        setColor((color2 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
        gotoxy(40, 13);cout << "Trabajador agregado exitosamente!" << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo5.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    }
};

void mostrarTrabajadores(ListaDobleTrabajadores& lista) {
    system("cls");
    if (lista.cabeza == nullptr) {
        gotoxy(40, 13); cout << "La lista esta vacia. No hay trabajadores para mostrar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    Nodo* actual = lista.cabeza;
    int contador = 1;

    while (actual != nullptr) {
        setColor((color2 + contador) % 16); // Cambia el color para cada trabajador

        cout << "*************************************************************************" << endl;

        // Identificar si es el primero, último u otro
        if (actual == lista.cabeza && actual == lista.cola) {
            setColor(13); // Color especial para único elemento
            cout << "*                  UNICO TRABAJADOR EN LA LISTA (" << contador << ")               *" << endl;
        }
        else if (actual == lista.cabeza) {
            setColor(14); // Color para primer trabajador
            cout << "*                    PRIMER TRABAJADOR (" << contador << ")                    *" << endl;
        }
        else if (actual == lista.cola) {
            setColor(12); // Color para último trabajador
            cout << "*                    ULTIMO TRABAJADOR (" << contador << ")                     *" << endl;
        }
        else {
            cout << "*                      TRABAJADOR (" << contador << ")                         *" << endl;
        }

        setColor((color2 + contador) % 16);
        cout << "*************************************************************************" << endl;
        cout << "*  Nodo actual en: " << actual << endl;
        cout << "*  Anterior: " << (actual->anterior ? to_string(actual->anterior->trabajador.id) + " (dir: " + to_string((long long)actual->anterior) + ")" : "NULL") << endl;
        cout << "*  Siguiente: " << (actual->siguiente ? to_string(actual->siguiente->trabajador.id) + " (dir: " + to_string((long long)actual->siguiente) + ")" : "NULL") << endl;
        cout << "*                                                                       *" << endl;
        cout << "*  Nombre: " << actual->trabajador.nombre << " (dir: " << &(actual->trabajador.nombre) << ")" << endl;
        cout << "*  ID: " << actual->trabajador.id << " (dir: " << &(actual->trabajador.id) << ")" << endl;
        cout << "*  Puesto: " << actual->trabajador.puesto << " (dir: " << &(actual->trabajador.puesto) << ")" << endl;
        cout << "*  Fecha Contratacion: " << actual->trabajador.fechaContratacion << " (dir: " << &(actual->trabajador.fechaContratacion) << ")" << endl;
        cout << "*  Teléfono: " << actual->trabajador.telefono << " (dir: " << &(actual->trabajador.telefono) << ")" << endl;

        // Resaltar si es primero o último
        if (actual == lista.cabeza) {
            setColor(14);
            cout << "*  Direccion: " << actual->trabajador.direccion << " (dir: " << &(actual->trabajador.direccion) << ") <===================== PRIMERO" << endl;
        }
        else if (actual == lista.cola) {
            setColor(12);
            cout << "*  Direccion: " << actual->trabajador.direccion << " (dir: " << &(actual->trabajador.direccion) << ") <===================== ULTIMO" << endl;
        }
        else {
            setColor((color2 + contador) % 16);
            cout << "*  Dirección: " << actual->trabajador.direccion << " (dir: " << &(actual->trabajador.direccion) << ")" << endl;
        }

        setColor((color2 + contador) % 16);
        cout << "*  Correo: " << actual->trabajador.correoElectronico << " (dir: " << &(actual->trabajador.correoElectronico) << ")" << endl;
        cout << "*  Salario: " << actual->trabajador.salario << " (dir: " << &(actual->trabajador.salario) << ")" << endl;

        // Información adicional sobre la posición
        if (actual == lista.cabeza && actual == lista.cola) {
            setColor(13);
            cout << "*  (Este es el unico nodo en la lista)                              *" << endl;
        }
        else if (actual == lista.cabeza) {
            setColor(14);
            cout << "*  (Este es el primer nodo de la lista)                              *" << endl;
        }
        else if (actual == lista.cola) {
            setColor(12);
            cout << "*  (Este es el ultimo nodo de la lista)                              *" << endl;
        }

        setColor((color2 + contador) % 16);
        cout << "*************************************************************************" << endl << endl;

        actual = actual->siguiente;
        contador++;
    }

    // Mostrar información general de la lista
    setColor(15);
    cout << "==========================================================" << endl;
    cout << "Resumen de la lista:" << endl;
    cout << "Primer trabajador (cabeza): ID " << lista.cabeza->trabajador.id
        << " en " << lista.cabeza << endl;
    cout << "Ultimo trabajador (cola): ID " << lista.cola->trabajador.id
        << " en " << lista.cola << endl;
    cout << "Total de trabajadores: " << (contador - 1) << endl;
    cout << "==========================================================" << endl;

    _getch();
}

void modificarTrabajador(ListaDobleTrabajadores& lista) {
    if (lista.cabeza == nullptr) {
        setColor((color2 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9

        gotoxy(40, 13);cout << "La lista esta vacia. No hay nada que Modificar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

        _getch();
        return;
    }
    int id;
    setColor((color2 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(40, 13); cout << "Ingrese el ID del trabajador a modificar: ";
    setColor((color2 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cin >> id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    cin.ignore();

    Nodo* actual = lista.cabeza;
    while (actual) {
        if (actual->trabajador.id == id) {
            int opcion = 1;
            char tecla;
            bool salir = false;
            do {
                system("cls");

                grafica(color2);
                int centro = 40;
                setColor((color2 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
                gotoxy(centro, 5); cout << "**********************************************************";
                gotoxy(centro, 6); cout << "*             MODIFICAR INFORMACION DEL TRABAJADOR      *";
                gotoxy(centro, 7); cout << "**********************************************************";
                gotoxy(centro, 8); cout << (opcion == 1 ? "--> " : "    ") << "1. Nombre: " << actual->trabajador.nombre << "   ||         (dir: " << &(actual->trabajador.nombre) << ")" << endl;
                gotoxy(centro, 9); cout << (opcion == 2 ? "--> " : "    ") << "2. Puesto: " << actual->trabajador.puesto << "   ||         (dir: " << &(actual->trabajador.puesto) << ")" << endl;
                gotoxy(centro, 10); cout << (opcion == 3 ? "--> " : "    ") << "3. Fecha de Contratacion: " << actual->trabajador.fechaContratacion << "   ||         (dir: " << &(actual->trabajador.fechaContratacion) << ")" << endl;
                gotoxy(centro, 11); cout << (opcion == 4 ? "--> " : "    ") << "4. Telefono: " << actual->trabajador.telefono << "   ||         (dir: " << &(actual->trabajador.telefono) << ")" << endl;
                gotoxy(centro, 12); cout << (opcion == 5 ? "--> " : "    ") << "5. Direccion: " << actual->trabajador.direccion << "   ||         (dir: " << &(actual->trabajador.direccion) << ")" << endl;
                gotoxy(centro, 13); cout << (opcion == 6 ? "--> " : "    ") << "6. Correo Electronico: " << actual->trabajador.correoElectronico << "   ||         (dir: " << &(actual->trabajador.correoElectronico) << ")" << endl;
                gotoxy(centro, 14); cout << (opcion == 7 ? "--> " : "    ") << "7. Salario: " << actual->trabajador.salario << "   ||         (dir: " << &(actual->trabajador.salario) << ")" << endl;
                gotoxy(centro, 15); cout << (opcion == 8 ? "--> " : "    ") << "8. Salir";

                tecla = _getch();
                if (tecla == 72 && opcion > 1) opcion--;  PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse// Flecha arriba
                if (tecla == 80 && opcion < 8) opcion++;  PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse// Flecha abajo
                if (tecla == 13) { // Enter para seleccionar
                    system("cls");
                    gotoxy(centro, 5);
                    setColor((9) % 16); // Cambia el color del texto, limita a un rango de 0-9
                    switch (opcion) {
                    case 1:
                        cout << "Nuevo Nombre: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.nombre);
                        break;
                    case 2:
                        cout << "Nuevo Puesto: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.puesto);
                        break;
                    case 3:
                        cout << "Nueva Fecha de Contratacion: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.fechaContratacion);
                        break;
                    case 4:
                        cout << "Nuevo Telefono: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.telefono);
                        break;
                    case 5:
                        cout << "Nueva Direccion: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.direccion);
                        break;
                    case 6:
                        cout << "Nuevo Correo Electronico: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        getline(cin, actual->trabajador.correoElectronico);
                        break;
                    case 7:
                        cout << "Nuevo Salario: ";
                        setColor((5) % 16); // Cambia el color del texto, limita a un rango de 0-9
                        cin >> actual->trabajador.salario;
                        cin.ignore();
                        break;
                    case 8:
                        salir = true;
                        break;
                    }
                }
            } while (!salir);
            return;
        }
        actual = actual->siguiente;
    }
    system("cls");
    gotoxy(40, 13);cout << "Trabajador con ID " << id << " no encontrado." << endl;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    _getch();
}

// Función para buscar un trabajador por ID y mostrar sus datos
void buscarTrabajador(ListaDobleTrabajadores& lista) {

    if (lista.cabeza == nullptr) {

        gotoxy(40, 13);cout << "La lista esta vacia. No hay nada que buscar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        _getch();
        return;
    }

    int id;
    gotoxy(40, 13); cout << "Ingrese el ID del trabajador a modificar: ";
    cin >> id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    cin.ignore();
    Nodo* actual = lista.cabeza;  // Comienza desde el primer nodo de la lista

    while (actual != nullptr) {
        if (actual->trabajador.id == id) {
            // Si el trabajador con el ID coincide, mostramos sus datos
            system("cls");

            grafica(color2);
            setColor((color2 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
            gotoxy(25, 7); cout << "*************************************************************************";
            gotoxy(25, 8); cout << "*                     INGRESO DE TRABAJADOR                             *";
            gotoxy(25, 9); cout << "*************************************************************************";
            gotoxy(25, 10); cout << "*                                                                       *";
            gotoxy(25, 11); cout << "*  Nombre: _______________________________________________              *";
            gotoxy(25, 12); cout << "*  ID: ___________________________________________________              *";
            gotoxy(25, 13); cout << "*  Puesto: _______________________________________________              *";
            gotoxy(25, 14); cout << "*  Fecha de Contratacion: ________________________________              *";
            gotoxy(25, 15); cout << "*  Telefono: _____________________________________________              *";
            gotoxy(25, 16); cout << "*  Direccion: ____________________________________________              *";
            gotoxy(25, 17); cout << "*  Correo Electronico: ___________________________________              *";
            gotoxy(25, 18); cout << "*  Salario: ______________________________________________              *";
            gotoxy(25, 19); cout << "*************************************************************************";

            setColor((color2 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
            gotoxy(39, 11);
            cout << actual->trabajador.nombre << "   ||         (dir: " << &(actual->trabajador.nombre) << ")" << endl;

            gotoxy(39, 12);
            cout << actual->trabajador.id << "        ||         (dir: " << &(actual->trabajador.id) << ")" << endl;

            gotoxy(39, 13);
            cout << actual->trabajador.puesto << "     ||       (dir:" << &(actual->trabajador.puesto) << ")" << endl;

            gotoxy(55, 14);
            cout << actual->trabajador.fechaContratacion << " || (dir:" << &(actual->trabajador.fechaContratacion) << ")" << endl;

            gotoxy(39, 15);
            cout << actual->trabajador.telefono << "    ||       (dir:" << &(actual->trabajador.telefono) << ")" << endl;

            gotoxy(41, 16);
            cout << actual->trabajador.direccion << "     ||     (dir:" << &(actual->trabajador.direccion) << ")" << endl;

            gotoxy(53, 17);
            cout << actual->trabajador.correoElectronico << " || (dir:" << &(actual->trabajador.correoElectronico) << ")" << endl;

            gotoxy(39, 18);
            cout << actual->trabajador.salario << "     ||        (dir:" << &(actual->trabajador.salario) << ")" << endl;

            _getch();
            return;
        }
        actual = actual->siguiente;  // Avanza al siguiente nodo
    }

    // Si no se encuentra el trabajador con el ID
    system("cls");
    gotoxy(40, 13);cout << "Trabajador con ID " << id << " no encontrado." << endl;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    _getch();
}

// Función para eliminar un trabajador por ID
void eliminarTrabajador(ListaDobleTrabajadores& lista) {

    Nodo* actual = lista.cabeza;  // Comienza desde el primer nodo de la lista
    if (lista.cabeza == nullptr) {

        gotoxy(40, 13);cout << "La lista esta vacia. No hay trabajadores para eliminar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        _getch();
        return;
    }
    int id;
    gotoxy(40, 13); cout << "Ingrese el ID del trabajador a eliminar: ";
    cin >> id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    cin.ignore();



    while (actual != nullptr) {
        if (actual->trabajador.id == id) {
            // Si encontramos al trabajador con el ID
            system("cls");
            grafica(color2);
            setColor((color2 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
            gotoxy(25, 4); cout << "*************************************************************************";
            gotoxy(25, 5); cout << "*               ELIMINACION DE TRABAJADOR                             *";
            gotoxy(25, 6); cout << "*************************************************************************";
            gotoxy(25, 7); cout << "* Trabajador a eliminar:";
            gotoxy(25, 8); cout << "* ID: " << actual->trabajador.id << " (dir: " << &(actual->trabajador.id) << ")" << endl;
            gotoxy(25, 9); cout << "* Nombre: " << actual->trabajador.nombre << " (dir: " << &(actual->trabajador.nombre) << ")" << endl;
            gotoxy(25, 10); cout << "* Puesto: " << actual->trabajador.puesto << " (dir: " << &(actual->trabajador.puesto) << ")" << endl;
            gotoxy(25, 11); cout << "* Fecha de Contratacion: " << actual->trabajador.fechaContratacion << " (dir: " << &(actual->trabajador.fechaContratacion) << ")" << endl;
            gotoxy(25, 12); cout << "* Telefono: " << actual->trabajador.telefono << " (dir: " << &(actual->trabajador.telefono) << ")" << endl;
            gotoxy(25, 13); cout << "* Direccion: " << actual->trabajador.direccion << " (dir: " << &(actual->trabajador.direccion) << ")" << endl;
            gotoxy(25, 14); cout << "* Correo: " << actual->trabajador.correoElectronico << " (dir: " << &(actual->trabajador.correoElectronico) << ")" << endl;
            gotoxy(25, 15); cout << "* Salario: " << actual->trabajador.salario << " (dir: " << &(actual->trabajador.salario) << ")" << endl;
            gotoxy(25, 16); cout << "*************************************************************************";
            _getch();



            if (actual == lista.cabeza) {
                // Si es el primer nodo (cabeza)
                lista.cabeza = actual->siguiente;
                if (lista.cabeza != nullptr) {
                    lista.cabeza->anterior = nullptr;
                }
            }
            else {
                // Si no es el primer nodo, ajustamos los punteros
                actual->anterior->siguiente = actual->siguiente;
                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }
            }

            // Liberar la memoria del nodo eliminado
            delete actual;

            grafica(color2);
            gotoxy(40, 13);cout << "Trabajador con ID " << id << " eliminado exitosamente." << endl;
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            _getch();
            return;
        }

        actual = actual->siguiente;  // Avanza al siguiente nodo
    }

    // Si no se encuentra el trabajador con el ID
    system("cls");
    gotoxy(40, 13);cout << "Trabajador con ID " << id << " no encontrado." << endl;
    _getch();
}

// Función para vaciar la lista de trabajadores, verificando si está vacía
void vaciarLista(ListaDobleTrabajadores& lista) {

    if (lista.cabeza == nullptr) {


        gotoxy(40, 13);cout << "La lista ya esta vacia. No hay nada que vaciar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        _getch();
        return;
    }

    Nodo* actual = lista.cabeza;

    while (actual != nullptr) {
        Nodo* siguienteNodo = actual->siguiente;  // Guardamos el siguiente nodo
        delete actual;  // Liberamos la memoria del nodo actual
        actual = siguienteNodo;  // Avanzamos al siguiente nodo
    }

    // Después de vaciar la lista, la cabeza debe ser nula
    lista.cabeza = nullptr;
    grafica(color2);
    gotoxy(40, 13);cout << "La lista de trabajadores ha sido vaciada." << endl;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
    _getch();
}


// Función para agregar un trabajador  
void agregarTrabajador(ListaDobleTrabajadores& lista) {
    Trabajador nuevoTrabajador;
    limpiarBuffer();
    system("cls");

    grafica(color2);
    setColor((color2 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(25, 7); cout << "*************************************************************************";
    gotoxy(25, 8); cout << "*                     INGRESO DE TRABAJADOR                             *";
    gotoxy(25, 9); cout << "*************************************************************************";
    gotoxy(25, 10); cout << "*                                                                       *";
    gotoxy(25, 11); cout << "*  Nombre: _______________________________________________              *";
    gotoxy(25, 12); cout << "*  ID: ___________________________________________________              *";
    gotoxy(25, 13); cout << "*  Puesto: _______________________________________________              *";
    gotoxy(25, 14); cout << "*  Fecha de Contratacion: ________________________________              *";
    gotoxy(25, 15); cout << "*  Telefono: _____________________________________________              *";
    gotoxy(25, 16); cout << "*  Direccion: ____________________________________________              *";
    gotoxy(25, 17); cout << "*  Correo Electronico: ___________________________________              *";
    gotoxy(25, 18); cout << "*  Salario: ______________________________________________              *";
    gotoxy(25, 19); cout << "*************************************************************************";

    setColor((color2 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(39, 11); getline(cin, nuevoTrabajador.nombre);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(39, 12); cin >> nuevoTrabajador.id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(39, 13); cin.ignore(); // Limpiar el buffer de entrada  
    getline(cin, nuevoTrabajador.puesto);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(55, 14); getline(cin, nuevoTrabajador.fechaContratacion);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(39, 15); getline(cin, nuevoTrabajador.telefono);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(41, 16); getline(cin, nuevoTrabajador.direccion);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(53, 17); getline(cin, nuevoTrabajador.correoElectronico);
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    gotoxy(39, 18);
    cin >> nuevoTrabajador.salario;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

    lista.agregarTrabajador(nuevoTrabajador);
    // Confirmación de adición  

}


void trabajadores() {
    ListaDobleTrabajadores lista;
    while (true) {
        system("cls"); // Limpia la pantalla


        int x = 45, y = 10;
        int opcion = 1;
        int tecla;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo1.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse

        while (true) {
            system("cls");
            int numero = generarNumeroAleatorio();
            grafica(numero);
            setColor((numero + 2) % 16); // Cambia el color del texto, limita a un rango de 0-9
            gotoxy(45, 7);cout << "  .:Sub Menu Trabajadores (LISTA DOBLE ENLAZADA) orden descendente:.";
            gotoxy(45, 8);cout << "Seleccione una opcion:";

            // Mostrar opciones
            for (int i = 1; i <= 7; i++) {
                gotoxy(x, y + i - 1);
                if (i == opcion) {
                    setColor(14);
                    cout << "-> ";
                }
                else {
                    setColor((numero + 3) % 16); // Cambia el color del texto, limita a un rango de 0-9
                    cout << "   ";
                }

                switch (i) {
                case 1: cout << "Agregar Trabajador"; break;
                case 2: cout << "Mostrar Trabajadores"; break;
                case 3: cout << "Modificar Informacion de Trabajador"; break;
                case 4: cout << "Buscar Informacion de Trabajador"; break;
                case 5: cout << "Eliminar Informacion de Trabajador"; break;
                case 6: cout << "Vaciar lista"; break;
                case 7: cout << "Regresar al menu principal"; break;
                }
            }

            tecla = _getch();       //capturamos la tecla precionada
            if (tecla == 224) {     // detectar si es una tecla flechas  
                tecla = _getch();   // capturar el nuevo valor 
                if (tecla == 72 && opcion > 1) { // Flecha arriba
                    opcion--;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                }
                else if (tecla == 80 && opcion < 7) { // Flecha abajo
                    opcion++;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                }
            }
            else if (tecla == 13) { // Enter para seleccionar
                break;
            }
        }

        setColor(15);
        if (opcion == 7) {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al salir
            Sleep(1000); // Espera un poco para que el sonido se reproduzca antes de cerrar
            break;
        } // Opción "Salir" cierra el programa

        switch (opcion) {
        case 1:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            agregarTrabajador(lista);
            break;
        case 2:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            mostrarTrabajadores(lista);
            break;
        case 3:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            modificarTrabajador(lista);
            break;
        case 4:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            buscarTrabajador(lista);
            break;
        case 5:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            eliminarTrabajador(lista);
            break;
        case 6:
            system("cls");
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            vaciarLista(lista);
            break;
        }
    }
}
