# hashmap-benchmark

| [中文版](./README.md) | [English Version](./README.en.md) |

## 简介

几个国内外最优秀的 C++ Hashmap 库的基准测试，包括 `Google abseil-cpp` 的哈希表 `absl::flat_hash_map`，还包括 aka::flat_hash_map、tsl::robin_map、ktprime::emhash7、ankerl::unordered_dense::map 等著名的哈希表，`jstd::robin_hash_map` 均取得了不错的成绩。

### C++ Hashmap

对以下 `C++` 开源库做了基准测试：

* [我自己](https://github.com/shines77) 写的 `jstd::robin_hash_map`：

    [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)

* [boost::unordered](https://github.com/boostorg/unordered) library 的 `boost::unordered::unordered_flat_map`：

    [boost::unordered::unordered_flat_map](https://github.com/MikePopoloski/boost_unordered)

* [skarupke](https://github.com/skarupke) 的 `aka::flat_hash_map`：

    [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)

* 来自 Google [abseil-cpp](https://github.com/abseil) 的 `absl::flat_hash_map` ：

    [https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

* 国人 [ktprime](https://github.com/ktprime) 的 `emhash5`，`emhash7`，`emhash8`：

    [https://github.com/ktprime/emhash](https://github.com/ktprime/emhash)

* [Tessil](https://github.com/Tessil) 的 `tsl::robin_map`：

    [https://github.com/Tessil/robin-map](https://github.com/Tessil/robin-map)

* [martinus](https://github.com/martinus) 的 `robin_hood::unordered_flat_map`：

    [https://github.com/martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing)

* [martinus](https://github.com/martinus) 的 `ankerl::unordered_dense::map`：

    [https://github.com/martinus/unordered_dense](https://github.com/martinus/unordered_dense)

### 第三方库更新记录

2025-01-13 更新：

- 新增了 [boost::unordered](https://github.com/boostorg/unordered) library [v1.85.0](https://github.com/boostorg/unordered) 的 [boost::unordered::unordered_flat_map](https://github.com/MikePopoloski/boost_unordered) (非官方库) 。

    请使用下面的命令单独更新 [boost::unordered] 库：

    ```bash
    git submodule update --init --recursive ./3rd_party/boost_unordered
    git submodule update --remote --recursive ./3rd_party/boost_unordered
    ```

- 我自己的 `jstd::robin_hash_map`：更新至最新版，并添加新的 `jstd::cluster_flat_map` 。

- Google [abseil-cpp] 的 `absl::flat_hash_map`：更新至最新版，最新 tag：20240722.rc2

- [ktprime] 的 `emhash`：更新至最新版

- [Tessil] 的 `tsl::robin_map`：更新至最新，最新 tag：1.3.0

- [martinus] 的 `ankerl::unordered_dense::map`：更新至最新版，最新 tag：4.5.0

- [martinus] 的 `robin_hood::unordered_flat_map`：更新至最新版，最新 tag：3.11.5

- [skarupke] 的 `aka::flat_hash_map`：无更新。

### 镜像与修复

由于 `github` 访问比较困难，使用的是 `gitee` 的镜像。

`flat_hash_map` 国内的镜像修改版：[https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` 国内的镜像：[https://gitee.com/shines77/abseil-cpp](https://gitee.com/shines77/abseil-cpp)

本人对 `flat_hash_map` 做了一些修改，并修复了一些 `bug`，以便能过在本测试代码中正常使用。

修改记录：

* 修复了 `ska::detailv3::KeyOrValueHasher<K, V, Hasher>` 存在的问题, 以便能够在 `/bench/time_hash_map.cpp` 中使用 `ska::flat_hash_map<K,V>` 和 `ska::bytell_hash_map<K,V>`;
* 修复了 `ska::bytell_hash_map<K,V>` 几个会导致死循环的 `bug`;
* 添加了 `bytell_hash_map.hpp` 中缺失的头文件 "#include \<limits\>";
* 为 `ska::flat_hash_map<K,V>` 和 `ska::bytell_hash_map<K,V>` 添加了静态成员函数 `name()`;

## 编译和使用方法

### 1. 克隆 Git 仓库

```bash
git clone https://gitee.com/shines77/hashmap-benchmark.git
# 或者
git clone https://github.com/shines77/hashmap-benchmark.git
```

### 2. 初始化子模块

由于引用了以上 `6` 个开源库，`clone` 完以后必须先初始化 `submodule`：

```shell
git submodule init
git submodule update --init --recursive
```

### 3. 更新子模块

更新子模块到最新版，如果你需要的话。

```shell
# 更新全部 submodule
# 保证已经初始化
git submodule update --init --recursive
# 从远端更新
git submodule update --remote --recursive

# 单独更新某个 submodule
git submodule update --init --recursive ./3rd_party/flat_hash_map
git submodule update --remote --recursive ./3rd_party/flat_hash_map

git submodule update --init --recursive ./3rd_party/boost_unordered
git submodule update --remote --recursive ./3rd_party/boost_unordered
```

### 4. 配置与编译

先配置和编译 `Google` 的 `abseil-cpp` 库：

```shell
# 切换到 abseil-cpp 根目录
cd ./3rd_party/abseil-cpp

# 创建 build 目录
mkdir build
cd build

# 生成 CMake makefile
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=17 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..

# 编译和安装
make
make install
```

### 5. 编译 benchmark

如果你已经成功编译了 `abseil-cpp`，再配置和编译 `hashmap-benchmark`：

```bash
# 从 ./3rd_party/abseil-cpp/build 切换回 hashmap-benchmark 根目录
cd ../../../

# 创建 build 目录
mkdir build
cd build

# 生成 CMake makefile
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install ..

# 编译 benchmark
make
```

如果 CMake 有缓存文件，请使用根目录下的 `./cmake-clean.sh` 脚本清理 CMake 有缓存文件。

### 6. 运行 benchmark

请在 ./build 目录下执行下列命令：

```bash
# 跟 Google sprasehash 开源库类似的测试（新版，推荐）
./bin/time_hash_map_new

# 只测试 <std::string, std::string>，节约时间（新版，推荐）
./bin/time_hash_map_new string

# 跟 Google sprasehash 开源库类似的测试（旧版, 不推荐）
./bin/time_hash_map

# 只测试大对象 (Key为32, 256字节)，节约时间（旧版, 不推荐）
./bin/time_hash_map big

# 低、中、高、超高 - 基数测试（推荐）
./bin/cardinal_bench

# 小数据集测试
./bin/benchmark

# 中数据集测试
./bin/benchmark ./data/Maven.keys.txt
```

### 7. 其他脚本

以下脚本在根目录下：

```bash
# 清理 cmake 的缓存和编译结果（便于重新配置和编译 benchmark），
# 不会删除 abseil-cpp 的安装目录 ./install
./cmake-clean.sh

# 重新执行本仓库的 cmake 配置，生成 makefile，
# 建议先执行 cmake-clean.sh，默认会创建 build 目录
./cmake-config.sh
```
