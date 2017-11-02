struct work {
	LIST* toProcess;
	int id;
};
typedef struct work Work;

Work* crearTrabajo(LIST* head, int id) {
	Work* nWork = malloc(sizeof(Work));
	nWork->toProcess = head;
	nWork->id = id;
	return nWork;
}

void liberarTrabajo(Work* trabajo) {
	liberarLista(trabajo->toProcess);
	free(trabajo);
}
