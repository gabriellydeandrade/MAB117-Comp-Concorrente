class Leitor extends Thread {
	private static final int NUM_VEZES = 10;

	private Buffer buf;
	private int id;

	public Leitor(Buffer buf, int id) {
		this.buf = buf;
		this.id = id;
	}

	public void run() {
		int c = NUM_VEZES;
		while ( c > 0 ) {
			for (int i = 0; i < this.buf.size(); i++) {
				Integer integer = new Integer(0);
				buf.take(i, integer);
				System.out.println("Leitor " + this.id + ": lendo index " + i + "valor " + integer);
			}
		}
	}
}

class Escritor extends Thread {
	private static final int NUM_VEZES = 10;

	private Buffer buf;
	private int id;

	public Escritor(Buffer buf, int id) {
		this.buf = buf;
		this.id = id;
	}

	public void run() {
		int c = NUM_VEZES;
		while ( c > 0 ) {
			for (int i = 0; i < this.buf.size(); i++) {
				buf.put(i, this.id);
				System.out.println("Escritor " + this.id + ": escrevendo no index " + i + " valor " + this.id);
			}
		}
	}
}

class Main {
	private static final int L = 2;
	private static final int E = 2;
	private static final int TAM = 10;

	public static void main(String args[]) {
		int i;
		Buffer buf = new Buffer(TAM);
		Leitor[] l = new Leitor[L];       // Threads leitores
		Escritor[] e = new Escritor[E];   // Threads escritores

		for (i=0; i<L; i++) {
			l[i] = new Leitor(buf, i+1);
			l[i].start(); 
		}
		for (i=0; i<E; i++) {
			e[i] = new Escritor(buf, i+1);
			e[i].start(); 
		}
	}
}
