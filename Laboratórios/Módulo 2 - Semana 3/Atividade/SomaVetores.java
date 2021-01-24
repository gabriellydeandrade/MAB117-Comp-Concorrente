/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 */
/* Implementando um programa concorrente para somar vetores */

class Vetor {
  static final int tamanhoVetor = 5;
  private int[] vetor = new int[tamanhoVetor];

  // Inicialização do vetor
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

}

// class Soma implements Thread {
//
// }

class SomaVetores {
  // Quantidade de threads
  // static final int quantidadeThreads = 2;

  public static void main(String[] args){
    Vetor A = new Vetor();
    Vetor B = new Vetor();
    Vetor C = new Vetor();

    // Inicializa vetores A e B com valores pré-definidos
    A.inicializa();
    B.inicializa();

    A.imprime();

  //   Thread[] threads = new thread[quantidadeThreads];

    // Recurso encapsulado que será utilziado por todas as threads
    // (se não for tratado) pode ter condição de corrida

  }
}
