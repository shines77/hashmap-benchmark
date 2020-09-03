package main

import (
	"fmt"
	"runtime"
)

var kIterations uint64 = 3000000

var header_fields = [64]string{
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
	"Last",
}

func runFindTest() {
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("  HashMap_find_benchmark()\n")
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("\n")

	var kHeaderFieldSize uint64 = uint64(len(header_fields))
	var kRepeatTimes uint64 = (kIterations / kHeaderFieldSize) + 1
	var i, j uint64

	var field_str []string
	var index_str []string
	for i = 0; i < kHeaderFieldSize; i++ {
		field_str = append(field_str, header_fields[i])
		index_str = append(index_str, fmt.Sprintf("%d", i))
	}

	{
		var hashmap map[string]string
		hashmap = make(map[string]string)
		for i = 0; i < kHeaderFieldSize; i++ {
			hashmap[field_str[i]] = index_str[i]
		}

		var checksum uint64 = 0
		var totalTime float64 = 0.0
		var sw StopWatch = *NewStopWatch()

		sw.Start()
		for i = 0; i < kRepeatTimes; i++ {
			for j = 0; j < kHeaderFieldSize; j++ {
				_, hasKey := hashmap[field_str[j]]
				if hasKey {
					checksum++
				}
			}
		}
		sw.Stop()

		totalTime = sw.GetElapsedMillisecs()

		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("  %-28s  ", "HashMap<String, String>")
		fmt.Printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime)
		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("\n")
	}
}

func runFind2Test() {
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("  HashMap_find2_benchmark()\n")
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("\n")

	var kHeaderFieldSize uint64 = uint64(len(header_fields))
	var kRepeatTimes uint64 = (kIterations / kHeaderFieldSize) + 1
	var i, j uint64

	var field_str []string
	var index_str []string
	for i = 0; i < kHeaderFieldSize; i++ {
		field_str = append(field_str, header_fields[i])
		index_str = append(index_str, fmt.Sprintf("%d", i))
	}

	{
		var hashmap map[string]string
		hashmap = make(map[string]string)
		for i = 0; i < kHeaderFieldSize; i++ {
			_, hasKey := hashmap[field_str[j]]
			if !hasKey {
				hashmap[field_str[i]] = index_str[i]
			}
		}

		var checksum uint64 = 0
		var totalTime float64 = 0.0
		var sw StopWatch = *NewStopWatch()

		sw.Start()
		for i = 0; i < kRepeatTimes; i++ {
			for j = 0; j < kHeaderFieldSize; j++ {
				_, hasKey := hashmap[field_str[j]]
				if hasKey {
					checksum++
				}
			}
		}
		sw.Stop()

		totalTime = sw.GetElapsedMillisecs()

		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("  %-28s  ", "HashMap<String, String>")
		fmt.Printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime)
		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("\n")
	}
}

func runInsertTest() {
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("  HashMap_insert_benchmark()\n")
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("\n")

	var kHeaderFieldSize uint64 = uint64(len(header_fields))
	var kRepeatTimes uint64 = (kIterations / kHeaderFieldSize) + 1
	var i, j uint64

	var field_str []string
	var index_str []string
	for i = 0; i < kHeaderFieldSize; i++ {
		field_str = append(field_str, header_fields[i])
		index_str = append(index_str, fmt.Sprintf("%d", i))
	}

	{
		var checksum uint64 = 0
		var totalTime float64 = 0.0
		var sw StopWatch = *NewStopWatch()

		for i = 0; i < kRepeatTimes; i++ {
			var hashmap map[string]string
			hashmap = make(map[string]string)

			sw.Start()
			for j = 0; j < kHeaderFieldSize; j++ {
				hashmap[field_str[j]] = index_str[j]
			}
			sw.Stop()

			checksum += uint64(len(hashmap))
			totalTime += sw.GetElapsedMillisecs()
		}

		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("  %-28s  ", "HashMap<String, String>")
		fmt.Printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime)
		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("\n")
	}
}

func runEraseTest() {
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("  HashMap_erase_benchmark()\n")
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("\n")

	var kHeaderFieldSize uint64 = uint64(len(header_fields))
	var kRepeatTimes uint64 = (kIterations / kHeaderFieldSize) + 1
	var i, j uint64

	var field_str []string
	var index_str []string
	for i = 0; i < kHeaderFieldSize; i++ {
		field_str = append(field_str, header_fields[i])
		index_str = append(index_str, fmt.Sprintf("%d", i))
	}

	{
		var checksum uint64 = 0
		var totalTime float64 = 0.0
		var sw StopWatch = *NewStopWatch()

		for i = 0; i < kRepeatTimes; i++ {
			var hashmap map[string]string
			hashmap = make(map[string]string)

			for j = 0; j < kHeaderFieldSize; j++ {
				hashmap[field_str[j]] = index_str[j]
			}
			checksum += uint64(len(hashmap))

			sw.Start()
			for j = 0; j < kHeaderFieldSize; j++ {
				delete(hashmap, field_str[j])
			}
			sw.Stop()

			checksum += uint64(len(hashmap))
			totalTime += sw.GetElapsedMillisecs()
		}

		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("  %-28s  ", "HashMap<String, String>")
		fmt.Printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime)
		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("\n")
	}
}

func runInsertEraseTest() {
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("  HashMap_insert_erase_benchmark()\n")
	// fmt.Printf("-------------------------------------------------------------------------\n")
	fmt.Printf("\n")

	var kHeaderFieldSize uint64 = uint64(len(header_fields))
	var kRepeatTimes uint64 = (kIterations / kHeaderFieldSize) + 1
	var i, j uint64

	var field_str []string
	var index_str []string
	for i = 0; i < kHeaderFieldSize; i++ {
		field_str = append(field_str, header_fields[i])
		index_str = append(index_str, fmt.Sprintf("%d", i))
	}

	{
		var checksum uint64 = 0
		var totalTime float64 = 0.0
		var sw StopWatch = *NewStopWatch()

		var hashmap map[string]string
		hashmap = make(map[string]string)

		sw.Start()
		for i = 0; i < kRepeatTimes; i++ {

			for j = 0; j < kHeaderFieldSize; j++ {
				hashmap[field_str[j]] = index_str[j]
			}
			checksum += uint64(len(hashmap))

			for j = 0; j < kHeaderFieldSize; j++ {
				delete(hashmap, field_str[j])
			}
			checksum += uint64(len(hashmap))
		}
		sw.Stop()

		totalTime += sw.GetElapsedMillisecs()

		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("  %-28s  ", "HashMap<String, String>")
		fmt.Printf("sum = %-10d  time: %8.3f ms\n", checksum, totalTime)
		fmt.Printf("-------------------------------------------------------------------------\n")
		fmt.Printf("\n")
	}
}

func hashmap_benchmark() {
	runFindTest()
	runInsertTest()
	runEraseTest()
	runInsertEraseTest()

	//time.Sleep(time.Duration(1) * time.Nanosecond)
}

func main() {
	runtime.GOMAXPROCS(1)

	fmt.Printf("\n")

	hashmap_benchmark()

	fmt.Printf("\n")
	fmt.Printf("https://github.com/shines77/hashmap_benchmark\n")
}
