#include"encabezado.h"



// Estructura para almacenar la información de un cliente
struct Cliente {
    string nombre;
    int identificacion;
    string telefono;
    string direccion;
    string correo;
    string tipo_cliente;  // Ej: "Socio", "Minorista", "Mayorista"
};


// Estructura Nodo para la lista circular
struct Nodo {
    Cliente cliente;
    Nodo* siguiente;  // Puntero al siguiente nodo
};

// Puntero cabeza para la lista circular
Nodo* cabeza = nullptr;

void agregarCliente(const Cliente& nuevoCliente);
void mostrarClienteEncontrado(Nodo* cliente, int centro, int& posicion);

// Función para seleccionar el tipo de cliente
void seleccionarTipoCliente(string& tipoCliente) {
    const string opciones[] = {
        "Socio",
        "Minorista",
        "Mayorista",
        "Distribuidor",
        "Otros"
    };
    int seleccion = 0;  // Índice de la opción seleccionada  
    int totalOpciones = sizeof(opciones) / sizeof(opciones[0]);

    while (true) {

        // Mostrar las opciones  
        for (int i = 0; i < totalOpciones; ++i) {
            gotoxy(25, 18 + i); // Mover el cursor a la línea correspondiente  
            if (i == seleccion) {
                setColor((7) % 16); // Cambia el color del texto, limita a un rango de 0-9
                cout << "> " << opciones[i] << " <";  // Resaltar la opción seleccionada  
            }
            else {
                setColor((3) % 16); // Cambia el color del texto, limita a un rango de 0-9
                cout << "  " << opciones[i] << "  ";
            }
        }

        // Capturar la tecla presionada  
        int tecla = _getch();
        if (tecla == 72) { // Tecla de flecha arriba  
            seleccion = (seleccion - 1 + totalOpciones) % totalOpciones; // Mover hacia arriba  
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        }
        else if (tecla == 80) { // Tecla de flecha abajo  
            seleccion = (seleccion + 1) % totalOpciones; // Mover hacia abajo  
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        }
        else if (tecla == 13) { // Enter  
            tipoCliente = opciones[seleccion]; // Asignar la opción seleccionada  
            break; // Salir del bucle  
        }
    }
}

int color3 = 0;

// Función para registrar un nuevo cliente
void registrarCliente() {
    Cliente nuevoCliente;
    system("cls");  // Limpiar la pantalla
    setColor((color3 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(25, 7); cout << "*************************************************************************";
    gotoxy(25, 8); cout << "*                     INGRESO DE CLIENTE                               *";
    gotoxy(25, 9); cout << "*************************************************************************";
    gotoxy(25, 10); cout << "*                                                                       *";
    gotoxy(25, 11); cout << "*  Nombre: _______________________________________________              *";
    gotoxy(25, 12); cout << "*  ID: ___________________________________________________              *";
    gotoxy(25, 13); cout << "*  Telefono: _____________________________________________              *";
    gotoxy(25, 14); cout << "*  Direccion: ____________________________________________              *";
    gotoxy(25, 15); cout << "*  Correo Electronico: ___________________________________              *";
    gotoxy(25, 16); cout << "*  Tipo de Cliente: ______________________________________              *";
    gotoxy(25, 17); cout << "*************************************************************************";
    setColor((color3 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(39, 11);    getline(cin, nuevoCliente.nombre);
    gotoxy(39, 12);    cin >> nuevoCliente.identificacion;
    cin.ignore();
    gotoxy(39, 13);    getline(cin, nuevoCliente.telefono);
    gotoxy(41, 14);    getline(cin, nuevoCliente.direccion);
    gotoxy(53, 15);    getline(cin, nuevoCliente.correo);
    gotoxy(41, 16);    seleccionarTipoCliente(nuevoCliente.tipo_cliente);  // Llamar a la función para seleccionar el tipo de cliente



    // Confirmación de adición
    system("cls");
    setColor((color3 + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(25, 7); cout << "*************************************************************************";
    gotoxy(25, 8); cout << "*                   CLIENTE REGISTRADO EXITOSAMENTE!                    *";
    gotoxy(25, 9); cout << "*************************************************************************";
    gotoxy(25, 10); cout << "*                                                                       *";
    gotoxy(25, 11); cout << "*  Nombre: _______________________________________________              *";
    gotoxy(25, 12); cout << "*  ID: ___________________________________________________              *";
    gotoxy(25, 13); cout << "*  Telefono: _____________________________________________              *";
    gotoxy(25, 14); cout << "*  Direccion: ____________________________________________              *";
    gotoxy(25, 15); cout << "*  Correo Electronico: ___________________________________              *";
    gotoxy(25, 16); cout << "*  Tipo de Cliente: ______________________________________              *";
    gotoxy(25, 17); cout << "*************************************************************************";
    setColor((color3 + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    gotoxy(39, 11);  cout << nuevoCliente.nombre << "___" << &nuevoCliente.nombre;
    gotoxy(39, 12);  cout << nuevoCliente.identificacion << "___" << &nuevoCliente.identificacion;
    gotoxy(39, 13);  cout << nuevoCliente.telefono << "___" << &nuevoCliente.telefono;
    gotoxy(41, 14);  cout << nuevoCliente.direccion << "___" << &nuevoCliente.direccion;
    gotoxy(53, 15);  cout << nuevoCliente.correo << "___" << &nuevoCliente.correo;
    gotoxy(45, 16);  cout << nuevoCliente.tipo_cliente << "___" << &nuevoCliente.tipo_cliente;
    _getch();
    // Agregar el cliente a la lista circular
    agregarCliente(nuevoCliente);

}



// Función para agregar un cliente a la lista circular
void agregarCliente(const Cliente& nuevoCliente) {
    Nodo* nuevoNodo = new Nodo;
    nuevoNodo->cliente = nuevoCliente;

    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
        nuevoNodo->siguiente = cabeza;  // Apunta a sí mismo, ya que es la única posición
    }
    else {
        Nodo* temp = cabeza;
        while (temp->siguiente != cabeza) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
        nuevoNodo->siguiente = cabeza;  // El nuevo nodo apunta de nuevo a la cabeza
    }
}


// Función para mostrar todos los clientes en la lista circular
void mostrarClientes() {
    system("cls");
    if (cabeza == nullptr) {
        gotoxy(49, 13);cout << "No hay clientes en la lista." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
        _getch();
        return;
    }

    Nodo* temp = cabeza;
    Nodo* ultimo = cabeza;

    // Encontrar el último nodo (el que apunta a cabeza)
    while (ultimo->siguiente != cabeza) {
        ultimo = ultimo->siguiente;
    }

    int contador = 1;
    do {
        setColor((12) % 16); // Cambia el color del texto, limita a un rango de 0-9
        cout << "*************************************************************************" << endl;

        // Determinar si es el primero, último u otro
        if (temp == cabeza) {
            setColor((13) % 16); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*                         PRIMER CLIENTE (" << contador << ")                      *" << endl;
        }
        else if (temp == ultimo) {
            setColor((14) % 16); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*                         ULTIMO CLIENTE (" << contador << ")                       *" << endl;
        }
        else {

            cout << "*                           CLIENTE (" << contador << ")                            *" << endl;
        }
        setColor((12) % 16); // Cambia el color del texto, limita a un rango de 0-9
        cout << "*************************************************************************" << endl;
        cout << "*                                                                       *" << endl;
        cout << "*  Nombre: ____" << temp->cliente.nombre << "___" << &temp->cliente.nombre << endl;
        cout << "*  ID: ____" << temp->cliente.identificacion << "___" << &temp->cliente.identificacion << endl;
        cout << "*  Telefono: ____" << temp->cliente.telefono << "___" << &temp->cliente.telefono << endl;
        // Determinar si es el primero, último u otro
        if (temp == cabeza) {
            setColor((12) % 13); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*  Direccion: ______" << temp->cliente.direccion << "___" << &temp->cliente.direccion << "                       <==========Primer Cliente" << endl;
        }
        else if (temp == ultimo) {
            setColor((14) % 16); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*  Direccion: ______" << temp->cliente.direccion << "___" << &temp->cliente.direccion << "                      <==========Ultimo Cliente" << endl;
        }
        else {

            cout << "*  Direccion: ______" << temp->cliente.direccion << "___" << &temp->cliente.direccion << endl;
        }
        setColor((12) % 12); // Cambia el color del texto, limita a un rango de 0-9
        cout << "*  Correo Electronico: ___" << temp->cliente.correo << "___" << &temp->cliente.correo << endl;
        cout << "*  Tipo de Cliente: ____" << temp->cliente.tipo_cliente << "___" << &temp->cliente.tipo_cliente << endl;

        if (temp == cabeza) {
            setColor((13) % 16); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*  (Este es el primer nodo de la lista circular)                     *" << endl;
        }
        else if (temp == ultimo) {
            setColor((14) % 16); // Cambia el color del texto, limita a un rango de 0-9
            cout << "*  (Este nodo apunta de vuelta al primer nodo)                       *" << endl;
        }
        setColor((12) % 16); // Cambia el color del texto, limita a un rango de 0-9
        cout << "*************************************************************************" << endl << endl;

        temp = temp->siguiente;
        contador++;
    } while (temp != cabeza);  // Se detiene cuando llega de nuevo a la cabeza

    _getch();
}


void modificarCliente() {
    system("cls");
    if (cabeza == nullptr) {
        setColor((color3 + 12) % 16);

        gotoxy(40, 13); cout << "La lista esta vacia. No hay clientes para modificar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }


    int id;
    setColor((color3 + 12) % 16);
    gotoxy(40, 13); cout << "Ingrese el ID del cliente a modificar: ";
    setColor((color3 + 11) % 16);
    cin >> id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC);
    cin.ignore();

    Nodo* actual = cabeza;
    bool encontrado = false;

    do {
        if (actual->cliente.identificacion == id) {
            encontrado = true;
            int opcion = 1;
            char tecla;
            bool salir = false;

            do {
                system("cls");
                grafica(color3);
                int centro = 40;

                setColor((color3 + 11) % 16);
                gotoxy(centro, 5); cout << "**********************************************************";
                gotoxy(centro, 6); cout << "*             MODIFICAR INFORMACION DEL CLIENTE          *";
                gotoxy(centro, 7); cout << "**********************************************************";
                gotoxy(centro, 8); cout << (opcion == 1 ? "--> " : "    ") << "1. Nombre: " << actual->cliente.nombre << "_____" << &actual->cliente.nombre;
                gotoxy(centro, 9); cout << (opcion == 2 ? "--> " : "    ") << "2. Telefono: " << actual->cliente.telefono << "_____" << &actual->cliente.nombre;
                gotoxy(centro, 10); cout << (opcion == 3 ? "--> " : "    ") << "3. Direccion: " << actual->cliente.direccion << "_____" << &actual->cliente.nombre;
                gotoxy(centro, 11); cout << (opcion == 4 ? "--> " : "    ") << "4. Correo Electronico: " << actual->cliente.correo << "_____" << &actual->cliente.nombre;
                gotoxy(centro, 12); cout << (opcion == 5 ? "--> " : "    ") << "5. Tipo de Cliente: " << actual->cliente.tipo_cliente << "_____" << &actual->cliente.nombre;
                gotoxy(centro, 13); cout << (opcion == 6 ? "--> " : "    ") << "6. Salir";

                tecla = _getch();
                if (tecla == 72 && opcion > 1) {
                    opcion--;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                if (tecla == 80 && opcion < 6) {
                    opcion++;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC);
                }

                if (tecla == 13) { // Enter
                    system("cls");
                    gotoxy(centro, 5);
                    setColor(9 % 16);

                    switch (opcion) {
                    case 1:
                        cout << "Nuevo Nombre: ";
                        setColor(5 % 16);
                        getline(cin, actual->cliente.nombre);
                        break;
                    case 2:
                        cout << "Nuevo Telefono: ";
                        setColor(5 % 16);
                        getline(cin, actual->cliente.telefono);
                        break;
                    case 3:
                        cout << "Nueva Direccion: ";
                        setColor(5 % 16);
                        getline(cin, actual->cliente.direccion);
                        break;
                    case 4:
                        cout << "Nuevo Correo Electronico: ";
                        setColor(5 % 16);
                        getline(cin, actual->cliente.correo);
                        break;
                    case 5:
                        cout << "Nuevo Tipo de Cliente: ";
                        setColor(5 % 16);
                        seleccionarTipoCliente(actual->cliente.tipo_cliente);  // Llamar a la función para seleccionar el tipo de cliente
                        break;
                    case 6:
                        salir = true;
                        break;
                    }
                }
            } while (!salir);
            break;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    if (!encontrado) {
        system("cls");
        gotoxy(40, 13); cout << "Cliente con ID " << id << " no encontrado." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
    }
}

void buscarCliente() {
    if (cabeza == nullptr) {
        //  setColor((color2 + 12) % 16);
        gotoxy(40, 13); cout << "La lista esta vacia. No hay clientes para buscar." << endl;
        // PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    int opcionBusqueda = 1;
    char tecla;
    bool salir = false;

    do {
        system("cls");
        // grafica(color2);
        int centro = 40;

        //setColor((color2 + 11) % 16);
        gotoxy(centro, 5); cout << "**********************************************************";
        gotoxy(centro, 6); cout << "*                  BUSCAR CLIENTE                        *";
        gotoxy(centro, 7); cout << "**********************************************************";
        gotoxy(centro, 8); cout << (opcionBusqueda == 1 ? "--> " : "    ") << "1. Buscar por ID";
        gotoxy(centro, 9); cout << (opcionBusqueda == 2 ? "--> " : "    ") << "2. Buscar por Nombre";
        gotoxy(centro, 10); cout << (opcionBusqueda == 3 ? "--> " : "    ") << "3. Volver al menu principal";

        tecla = _getch();
        if (tecla == 72 && opcionBusqueda > 1) {
            opcionBusqueda--;
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC);
        }
        if (tecla == 80 && opcionBusqueda < 3) {
            opcionBusqueda++;
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC);
        }

        if (tecla == 13) { // Enter
            system("cls");
            grafica(color3);
            gotoxy(centro, 5);
            setColor(9 % 16);

            switch (opcionBusqueda) {
            case 1: { // Buscar por ID
                int id;
                gotoxy(centro, 6); cout << "Ingrese el ID del cliente a buscar: ";
                setColor(5 % 16);
                cin >> id;
                cin.ignore();
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC);

                Nodo* actual = cabeza;
                bool encontrado = false;

                do {
                    if (actual->cliente.identificacion == id) {
                        encontrado = true;
                        int posicion2 = 8;
                        mostrarClienteEncontrado(actual, centro, posicion2);
                        break;
                    }
                    actual = actual->siguiente;
                } while (actual != cabeza);

                if (!encontrado) {
                    gotoxy(centro, 8); cout << "Cliente con ID " << id << " no encontrado." << endl;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                _getch();
                break;
            }
            case 2: { // Buscar por Nombre
                string nombre;
                gotoxy(centro, 6); cout << "Ingrese el nombre del cliente a buscar: ";
                setColor(5 % 16);
                getline(cin, nombre);
                PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC);

                Nodo* actual = cabeza;
                bool encontrado = false;
                int posicion = 8;

                do {
                    if (actual->cliente.nombre.find(nombre) != string::npos) {
                        encontrado = true;
                        mostrarClienteEncontrado(actual, centro, posicion);
                        posicion += 10;
                    }
                    actual = actual->siguiente;
                } while (actual != cabeza);

                if (!encontrado) {
                    gotoxy(centro, 8); cout << "No se encontraron clientes con ese nombre." << endl;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
                }
                _getch();
                break;
            }
            case 3:
                salir = true;
                break;
            }
        }
    } while (!salir);
}

// Función auxiliar para mostrar la información del cliente encontrado
void mostrarClienteEncontrado(Nodo* cliente, int centro, int& posicion) {
    setColor((color3 + 11) % 16);
    gotoxy(centro, posicion++); cout << "**********************************************************";
    gotoxy(centro, posicion++); cout << "*                   CLIENTE ENCONTRADO                   *";
    gotoxy(centro, posicion++); cout << "**********************************************************";
    gotoxy(centro, posicion++); cout << "*  Nombre: " << cliente->cliente.nombre << "____" << &cliente->cliente.nombre;
    gotoxy(centro, posicion++); cout << "*  ID: " << cliente->cliente.identificacion << "____" << &cliente->cliente.identificacion;
    gotoxy(centro, posicion++); cout << "*  Telefono: " << cliente->cliente.telefono << "____" << &cliente->cliente.telefono;
    gotoxy(centro, posicion++); cout << "*  Direccion: " << cliente->cliente.direccion << "____" << &cliente->cliente.direccion;
    gotoxy(centro, posicion++); cout << "*  Correo Electronico: " << cliente->cliente.correo << "____" << &cliente->cliente.correo;
    gotoxy(centro, posicion++); cout << "*  Tipo de Cliente: " << cliente->cliente.tipo_cliente << "____" << &cliente->cliente.tipo_cliente;
    gotoxy(centro, posicion++); cout << "**********************************************************";
    posicion++; // Espacio adicional entre clientes
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion3.wav", NULL, SND_FILENAME | SND_ASYNC);
}




void eliminarCliente() {
    if (cabeza == nullptr) {
        system("cls");
        setColor((color3 + 12) % 16);
        gotoxy(40, 13); cout << "La lista esta vacia. No hay clientes para eliminar." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }
    system("cls");
    int id;
    setColor((color3 + 12) % 16);
    gotoxy(40, 13); cout << "Ingrese el ID del cliente a eliminar: ";
    setColor((color3 + 11) % 16);
    cin >> id;
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion4.wav", NULL, SND_FILENAME | SND_ASYNC);
    cin.ignore();

    Nodo* actual = cabeza;
    Nodo* anterior = nullptr;
    bool encontrado = false;
    bool esUnicoNodo = (cabeza->siguiente == cabeza);
    bool esPrimerNodo = false;
    bool esUltimoNodo = false;

    // Buscar el nodo a eliminar
    do {
        if (actual->cliente.identificacion == id) {
            encontrado = true;

            // Determinar si es el último nodo
            if (actual->siguiente == cabeza && !esUnicoNodo) {
                esUltimoNodo = true;
            }

            // Determinar si es el primer nodo (solo si no es el único)
            if (actual == cabeza && !esUnicoNodo) {
                esPrimerNodo = true;
            }

            break;
        }
        anterior = actual;
        actual = actual->siguiente;
    } while (actual != cabeza);

    if (!encontrado) {
        system("cls");
        gotoxy(40, 15); cout << "Cliente con ID " << id << " no encontrado." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    // Mostrar confirmación con información del caso especial
    system("cls");
    grafica(color3);
    int centro = 40;
    setColor((color3 + 11) % 16);

    gotoxy(centro, 5); cout << "**********************************************************";
    gotoxy(centro, 6); cout << "*           CONFIRMACION DE ELIMINACION                 *";
    gotoxy(centro, 7); cout << "**********************************************************";

    // Mostrar información del caso especial
    if (esUnicoNodo) {
        setColor((color3 + 12) % 16);
        gotoxy(centro, 8); cout << "* ATENCION: Este es el UNICO cliente en la lista         *";
        gotoxy(centro, 9); cout << "* La lista quedara VACIA después de esta operacion      *";
    }
    else if (esPrimerNodo) {
        setColor((color3 + 14) % 16);
        gotoxy(centro, 8); cout << "* ATENCION: Este es el PRIMER cliente de la lista       *";
    }
    else if (esUltimoNodo) {
        setColor((color3 + 14) % 16);
        gotoxy(centro, 8); cout << "* ATENCION: Este es el ULTIMO cliente de la lista       *";
    }
    else {
        gotoxy(centro, 8); cout << "*                                                     *";
    }

    setColor((color3 + 11) % 16);
    gotoxy(centro, 10); cout << "*  Esta seguro que desea eliminar al siguiente cliente?  *";
    gotoxy(centro, 11); cout << "*  Nombre: " << actual->cliente.nombre;
    gotoxy(centro, 12); cout << "*  ID: " << actual->cliente.identificacion;
    gotoxy(centro, 13); cout << "*  Telefono: " << actual->cliente.telefono;
    gotoxy(centro, 14); cout << "**********************************************************";
    gotoxy(centro, 15); cout << "Presione 'S' para confirmar o cualquier otra tecla para cancelar";
    gotoxy(centro, 16); cout << "**********************************************************";

    char confirmacion = _getch();
    if (tolower(confirmacion) != 's') {
        gotoxy(centro, 18); cout << "Eliminacion cancelada." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    // Eliminar el nodo
    if (esUnicoNodo) {
        delete cabeza;
        cabeza = nullptr;
    }
    else if (actual == cabeza) { // Eliminar primer nodo
        Nodo* ultimo = cabeza;
        while (ultimo->siguiente != cabeza) {
            ultimo = ultimo->siguiente;
        }
        cabeza = cabeza->siguiente;
        ultimo->siguiente = cabeza;
        delete actual;
    }
    else { // Eliminar nodo intermedio o final
        anterior->siguiente = actual->siguiente;
        delete actual;
    }

    gotoxy(centro, 18); cout << "Cliente eliminado correctamente." << endl;
    if (esUnicoNodo) {
        gotoxy(centro, 19); cout << "La lista de clientes ahora esta vacia." << endl;
    }
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion3.wav", NULL, SND_FILENAME | SND_ASYNC);
    _getch();
}


void vaciarListaClientes() {
    if (cabeza == nullptr) {
        setColor((color3 + 12) % 16);
        system("cls");
        gotoxy(40, 13); cout << "La lista de clientes ya esta vacia." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    // Mostrar confirmación con advertencia
    system("cls");
    grafica(color3);
    int centro = 40;

    setColor((color3 + 12) % 16);
    gotoxy(centro, 5); cout << "**********************************************************";
    gotoxy(centro, 6); cout << "*          ADVERTENCIA: VACIAR LISTA DE CLIENTES         *";
    gotoxy(centro, 7); cout << "**********************************************************";
    gotoxy(centro, 8); cout << "*  Esta operacion eliminara TODOS los clientes registrados *";
    gotoxy(centro, 9); cout << "*  y no se podra recuperar la informacion.                *";

    // Mostrar cantidad de clientes que serán eliminados
    int contador = 0;
    Nodo* temp = cabeza;
    do {
        contador++;
        temp = temp->siguiente;
    } while (temp != cabeza);

    setColor((color3 + 14) % 16);
    gotoxy(centro, 10); cout << "*  Numero de clientes que seran eliminados: " << contador << "       *";

    setColor((color3 + 12) % 16);
    gotoxy(centro, 11); cout << "**********************************************************";
    gotoxy(centro, 12); cout << "*  ¿Esta completamente seguro que desea continuar?        *";
    gotoxy(centro, 13); cout << "*  Presione 'S' para confirmar o cualquier otra tecla     *";
    gotoxy(centro, 14); cout << "*  para cancelar la operacion.                           *";
    gotoxy(centro, 15); cout << "**********************************************************";

    char confirmacion = _getch();
    if (tolower(confirmacion) != 's') {
        system("cls");
        gotoxy(40, 13);gotoxy(centro, 17); cout << "Operacion cancelada. La lista no ha sido modificada." << endl;
        PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo8.wav", NULL, SND_FILENAME | SND_ASYNC);
        _getch();
        return;
    }

    // Vaciar la lista
    Nodo* actual = cabeza;
    Nodo* siguiente = nullptr;

    do {
        siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    } while (actual != cabeza);

    cabeza = nullptr;

    // Mostrar resultado
    system("cls");
    grafica(color3);
    setColor((color3 + 10) % 16);
    gotoxy(centro, 8); cout << "**********************************************************";
    gotoxy(centro, 9); cout << "*       LISTA DE CLIENTES VACIADA CORRECTAMENTE         *";
    gotoxy(centro, 10); cout << "**********************************************************";
    gotoxy(centro, 11); cout << "*  Se han eliminado " << contador << " clientes de la lista.              *";
    gotoxy(centro, 12); cout << "*  La lista de clientes ahora esta completamente vacia.  *";
    gotoxy(centro, 13); cout << "**********************************************************";
    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion3.wav", NULL, SND_FILENAME | SND_ASYNC);
    _getch();
}


void cliente() {

    int opcion;

    while (true) {
        system("cls"); // Limpia la pantalla

        int x = 45, y = 10;
        int opcion = 1;
        int tecla;

        while (true) {
            system("cls");
            int numero = generarNumeroAleatorio();
            grafica(numero);
            setColor((numero + 2) % 16); // Cambia el color del texto, limita a un rango de 0-9
            gotoxy(45, 7);cout << "  .:SubMenu Compra de cafe (Lista simpple) orden Natural:.";
            gotoxy(45, 8);cout << " Seleccione una opcion";

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
                case 1: cout << "Registrar Cliente "; break;
                case 2: cout << "Ver datos delos clientes"; break;
                case 3: cout << "Modificar informacion de cliente"; break;
                case 4: cout << "Buscar una cliente"; break;
                case 5: cout << "Eliminar una cliente"; break;
                case 6: cout << "vaciar lista";break;
                case 7: cout << "Regrasar el menu Principal"; break;
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
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            Sleep(1000); // Espera un poco para que el sonido se reproduzca antes de cerrar
            break;
        } // Opción "Salir" cierra el programa

        switch (opcion) {
        case 1:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo4.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            registrarCliente();
            break;
        case 2:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            mostrarClientes();
            _getch();
            break;
        case 3:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            modificarCliente();
            break;
        case 4:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            buscarCliente();
            break;
        case 5:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            eliminarCliente();
            break;
        case 6:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
            vaciarListaClientes();
            break;
        }
    }
}
