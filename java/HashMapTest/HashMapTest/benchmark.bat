@echo off
@rem See: https://www.cnblogs.com/mq0036/p/3885399.html
@rem java net.i77studio.hashmap.test.Program .\output\HashMapTest.jar

javac .\src\net\i77studio\hashmap\test\StopWatch.java .\src\net\i77studio\hashmap\test\HashMapBenchmark.java .\src\net\i77studio\hashmap\test\NoConstantPoolHashMapBenchmark.java .\src\net\i77studio\hashmap\test\Program.java ".\src\net\i77studio\hashmap\test\package-info.java"
jar cfm .\output\HashMapTest.jar .\output\HashMapTest.Manifest -C .\src\ .\net\i77studio\hashmap\test\StopWatch.class -C .\src\ .\net\i77studio\hashmap\test\HashMapBenchmark.class -C .\src\ .\net\i77studio\hashmap\test\NoConstantPoolHashMapBenchmark.class -C .\src\ .\net\i77studio\hashmap\test\Program.class .\data\header_fields.txt .\data\value_fields.txt
java -jar .\output\HashMapTest.jar
@rem java net.i77studio.hashmap.test.Program .\output\HashMapTest.jar
pause
