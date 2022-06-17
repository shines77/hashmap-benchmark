# hashmap-benchmark

A hash map benchmark, include Java, CSharp, golang and so on.

## Install

Dependent on:

* `jstd_hashmap`: https://gitee.com/shines77/jstd_hashmap
* `flat_hash_map`: https://github.com/skarupke/flat_hash_map

由于 `github` 访问比较困难，`flat_hash_map` 使用的是 `gitee` 的镜像，本人做了一些修改，并修正了一些 `bug`，以便能过在本测试代码中正常使用。

`flat_hash_map` 国内的镜像修改版：[https://gitee.com/shines77/flat_hash_map](https://gitee.com/shines77/flat_hash_map)

做了如下修改：

* Fixed ska::detailv3::KeyOrValueHasher<K, V, Hasher> for /bench/time_hash_map.cpp to use ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;
* Fixed some bugs about ska::bytell_hash_map<K,V> some dead cycle codes;
* Added missing header file "#include \<limits\>" in bytell_hash_map.hpp;
* Added static name() member method for ska::flat_hash_map<K,V> and ska::bytell_hash_map<K,V>;

### 1. Update submodule(s)

```shell
git submodule init
git submodule update --remote --recursive
```

### 2. Configure and Compile

configure and compile `abseil-cpp`:

```shell
cd ./3rd_party/abseil-cpp
mkdir build && cd build
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_ENABLE_INSTALL=ON -DCMAKE_CXX_STANDARD=14 -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_INSTALL_PREFIX=../../../install ..
make
```

configure and compile `hashmap-benchmark`:

```shell
cmake -DABSL_BUILD_TESTING=OFF -DABSL_USE_GOOGLETEST_HEAD=OFF -DABSL_PROPAGATE_CXX_STD=ON -DCMAKE_PREFIX_PATH=./install .
make
````

### 3. Run

```shell
./bin/time_hash_map
或
./bin/benchmark ./data/Maven.jar.updates.txt
```
