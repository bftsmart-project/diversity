package bftsmart.diversity.benchmarks.general;

import java.util.*;
import java.io.*;

public abstract class Operation {

	public List<float> run(int iterations) {
		LinkedList<float> runtimes = new LinkedList<float>();
		for (int i = 0; i < iterations; i++) {
			long startTime = System.nanoTime();
			op();
			long estimatedTime = System.nanoTime() - startTime;
			runtimes.addLast(estimatedTime / 1000.0);
		}
		return runtimes;
	}

	public float run() {
		return this.run(1).get(0);
	}

	public abstract void op();
}
