// Cabeçalho utilizado no programa concorrente
int *merge_sort_concorrente(int tamanho_vetor, int *vetor, int qtd_threads);
void merge_sort(int *vetor, int inicio, int fim);
void merge(int *vetor, int inicio, int meio, int fim);
void *tarefa(void *arg);
void imprime_vetor(int tamanho_vetor, int *vetor);

// Cabeçalho utilizado no arquivo de testes
void teste();
