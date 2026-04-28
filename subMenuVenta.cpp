#include"encabezado.h"


#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define ENTER 13

struct Ventas
{
	int ID_Ventas;
	int ID_Cliente;

	string Tipo_de_Cafe_ventido;
	string Fecha_de_venta;
	int Precio_Unitario;
	int Cantidad_vendida;

	string Descuento;
	string Total;
	string Metodo_de_pago;
} VENTAS;

struct Nodop
{
	Ventas datos;
	Nodop* siguiente;
};

Nodop* pila = NULL;

void Menu_Pila();
void DibujarRecuadro();
void dibujarCuadro3(int x1, int y1, int x2, int y2);
void Insertar_Nodop(Nodop*&, int, int, int, int, string, string, string, string, string);
void Mostrar_Pila(Nodop*&, int, int, int, int, string, string, string, string, string);
void Modificar_Nodop(Nodop*&, int, int, int, int, string, string, string, string, string);
int Opciones_Menu(const char* titulo, const char* opciones[], int n);
void Eliminar_Nodop(Nodop*&, int, int, int, int, string, string, string, string, string);
void Buscar_Nodop(Nodop*&, int, int, int, int, string, string, string, string, string);
void dibujarFigura(int opcion);
void Vaciar_Pila(Nodop*& pila);

void clearCin()
{
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

void Insertar_Nodop(Nodop*& pila, int a, int b, int c, int d, string h, string i, string j, string l, string m) {
	char continuar;
	i = obtenerFechaActual();  // Obtener la fecha de venta actual

	do {
		Nodop* nuevo_nodo = new Nodop();
		system("cls");
		cin.clear();
		cin.ignore(100, '\n');  // Limpiar el buffer de entrada
		DibujarRecuadro();

		gotoxy(40, 7); cout << a;
		gotoxy(40, 8); cin >> b;
		cin.ignore(100, '\n');  // Limpiar buffer  

		gotoxy(48, 13); getline(cin, h);
		gotoxy(48, 14); cout << i;
		gotoxy(48, 15); cin >> c;
		gotoxy(48, 16); cin >> d;
		cin.ignore(100, '\n');  // Limpiar buffer  

		gotoxy(48, 21); getline(cin, j);

		// Calcular total
		float precio_total = c * d;
		float descuento = 0.0;
		try {
			descuento = stof(j) / 100.0 * precio_total;
		}
		catch (const invalid_argument& e) {
			cout << "Error: Descuento no válido. Se tomará como 0%.\n";
		}
		float total_final = precio_total - descuento;

		gotoxy(48, 22); cout << total_final;
		gotoxy(48, 23); getline(cin, m);

		// Asignar datos
		nuevo_nodo->datos.ID_Ventas = a;
		nuevo_nodo->datos.ID_Cliente = b;
		nuevo_nodo->datos.Tipo_de_Cafe_ventido = h;
		nuevo_nodo->datos.Fecha_de_venta = i;
		nuevo_nodo->datos.Precio_Unitario = c;
		nuevo_nodo->datos.Cantidad_vendida = d;
		nuevo_nodo->datos.Descuento = j;
		nuevo_nodo->datos.Total = to_string(total_final);
		nuevo_nodo->datos.Metodo_de_pago = m;

		// Insertar nodo en la pila
		nuevo_nodo->siguiente = pila;
		pila = nuevo_nodo;

		gotoxy(2, 27);
		cout << " *- NODO INGRESADO  -*" << endl << endl;

		// Mostrar pila
		Mostrar_Pila(pila, pila->datos.Cantidad_vendida, pila->datos.Precio_Unitario, pila->datos.ID_Ventas,
			pila->datos.ID_Cliente, pila->datos.Tipo_de_Cafe_ventido, pila->datos.Fecha_de_venta,
			pila->datos.Descuento, pila->datos.Total, pila->datos.Metodo_de_pago);

		// Preguntar si desea ingresar otro nodo
		gotoxy(5, 29);
		cout << endl << " DESEA INGRESAR OTRO NODO DE DATOS? (s/n): ";
		cin >> continuar;
		cin.ignore(100, '\n');  // Limpiar buffer antes de repetir el ciclo

	} while (continuar == 's' || continuar == 'S');
}

void Vaciar_Pila(Nodop*& pila) {
	if (pila == NULL) {
		gotoxy(27, 14);
		cout << " *- LA PILA SE ENCUENTRA VACIA -* " << endl;
		cout << " Presione una tecla para continuar...";
		_getch(); // Espera una tecla antes de salir
		return;
	}

	// Vaciar la pila
	while (pila != nullptr) {
		Nodop* temp = pila;  // Guardar referencia al nodo actual
		pila = pila->siguiente;  // Mover la pila al siguiente nodo
		delete temp;  // Liberar memoria del nodo eliminado
	}

	// Mensaje después de vaciar la pila
	gotoxy(27, 14);
	cout << " *- LA PILA HA SIDO VACIADA CORRECTAMENTE -* " << endl;
	cout << " Presione una tecla para continuar...";
	_getch(); // Espera una tecla antes de salir
}


void Mostrar_Pila(Nodop*& pila, int a, int b, int c, int d, string h, string i, string j, string l, string m)
{
	// Verificar si la pila está vacía
	if (pila == NULL) {
		gotoxy(27, 14);
		cout << " *- LA PILA SE ENCUENTRA VACIA -* " << endl;
		cout << " Presione una tecla para continuar...";
		_getch(); // Espera una tecla antes de salir
		return;
	}

	Nodop* aux = pila; // Puntero auxiliar para recorrer la pila
	Nodop* ultimo = pila; // Puntero para encontrar el último nodo

	// Encontrar el último nodo recorriendo la pila
	while (ultimo->siguiente != NULL) {
		ultimo = ultimo->siguiente;
	}

	// Recorrer la pila y mostrar los datos
	while (aux != NULL) {
		cout << "----------------------------------" << endl;

		if (aux == pila) { // Si es el primer nodo (top de la pila)
			cout << " \xDB \xDB * Este es el ULTIMO nodo de la pila *\xDB \xDB " << endl << endl;
		}

		cout << "=== Detalles De Ventas ===" << "\n";
		cout << " ID venta: " << aux->datos.ID_Ventas << "    \xAE " << &aux->datos.ID_Ventas << "\n";
		cout << " ID cliente: " << aux->datos.ID_Cliente << "   " << &aux->datos.ID_Cliente << "\n";
		cout << " Tipo de Cafe: " << aux->datos.Tipo_de_Cafe_ventido << "    \xAE " << &aux->datos.Tipo_de_Cafe_ventido << "\n";
		cout << " Fecha de venta: " << aux->datos.Fecha_de_venta << "    \xAE " << &aux->datos.Fecha_de_venta << "\n";
		cout << " Precio Unitario: " << aux->datos.Precio_Unitario << "    \xAE " << &aux->datos.Precio_Unitario << "\n";
		cout << " Cantidad Vendida: " << aux->datos.Cantidad_vendida << "    \xAE " << &aux->datos.Cantidad_vendida << "\n";
		cout << " Descuento: " << aux->datos.Descuento << "    \xAE " << &aux->datos.Descuento << "\n";
		cout << " Metodo de pago: " << aux->datos.Metodo_de_pago << "    \xAE " << &aux->datos.Metodo_de_pago << "\n";
		cout << " Total: " << aux->datos.Total << "    \xAE " << &aux->datos.Total << "\n" << endl;

		if (aux == ultimo) { // Si es el último nodo (base de la pila)
			cout << " \xDB \xDB * Este es el PRIMER nodo de la pila *\xDB \xDB " << endl << endl;
		}

		cout << "---------------------------------- " << endl << endl;

		aux = aux->siguiente; // Avanzar al siguiente nodo
	}

	cout << " Presione una tecla para continuar...";
	_getch(); // Espera una tecla para continuar
}


void Modificar_Nodop(Nodop*& pila, int a, int b, int c, int d, string h, string i, string j, string l, string m)
{


	// Verificar si la pila está vacía
	if (pila == NULL) {
		gotoxy(27, 14);
		cout << " *- LA PILA SE ENCUENTRA VACIA -* " << endl;
		cout << " Presione una tecla para continuar...";
		_getch(); // Espera una tecla antes de salir
		return;
	}
	Nodop* aux = new Nodop;
	aux = pila;

	bool encontrado = false;
	int id_v, id_c;

	if (pila != NULL)
	{
		clearCin();
		gotoxy(40, 5); cout << "INGRESE LOS DATOS QUE SE SOLICITAN\n";
		gotoxy(54, 10); cout << " ID Ventas: ";
		gotoxy(54, 11); cin >> id_v;
		gotoxy(54, 13); cout << " ID CLIENTE: ";
		gotoxy(54, 14); cin >> id_c;

		while (pila != NULL && encontrado != true)
		{
			if (aux != NULL)
			{
				if (aux->datos.ID_Ventas == id_v && aux->datos.ID_Cliente == id_c)
				{
					cout << " *- EL USUARIO FUE ENCONTRADO *- ";
					cout << "----------------------------------";
					cout << endl << &aux->datos << "\n";
					cout << endl << " ID venta: " << aux->datos.ID_Ventas << "\n";
					cout << endl << " ID cliente: " << aux->datos.ID_Cliente << "\n";
					cout << endl << " Tipo de Cafe: " << aux->datos.Tipo_de_Cafe_ventido << "\n";
					cout << endl << " Fecha de venta: " << aux->datos.Fecha_de_venta << "\n";
					cout << endl << " Precio Unitario: " << aux->datos.Precio_Unitario << "\n";
					cout << endl << " Cantidad Vendida: " << aux->datos.Cantidad_vendida << "\n";
					cout << endl << " Descuento: " << aux->datos.Descuento << "\n";
					cout << endl << " Metodo de pago: " << aux->datos.Metodo_de_pago << "\n";
					cout << endl << " Total: " << aux->datos.Total << "\n";
					cout << "----------------------------------\n";

					system("pause");
					encontrado = true;

					bool rep = true;
					int op = 0;
					const char* titulo = "-*-*-*- OPCIONES DE MODIFICACION DEL NODO -*-*-*-";
					const char* opciones[] = { "*- ID VENTAS -*","*- ID CLIENTE -*","*- Tipo de Cafe -*","*- Fecha de venta -*", "*- Precio Unitario -*", "*- Cantidad Vendida -*", "*- Descuento -*", "*- Metodo de pago -*","*- Total -*","*- regreasr -*","*- salir -*" };
					int no = 10;
					cout << "Seleccione el parametro que desee modificar del nodo";

					do
					{
						op = Opciones_Menu(titulo, opciones, no);
						switch (op) {
							system("cls");
						case 1:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							cin >> aux->datos.ID_Ventas;
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 2:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							cin >> aux->datos.ID_Cliente;
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 3:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							cin >> aux->datos.Tipo_de_Cafe_ventido;
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 4:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							getline(cin, aux->datos.Fecha_de_venta);
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;

						case 5:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							cin >> aux->datos.Precio_Unitario;
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 6:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							cin >> aux->datos.Cantidad_vendida;
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 7:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							getline(cin, aux->datos.Descuento);
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;
						case 8:
							system("cls");
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							getline(cin, aux->datos.Metodo_de_pago);
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;

						case 9:
							cout << endl << "\t Ingrese El Nuevo Dato Para Este Nodo: ";
							getline(cin, aux->datos.Total);
							cout << endl << " *- NODO MODIFICADO -* " << endl << endl;
							break;


						case 10:
							PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Cancel.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
							system("cls");
							rep = false;
							break;
						}
						system("cls");

						cout << " *- FUERON MODIFICADOS *- ";
						cout << endl << "----------------------------------";
						cout << endl << &aux->datos << "\n";
						cout << endl << " ID venta: " << aux->datos.ID_Ventas << "\n";
						cout << endl << " ID cliente: " << aux->datos.ID_Cliente << "\n";
						cout << endl << " Tipo de Cafe: " << aux->datos.Tipo_de_Cafe_ventido << "\n";
						cout << endl << " Fecha de venta: " << aux->datos.Fecha_de_venta << "\n";
						cout << endl << " Precio Unitario: " << aux->datos.Precio_Unitario << "\n";
						cout << endl << " Cantidad Vendida: " << aux->datos.Cantidad_vendida << "\n";
						cout << endl << " Descuento: " << aux->datos.Descuento << "\n";
						cout << endl << " Metodo de pago: " << aux->datos.Metodo_de_pago << "\n";
						cout << endl << " Total: " << aux->datos.Total << "\n";
						cout << endl << "----------------------------------" << endl;

					} while (rep);

					return;
				}
				else
				{
					aux = aux->siguiente; //Avanzar al siguiente nodo
				}
			}
			else
			{
				gotoxy(40, 14); cout << " *- NO SE ENCONTRO EL NODO -* ";

				return;
			}
		}
	}
	else
	{
		gotoxy(40, 14); cout << " *- LA PILA SE ENCUENTRA VACIA -* ";

		return;
	}
}

int Opciones_Menu(const char* titulo, const char* opciones[], int n) {
	int opcionSeleccionada = 1;
	int tecla;
	bool repite = true;

	do {
		// Limpiar solo la parte del menú, no toda la pantalla
		system("cls");
		int numero = generarNumeroAleatorio();
		grafica(numero);
		// Mostrar el título del menú
		gotoxy(40, 5); cout << titulo;

		// Mostrar las opciones del menú
		for (int i = 0; i < n; i++) {
			gotoxy(40, 11 + i);
			if (i + 1 == opcionSeleccionada) {
				cout << "\xFE " << opciones[i]; // Resaltar la opción seleccionada
			}
			else {
				cout << "   " << opciones[i]; // Opción normal
			}
		}

		// Mostrar instrucciones
		gotoxy(13, 24);
		cout << "USANDO LAS TECLAS DE DIRECCION ESCOJA UNA OPCION, PRESIONE ENTER PARA INGRESAR.";

		// Dibujar la figura correspondiente a la opción seleccionada
		dibujarFigura(opcionSeleccionada); // Llama a la función para dibujar

		// Captura de teclas
		tecla = _getch();
		if (tecla == 0 || tecla == 224) { // Si es una tecla especial
			tecla = _getch(); // Captura el código real de la tecla
		}

		// Procesar entrada del usuario
		switch (tecla) {
		case TECLA_ARRIBA:
			opcionSeleccionada--;
			if (opcionSeleccionada < 1) {
				opcionSeleccionada = n;
			}

			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			break;
		case TECLA_ABAJO:
			opcionSeleccionada++;
			if (opcionSeleccionada > n) {
				opcionSeleccionada = 1;
			}

			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_PauseMenu_Cursor.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			break;
		case ENTER:
			repite = false; // Salir del bucle para seleccionar la opción
			break;
		}

	} while (repite);

	return opcionSeleccionada;
}

// Función para dibujar una figura (puedes personalizarla)
void dibujarFigura(int opcion) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// Limpiar solo el área del dibujo (opcional)
	for (int i = 5; i <= 13; i++) {
		gotoxy(10, i); cout << "             "; // Limpiar 13 espacios
	}

	// Dibujar una figura según la opción seleccionada
	switch (opcion) {


	case 1:
		SetConsoleTextAttribute(hConsole, 1); // Verde claro
		gotoxy(10, 5); cout << "      . . .\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     { { {\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 2:
		SetConsoleTextAttribute(hConsole, 2); // Verde claro
		gotoxy(10, 5); cout << "     { { {\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     ' ' '\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 3:
		SetConsoleTextAttribute(hConsole, 3); // Verde claro
		gotoxy(10, 5); cout << "      . . .\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     { { {\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 4:
		SetConsoleTextAttribute(hConsole, 4); // Verde claro
		gotoxy(10, 5); cout << "     { { {\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     ' ' '\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 5:
		SetConsoleTextAttribute(hConsole, 5); // Verde claro
		gotoxy(10, 5); cout << "      . . .\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     { { {\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";

		break;
	case 6:
		SetConsoleTextAttribute(hConsole, 6); // Verde claro
		gotoxy(10, 5); cout << "     { { {\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     ' ' '\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 7:
		SetConsoleTextAttribute(hConsole, 7); // Verde claro
		gotoxy(10, 5); cout << "      . . .\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     { { {\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 8:
		SetConsoleTextAttribute(hConsole, 8); // Verde claro
		gotoxy(10, 5); cout << "     { { {\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     ' ' '\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
		break;
	case 9:
		SetConsoleTextAttribute(hConsole, 9); // Verde claro
		gotoxy(10, 5); cout << "      . . .\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     { { {\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	case 10:
		SetConsoleTextAttribute(hConsole, 10); // Verde claro
		gotoxy(10, 5); cout << "     { { {\n";
		gotoxy(10, 7); cout << "      } } }\n";
		gotoxy(10, 8); cout << "     ' ' '\n";
		gotoxy(10, 9); cout << "    +-----+\n";
		gotoxy(10, 10); cout << "    |     |]\n";
		gotoxy(10, 11); cout << "    `-----'\n";
		gotoxy(10, 12); cout << "  ___________\n";
		gotoxy(10, 13); cout << "  `---------'\n";
		break;
	}
}

void Menu_Pila() {
	bool rep = true;
	int op = 0;
	const char* titulo = " MENU VENTAS (PILA) orden LIFO ";
	const char* opciones[] = { "*-Ingreso de datos-*", "*-Ver datos-*", "*-Modificar Datos-*", "*-Buscar datos-*", "*-Eliminar datos-*","*- Vaciar pila -*", "*- SALIR -*" };
	int no = 7;

	do {
		system("cls"); // Limpia la pantalla antes de dibujar

		op = Opciones_Menu(titulo, opciones, no);

		// Dibujar la figura correspondiente a la opción seleccionada
		dibujarFigura(op);

		switch (op) {
		case 1:
			system("cls");
			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			Insertar_Nodop(pila, VENTAS.Cantidad_vendida, VENTAS.Precio_Unitario, VENTAS.ID_Ventas, VENTAS.ID_Cliente, VENTAS.Tipo_de_Cafe_ventido, VENTAS.Fecha_de_venta, VENTAS.Descuento, VENTAS.Total, VENTAS.Metodo_de_pago);
			break;
		case 2:
			system("cls");

			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			Mostrar_Pila(pila, VENTAS.Cantidad_vendida, VENTAS.Precio_Unitario, VENTAS.ID_Ventas, VENTAS.ID_Cliente, VENTAS.Tipo_de_Cafe_ventido, VENTAS.Fecha_de_venta, VENTAS.Descuento, VENTAS.Total, VENTAS.Metodo_de_pago);
			break;
		case 3:
			system("cls");
			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			Modificar_Nodop(pila, VENTAS.Cantidad_vendida, VENTAS.Precio_Unitario, VENTAS.ID_Ventas, VENTAS.ID_Cliente, VENTAS.Tipo_de_Cafe_ventido, VENTAS.Fecha_de_venta, VENTAS.Descuento, VENTAS.Total, VENTAS.Metodo_de_pago);
			break;
		case 4:
			system("cls");
			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			system("cls");
			Buscar_Nodop(pila, VENTAS.Cantidad_vendida, VENTAS.Precio_Unitario, VENTAS.ID_Ventas, VENTAS.ID_Cliente, VENTAS.Tipo_de_Cafe_ventido, VENTAS.Fecha_de_venta, VENTAS.Descuento, VENTAS.Total, VENTAS.Metodo_de_pago);
			break;
		case 5:
			system("cls");
			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			system("cls");
			Eliminar_Nodop(pila, VENTAS.Cantidad_vendida, VENTAS.Precio_Unitario, VENTAS.ID_Ventas, VENTAS.ID_Cliente, VENTAS.Tipo_de_Cafe_ventido, VENTAS.Fecha_de_venta, VENTAS.Descuento, VENTAS.Total, VENTAS.Metodo_de_pago);
			break;

		case 6:
			system("cls");
			PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\OOT_MainMenu_Select.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
			system("cls");
			Vaciar_Pila(pila);
			break;

		case 7:
			rep = false;
			break;
		}


	} while (rep);
}

//Funcion para buscar un nodo
void Buscar_Nodop(Nodop*& pila, int a, int b, int c, int d, string h, string i, string j, string l, string m)
{


	// Verificar si la pila está vacía
	if (pila == NULL) {
		gotoxy(27, 14);
		cout << " *- LA PILA SE ENCUENTRA VACIA -* " << endl;
		cout << " Presione una tecla para continuar...";
		_getch(); // Espera una tecla antes de salir
		return;
	}
	Nodop* aux = new Nodop;
	aux = pila;

	bool encontrado = false;
	int id_v, id_c;

	if (pila != NULL)
	{
		cout << " *- SE ESTA BUSCANDO UN NODO *- " << endl;
		clearCin();

		gotoxy(40, 5); cout << "INGRESE LOS DATOS QUE SE SOLICITAN\n";
		gotoxy(54, 10); cout << " ID Ventas: ";
		gotoxy(54, 11); cin >> id_v;
		gotoxy(54, 13); cout << " ID CLIENTE: ";
		gotoxy(54, 14); cin >> id_c;

		while (pila != NULL && encontrado != true)
		{
			if (aux != NULL)
			{
				if (aux->datos.ID_Ventas == id_v && aux->datos.ID_Cliente == id_c)

				{
					cout << " *- EL Nodo FUE ENCONTRADO *- ";
					cout << endl << "----------------------------------";
					cout << endl << &aux->datos << "\n";
					cout << endl << " ID venta: " << aux->datos.ID_Ventas << "\n";
					cout << endl << " ID cliente: " << aux->datos.ID_Cliente << "\n";
					cout << endl << " Tipo de Cafe: " << aux->datos.Tipo_de_Cafe_ventido << "\n";
					cout << endl << " Fecha de venta: " << aux->datos.Fecha_de_venta << "\n";
					cout << endl << " Precio Unitario: " << aux->datos.Precio_Unitario << "\n";
					cout << endl << " Cantidad Vendida: " << aux->datos.Cantidad_vendida << "\n";
					cout << endl << " Descuento: " << aux->datos.Descuento << "\n";
					cout << endl << " Metodo de pago: " << aux->datos.Metodo_de_pago << "\n";
					cout << endl << " Total: " << aux->datos.Total << "\n";
					cout << "----------------------------------" << endl;

					encontrado = true;
				}
				else
				{
					aux = aux->siguiente; //Avanzar al siguiente nodo
				}
			}
			else
			{
				gotoxy(40, 14); cout << " *- NO SE ENCONTRO EL NODO -* ";
				return;
			}
		}
	}
	else
	{
		gotoxy(40, 14); cout << " *- LA PILA SE ENCUENTRA VACIA -* ";
		return;
	}

	_getch();
}

void Eliminar_Nodop(Nodop*& pila, int a, int b, int c, int d, string h, string i, string j, string l, string m) {
	// Verificar si la pila está vacía
	if (pila == NULL) {
		gotoxy(27, 14);
		cout << " *- LA PILA SE ENCUENTRA VACIA -* " << endl;
		cout << " Presione una tecla para continuar...";
		_getch(); // Espera una tecla antes de salir
		return;
	}

	// Obtener el nodo a eliminar (el último insertado, que está en la cima)
	Nodop* aux = pila;

	system("cls");
	// Mostrar los datos del nodo que se va a eliminar
	cout << " *- Eliminando el último dato ingresado -* " << endl;
	cout << "----------------------------------" << endl;
	cout << " ID VENTAS: " << aux->datos.ID_Ventas << endl;
	cout << " ID CLIENTE: " << aux->datos.ID_Cliente << endl;
	cout << " TIPO DE CAFE: " << aux->datos.Tipo_de_Cafe_ventido << endl;
	cout << " FECHA DE VENTAS: " << aux->datos.Fecha_de_venta << endl;
	cout << " PRECIO UNITARIO: " << aux->datos.Precio_Unitario << endl;
	cout << " CANTIDAD VENDIDA: " << aux->datos.Cantidad_vendida << endl;
	cout << " DESCUENTO: " << aux->datos.Descuento << endl;
	cout << " TOTAL: " << aux->datos.Total << endl;
	cout << " METODO DE PAGO: " << aux->datos.Metodo_de_pago << endl;
	cout << "----------------------------------" << endl;

	cout << " Presione una tecla para confirmar la eliminación...";
	_getch(); // Espera una tecla antes de eliminar

	// Eliminar el nodo de la cima
	pila = aux->siguiente; // Actualizar el tope de la pila
	delete aux;            // Liberar la memoria del nodo eliminado

	cout << endl << " *- NODO ELIMINADO CORRECTAMENTE -* " << endl;
	cout << " Presione una tecla para continuar...";
	_getch(); // Pausa final antes de salir
}

void DibujarRecuadro() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls"); // Limpiar la pantalla
	int numero = generarNumeroAleatorio();
	grafica(numero);
	SetConsoleTextAttribute(hConsole, 15);

	dibujarCuadro3(10, 0, 100, 4); // Dibujar el cuadro
	dibujarCuadro3(10, 4, 100, 25); // Dibujar el cuadro

	dibujarCuadro3(10, 10, 100, 10); // Dibujar el cuadro
	dibujarCuadro3(10, 18, 100, 18); // Dibujar el cuadro

	gotoxy(100, 4); cout << "\xDB";
	gotoxy(10, 4); cout << "\xDB";
	gotoxy(10, 10); cout << "\xDB";
	gotoxy(10, 18); cout << "\xDB";
	gotoxy(100, 18); cout << "\xDB";
	gotoxy(100, 10); cout << "\xDB";
	gotoxy(100, 25); cout << "\xDB";
	gotoxy(10, 25); cout << "\xDB";

	gotoxy(40, 2); cout << "RECIBO DE VENTA DE CAFE";

	gotoxy(40, 6); cout << "INFORMACION DE LA VENTA";
	gotoxy(20, 7); cout << "ID De la venta :";
	gotoxy(20, 8); cout << "ID del cliente :";

	gotoxy(40, 12); cout << "DETALLES DE LA VENTA";
	gotoxy(20, 13); cout << "Tipo de cafe :";
	gotoxy(20, 14); cout << "Fecha de la Venta :";
	gotoxy(20, 15); cout << "Precio por kg :";
	gotoxy(20, 16); cout << "Cantidad vendida (kg) : ";

	gotoxy(40, 20); cout << "TOTAL";
	gotoxy(20, 21); cout << "Descuento : ";
	gotoxy(20, 22); cout << "Total de la Venta : ";
	gotoxy(20, 23); cout << "Forma de Pago :    ";
}

void dibujarCuadro3(int x1, int y1, int x2, int y2) {
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
