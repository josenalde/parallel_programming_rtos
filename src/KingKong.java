class KingKong {
    public static synchronized void main(String[] args) {
        Thread t = new Thread() {
            public void run() {
                kong();
            }
        };
        t.start();
        System.out.println("king ");
    }

    public static synchronized void kong() {
        System.out.println("kong ");
    }
}

/* mais de paralelismo em Java: volatile, java.util.concurrent.atomic, java.util.concurrent.locks, executor interface,
 * BlockingQueues, ConcurrentMap, ConcurrentSkipListMap
 */