/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Implementando um programa concorrente para somar vetores */

class Vetor {
  static final int tamanhoVetor = 5;
  private int[] vetor = new int[tamanhoVetor];

  // Inicialização do vetor com valores pré-definidos
  public void inicializa(){
    for (int i=0; i<tamanhoVetor; i++){
      vetor[i] = i;
    }
  }

  // Construtor do vetor (vetor começa vazio)
  public Vetor(){
    for (int i=0; i<tamanhoVetor; i++){
      vetor[i] = 0;
    }
  }

  // Impressão do vetor
  public void imprime(){
    for (int i=0; i<tamanhoVetor; i++){
      System.out.println(vetor[i]);
    }
  }

  public int tamanho(){
    return this.vetor.length;
  }

  public int[] pega(){
    return this.vetor;
  }

}

class TarefaSoma extends Thread {

  // Identificador da threads
  private int id;

  Vetor A;
  Vetor B;
  Vetor C;

  // public TarefaSoma(int vetor[], int posicaoInicial, int posicaoFinal){
  //   this.vetor = vetor;
  //   this.posicaoInicial = posicaoInicial;
  //   this.posicaoFinal = posicaoFinal;
  // }

  public TarefaSoma(Vetor A, Vetor B, Vetor C){
    this.A = A;
    this.B = B;
    this.C = C;
  }

  // Tarefa a ser executada pelas threads
  public void run(){
    System.out.println("Thread " + this.id + " iniciou!");
    for (int i=0; i<A.tamanho(); i++) {

       C.pega()[i] = A.pega()[i] + B.pega()[i]; // Soma dos vetores
    }
    System.out.println("Thread " + this.id + " terminou!");
  }

}

class SomaVetores {
  // Quantidade de threads
  static final int quantidadeThreads = 1;

  public static void main(String[] args){
    Vetor A = new Vetor();
    Vetor B = new Vetor();
    Vetor C = new Vetor();

    // Inicializa vetores A e B com valores pré-definidos
    A.inicializa();
    B.inicializa();

    Thread[] threads = new Thread[quantidadeThreads];

    // Recurso encapsulado que será utilziado por todas as threads
    // (se não for tratado) pode ter condição de corrida

    //Cria as threads
    for (int i=0; i<threads.length; i++) {
       threads[i] = new TarefaSoma(A, B, C);
    }

    //Inicia as threads
    for (int i=0; i<threads.length; i++) {
       threads[i].start();
    }

    //Espera pelo termino de todas as threads
    for (int i=0; i<threads.length; i++) {
       try { threads[i].join(); } catch (InterruptedException e) { return; }
    }

    System.out.println("Valor do vetor C (resultante):");
    C.imprime();

  }
}
