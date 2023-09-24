import java.util.*;
//import java.util.concurrent.*;

public class Count3Sync implements Runnable {
    private static final int ARRAY_LENGTH = 1000000; //para validação usar, por exemplo, 16
    private static final int MAX_THREADS = 8;
    private static final int MAX_RANGE=100; //para validação usar, por exemplo, 10
    private static final Random random = new Random();
    private static int count = 0;
    private static Object lock = new Object();
    private static int[] array;
    private static Thread[] t;

    private int startIndex;
    private int elements;
    private int myCount = 0;

    public Count3Sync(int start, int elem) {
        startIndex = start;
        elements = elem;
    }

    public static void main(String[] args) {
        array = new int[ARRAY_LENGTH];
        t = new Thread[MAX_THREADS];
        for (int i = 0; i < array.length; i++) {
            array[i] = random.nextInt(MAX_RANGE);
            //System.out.print(array[i] + ",");
        }
        Count3Sync[] counters = new Count3Sync[MAX_THREADS];
        int lengthPerThread = ARRAY_LENGTH / MAX_THREADS;

        for (int i = 0; i < counters.length; i++) {
            counters[i] = new Count3Sync(i*lengthPerThread, lengthPerThread);
        }
        //rodar threads
        for (int i = 0; i < counters.length; i++) {
            t[i] = new Thread(counters[i]);
            t[i].start();
        }
        //reunir threads após cada finalização
        for (int i = 0; i < counters.length; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                //faz nada
            }
        }

        System.out.println("Contagem: " + count);
    }

    public void run() {
        for (int i = 0; i < elements; i++) {
            if (array[startIndex + i] == 3) {
                myCount++;
            }
        }
        synchronized(lock) {
            count += myCount;
        }
    }
}