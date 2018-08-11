
//#define NO_STRING_INTERNING

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using jstd.Collections;

#if NO_STRING_INTERNING
[assembly: CompilationRelaxationsAttribute(CompilationRelaxations.NoStringInterning)]
#endif
namespace DictionaryTest
{
    class Program
    {
#if DEBUG
        static readonly int kIterations = 10000;
#else
        static readonly int kIterations = 3000000;
#endif

        static String[] header_fields = {
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
            //"Cache-Control",
            //"Connection",
            "Content-Encoding",
            "Content-Language",
            //"Content-Length",
            "Content-Disposition",
            //"Content-MD5",
            "Content-Range",
            //"Content-Type",
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
            //"X-Content-Type-Options",
            //"X-Powered-By",
            //"X-XSS-Protection",

            "Last"
        };

        static void hashtable_find_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
            int kRepeatTimes = (kIterations / kHeaderFieldSize);

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

#if DEBUG
            if (String.IsInterned(field_str[0]) != null)
                Console.WriteLine("Has interned. " + String.IsInterned(field_str[0]));
            else
                Console.WriteLine("No interned. [Null]");

            if (String.IsInterned(index_str[0]) != null)
                Console.WriteLine("Has interned. " + String.IsInterned(index_str[0]));
            else
                Console.WriteLine("No interned. [Null]");
#endif

            {
                Dictionary<String, String> dict = new Dictionary<String, String>();
                for (int i = 0; i < kHeaderFieldSize; ++i)
                {
                    if (!dict.ContainsKey(field_str[i]))
                        dict.Add(field_str[i], index_str[i]);
                }

                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                sw.Restart();
                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        bool hasKey = dict.ContainsKey(field_str[j]);
                        if (hasKey)
                        {
                            checksum++;
                        }
                    }
                }
                sw.Stop();

                totalTime += sw.Elapsed.TotalMilliseconds;

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_find_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_find_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_find_benchmark_impl();
        }

        static void hashtable_insert_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 100;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                sw.Restart();
                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    Dictionary<String, String> dict = new Dictionary<String, String>();
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    checksum += dict.Count();                    
                }
                sw.Stop();

                totalTime += sw.Elapsed.TotalMilliseconds;

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_insert_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_insert_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_insert_benchmark_impl();
        }

        static void hashtable_insert1_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 100;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    Dictionary<String, String> dict = new Dictionary<String, String>();

                    sw.Restart();
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    sw.Stop();

                    checksum += dict.Count();
                    totalTime += sw.Elapsed.TotalMilliseconds;
                }

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_insert1_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_insert1_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_insert1_benchmark_impl();
        }

        static void hashtable_insert2_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 100;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                Dictionary<String, String> dict = new Dictionary<String, String>();

                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    sw.Restart();
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    sw.Stop();

                    checksum += dict.Count();
                    totalTime += sw.Elapsed.TotalMilliseconds;

                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        dict.Remove(field_str[j]);
                    }
                }

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_insert2_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_insert2_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_insert2_benchmark_impl();
        }

        static void hashtable_erase_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 100;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    Dictionary<String, String> dict = new Dictionary<String, String>();

                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    checksum += dict.Count();

                    sw.Restart();
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        dict.Remove(field_str[j]);
                    }
                    sw.Stop();

                    checksum += dict.Count();
                    totalTime += sw.Elapsed.TotalMilliseconds;
                }

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_erase_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_erase_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_erase_benchmark_impl();
        }

        static void hashtable_insert_erase_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 100;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                Dictionary<String, String> dict = new Dictionary<String, String>();

                sw.Restart();
                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    checksum += dict.Count();

                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        dict.Remove(field_str[j]);
                    }
                    checksum += dict.Count();                    
                }
                sw.Stop();

                totalTime += sw.Elapsed.TotalMilliseconds;

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "Dictionary<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_insert_erase_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_insert_erase_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_insert_erase_benchmark_impl();
        }

        static void hashtable_rehash_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 200;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                int buckets = 128;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                DictionaryEx<String, String> dict = new DictionaryEx<String, String>();
                for (int i = 0; i < kHeaderFieldSize; ++i)
                {
                    if (!dict.ContainsKey(field_str[i]))
                        dict.Add(field_str[i], index_str[i]);
                }

                sw.Restart();
                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    checksum += dict.Count();

                    buckets = 128;
                    dict.Resize(buckets);
                    checksum += dict.BucketCount;

                    for (int j = 0; j < 7; ++j)
                    {
                        buckets *= 2;
                        dict.Resize(buckets);
                        checksum += dict.BucketCount;
                    }
                }
                sw.Stop();

                totalTime += sw.Elapsed.TotalMilliseconds;

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "DictionaryEx<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_rehash_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_rehash_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_rehash_benchmark_impl();
        }

        static void hashtable_rehash2_benchmark_impl()
        {
            int kHeaderFieldSize = header_fields.Length;
#if DEBUG
            int kRepeatTimes = 200;
#else
            int kRepeatTimes = (kIterations / kHeaderFieldSize / 2);
#endif

            String[] field_str = new String[kHeaderFieldSize];
            String[] index_str = new String[kHeaderFieldSize];
            for (int i = 0; i < kHeaderFieldSize; ++i)
            {
#if NO_STRING_INTERNING
                field_str[i] = String.Format("{0}", header_fields[i].ToCharArray());
                index_str[i] = String.Format("{0}", i);
#else
                field_str[i] = String.Intern(header_fields[i]);
                index_str[i] = String.Intern(String.Format("{0}", i));
#endif
            }

            {
                long checksum = 0;
                int buckets = 128;
                double totalTime = 0.0;
                Stopwatch sw = new Stopwatch();

                sw.Restart();
                for (int i = 0; i < kRepeatTimes; ++i)
                {
                    DictionaryEx<String, String> dict = new DictionaryEx<String, String>();
                    for (int j = 0; j < kHeaderFieldSize; ++j)
                    {
                        if (!dict.ContainsKey(field_str[j]))
                            dict.Add(field_str[j], index_str[j]);
                    }
                    checksum += dict.Count();
                    checksum += dict.BucketCount;

                    buckets = 128;
                    dict.Resize(buckets);
                    checksum += dict.BucketCount;

                    for (int j = 0; j < 7; ++j)
                    {
                        buckets *= 2;
                        dict.Resize(buckets);
                        checksum += dict.BucketCount;
                    }
                }
                sw.Stop();

                totalTime += sw.Elapsed.TotalMilliseconds;

                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("  {0,-28}  ", "DictionaryEx<String, String>");
                Console.Write("sum = {0,-10:g}  time: {1,8:f} ms\n", checksum, totalTime);
                //Console.Write("-------------------------------------------------------------------------\n");
                Console.Write("\n");
            }
        }

        static void dictionary_rehash2_benchmark()
        {
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("  dictionary_rehash2_benchmark()\n");
            Console.Write("-------------------------------------------------------------------------\n");
            Console.Write("\n");

            hashtable_rehash2_benchmark_impl();
        }

        static void dictionary_benchmark()
        {
            dictionary_find_benchmark();

            dictionary_insert_benchmark();
            dictionary_insert1_benchmark();
            dictionary_insert2_benchmark();

            dictionary_erase_benchmark();
            //dictionary_erase2_benchmark();

            dictionary_insert_erase_benchmark();

            dictionary_rehash_benchmark();
            dictionary_rehash2_benchmark();
        }

        public static void Main(string[] args)
        {
            Console.WriteLine("");

            dictionary_benchmark();

            //Console.Write("\n");
            Console.Write("Press any key ...");
            Console.ReadKey();
        }
    }
}
