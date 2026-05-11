#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char usuario[100];
    int nota;
}Calificacion;

typedef struct {
  char id[100];
  char title[150];
  char director[100];
  List *genres;
  int year;
  List *Calificaciones;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");
  puts("1) Cargar Películas");
  puts("2) Buscar por Categoria");
  puts("3) Buscar por Director");
  puts("4) Buscar por Decada");
  puts("5) Busqueda Avanzada (Género + Decada)");
  puts("6) Gestionar Watchlist");
  puts("7) Calificar Pelicula");
  puts("8) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

void mostrarPelicula(Film *peli){
  printf("ID: %s.\n", peli->id);
  printf("Titulo: %s.\n", peli->title);
  printf("Director: %s.\n", peli->director);
  printf("Año: %d.\n", peli->year);
  printf("Géneros: ");
  char *genres = list_first(peli->genres);
  while(genres != NULL){
    printf("%s ", genres);
    genres = list_next(peli->genres);
  }
  printf("\n");

  Calificacion *califica = list_first(peli->Calificaciones);
  if(califica == NULL){
    printf("Calificaciones de usuarios -> (sin calificaciones)\n");
  }
  else{
    printf("Calificaciones de usuarios -> ");
    while(califica != NULL){
      printf("%s: %d", califica->usuario, califica->nota);
      califica = list_next(peli->Calificaciones);
      if(califica != NULL) printf(" | ");
    }
    printf("\n");
  }
  printf("========================================\n");
}


/**
 * Carga películas desde un archivo CSV y las almacena en un mapa por ID.
 */
void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirector) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL){
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  // Leer y parsear una línea del archivo CSV. La función devuelve un array de
  // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    if(campos[1] == NULL || campos[5] == NULL || campos[10] == NULL || campos[11] == NULL || campos[14] == NULL) continue;
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    strncpy(peli->id, campos[1], 99);
    peli->id[99] = '\0';
    strncpy(peli->title, campos[5], 149);
    peli->title[149] = '\0';
    strncpy(peli->director, campos[14], 99);
    peli->director[99] = '\0';
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->Calificaciones = list_create();

    
    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
    // Obtiene el primer género de la lista de géneros de la película
    char *genre = list_first(peli->genres);
    // Itera sobre cada género de la película
    while (genre != NULL) {
      while(*genre == ' ') genre++;

      // Creo copia de los generos sin el espacio del principio
      char genreCopy[100];
      strncpy(genreCopy, genre, 99);
      genreCopy[99] = '\0';
      // Busca el género en el mapa pelis_bygenres
      MapPair *genre_pair = map_search(pelis_bygenres, genreCopy);    
      // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
      if (genre_pair == NULL) {
        List *new_list = list_create();
        list_pushBack(new_list, peli);
        map_insert(pelis_bygenres, strdup(genreCopy), new_list);
      } else {
        // Si el género ya existe en el mapa, obtén la lista y agrega la película
        List *genre_list = (List *)genre_pair->value;
        list_pushBack(genre_list, peli);
      }

      // Avanza al siguiente género en la lista
      genre = list_next(peli->genres);
    }

    // Insertar por director
    MapPair* director_Pair = map_search(pelis_bydirector, peli->director);
    if(director_Pair == NULL){
      List *newList = list_create();
      list_pushBack(newList, peli);
      map_insert(pelis_bydirector, peli->director, newList);
    }
    else list_pushBack((List*) director_Pair->value, peli);
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  printf("Peliculas cargadas correctamente.\n");
}

void buscarPorCategoria(Map *pelis_bygenres){
  limpiarPantalla();
  char categoria[100]; // Buffer para almacenar la categoria de la película

  // Solicita al usuario la categoria de la película
  printf("Ingrese la categoria de la película: ");
  scanf(" %[^\n]", categoria); // Lee la categoria del teclado

  MapPair *pair = map_search(pelis_bygenres, categoria);
  if(pair == NULL){
    printf("No se encontrar peliculas del genero: '%s'\n", categoria);
    return;
  }

  List *lista = (List*) pair->value;
  Film *peli = list_first(lista);
  int count = 0;
  while(peli != NULL){
    mostrarPelicula(peli);
    count++;
    peli = list_next(lista);
  }
  printf("Total: %d pelicula(s) encontrada(s).\n", count);
}

void buscarPorDirector(Map *pelis_bydirector){
  limpiarPantalla();
  char director[100];
  printf("Ingrese el nombre del director: ");
  scanf(" %[^\n]", director);

  MapPair *pair = map_search(pelis_bydirector, director);
  if(pair == NULL){
    printf("No se encontrar peliculas del director: '%s'\n", director);
    return;
  }

  List *lista = (List*) pair->value;
  Film *peli = list_first(lista);
  int count = 0;
  while(peli != NULL){
    mostrarPelicula(peli);
    count++;
    peli = list_next(lista);
  }
  printf("Total: %d pelicula(s) encontrada(s).\n", count);
}

void buscarPorDecada(Map *pelis_byid){
  limpiarPantalla();
  int anio;
  printf("Ingrese el año de incio de la década (ej: 1990): ");
  scanf(" %d", &anio);

  int decadaInicio = (anio/10) * 10;
  int decadaFin = decadaInicio + 9;
  printf("Mostrando peliculas de %d a %d:\n", decadaInicio, decadaFin);
  printf("========================================");

  MapPair *pair = map_first(pelis_byid);
  int count = 0;
  while(pair != NULL){
    Film *peli = (Film*) pair->value;
    if(peli->year >= decadaInicio && peli->year <= decadaFin){
      mostrarPelicula(peli);
      count++;
    }
    pair = map_next(pelis_byid);
  }

  if(count == 0){
    printf("No se encontrar peliculas en esa decada.\n");
  }
  else{
    printf("Total: %d pelicula(s) encontra(s).\n", count);
  }
}

//FUNCIÓN 5(PREGUNTA)
void buscarPorGeneroYDecada(Map* pelis_bygenres){
  limpiarPantalla();
  char generoBuscado[100];
  int decada;
  printf("\nIngrese genero: ");
  scanf(" %[^\n]", generoBuscado);
  printf("Ingrese decada (ej: 1990): ");
  scanf("%d", &decada);
  MapPair *aux = map_search(pelis_bygenres, generoBuscado);
  if(aux == NULL){
    printf("No se encontraron peliculas con el genero: '%s'.\n", generoBuscado);
    return;
  }

  List *lista = (List*) aux->value;
  Film *peli = list_first(lista);
  int count = 0;
  while(peli != NULL){
    if(peli->year >= decada && peli->year <= decada+9){
      mostrarPelicula(peli);
      count++;
    }
    peli = list_next(lista);
  }
  if(count == 0){
    printf("No se encontraron peliculas con esos criterios.\n");
  }
  else{
    printf("Total: %d pelicula(s) encontrada(s).\n", count);
  }
  
}

//FUNCION 6(PREGUNTA)

void agregarWatchlist(Map* pelis_byid, List* watchlist) {
  limpiarPantalla();
  char idBuscado[100];
  printf("Ingrese ID pelicula: ");
  scanf(" %[^\n]", idBuscado);
  MapPair *peliPair = map_search(pelis_byid, idBuscado);
  
  if(peliPair == NULL){
    printf("\nLa pelicula no existe.\n");
    return;
  }
  Film *peli = (Film*) peliPair->value;
  Film *aux = list_first(watchlist);
  while(aux != NULL){
    if(strcmp(aux->id, peli->id) == 0){
      printf("La pelicula ya esta en su watchlist.\n");
      return;
    }
    aux = list_next(watchlist);
  }
  list_pushBack(watchlist, peli);
  printf("Pelicula '%s' agregada correctamente.\n", peli->title);
}

void eliminarWatchlist(List* watchlist){
  limpiarPantalla();
  char idBuscado[100];
  printf("Ingrese ID pelicula a eliminar: ");
  scanf(" %[^\n]", idBuscado);
  
  Film *peli = list_first(watchlist);
  while(peli != NULL){
    if(strcmp(peli->id, idBuscado) == 0){
        list_popCurrent(watchlist);
        printf("Pelicula eliminada de la Watchlist.\n");
        return;
      }
    peli = list_next(watchlist);
  }
  printf("\nLa pelicula no esta en watchlist.\n");
}

void mostrarWatchlist(List* watchlist) {
  limpiarPantalla();
  Film* peli = list_first(watchlist);
  if(peli == NULL) {
    printf("La watchlist esta vacia.\n");
    return;
  }
  printf("\n========== WATCHLIST ==========\n");
  while(peli != NULL){
      mostrarPelicula(peli);
      peli = list_next(watchlist);
  }
}

//FUNCION 7 (PREGUNTA)

void calificarPelicula(Map* peli_byid) {
  limpiarPantalla();
  char idBuscado[100];
  char usuario[100];
  int nota;
  
  printf("Ingrese ID pelicula: ");
  scanf(" %[^\n]", idBuscado);
  
  MapPair *peliPair = map_search(peli_byid, idBuscado);
  
  if(peliPair == NULL){
    printf("Pelicula no encontrada.\n");
    return;
  }
  
  Film *peli = (Film*) peliPair->value;
  printf("Ingrese usuario: ");
  scanf(" %[^\n]", usuario);
  printf("Ingrese nota (1-10): ");
  scanf("%d", &nota);
  
  if(nota < 1 || nota > 10){
      printf("Nota invalida.\n");
      return;
  }
  Calificacion *c = list_first(peli->Calificaciones);
    while(c != NULL){
      if(strcmp(c->usuario, usuario) == 0){
          c->nota = nota;
          printf("Calificacion actualizada.\n");
          return;
      }
    c = list_next(peli->Calificaciones);
  }
  
  Calificacion* nueva = malloc(sizeof(Calificacion));
  strcpy(nueva->usuario, usuario);
  nueva->nota = nota;
  list_pushBack(peli->Calificaciones, nueva);
  printf("Calificacion agregada correctamente.\n");
}

int main() {
  char opcion;// Variable para almacenar una opción ingresada por el usuario
  char funcionalidad;
               // (sin uso en este fragmento)

  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirector = map_create(is_equal_str);
  List *watchlist = list_create();

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
      case '1':
        cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirector);
        break;
      case '2':
        buscarPorCategoria(pelis_bygenres);
        break;
      case '3':
        buscarPorDirector(pelis_bydirector);
        break;
      case '4':
        buscarPorDecada(pelis_byid);
        break;
      case '5':
        buscarPorGeneroYDecada(pelis_bygenres);
        break;
      case '6':
        limpiarPantalla();
        puts("========================================");
        puts("          Gestion de Watchlist");
        puts("========================================");
        puts("1) Agregar Pelicula");
        puts("2) Eliminar Pelicula");
        puts("3) Mostrar Watchlist");
        printf("Ingrese su opcion: ");
        scanf(" %c", &funcionalidad);
        switch(funcionalidad){
          case '1':
            // agregar pelicula
            agregarWatchlist(pelis_byid, watchlist);
          break;
          case '2':
            // eliminar pelicula
            eliminarWatchlist(watchlist);
          break;
          case '3':
            // mostrar watchlist
            mostrarWatchlist(watchlist);
          break;
          default:
          printf("Opcion no vaida.\n");
        }
        break;
      case '7':
        calificarPelicula(pelis_byid);
        break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
