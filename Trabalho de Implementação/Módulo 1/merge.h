// Cabeçalho utilizado no programa concorrente
int *merge_sort(int tamanho_vetor, int *vetor, int qtd_threads);
void divide(int *vetor, int inicio, int fim);
void merge(int *vetor, int inicio, int meio, int fim);
void *tarefa(void *arg);
void imprime_vetor(int tamanho_vetor, int *vetor);

// Cabeçalho utilizado no arquivo de testes
void teste();
