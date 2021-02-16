typedef struct {
    int *vetor;
    int inicio;
    int fim;
} threadArgs;

// Cabeçalho utilizado no programa concorrente
int *quicksort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads);
void quicksort(int *vetor, int inicio, int fim);
int particao(int *vetor, int inicio, int fim);
void produtor_tarefa(int *vetor, int inicio, int fim);
void *consumidor_tarefa(void *arg);
void imprime_vetor(long long int tamanho_vetor, int *vetor);

// Cabeçalho do buffer
//void inicia_buffer();
void insere_buffer(threadArgs item);
threadArgs retira_buffer(int id);

// Cabeçalho utilizado no arquivo de testes
void teste();
