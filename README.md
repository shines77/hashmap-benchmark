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

# 如果需要单独更新某个 submodule
git submodule update --init --recursive 3rd_party/abseil-cpp

# 如果有必要，手动删除旧的 ./3rd_part 或 ./3rd_party 目录
rm -rf ./3rd_part
```

2025-02-12 更新：

- `./3rd_part` 路径恢复为 `./3rd_party`，包括 `CMakeLists.txt`、`.gitmodules` 和 MSVC 工程文件中的路径；

- jstd::group15_flat_map，jstd::group16_flat_map 已经基本稳定；

2025-01-27 更新：

- `./3rd_party` 路径更改为 `./3rd_part`，包括 `CMakeLists.txt`、`.gitmodules` 和 MSVC 工程文件中的路径；

- `./3rd_party/unordered_dens` 路径更改为 `./3rd_part/unordered_dense`；

2025-01-13 更新：

如果你这次更新之后才 clone 本仓库，则不必做下面的步骤。

- 新增了 [boost::unordered](https://github.com/boostorg/unordered) library [v1.85.0](https://github.com/boostorg/unordered) 的 [boost::unordered::unordered_flat_map](https://github.com/MikePopoloski/boost_unordered) (非官方库) 。

    请单独更新 [boost::unordered] 库。

- 我自己的 `jstd::robin_hash_map`：更新至最新版，并添加了新的哈希表 `jstd::cluster_flat_map` 。

- Google [abseil-cpp] 的 `absl::flat_hash_map`：更新至最新版，最新 tag：20240722.rc2

    由于我更改了 [abseil-cpp] 仓库的 URL，如果你在这之前就拉取过 /3rd_party/abseil-cpp，请使用下列的命令更新 submodule ：

    ```bash
    # 先删除旧的 /3rd_party/abseil-cpp 目录
    cd ./3rd_party
    rm -rf ./abseil-cpp

    # 使用 sync 命令同步为新的 URL，并重新初始化 abseil-cpp
    git config -f .gitmodules submodule.3rd_party/abseil-cpp.branch master
    git pull
    git submodule sync
    git submodule update --init --recursive 3rd_party/abseil-cpp
    ```

- [ktprime] 的 `emhash`：更新至最新版

- [Tessil] 的 `tsl::robin_map`：更新至最新，最新 tag：1.3.0

- [martinus] 的 `ankerl::unordered_dense::map`：更新至最新版，最新 tag：4.5.0

- [martinus] 的 `robin_hood::unordered_flat_map`：更新至最新版，最新 tag：3.11.5

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

```bash
# 清理 cmake 的缓存和编译结果（便于重新配置和编译 benchmark）,
# 效果类似 make clean, 但更彻底
./cmake-clean.sh

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

### 8.3 更新本仓库到最新版

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
