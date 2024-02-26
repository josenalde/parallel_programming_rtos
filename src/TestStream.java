import java.util.Arrays;
import java.util.List;
import java.util.concurrent.ForkJoinPool;

public class TestStream {
    public static void main(String[] args) {
        
        System.setProperty("java.util.concurrent.ForkJoinPool.common.parallelism", "4");
        //System.out.println(System.getProperty("java.util.concurrent.ForkJoinPool.common.parallelism"));
        ForkJoinPool p = ForkJoinPool.commonPool();
        System.out.println(p.getPoolSize());
        // stream sequencial
        List<Integer> listOfNumbers = Arrays.asList(1, 2, 3, 4, 10, 20, 30, 40);
        listOfNumbers
            .stream()
            .forEach(number ->
                System.out.println(number + " " + Thread.currentThread().getName())
        );
        System.out.println(p.getPoolSize());
        System.out.println("Regiao Paralela: \n");
        // stream em paralelo
        listOfNumbers
            .parallelStream()
            .forEach(number ->
                System.out.println(number + " " + Thread.currentThread().getName())
        );
        
        // soma em paralelo e reduz para sum, adicionando 5 à soma total (0 valor inicial)
        int sum = listOfNumbers
            .parallelStream()
            .reduce(0, Integer::sum) + 5; // lambda: (subtotal, element) -> subtotal + element)
        System.out.println(sum);
        System.out.println(p.getPoolSize());
    }
}
      
