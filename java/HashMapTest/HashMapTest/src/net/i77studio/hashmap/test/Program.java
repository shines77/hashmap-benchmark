package net.i77studio.hashmap.test;

class Program {
    public static void main(String[] args) {
        /*
        System.out.println("args.length = " + args.length);
        if (args.length > 0) {
            System.out.println("args values:");
            for (int i = 0; i < args.length; i++)
                System.out.println(args[i]);
        }
        //*/

        System.out.println();

        HashMapBenchmark hashMapBenchmark = new HashMapBenchmark();
        hashMapBenchmark.run();

        System.out.printf("\n");
        System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        NoConstantPoolHashMapBenchmark noConstantPoolHashMapBenchmark = new NoConstantPoolHashMapBenchmark();
        noConstantPoolHashMapBenchmark.run();
    }
}
