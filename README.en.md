# hashmap-benchmark

| [中文版](./README.md) | [English Version](./README.en.md) |

## English / 英文版

Several benchmark of `C++` high performance hashmap, even include Java, C#, golang hashmap benchmark.

## Compile and usage

Dependent on:

* `jstd_hashmap`: [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)
* `flat_hash_map`: [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)
* `Google` - `abseil-cpp`: [https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

Since `github` is difficult to access in China, so use a mirror repository of `gitee`.

`flat_hash_map` China mirror and modify: [https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` China mirror：[https://gitee.com/arjing/abseil-cpp](https://gitee.com/arjing/abseil-cpp)

I made some changes to `flat_hash_map` and fixed some `bugs` so that it can be used normally in this test code.

ChangeLog:

* Fixed ska::detailv3::KeyOrValueHasher<K, V, Hasher> for /bench/time_hash_map.cpp to use ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;
* Fixed some bugs about ska::bytell_hash_map<K,V> some dead cycle codes;
* Added missing header file "#include \<limits\>" in bytell_hash_map.hpp;
* Added static name() member method for ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;

### 1. Update submodule(s)

Since the above 3 open source libraries are referenced, the submodule must be updated after clone:

```shell
git submodule init
git submodule update --init --recursive
```

`git submodule update --init --recursive` is updated to the version of the `sub-module` specified by this repository. It is recommended to use this command.

If you want to update to the latest version of the sub-module, you can use the following command:

```shell
git submodule update --remote --recursive
```

### 2. Configure and Compile

First, configure and compile `abseil-cpp`:

```shell
cd ./3rd_party/abseil-cpp
mkdir build && cd build
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=14 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..
make
make install
```

Configure and compile `hashmap-benchmark`:

```bash
# switch to hashmap-benchmark root dir
cd ../../../
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install .
make
````

### 3. Run benchmark

```bash
# like Google sprasehash benchmark code
./bin/time_hash_map

# Small test case
./bin/benchmark

# Middle test case
./bin/benchmark ./data/Maven.keys.txt

# Small, Middle, Big, Huge - Cardinal benchmark
./bin/cardinal_bench
```
