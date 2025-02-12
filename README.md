# hashmap-benchmark

| [中文版](./README.md) | [English Version](./README.en.md) |

## 简介

国内外最优秀的几个 C++ Hashmap 库的基准测试，包括 `Boost` 的 `boost::unordered_flat_map`，`abseil-cpp` 库的 `absl::flat_hash_map`，以及 `aka::flat_hash_map`、`tsl::robin_map`、`ktprime::emhash7`、`ankerl::unordered_dense::map` 等知名的哈希表，`jstd::robin_hash_map` 在 Linux 下的 stl 默认的整型哈希函数下有不错的表现，`jstd::group15_flat_map` 跟 `boost::unordered_flat_map` 性能非常接近，且 insert 性能更优。

由于 `boost::unordered_flat_map` 需要 `C++ 20` 才能编译，所以本仓库的 CMake `CMakeFiles.txt` 编译要求升级为 `C++ 20`，自带的 `Visual Studio 2015` 工程只需要 `C++ 11` 即可。

### C++ Hashmap

对以下 `C++` 开源库做了基准测试：

* C++ 标准库 STL 自带的 `std::unordered_map`，这是一个用于比较的基准标准。

* [我自己](https://github.com/shines77) 写的 `jstd::robin_hash_map`，`jstd::group15_flat_map`：

    [https://gitee.com/shines77/jstd_hashmap](https://gitee.com/shines77/jstd_hashmap)

* 来自 Boost library [boost::unordered](https://github.com/boostorg/unordered) 的 `boost::unordered::unordered_flat_map`：

    [https://github.com/MikePopoloski/boost_unordered](https://github.com/MikePopoloski/boost_unordered)

* 来自 Google [abseil-cpp](https://github.com/abseil) 的 `absl::flat_hash_map` ：

    [https://github.com/abseil/abseil-cpp](https://github.com/abseil/abseil-cpp)

* 国人 [ktprime](https://github.com/ktprime) 的 `emhash5`，`emhash7`，`emhash8`，`emilib2o`：

    [https://github.com/ktprime/emhash](https://github.com/ktprime/emhash)

* [skarupke](https://github.com/skarupke) 的 `aka::flat_hash_map`：

    [https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)

* [Tessil](https://github.com/Tessil) 的 `tsl::robin_map`：

    [https://github.com/Tessil/robin-map](https://github.com/Tessil/robin-map)

* [martinus](https://github.com/martinus) 的 `robin_hood::unordered_flat_map`：

    [https://github.com/martinus/robin-hood-hashing](https://github.com/martinus/robin-hood-hashing)

* [martinus](https://github.com/martinus) 的 `ankerl::unordered_dense::map`：

    [https://github.com/martinus/unordered_dense](https://github.com/martinus/unordered_dense)

### 关于 jackson_bench

2024年12月21日，[ktprime](https://github.com/ktprime) 在 [issues](https://github.com/shines77/hashmap-benchmark/issues/1) 中给我推荐了 2024 年最新的一个哈希表性能测试，原文：[An Extensive Benchmark of C and C++ Hash Tables](https://jacksonallan.github.io/c_cpp_hash_tables_benchmark/)，后来我给翻译了一下，译文在这：[2024年 Jackson Allan 关于C和C++哈希表的广泛基准测试(翻译)](https://gitee.com/shines77/my_docs/blob/master/tech/hash/hash-table/%E5%85%B3%E4%BA%8EC%E5%92%8CC++%E5%93%88%E5%B8%8C%E8%A1%A8%E7%9A%84%E5%B9%BF%E6%B3%9B%E5%9F%BA%E5%87%86%E6%B5%8B%E8%AF%95(%E7%BF%BB%E8%AF%91).md)，这也重新泛起了我对哈希表的兴趣。

看了一下测试结果，`boost::unordered_flat_map` 的性能总体上来说还是比较好的，当我在 boost 官方的介绍里看懂了其原理后，有了一个想法。我的思路是把 group 的大小从 15 字节改为 16，同时把 owerflow bit 从 8 bits 改为 16 bits。这就是 jstd::group16_flat_map，从实践来看，虽然 overflow bit 变大了，能减少查找时搜索的次数，但是 hash 值也从 8 bits 减少为了 7 bits，因为剩下的一个 bit 用来做 overflow 了，hash 冲突的可能增大了，导致 key 的比较次数比 boost 的可能要更多，并且由于 overflow bit 变复杂了，同时 SIMD 指令出来也比 boost 复杂一点，导致总体性能比 boost 的略差一点。后来，我实验性的写了跟 boost 原理一模一样的 jstd::group15_flat_map，性能虽然接近了 boost ，但由于编译器的优化问题，除了插入性能比 boost 稍好一点以外，其他都比 boost 的略差一些，但差得不多。

我采用了上面这篇文章中所提到的测试代码，源码在这里：[https://github.com/JacksonAllan/c_cpp_hash_tables_benchmark](https://github.com/JacksonAllan/c_cpp_hash_tables_benchmark) 。仔细的研究了一下，发现还是有不少不足，我基于他的代码改写了一下，这就是本仓库中的 `/bench/jackson_bench`，改动如下：

- 去掉了 C 哈希表的测试，虽然你还是可以加上去的，专注于 C++ 的哈希表。

- 不再使用统一的 max_load_factor，使用各个 hashmap 默认的 mlf，理由是：虽然统一 mlf 更公平，但是每种哈希表有自己的特点，在不同的 mlf 下，性能会有差异。何况在实际的使用中，你不可能把自己要用的哈希表设置在一个性能不太好的 mlf 吧。

- 每个 blueprint（蓝图）的测试总数据量不再是固定的 KEY_COUNT 个（原代码中这个值是 200,000，数据量太小），而是按总的内存使用量 BENCHMARK_TOTAL_BYTES (64 * 1024 * 1024)，64M Bytes 除以（每个 blueprint 的 element 大小 + 一个测试的 key 值的大小，即 (sizeof(value_type) + sizeof(key_type)) ）来决定总的测试数据量，也就是总的内存占用保持在大约 64M Bytes 左右；

- 每次测量用时的间隔由 500 个元素改为了 2000 个元素，时间粒度更大，误差更小，并且去掉一些不必要的 if 判断，具体可以看代码。每次 insert, find, erase 的元素的个数还是保持为 1000 个元素。

- 更改每个 benchmard_id 的测试顺序，让 RUN_COUNT 循环时对同一个 Hashmap 的同一个 BluePrint 的同一个 benchmark_id 重复测试 N 遍，这样更合理一点。重复测试是为了降低测试代码受缓存的影响而导致测试结果的摆动，多次重复测试就是为了消除这些误差。而不是对同一个 Hashmap 的不同的 benchmark_id 循环 N 遍。RUN_COUNT 减少为 5 次，实测感觉 7 次可能更准确一点，但 5 次也基本够了。

- 原代码中的 insert() 接口定义如下：

    ```cpp
    static void insert(table_type & table, const blueprint::key_type & key)
    {
        table[key] = typename blueprint::value_type();
    }
    ```

    其实这测的不是 insert()，而是测 try_emplace() 的性能，我给统一更改为了 emplace(key, value_type()) 接口，将来还会增加别的接口测试，具体可以参考 `/bench/time_hash_map_new` 的测试。

- iteration 的测试，只访问 key 值，不访问 value 值，cache 污染更少。

- 每次开始测试一个 benchmark routine 时，重置随机数产生器的随机数种子 (seed)，这样每个 benchmark routine 所产生的随机数序列都是一样的，虽然这个设定对结果的影响很小，但理论上更公平一点。

- 一开始我采用了每个 benchmark routine 开测前都刷新缓存，并 sleep(1000ms)，但这样整个测试过程太慢了。后来改为，只在循环每个 benchmark routine RUN_COUNT 次前，刷新缓存和 sleep()，这样整个测试耗时会减少很多，虽然好像前者的测试结果更稳定一点。

- 平均用时计算方法更改为，去掉一个最大和最小值，然后求平均值。原代码的方法是去掉两个最大的值，以及去掉两个最小的值，然后求平均，并且 RUN_COUNT 次数是 14 次，我减少到了 5 次。

- 原代码中的各个 benchmark routine 有些是混杂在一起的，虽然这样可以节省测试时间，但也许对测试结果有影响，我都给拆开来了，各个测试 routime 是独立的。

- 测试的过程中，中途会显示已完成测试的部分结果，更早的能看到测试结果，无需漫长的等待。

### 第三方库更新记录

每次更新如果改变了 `submodules` 的路径或配置，可以尝试使用下列的命令：

```bash
# 第一次 git pull 是为了拉取最新的 .gitmodules
git pull
# 更新/同步 .gitmodules 文件中的内容
git submodule sync
# 第二次 git pull 是为了确保所有内容都更新了, 不一定是必须的
git pull
# 更新 ./3rd_party 目录的 submodules
git submodule update --init --recursive

# 如果某个 submodule 不能正常更新, 先删除该 submodule 的目录，再更新该 submodule，例如：
rm -rf ./3rd_party/abseil-cpp
git submodule update --init --recursive 3rd_party/abseil-cpp

# 如果有必要，手动删除旧的 ./3rd_part 或 ./3rd_party 目录
rm -rf ./3rd_part
```

2025-02-12 更新：

- `./3rd_part` 路径恢复为 `./3rd_party`，包括 `CMakeLists.txt`、`.gitmodules` 和 MSVC 工程文件中的路径。

- jstd::group15_flat_map，jstd::group16_flat_map 已经基本稳定。

- 新增了 `/bench/jackson_bench` 测试，具体可参考上文的介绍。

- `/bench/time_hash_map_new` 也已做了部分修改，计划改成跟 `jackson_bench` 这样的结构，扩展性更好一点。

2025-01-27 更新：

- `./3rd_party` 路径更改为 `./3rd_part`，包括 `CMakeLists.txt`、`.gitmodules` 和 MSVC 工程文件中的路径。

- `./3rd_party/unordered_dens` 路径更改为 `./3rd_part/unordered_dense`。

- `jstd_hashmap`：v 1.0.1 版本。

2025-01-13 更新：

- 新增了 [boost::unordered](https://github.com/boostorg/unordered) library [v1.85.0](https://github.com/boostorg/unordered) 的 [boost::unordered::unordered_flat_map](https://github.com/MikePopoloski/boost_unordered) (非官方库) 。

    请单独更新 [boost::unordered] 库，仅该次更新时需要。

- 我自己的 `jstd::robin_hash_map`：更新至最新版，并添加了新的哈希表 `jstd::cluster_flat_map` 。

- Google [abseil-cpp] 的 `absl::flat_hash_map`：更新至最新版，最新 tag：20240722.rc2

    由于我更改了 [abseil-cpp] 仓库的 URL，请尝试使用前面的方法单独更新，仅该次更新时需要。

- [ktprime] 的 `emhash`：更新至最新版。

- [Tessil] 的 `tsl::robin_map`：更新至最新，最新 tag：1.3.0 。

- [martinus] 的 `ankerl::unordered_dense::map`：更新至最新版，最新 tag：4.5.0 。

- [martinus] 的 `robin_hood::unordered_flat_map`：更新至最新版，最新 tag：3.11.5 。

- [skarupke] 的 `aka::flat_hash_map`：无更新。

### 镜像与修复

由于 `github` 访问比较困难，使用的是 `gitee` 的镜像。

`aka::flat_hash_map` 国内的镜像修改版：[https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

`abseil-cpp` 国内的镜像：[https://gitee.com/shines77/abseil-cpp](https://gitee.com/shines77/abseil-cpp)

本人对 `aka::flat_hash_map` 做了一些修改，并修复了一些 `bug`，以便能过在本测试代码中正常使用。

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

由于引用了数个第三方库，`clone` 完以后必须先初始化 `submodule`：

```bash
git submodule init
git submodule update --init --recursive
```

### 3. 配置与编译

由于 `abseil-cpp` 已经改为源码编译，不再需要单独编译和安装 `abseil-cpp`，相关的编译命令作为备份放在文档最后。

```bash
mkdir ./build
cd build

cmake ..
make
```

如果想同时安装不同的 gcc 版本，并随意切换，请参考该文档：[Ubuntu 20.04 安装 gcc 各版本并随意切换](https://gitee.com/shines77/my_docs/blob/master/linux/ubuntu/Ubuntu-18.04-%E5%AE%89%E8%A3%85GCC%E5%90%84%E7%89%88%E6%9C%AC%E5%B9%B6%E9%9A%8F%E6%84%8F%E5%88%87%E6%8D%A2.md)

有些时候想删除 CMake 的缓存文件，可以把 `./cmake-clean.sh` 文件拷贝到你的 build 目录，再运行，效果类似于 `make clean`，但更完整。

```bash
cp ./cmake-clean.sh ./build
cd build
./cmake-clean.sh
```

### 4. 使用 clang 编译

本仓库在 `./clang` 目录下为 `clang` 定制了专用的 `CMakeLists.txt`。

请先安装 `clang` 以及对应的 C++ 库 `libc++`，需要支持 `C++ 20` 的 `clang` 版本，例如：

```bash
# 安装 clang 18.0 及对应的 libc++
sudo apt-get install clang-18 llvm-18 libc++-18-dev libc++abi-18-dev
```

请阅读下列推荐的文档，做好 `clang-18` 到 `/usr/bin/clang`，`/usr/bin/clang++` 的软连接。

如果想同时安装不同的 clang 版本，并随意切换，请参考该文档：[Ubuntu 20.04 安装 clang 各版本并随意切换](https://gitee.com/shines77/my_docs/blob/master/linux/ubuntu/Ubuntu-16.04-%E5%AE%89%E8%A3%85clang%E7%89%88%E6%9C%AC%E5%B9%B6%E9%9A%8F%E6%84%8F%E5%88%87%E6%8D%A2.md)

**配置与编译**

切换到根目录下的 `clang` 目录下即可，无需再创建 build 目录。

```bash
cd clang
cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
make
```

在 `./clang/CMakeLists.txt` 中已经做了路径的转换，请使用上面的命令，而不是 `cmake ..` 。

### 5. 更新到最新版并编译

请在你的 build 目录下执行下列命令，例如：`./build` 目录。

```bash
cd build
git pull
git submodule update --init --recursive
make
```

### 6. 运行 benchmark

请在你的 build 目录下，例如 `./build` ，执行下列命令：

```bash
# 根据 Jackson Allan 的基准测试改进而来, 推荐
./bin/jackson_bench

# 根据 Google sprasehash 开源库改进的基准测试, 推荐

# 默认迭代次数为 10000000（无参数时）
./bin/time_hash_map_new

# 迭代次数 8000000
./bin/time_hash_map_new 8000000

# 只测试 (K = std::string, V = std::string)，节约时间
./bin/time_hash_map_new string

# 低、中、高、超高 - 基数测试，主要测试 insert() 和 find()
./bin/cardinal_bench


# 以下测试已不更新，不推荐

# 跟 Google sprasehash 开源库类似的测试，请尝试新版本 time_hash_map_new

# 默认迭代次数为 10000000（无参数时）
./bin/time_hash_map

# 迭代次数 8000000
./bin/time_hash_map 8000000

# 小数据集测试
./bin/benchmark

# 中数据集测试
./bin/benchmark ./data/Maven.keys.txt
```

### 7. 其他脚本

下列脚本请拷贝到你的 build 目录下再执行，例如：`./build`。

1. `cmake-clean.sh`

```bash
# 清理 cmake 的缓存和编译结果（便于重新配置和编译 benchmark）,
# 效果类似 make clean, 但更彻底
./cmake-clean.sh
```

2. `cmake-update.sh`

```bash
# 一键更新和编译，但建议先拷贝到 build 目录
./cmake-update.sh

# 内容如下：
git pull
git submodule update --init --recursive
make
```

### 8. 附录：备份文档（已失效）

#### 8.1 配置与编译

先配置和编译 `Google` 的 `abseil-cpp` 库：

```bash
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

#### 8.2 编译 benchmark

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

#### 8.3 更新本仓库到最新版

如果你已经编译了 `abseil-cpp` and `hashmap-benchmark`, 并且向更新到本仓库的最新版本。

你可以使用下列的命令：

```bash
cd build

git pull
git submodule update --init --recursive
# or
git submodule update --init --recursive ../3rd_party/jstd_hashmap

make
```

#### 8.4 其他备份

```bash
# 指定 ./3rd_party/abseil-cpp 的分支为 master
git config -f .gitmodules submodule.3rd_party/abseil-cpp.branch master
```
