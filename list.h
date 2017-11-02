struct list {
    int n;
    struct list *next;
};

typedef struct list LIST;

LIST* crearNodo(int n, LIST* next) {
  LIST* nList = malloc(sizeof(LIST));
  nList->n = n;
  nList->next = next;
}

/**
 * Imprime en consola el valor de los nodos pertenecientes a una lista.
 * @param  head          Puntero a la cabeza de la lista.
 */
void imprimirLista(LIST* head) {
  LIST* aux = head;
  while (aux) {
    printf("%d\n", aux->n);
    aux = aux->next;
  }
}

/**
 * Libera la memoria de los elementos de una lista.
 * @param  head         Nodo a partir del cual se liberara memoria.
 */
void liberarLista(LIST* head) {
  LIST* aux;
  while (head) {
    aux = head;
    head = head->next;
    free(aux);
  }
}

/**
 * Cuenta el numero de nodos en una lista.
 * @param  head              Puntero a la cabeza de la lista de la cual contaremos nodos.
 * @return                   Cantidad de nodos de la lista.
 */
int numeroDeElementos(LIST* head){
  int resultado = 0;
  LIST* aux = head;
  while (aux) {
    aux = aux->next;
    resultado++;
  }

  return resultado;
}

/**
 * Clona un nodo creando una copia en memoria a partir del nodo que reciba. Se puede usar para duplicar una lista
 * @param  nodo       Nodo que copiar
 * @return            Puntero al nuevo nodo duplicado
 */
LIST* clonarNodo(LIST* nodo) {
  if (!nodo)
    return 0;

  LIST* dup = malloc(sizeof(LIST));
  dup->n = nodo->n;
  dup->next = clonarNodo(nodo->next);

  return dup;
}

/**
 * A partir de una lista, devuelve otra compuesta por elementos de esta en un rango definido.
 * @param  head     Lista de la cual queremos una sublista
 * @param  start    Indice de elemento que sera el primero de la sublista (0 indexed)
 * @param  end      Indice de elemento que sera el ultimo de la sublista (0 indexed)
 * @return          Puntero a la cabeza de la sublista
 */
LIST* subLista(LIST* head, int start, int end) {
  if (end < start)
    return NULL;

  if (start > numeroDeElementos(head))
    return NULL;

  LIST* dupLista = clonarNodo(head);

  for (int i = 0; i<start; i++) {
    if (dupLista->next) {
      LIST* next = dupLista->next;
      free(dupLista);
      dupLista = next;
    }
  }

  int amount = end - start;
  LIST* lastNode = dupLista;

  for (int i=0; i < amount; i++) {
    if (lastNode) {
      lastNode = lastNode->next;
    }
  }
  liberarLista(lastNode->next);
  lastNode->next = NULL;

  return dupLista;
}
