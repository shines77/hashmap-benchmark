package net.i77studio.hashmap.test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class NoConstantPoolHashMapBenchmark {
    private final int kIterations = 3000000;

    @SuppressWarnings({ "unchecked", "rawtypes" })
    private List<String> header_fields = new ArrayList();

    @SuppressWarnings({ "unchecked", "rawtypes" })
    private List<String> value_fields = new ArrayList();

    private final String[] const_header_fields = {
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

    public static int readFileByLines(String filename, List<String> list) {
        File file = new File(filename);
        BufferedReader reader = null;
        list.clear();
        int line = 0;
        try {
            reader = new BufferedReader(new FileReader(file));
            String lineString = null;
            while ((lineString = reader.readLine()) != null) {
                // System.out.println("line # " + (line + 1) + ": [" + lineString + "]");
                list.add(lineString);
                line++;
            }
            reader.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e1) {
                }
            }
        }
        return line;
    }

    void runFindTest() {
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("  HashMap_find_benchmark()\n");
        // System.out.printf("-------------------------------------------------------------------------\n");
        System.out.printf("\n");

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
        }

        {
            HashMap<String, String> map = new HashMap<String, String>();
            for (int i = 0; i < kHeaderFieldSize; ++i) {
                if (!map.containsKey(field_str[i].toString())) {
                    map.put(field_str[i].toString(), index_str[i].toString());
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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

        int kHeaderFieldSize = this.header_fields.size();
        int kRepeatTimes = (this.kIterations / kHeaderFieldSize);

        String[] field_str = new String[kHeaderFieldSize];
        String[] index_str = new String[kHeaderFieldSize];
        for (int i = 0; i < kHeaderFieldSize; ++i) {
            field_str[i] = header_fields.get(i);
            index_str[i] = value_fields.get(i);
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
        int header_lines = readFileByLines(".\\data\\header_fields.txt", this.header_fields);
        if (header_lines <= 0) {
            System.out.printf("-------------------------------------------------------------------------\n\n");
            System.out.println("Error: header_fields.txt have no data.");
            return;
        } else {
            System.out.printf("Have read %d lines data in [header_fields.txt].\n", header_lines);
        }
        int value_lines = readFileByLines(".\\data\\value_fields.txt", this.value_fields);
        if (value_lines <= 0) {
            System.out.printf("-------------------------------------------------------------------------\n\n");
            System.out.println("Error: value_fields.txt have no data.");
            return;
        } else {
            System.out.printf("Have read %d lines data in [value_fields.txt].\n", value_lines);
        }

        System.out.printf("-------------------------------------------------------------------------\n\n");

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
