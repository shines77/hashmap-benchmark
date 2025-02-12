# hashmap-benchmark

| [中文版](./README.md) | [English Version](./README.en.md) |

## Overview

Benchmarking several of the best C++ Hashmap libraries both domestically and internationally, including the hash table from Google abseil cpp `absl:flat_hash_map`， Also includes  Famous hash tables such as `aka::flat_cash_map`, `tsl::robin_map`, `ktprime::emhash7`, `ankerl::unordered_deny::map` and `boost::unordered_flat_map` etc, `jstd::robin_hash_map` has achieved good results.

Due to `boost:unrdered_flat_map` requires 'C++ 20' to compile, so the compilation of the repository's CMakeFiles. txt needs to be upgraded to 'C++ 20'. The built-in `Visual Studio 2015' project only requires 'C++ 11'.

### C++ Hashmap

The following `C++` open source libraries were benchmade:

* The C++ standard library comes with ` std:: unordered_map`, This is a benchmark standard used for comparison.

* (Myself) [shines77](https://github.com/shines77)'s `jstd::robin_hash_map`, `jstd::group15_flat_map`:

    [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)

* From Boost [boost::unordered](https://github.com/boostorg/unordered) library's `boost::unordered::unordered_flat_map`:

    [https://github.com/MikePopoloski/boost_unordered](https://github.com/MikePopoloski/boost_unordered)

* From Google [abseil-cpp](https://github.com/abseil)'s `absl::flat_hash_map`:

    [https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

* (Chinese) [ktprime](https://github.com/ktprime)'s `emhash5`, `emhash7`, `emhash8`, `emilib2o`:

    [https://github.com/ktprime/emhash](https://github.com/ktprime/emhash)

* [skarupke](https://github.com/skarupke)'s `aka::flat_hash_map`:

    [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)

* [Tessil](https://github.com/Tessil)'s `tsl::robin_map`:

    [https://github.com/Tessil/robin-map](https://github.com/Tessil/robin-map)

* [martinus](https://github.com/martinus)'s `robin_hood::unordered_flat_map`:

    [https://github.com/martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing)

* [martinus](https://github.com/martinus)'s `ankerl::unordered_dense::map`:

    [https://github.com/martinus/unordered_dense](https://github.com/martinus/unordered_dense)

### Third party library update

01/27/2025 update:

- Original path `/ 3rd-party ` is corrected to `/ 3rd_part`;

- Original path `/ 3rd-party/unodered_dons' is corrected to '/ 3rd_part/unordered_dense`;

- Fix the `/ 3rd_part ` path in `CMakeLists.txt` file；

- Fix the `/ 3rd_part ` path in `.gitmodules` file.

If you only clone this repository after this update, there is no need to do the following separate update steps.

```bash
git pull
git submodule sync
git pull
git submodule update --init --recursive
rm -rf ./3rd_party
```

01/13/2025 update:

If you only clone this repository after this update, there is no need to do the following separate update steps.

- Added new [boost::unordered](https://github.com/boostorg/unordered) library [v1.85.0](https://github.com/boostorg/unordered) 's [boost::unordered::unordered_flat_map](https://github.com/MikePopoloski/boost_unordered) (Unofficial Library) .

    Please use the following command to update the [boost::unordered] library separately:

    ```bash
    git submodule update --init --recursive ./3rd_party/boost_unordered
    git submodule update --remote --recursive ./3rd_party/boost_unordered
    ```

- Myself `jstd::robin_hash_map`: Update to the latest version，and added new hashmap `jstd::cluster_flat_map` .

- Google [abseil-cpp] 的 `absl::flat_hash_map`: Update to the latest version, lastest tag: 20240722.rc2

    Since I have changed the URL of the [abseil cpp] repository, if you have previously pulled /3rd_party/abseil-cpp, please use the following command to update the submodule:

    ```bash
    # First, delete the old /3rd_party/abseil-cpp directory
    cd ./3rd_party
    rm -rf ./abseil-cpp

    # Use the sync command to synchronize to a new URL, and reinitialize abseil-cpp
    git config -f .gitmodules submodule.3rd_party/abseil-cpp.branch master
    git submodule sync
    git submodule update --init --recursive 3rd_party/abseil-cpp
    ```

- [ktprime] 的 `emhash`: Update to the latest version

- [Tessil] 的 `tsl::robin_map`: Update to the latest version, lastest tag: 1.3.0

- [martinus] 的 `ankerl::unordered_dense::map`: Update to the latest version, lastest tag: 4.5.0

- [martinus] 的 `robin_hood::unordered_flat_map`: Update to the latest version, lastest tag: 3.11.5

- [skarupke] 的 `aka::flat_hash_map`: No update.

### Mirror and fixed

Because `github` is difficult to access in China, so I use a mirror repository of `gitee.com`.

`aka::flat_hash_map` China mirror and modified version: [https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` China mirror: [https://gitee.com/shines77/abseil-cpp](https://gitee.com/shines77/abseil-cpp)

I made some changes to `aka::flat_hash_map` and fixed some `bugs` so that it can be used normally in this test code.

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

```bash
git submodule init
git submodule update --init --recursive
```

### 3. Update submodule(s)

`git submodule update --init --recursive` is updated to the version of the `sub-module` specified by this repository. It is recommended to use this command.

If you want to update to the latest version of the sub-module, you can use the following command:

```bash
git submodule update --remote --recursive
```

```bash
# Update a submodule separately
git submodule update --init --recursive ./3rd_party/jstd_hashmap
git submodule update --init --recursive ./3rd_party/boost_unordered
````

### 4. Configure and Compile

First, configure and compile `abseil-cpp`:

```bash
cd ./3rd_party/abseil-cpp
mkdir build
cd build

cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=17 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..
make
make install
```

### 5. Compile benchmark

And then, Configure and compile `hashmap-benchmark`:

```bash
# Switch from ./3rd_party/abseil-cpp/build to hashmap-benchmark root dir
cd ../../../
mkdir build
cd build

cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install ..
make
```

If there is a cached file in CMake, please copy the `./cmake-clean.sh` script to your build directory, for example: `./build`。Run it again to clean up the cache files of CMake.

`./cmake-clean.sh` only cleans up benchmark files and does not affect the already compiled `abseil-cpp` library.

### 6. Update to lastest version

If you have compiled `abseil-cpp` and `hashmap-benchmark`, and wanna update to lastest version of this repository.

You can use following command:

```bash
cd build

git pull
git submodule update --init --recursive
# or
git submodule update --init --recursive ../3rd_party/jstd_hashmap

make
```

### 7. Run benchmark

Please go to your build directory, for example `./build`, execute the following command:

```bash
# like Google sprasehash benchmark code (New version, recommend)
./bin/time_hash_map_new

# Only benchmark <std::string, std::string>, save time. (New version, recommend)
./bin/time_hash_map_new string

# like Google sprasehash benchmark code (Old version, not recommend)
./bin/time_hash_map

# Only benchmark big object (Key is 32bytes, 256bytes), save time. (Old version, not recommend)
./bin/time_hash_map big

# Small, Middle, Big, Huge - Cardinal benchmark (recommend)
./bin/cardinal_bench

# Small test case
./bin/benchmark

# Middle test case
./bin/benchmark ./data/Maven.keys.txt
```

### 8. Other script

```bash
# Clean up the cache and compilation results of cmake (easy to reconfigure and compile)
./cmake-clean.sh

# Configure this repository, it is recommended to execute cmake-clean.sh first
./cmake-config.sh
```
