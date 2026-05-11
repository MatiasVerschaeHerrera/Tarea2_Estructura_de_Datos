# StreamFinder — Tarea 2 Estructura de Datos

Aplicación de consola desarrollada en lenguaje C para gestionar y explorar un catálogo de películas utilizando TDAs personalizados como Map y List.

El sistema permite cargar películas desde un archivo CSV, realizar búsquedas eficientes, gestionar una watchlist personalizada y registrar calificaciones de usuarios.

---

# Integrantes

- Matías Verschae Herrera
- Mathias Donoso Tapia

---

# Tecnologías Utilizadas

- Lenguaje C
- GCC
- TDAs personalizados:
  - Map
  - List

---

# Estructura del Proyecto

```bash
Tarea2_Estructura_de_Datos/
├── tarea2.c
├── tdas/
│   ├── extra.c
│   ├── extra.h
│   ├── list.c
│   ├── list.h
│   ├── map.c
│   └── map.h
├── data/
│   └── Top1500.csv
└── README.md
```

---

# Compilación

Para compilar el proyecto:

```bash
gcc -o tarea2_app tarea2.c tdas/extra.c tdas/list.c tdas/map.c
```

---

# Ejecución

Ejecutar el programa:

```bash
./tarea2_app
```

---

# Carga de Datos

El programa utiliza el archivo:

```bash
data/Top1500.csv
```

Asegúrese de que el archivo exista antes de ejecutar la opción de carga de películas.

---

# Funcionalidades Implementadas

## 1. Cargar Películas

Carga películas desde un archivo CSV y almacena la información en memoria utilizando mapas para optimizar las búsquedas.

Cada película contiene:
- ID
- Título
- Director
- Géneros
- Año
- Calificaciones de usuarios

---

## 2. Buscar por Categoría

Permite buscar películas según género.

Ejemplos:
- Action
- Drama
- Comedy
- Sci-Fi

La búsqueda se realiza de manera eficiente utilizando un `Map`.

---

## 3. Buscar por Director

Permite listar todas las películas asociadas a un director específico.

Ejemplos:
- Christopher Nolan
- Steven Spielberg
- Quentin Tarantino

---

## 4. Buscar por Década

Muestra películas pertenecientes a una década específica.

Ejemplo:

```bash
1990
```

Mostrará películas entre:

```bash
1990 - 1999
```

---

## 5. Búsqueda Avanzada (Género + Década)

Permite combinar:
- Género
- Década

Ejemplo:

```bash
Genero: Action
Decada: 2000
```

---

## 6. Gestión de Watchlist

El sistema permite mantener una lista personalizada de películas pendientes.

### Funciones:
- Agregar película por ID
- Eliminar película
- Mostrar watchlist

El programa valida que el ID exista antes de agregarlo.

---

## 7. Calificar Películas

Permite:
- Ingresar usuario
- Asignar nota del 1 al 10
- Actualizar una calificación existente

Las calificaciones aparecen automáticamente al mostrar una película.

Ejemplo:

```bash
Matias: 10 | Javiera: 9
```

---

# Uso de TDAs

## Map

Se utilizan mapas para búsquedas rápidas por:
- ID
- Género
- Director

Esto evita recorrer linealmente todas las películas.

## List

Se utilizan listas para:
- Géneros
- Watchlist
- Calificaciones
- Películas agrupadas

---

# Ejemplo de Uso

```bash
========================================
     Base de Datos de Películas
========================================
1) Cargar Películas
2) Buscar por Categoria
3) Buscar por Director
4) Buscar por Decada
5) Busqueda Avanzada (Género + Decada)
6) Gestionar Watchlist
7) Calificar Pelicula
8) Salir
```

Ejemplo:

```bash
Ingrese su opción: 2
Ingrese la categoria de la película: Action
```

Salida:

```bash
Titulo: The Dark Knight
Director: Christopher Nolan
Año: 2008
Generos: Action Crime Drama

Calificaciones de usuarios ->
Matias: 10 | Javiera: 9
```

---

# Requisitos de Desempeño

El programa cumple con los requisitos solicitados:

- Uso eficiente de TDAs
- Búsquedas rápidas mediante `Map`
- Validación de IDs en Watchlist
- Navegación mediante menú interactivo
- Gestión dinámica de memoria

---

# Problemas Conocidos

- El archivo CSV debe estar ubicado en:

```bash
data/Top1500.csv
```

- Algunas películas podrían contener caracteres especiales dependiendo del CSV utilizado.

---

# Posibles Mejoras Futuras

- Buscar películas por título
- Ordenar películas por rating
- Exportar Watchlist
- Interfaz gráfica
- Recomendaciones automáticas

---

# Conclusión

Este proyecto permitió aplicar estructuras de datos eficientes en lenguaje C mediante el uso de mapas y listas dinámicas, optimizando búsquedas y gestión de información sobre un catálogo masivo de películas.

---

# Licencia

Proyecto desarrollado únicamente con fines académicos.
