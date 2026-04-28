
#include"encabezado.h";
int generarNumeroAleatorio();

void limpiarBuffer() {
    cin.clear();           // Limpia errores en cin
    cin.ignore(10000, '\n'); // Descarta cualquier entrada previa
}

void mostrarMenu() {
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
            gotoxy(45, 7);cout << "  .:Menu Principal:.";
            gotoxy(45, 8);cout << "Seleccione una opcion:";

            // Mostrar opciones
            for (int i = 1; i <= 7; i++) {
                gotoxy(x, y + i - 1);
                if (i == opcion) {
                    //  setColor(14);
                    cout << "-> ";
                }
                else {
                    //setColor((numero + 3) % 16); // Cambia el color del texto, limita a un rango de 0-9
                    cout << "   ";
                }

                switch (i) {
                case 1: cout << "Compra (cola)"; break;
                case 2: cout << "Venta (pila)"; break;
                case 3: cout << "Producto(lista simple)"; break;
                case 4: cout << "Trabajadores(lista doblemente enlazada)";break;
                case 5: cout << "Clientes (lista circular)";break;
               
                case 6: cout << "Salir"; break;
                }
            }

            tecla = _getch();       //capturamos la tecla precionada
            if (tecla == 224) {     // detectar si es una tecla flechas  
                tecla = _getch();   // capturar el nuevo valor 
                if (tecla == 72 && opcion > 1) { // Flecha arriba
                    opcion--;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                }
                else if (tecla == 80 && opcion < 6) { // Flecha abajo
                    opcion++;
                    PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo3.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                }
            }
            else if (tecla == 13) { // Enter para seleccionar
                break;
            }
        }

        // setColor(15);
        if (opcion == 6) {
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\notificacion8.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al salir
            Sleep(1000); // Espera un poco para que el sonido se reproduzca antes de cerrar
            break;
        } // Opción "Salir" cierra el programa

        switch (opcion) {
        case 1:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
              compra();
            break;
        case 2:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
                Menu_Pila();
            break;
        case 3:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
              Menu_Lista();
            break;
        case 4:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
              trabajadores();
            break;
        case 5:
            PlaySound(L"C:\\Users\\Castr\\source\\repos\\proyectoParcial\\x64\\Debug\\nuevo6.wav", NULL, SND_FILENAME | SND_ASYNC); // Sonido al moverse
           cliente();
            break;
     
        }
    }
}



void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void grafica(int color) {

    setColor((color + 1) % 16); // Cambia el color del texto, limita a un rango de 0-9

    setColor((color + 8) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "     .       .    )     .        .   .       *             .    .   .. .     .     .         .   .           . .    .    . " << endl;
    setColor((color + 9) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << ".       *             .     .        .   .   .       *             .         .            .    .     .     .    .  " << endl;
    setColor((color + 10) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "            .      .               .   .       *             .         .     .                . .        .    .    " << endl;
    setColor((color + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "       .       .   .                   .         .   .       *             .         .      .       .             ." << endl;
    setColor((color + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "                 .       .             .                      .         .      .            *        . .    .      " << endl;
    setColor((color + 13) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "     .       .            .   .         .         .    .       *    )         .         .   .       .  .   .  .  .  " << endl;
    setColor((color + 14) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << ".       *             .     .         .   .       *             .         .    *             .         .   .    . ." << endl;
    setColor((color + 15) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "            .      .    .   .        .   .. .  .         .       .   .       *             .         .        *          . " << endl;
    setColor((color + 16) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "       .       .   .       *             .    .   .       *             .         .           .    .   .   .   .   " << endl;
    setColor((color + 17) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "                 .      .   .                .         .       *             .         .            *   .  .  .    " << endl;
    setColor((color + 18) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "     .   '               .       .       *   .   .       *             .         .          *      .         .    ." << endl;
    setColor((color + 19) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "               - .    .   .               .         .     .      .       *             .         .      .     *    " << endl;
    setColor((color + 1) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "     .       .          .        .   .       *             .         .     .         .   .           . .    .    . " << endl;
    setColor((color + 2) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << ".       *             .     .        .   .   .       *             .         .            .    .     .     .    .  " << endl;
    setColor((color + 3) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "            .      .               .   .       *             .         .     .                . .        .    .    " << endl;
    setColor((color + 4) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "       .       .   .                   .     )    .   .       *             .         .      .       .            ." << endl;
    setColor((color + 5) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "                 .       .             .                      .         .      .            *        . .    .      " << endl;
    setColor((color + 6) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "     .   '               .       .       *      .   .     .         .             .         .      .     .     .   " << endl;
    setColor((color + 7) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "             _.---._   .      .       *          .   .                .         .         .         .     .       *" << endl;
    setColor((color + 8) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "   *       .'       '.   .    .   .        .   .. .  .         .       .   .       *            .       .          ." << endl;
    setColor((color + 9) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "      _.-~===========~-.          .     .        .   .   .       *             .         .            .    .      ._" << endl;
    setColor((color + 10) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "      (___________________)       .  .       .   .                   .     )       .         .      .       .      *" << endl;
    setColor((color + 11) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << " __  .'     \_______/   .'  ______        __- .    .   .         .         .     .      .       *             .     " << endl;
    setColor((color + 12) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "   |              .'  .'   |      |      |  |    .   .         .       *             .         .      .       .     " << endl;
    setColor((color + 13) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "   |             '         |      |      |  |      .   .    . .      .            .         .   .       *           " << endl;
    setColor((color + 14) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << "   |                       |      |   ___|  |_      .     .         .   .            .    *             .         . " << endl;
    setColor((color + 15) % 16); // Cambia el color del texto, limita a un rango de 0-9
    cout << " __|_______________________|__..--~~~~     ~--.  _ - .    .   .              .      .       *     .   .. .        . " << endl;
    setColor((color + 16) % 16); // Cambia el color del texto, limita a un rango de 0-9

}

// Función para pausar la ejecución durante un cierto tiempo (en milisegundos)
void delay(int ms) {
    this_thread::sleep_for(chrono::milliseconds(ms));
}


// Función que genera un número aleatorio entre 1 y 10
int generarNumeroAleatorio() {
    return (rand() % 16) + 1;
}


