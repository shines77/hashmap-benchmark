/**
 * @author XiongHui Guo
 * @date 2018/08/07 12:49:00
 */
package net.i77studio.hashmap.test;

import java.util.HashMap;

/**
 * @author XiongHui Guo
 * @date 2018/08/07 12:49:00
 */
public class HashMapBenchmark {
    private final int kIterations = 3000000;

    private final String[] header_fields = {
            // Request
            "Accept",
            "Accept-Charset",
            "Accept-Encoding",
            "Accept-Language",
            "Authorization",
            "Cache-Control",
            "Connection",
            "Cookie",
            "Content-Length",
            "Content-MD5",
            "Content-Type",
            "Date",
            "DNT",
            "From",
            "Front-End-Https",
            "Host",
            "If-Match",
            "If-Modified-Since",
            "If-None-Match",
            "If-Range",
            "If-Unmodified-Since",
            "Max-Forwards",
            "Pragma",
            "Proxy-Authorization",
            "Range",
            "Referer",
            "User-Agent",
            "Upgrade",
            "Via",
            "Warning",
            "X-ATT-DeviceId",
            "X-Content-Type-Options",
            "X-Forwarded-For",
            "X-Forwarded-Proto",
            "X-Powered-By",
            "X-Requested-With",
            "X-XSS-Protection",

            // Response
            "Access-Control-Allow-Origin",
            "Accept-Ranges",
            "Age",
            "Allow",
            // "Cache-Control",
            // "Connection",
            "Content-Encoding",
            "Content-Language",
            // "Content-Length",
            "Content-Disposition",
            // "Content-MD5",
            "Content-Range",
            // "Content-Type",
            "Date",
            "ETag",
            "Expires",
            "Last-Modified",
            "Link",
            "Location",
            "P3P",
            "Proxy-Authenticate",
            "Refresh",
            "Retry-After",
            "Server",
            "Set-Cookie",
            "Strict-Transport-Security",
            "Trailer",
            "Transfer-Encoding",
            "Vary",
            "Via",
            "WWW-Authenticate",
            // "X-Content-Type-Options",
            // "X-Powered-By",
            // "X-XSS-Protection",

            "Last"
    };

    void runFindTest() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_find_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            HashMap<String, String> map = new HashMap<String, String>();
            for (int i = 0; i < kHeaderFieldSize; ++i) {
                if (!map.containsKey(field_str[i])) {
                    map.put(field_str[i], index_str[i]);
                }
            }

            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            sw.start();
            for (int i = 0; i < kRepeatTimes; ++i) {
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    boolean hasKey = map.containsKey(field_str[j]);
                    if (hasKey) {
                        checksum++;
                    }
                }
            }
            sw.stop();

            totalTime += sw.getElapsedMillsecs();

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runInsertTest() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_insert_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            for (int i = 0; i < kRepeatTimes; ++i) {
                HashMap<String, String> map = new HashMap<String, String>();

                sw.start();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }
                sw.stop();

                checksum += map.size();
                totalTime += sw.getElapsedMillsecs();
            }

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runInsert1Test() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_insert1_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            HashMap<String, String> map = new HashMap<String, String>();

            for (int i = 0; i < kRepeatTimes; ++i) {
                sw.start();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }
                sw.stop();

                checksum += map.size();
                totalTime += sw.getElapsedMillsecs();

                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
            }

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runInsert2Test() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_insert2_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            sw.start();
            for (int i = 0; i < kRepeatTimes; ++i) {
                HashMap<String, String> map = new HashMap<String, String>();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }
                checksum += map.size();
            }
            sw.stop();

            totalTime += sw.getElapsedMillsecs();

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runEraseTest() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_erase_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            for (int i = 0; i < kRepeatTimes; ++i) {
                HashMap<String, String> map = new HashMap<String, String>();

                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }

                sw.start();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
                sw.stop();

                checksum += map.size();
                totalTime += sw.getElapsedMillsecs();
            }

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runErase1Test() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_erase1_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            HashMap<String, String> map = new HashMap<String, String>();

            for (int i = 0; i < kRepeatTimes; ++i) {
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }

                sw.start();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
                sw.stop();

                checksum += map.size();
                totalTime += sw.getElapsedMillsecs();
            }

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runErase2Test() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_erase2_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            sw.start();
            for (int i = 0; i < kRepeatTimes; ++i) {
                HashMap<String, String> map = new HashMap<String, String>();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }

                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
                checksum += map.size();
            }
            sw.stop();

            totalTime += sw.getElapsedMillsecs();

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runInsertEraseTest() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_insert_erase_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            HashMap<String, String> map = new HashMap<String, String>();

            sw.start();
            for (int i = 0; i < kRepeatTimes; ++i) {
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }
                checksum += map.size();

                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
                checksum += map.size();
            }
            sw.stop();

            totalTime += sw.getElapsedMillsecs();

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void runInsertErase2Test() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_insert_erase2_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.length;
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize) + 1;

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = this.header_fields[i];
            index_str[i] = String.format("%d", i).intern();
        }

        {
            long checksum = 0;
            double totalTime = 0.0;
            StopWatch sw = new StopWatch();

            sw.start();
            for (int i = 0; i < kRepeatTimes; ++i) {
                HashMap<String, String> map = new HashMap<String, String>();
                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.put(field_str[j], index_str[j]);
                }
                checksum += map.size();

                for (int j = 0; j < kHeaderFieldSize; ++j) {
                    map.remove(field_str[j]);
                }
                checksum += map.size();
            }
            sw.stop();

            totalTime += sw.getElapsedMillsecs();

            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("  %-28s  ", "HashMap<String, String>");
            System.out.printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime);
            System.out.printf("-------------------------------------------------------------------------\n");
            System.out.printf("\n");
        }
    }

    void run() {
        // Find
        this.runFindTest();

        // Insert
        this.runInsertTest();
        // this.runInsert1Test();
        // this.runInsert2Test();

        // Erase
        this.runEraseTest();
        // this.runErase1Test();
        // this.runErase2Test();

        // Insert and Erase
        this.runInsertEraseTest();
        // this.runInsertErase2Test();

        // System.out.printf("-------------------------------------------------------------------------\n");
    }
}
