package bftsmart.diversity.benchmarks.general;

import java.util.*;
import java.io.*;

public class ParallelBenchRunner<T>
{

private BenchCase<Object>[] bcases;

public ParallelBenchRunner(BenchCase<Object>[] cases)
{
	this.bcases = cases;
}

public void run()
{
	Thread[] setups = new Thread[bcases.length];
	for (int i = 0; i < bcases.length; i++)
	{
		setups[i] = new Thread(() -> {
			System.out.println("Setting up...");
			System.out.println(i);
			bcases[i].setup();
			System.out.println("Finished.");
			System.out.println(i);
		});
		setups[i].start();
	}

	for (int i = 0; i < bcases.length; i++)
        {
		setups[i].join();
	}

        Thread[] warmups = new Thread[bcases.length];
        for (int i = 0; i < bcases.length; i++)
        {
                warmups[i] = new Thread(() -> {
                        System.out.println("Warming up...");
                        System.out.println(i);
                        bcases[i].warmup();
                        System.out.println("Finished.");
                        System.out.println(i);

                });
                warmups[i].start();
        }


        for (int i = 0; i < bcases.length; i++)
        {
                warmups[i].join();
        }


        Thread[] execs = new Thread[bcases.length];
        for (int i = 0; i < bcases.length; i++)
        {
                execs[i] = new Thread(() -> {
                        System.out.println("Executing...");
                        System.out.println(i);
                        bcases[i].execute();
                        System.out.println("Finished.");
                        System.out.println(i);

                });
                execs[i].start();
        }

        for (int i = 0; i < bcases.length; i++)
        {
                execs[i].join();
        }


        Thread[] setdowns = new Thread[bcases.length];
        for (int i = 0; i < bcases.length; i++)
        {
                setdowns[i] = new Thread(() -> {
                        System.out.println("Setting down...");
                        System.out.println(i);
                        bcases[i].setdown();
                        System.out.println("Finished.");
                        System.out.println(i);

                });
                setdowns[i].start();
        }

        for (int i = 0; i < bcases.length; i++)
        {
                setdowns[i].join();
        }
}


}
