

#  Sistema-Integral-Cafetalero-SIC-

Este proyecto es una aplicación de escritorio desarrollada en **C++** que utiliza la librería gráfica **SFML**. Su objetivo principal es gestionar registros de compras de café de manera eficiente, utilizando un **Árbol AVL** para organizar la información.

##  Lógica y Estructura de Datos

El corazón de este sistema es el **Árbol AVL**, una estructura de datos de búsqueda binaria que se auto-balancea. A diferencia de un árbol binario común, el AVL garantiza que el tiempo de búsqueda, inserción y eliminación sea siempre **$O(\log n)$**.

### ¿Cómo funciona el balanceo?
El sistema calcula el **Factor de Equilibrio (FE)** de cada nodo:
$$FE = Altura(Derecha) - Altura(Izquierda)$$

Si el factor de equilibrio se sale del rango [-1, 1], el algoritmo ejecuta **rotaciones** (Simples o Dobles) para reestructurar el árbol y mantenerlo óptimo. Esto evita que el sistema se vuelva lento incluso con miles de registros.



##  Funcionalidades

* **Interfaz Gráfica Interactiva:** Menús dinámicos con soporte para navegación mediante teclado y efectos de sonido.
* **Gestión de Registros (CRUD):**
    * **Ingreso:** Registro de compras con validación.
    * **Búsqueda:** Localización instantánea por ID.
    * **Eliminación:** Remoción de nodos manteniendo el balance del árbol.
    * **Modificación:** Actualización de datos existentes.
* **Visualización:** Recorridos de la estructura para verificar la organización de los datos.

##  Tecnologías Utilizadas

* **Lenguaje:** C++ (Enfoque en Programación Orientada a Objetos).
* **Gráficos y Audio:** [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/).
* **Estructura de Datos:** Árbol AVL (Implementación manual).
* **IDE:** Visual Studio 2022.

##  Configuración para Compilación

El proyecto está configurado con **rutas relativas** para ser portable.
1. Asegúrate de tener instaladas las herramientas de compilación **v145** (Visual Studio 2022).
2. El proyecto busca las librerías de SFML en la carpeta raíz mediante `$(ProjectDir)`.
3. Compilar en modo **Debug/Release** para arquitectura **x64**.

