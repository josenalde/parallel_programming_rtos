import java.util.Arrays;
import java.util.List;

public class TestStream {
    public static void main(String[] args) {
        // stream sequencial
        List<Integer> listOfNumbers = Arrays.asList(1, 2, 3, 4, 10, 20, 30, 40);
        listOfNumbers
            .stream()
            .forEach(number ->
                System.out.println(number + " " + Thread.currentThread().getName())
        );
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
}
}
