class Buffer {
	private int[] buf;
	private boolean[] full;

	public Buffer(int size) {
		this.buf = new int[size];
		this.full = new boolean[size];
	}

	public synchronized void put(int index, int value) {
		try {
			while ( this.full[index] ) {
				System.out.println("Putting at index " + index + ": waiting");
				this.wait();
				System.out.println("Putting at index " + index + ": notified");

			}

			this.buf[index] = value;
			this.full[index] = true;

			System.out.println("Putting at index " + index + ": new value " + value); 

		} catch (InterruptedException e) {
			System.out.println(e);
		} finally {
			this.notifyAll();
		}
	}

	public synchronized int take(int index, Integer value) {
		int val = 0;
		try {
			while ( !this.full[index] ) {
				System.out.println("Taking from index " + index + ": waiting");
				this.wait();
				System.out.println("Taking from index " + index + ": notified");
			}

			value = this.buf[index];
			val = this.buf[index];
			this.full[index] = false;

			System.out.println("Taking from index " + index + ": returned value " + value); 

		} catch (InterruptedException e) {
			System.out.println(e);
		} finally {
			this.notifyAll();
		}
		return val;
	}

	public int size() {
		return this.buf.length;
	}

	public synchronized void print() {
		System.out.print("Buffer: "); 
		for (int i = 0; i < this.buf.length; i++) {
			if ( this.full[i] ) {
				System.out.print( this.buf[i] + " "); 
			} else {
				System.out.print("() "); 
			}
		}
		System.out.println(); 
	}
}
