import java.util.Arrays;
import java.util.List;
//import java.util.concurrent.ForkJoinPool;

//import org.junit.Test;

//import static org.junit.Assert.assertEquals;

public class TestStream {
    public static void main(String[] args) {
        // stream sequencial
        List<Integer> listOfNumbers = Arrays.asList(1, 2, 3, 4);
        listOfNumbers.stream().forEach(number ->
        System.out.println(number + " " + Thread.currentThread().getName())
        );
        System.out.println("\n");
        // stream em paralelo
        listOfNumbers.parallelStream().forEach(number ->
        System.out.println(number + " " + Thread.currentThread().getName())
        );

        // soma em paralelo e reduz para sum, adicionando 5 à soma total
        //int sum = listOfNumbers.parallelStream().reduce(0, Integer::sum) + 5;
        //System.out.println(sum);
        //assertEquals(sum).isNotEqualTo(15);         
}
}
