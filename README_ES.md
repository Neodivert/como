# COMO: Cooperative Modeller

![COMO Screenshot](https://github.com/moisesjbc/como/blob/master/img/como.png "COMO")

[English translation here](https://github.com/moisesjbc/como/blob/master/README_ES.md)

## Resumen

COMO (Cooperative Modeller) es un modelador de escenas 3D cooperativo. Permite que múltiples usuarios se conecten a través de Internet y trabajen sobre la misma escena 3D al mismo tiempo.


**Este proyecto está "completado"**, lo que significa que no preveo incluir nuevas funcionalidades por ahora.


### Un modelador de escenas

El modelador cooperativo COMO está orientado a la creación colaborativa de escenas más que al desarrollo de modelos individuales. Esto implica principalmente que:

* Dos usuarios no pueden estar trabajando sobre el mismo objeto 3D al mismo tiempo. Sólo el usuario que selecciona un objeto (una malla, un material, etc) puede transformarlo hasta que lo desbloquee.
* No se permite la edición a nivel de vértices y aristas de las mallas 3D presentes en la escena.

La idea es que múltiples usuarios creen una escena de, por ejemplo, el interior de un edificio, combinando primitivas "pared", "silla", "mesa", etc. COMO no está pensado para que múltiples personas modelen un personaje humano desde cero, por ejemplo.
Otro aspecto a tener en cuenta sobre COMO es que se trata de un sofware experimental centrado en la sincronización de escenas 3D a través de la red. Como tal, no espera competir con las grandes suites 3D como Blender o Maya.


## Funcionalidades

A continuación se listan las principales acciones que se pueden realizar con el modelador cooperativo COMO:

* Crear una escena 3D accesible desde la red así como conectarse a escenas 3D ya creadas por otros usuarios (se requiere la IP del servidor).
* Importar y visualizar modelos 3D desde ficheros .obj. Los modelos pueden contener texturas siempre que éstas tengan alguno de los formatos reconocidos por la librería SDL2_image (png, jpg, tga, etc).
* Aplicar transformaciones sencillas a conjuntos de objetos 3D: traslaciones, rotaciones y escalados.
* Modificar los parámetros de los materiales asociados a cada malla de la escena: reflexión difusa, especular y ambiente, color del material, etc.
* Crear y editar múltiples luces direccionales (hasta un máximo definido por el servidor).
* Crear primitivas geométricas (cubos, conos, cilindros y esferas) y aplicar una textura diferente a cada cara de los mismas.
* Visualizar y editar la escena a través de un máximo de 4 visores. Para cada visor se puede establecer la vista (frontal, desde arriba, etc) y la proyección (ortogonal o perspectiva) utilizada.
* Salvar la escena actual en un fichero para recuperarla más adelante (sólo en el servidor).

Cómo puedes comprobar, cualquier suite 3D ofrece éstas y muchas otras características, pero no todas permiten manipular una escena con tus amigos o compañeros ;-).


## Características técnicas

### Sistema operativo

Aunque COMO se ha desarrollado desde el principio para ser multiplataforma, actualmente sólo funciona en Ubuntu de 64 bits. Con el cambio de un pequeño fragmento de código y alguna que otra corrección, en teoría debería funcionar sin problemas en otras plataformas... en teoría.

### Libraries

COMO makes use of the following libraries:

El software hace uso de las siguientes librerías:

* [Qt 5.2.1](http://qt-project.org/) para la interfaz de usuario.
* [Boost 1.55.0](http://www.boost.org/) para la comunicación en red, los hilos y el sistema de directorios.
* [OpenGL 4.2](http://www.opengl.org/) a través de la librería [Mesa](http://www.mesa3d.org/) para los gráficos 3D.
* [SDL2](https://www.libsdl.org/) y [SDL_image 2.0](https://www.libsdl.org/projects/SDL_image/) para la carga de texturas.
* [GLM 0.9.5.1](http://glm.g-truc.net/) para el cálculo con vectores y matrices.

COMO también depende del programa "gnome-terminal" para crear un servidor desde el cliente.


## Construir COMO (Ubuntu 14.04)

### Dependencias
1. Instalar Qt 5.2 o superior. Para ello se puede usar el [instalador online](http://qt-project.org/downloads) o también se puede instalar Qt desde [este repositorio](https://launchpad.net/~ubuntu-sdk-team/+archive/ppa) ejecutando los siguientes comandos en una terminal:
   ```
   sudo add-apt-repository ppa:ubuntu-sdk-team/ppa
   sudo apt-get update
   sudo apt-get install qtdeclarative5-dev qtcreator
   ```

2. Ejecutar los siguientes comandos para mostrar la versión de OpenGL usada. Comprobar que sea la 4.2 o superior.
   ```
   sudo apt-get install mesa-utils
   glxinfo | grep OpenGL
   ```

3. Instalar el resto de dependencias (comprobando que las versiones sean iguales o superiores a las indicadas en "Dependencias".
   ```
   sudo apt-get install gnome-terminal libglm-dev libboost1.55-dev libboost-system1.55.0 libboost-filesystem1.55.0 libboost-thread1.55.0 libboost-system1.55-dev libboost-thread1.55-dev libboost-filesystem1.55-dev gnome-terminal libsdl2-image-dev
   ```

### Construcción

1. Descargar la última versión de este repositorio [aquí](https://github.com/moisesjbc/como)

2. Entrar en el directorio descargado.

   ```
   cd <directorio>
   ```

3. Inicializar y actualizar los submódulos del repositorio:

   ```
   git submodule update --init --recursive
  ```

4. Abrir el fichero "project/client/como_project.pro" con Qt Creator y establecer "../../bin/client" como el directorio de construcción (build directory).

5. Abrir el fichero "project/server/server.pro" con Qt Creator y establecer "../../bin/server" como el directorio directorio de construcción (build directory).

6. Opcionalmente añadir "-j (n)" a "make additional arguments" en las propiedades de ambos proyectos, donde (n) es el número de hilos que se usarán para la construcción del software.

7. Construir ambos proyectos en modo "release", ¡y disfrutar!


## Multimedia 

### Documentación

[Echa un vistazo a la wiki de COMO (en inglés)](https://github.com/moisesjbc/como/wiki) para aprender a utilizar COMO.

### Vídeos

En [esta lista de reproducción de Youtube](http://www.youtube.com/playlist?list=PLfi4ueBWh9m5T1Gp72CyoKc2hi_RGa_NG) se encuentran los vídeos que he subido sobre el modelador cooperativo COMO.
