# hashmap-benchmark

## Chinese / 中文版

几个 `C++` 高性能哈希表的 `benchmark`，还包括 `Java`, `C#`, `golang` 等哈希表的 `benchmark`.

## 编译和使用方法

依赖以下开源库:

* `jstd_hashmap`: [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)
* `flat_hash_map`: [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)
* `Google` 的 `abseil-cpp` 库：[https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

由于 `github` 访问比较困难，使用的是 `gitee` 的镜像。

`flat_hash_map` 国内的镜像修改版：[https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` 国内的镜像：[https://gitee.com/arjing/abseil-cpp](https://gitee.com/arjing/abseil-cpp)

本人对 `flat_hash_map` 做了一些修改，并修复了一些 `bug`，以便能过在本测试代码中正常使用。

修改记录：

* 修复了 `ska::detailv3::KeyOrValueHasher<K, V, Hasher>` 存在的问题, 以便能够在 `/bench/time_hash_map.cpp` 中使用 `ska::flat_hash_map<K,V>` 和 `ska::bytell_hash_map<K,V>`;
* 修复了 `ska::bytell_hash_map<K,V>` 几个会导致死循环的 `bug`;
* 添加了 `bytell_hash_map.hpp` 中缺失的头文件 "#include \<limits\>";
* 为 `ska::flat_hash_map<K,V>` 和 `ska::bytell_hash_map<K,V>` 添加了静态成员函数 `name()`;

### 1. 更新子模块

由于引用了以上 `3` 个开源库，`clone` 完以后必须先更新 `submodule`：

```shell
git submodule init
git submodule update --remote --recursive
```

### 2. 配置与编译

先配置和编译 `Google` 的 `abseil-cpp` 库：

```shell
cd ./3rd_party/abseil-cpp
mkdir build && cd build
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=14 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..
make
make install
```

配置和编译 `hashmap-benchmark`：

```shell
# 切换回 hashmap-benchmark 根目录
cd ../../../
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install .
make
````

### 3. 运行 benchmark

```shell
# 跟 Google sprasehash 开源库类似的测试
./bin/time_hash_map

# 小数据集测试
./bin/benchmark

# 中数据集测试
./bin/benchmark ./data/Maven.jar.updates.txt

# 低、中、高、超高 - 基数测试
./bin/cardinal_bench
```

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

```shell
# switch to hashmap-benchmark root dir
cd ../../../
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install .
make
````

### 3. Run benchmark

```shell
# like Google sprasehash benchmark code
./bin/time_hash_map

# Small test case
./bin/benchmark

# Middle test case
./bin/benchmark ./data/Maven.keys.txt

# Small, Middle, Big, Huge - Cardinal benchmark
./bin/cardinal_bench
```
