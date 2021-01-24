/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Implementando um programa concorrente para somar vetores */


class Vetor {
  private int tamanhoVetor;
  private int[] vetor;

  // Inicialização do vetor com valores pré-definidos
  public void inicializa(){
    for (int i=0; i<tamanhoVetor; i++){
      vetor[i] = i;
    }
  }

  // Construtor do vetor (vetor começa vazio)
  public Vetor(int tamanhoVetor){
    this.tamanhoVetor = tamanhoVetor;
    this.vetor = new int[tamanhoVetor];

    for (int i=0; i<tamanhoVetor; i++){
      vetor[i] = 0;
    }
  }

  // Impressão do vetor
  public void imprime(){
    for (int i=0; i<tamanhoVetor; i++){
      System.out.print(vetor[i] + " ");
    }
    System.out.print("\n");
  }

  public int tamanho(){
    return this.vetor.length;
  }

  public int[] vetor(){
    return this.vetor;
  }

}

class TarefaSoma extends Thread {

  // Identificador da threads
  private int id;

  Vetor A;
  Vetor B;
  Vetor C;
  int quantidadeThreads;

  public TarefaSoma(int id, Vetor A, Vetor B, Vetor C, int quantidadeThreads){
    this.id = id;
    this.A = A;
    this.B = B;
    this.C = C;
    this.quantidadeThreads = quantidadeThreads;
  }

  // Tarefa a ser executada pelas threads
  public void run(){
    // System.out.println("Thread " + this.id + " iniciou!");
    for (int i=this.id; i<A.tamanho(); i+=quantidadeThreads) {

       C.vetor()[i] = A.vetor()[i] + B.vetor()[i]; // Soma dos vetores
    }
    // System.out.println("Thread " + this.id + " terminou!");
  }

}

public class SomaVetores{
  // Quantidade de threads

  public static void main(String[] args){
    int quantidadeThreads;
    int tamanhoVetor;

    if (args.length > 1) {
      quantidadeThreads = Integer.parseInt(args[0]);
      tamanhoVetor =  Integer.parseInt(args[1]);
    }
    else {
      System.out.println("Informe a quantidade de threads e o tamanho do vetor");
      return;
    }

    Vetor A = new Vetor(tamanhoVetor);
    Vetor B = new Vetor(tamanhoVetor);
    Vetor C = new Vetor(tamanhoVetor);

    // Inicializa vetores A e B com valores pré-definidos
    A.inicializa();
    B.inicializa();

    System.out.println("Valor do vetor A:");
    A.imprime();

    System.out.println("Valor do vetor B:");
    B.imprime();

    Thread[] threads = new Thread[quantidadeThreads];

    //Cria as threads
    for (int i=0; i<threads.length; i++) {
       threads[i] = new TarefaSoma(i, A, B, C, quantidadeThreads);
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
