#pragma once
#include"encabezado.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include <conio.h> // Para _getch()
#include <windows.h>
#include <iomanip>

#include <iostream>
#include <windows.h>  // Manipulación de la consola y colores (gotoxy(), setColor()).
#include <thread>     // reproducir musica, evita que el programa se bloquee miestras suena la musica 
#include <chrono>     //Pausas en milisegundos (std::this_thread::sleep_for).
#include <conio.h>    // Detección de teclas, Para usar _kbhit() y _getch()
#include <string>  
#include <iomanip>  // Para std::put_time  
#include <ctime>    // Para std::time_t, std::localtime  
#include < mmsystem.h >  //reproduccion de audio con PlaySound()


using namespace std;

#include <sstream>

#include <vector>

// Función para pausar la ejecución durante un cierto tiempo (en milisegundos)

void gotoxy(int x, int y);


void mostrarMenu();
void setColor(int color);  //cambiar color 

#include <sstream>

#include <vector>


using namespace std;


void delay(int ms);        //pausar
void setColor(int color);  //cambiar color 

int generarNumeroAleatorio();
void grafica(int color);
void limpiarBuffer();

// menu principal 
void mostrarMenu();

//submenu compra
void compra();
string obtenerFechaActual();

//submenu venta
void Menu_Pila();

//submenu producto
void Menu_Lista();

//submenu trabajadores
void trabajadores();

//submenu cliente
void cliente();
 

//buena rola 