import java.util.Arrays;
import java.util.List;
import java.util.Random;
import java.util.concurrent.ForkJoinPool;

public class TestStreamSum {
    public static void main(String[] args) {

        ForkJoinPool p = ForkJoinPool.commonPool();

        final int TAM = 1000; // tamanho da lista
        Integer[] numbers = new Integer[TAM];
        Random random = new Random();

        for (int i = 0; i < TAM; i++) {
            numbers[i] = random.nextInt(20);
        }

        List<Integer> listOfNumbers = Arrays.asList(numbers);
        long tempoInicial = System.currentTimeMillis();
        listOfNumbers
                .stream()
                .forEach(number -> System.out.println(number + " " + Thread.currentThread().getName()));

        long dif = System.currentTimeMillis() - tempoInicial;
        System.out.println("Tempo serial IO: " + dif + " ms");
        // --------------------------------------------------------------------
        // Soma serial
        int s = 0;
        long tib = System.currentTimeMillis();
        System.out.println(tib);
        for (int i : listOfNumbers) {
            s += i;
        }
        s += 5;
        System.out.println(System.currentTimeMillis());
        long difb = System.currentTimeMillis() - tib;
        System.out.println("Tempo serial Soma: " + difb + " ms " + "com thread " + Thread.currentThread().getName());
        System.out.println("soma serial: " + s);
        System.out.println("----------------------------------------");
        System.out.println("Entrando na Regiao Paralela (fork): ");
        tempoInicial = System.currentTimeMillis();
        // stream em paralelo
        listOfNumbers
                .parallelStream()
                .forEach(number -> System.out.println(number + " " + Thread.currentThread().getName()));
        dif = System.currentTimeMillis() - tempoInicial;
        System.out.println("Tempo paralelo IO: " + dif + " ms com " + (p.getPoolSize() + 1) + " threads");

        // soma em paralelo e reduz para sum, adicionando 5 à soma total (0 valor
        // inicial)
        tempoInicial = System.currentTimeMillis();
        int sum = listOfNumbers
                .parallelStream()
                .reduce(0, Integer::sum) + 5; // lambda: (subtotal, element) -> subtotal + element)
        dif = System.currentTimeMillis() - tempoInicial;
        System.out.println("Tempo paralelo Soma: " + dif + " ms com " + (p.getPoolSize() + 1) + " threads");
        System.out.println("Soma paralela: " + sum);

    }
}
