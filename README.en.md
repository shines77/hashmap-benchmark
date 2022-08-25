# hashmap-benchmark

| [中文版](./README.md) | [English Version](./README.en.md) |

## English / 英文版

Several benchmark of `C++` high performance hashmap, even include Java, C#, golang hashmap benchmark.

### Overview

The following `C++` open source libraries were benchmade:

* (Myself) [shines77](#)'s `jstd_hashmap`:

    [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)

* [skarupke](#)'s `aka::flat_hash_map`:

    [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)

* [Google](#)'s `abseil-cpp`:

    [https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

* [ktprime](#)'s `emhash`:

    [https://github.com/ktprime/emhash.git](https://github.com/ktprime/emhash.git)

* [Tessil](#)'s `tsl::robin_map`:

    [https://github.com/Tessil/robin-map.git](https://github.com/Tessil/robin-map.git)

* [martinus](#)'s `robin_hood::unordered_flat_map`:

    [https://github.com/martinus/robin-hood-hashing.git](https://github.com/martinus/robin-hood-hashing.git)

* [martinus](#)'s `ankerl::unordered_dense::map`:

    [https://github.com/martinus/unordered_dense.git](https://github.com/martinus/unordered_dense.git)

### Mirror and fixed

Because `github` is difficult to access in China, so I use a mirror repository of `gitee.com`.

`flat_hash_map` China mirror and modified version: [https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` China mirror: [https://gitee.com/arjing/abseil-cpp](https://gitee.com/arjing/abseil-cpp)

I made some changes to `flat_hash_map` and fixed some `bugs` so that it can be used normally in this test code.

ChangeLog:

* Fixed ska::detailv3::KeyOrValueHasher<K, V, Hasher> for /bench/time_hash_map.cpp to use ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;
* Fixed some bugs about ska::bytell_hash_map<K,V> some dead cycle codes;
* Added missing header file "#include \<limits\>" in bytell_hash_map.hpp;
* Added static name() member method for ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;

## Compile and usage

### 1. Clone git repository

```bash
git clone https://github.com/shines77/hashmap-benchmark.git
or
git clone https://gitee.com/shines77/hashmap-benchmark.git
```

### 2. Init submodule(s)

Since the above 3 open source libraries are referenced, the submodule must be init and update after clone:

```shell
git submodule init
git submodule update --init --recursive
```

`git submodule update --init --recursive` is updated to the version of the `sub-module` specified by this repository. It is recommended to use this command.

If you want to update to the latest version of the sub-module, you can use the following command:

```shell
git submodule update --remote --recursive
```

### 3. Configure and Compile

First, configure and compile `abseil-cpp`:

```shell
cd ./3rd_party/abseil-cpp
mkdir build && cd build
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=17 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..
make
make install
```

And then, Configure and compile `hashmap-benchmark`:

```bash
# Switch from ./3rd_party/abseil-cpp/build to hashmap-benchmark root dir
cd ../../../
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install .
make
```

### 4. Update to lastest version

If you have compiled `abseil-cpp` and `hashmap-benchmark`, and wanna update to lastest version of this repository.

You can use following command:

```bash
git pull
git submodule update --init --recursive

make
```

### 5. Run benchmark

```bash
# like Google sprasehash benchmark code (New version)
./bin/time_hash_map_new

# Only benchmark <std::string, std::string>, save time.
./bin/time_hash_map_new string

# like Google sprasehash benchmark code (Old version)
./bin/time_hash_map

# Small, Middle, Big, Huge - Cardinal benchmark
./bin/cardinal_bench

# Small test case
./bin/benchmark

# Middle test case
./bin/benchmark ./data/Maven.keys.txt
```

### 6. Other script

```bash
# Clean up the cache and compilation results of cmake (easy to reconfigure and compile)
./cmake-clean.sh

# Configure this repository, it is recommended to execute cmake-clean.sh first
./cmake-config.sh
```
