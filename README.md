# hashmap-benchmark

A hash map benchmark, include Java, CSharp, golang and so on.

## Install

Dependent on:

* `jstd_hashmap`: https://gitee.com/shines77/jstd_hashmap
* `flat_hash_map`: https://github.com/skarupke/flat_hash_map

由于 `github` 访问比较困难，`flat_hash_map` 使用的是 `gitee` 的镜像，本人做了一些修改，并修正了一些 `bug`，以便能过在本测试代码中正常使用。

`flat_hash_map` 国内的镜像修改版：[https://github.com/skarupke/flat_hash_map](https://github.com/skarupke/flat_hash_map)

### 1. Update submodule(s)

```shell
git submodule init
git submodule update --remote --recursive
```

### 2. Configure and Compile

```shell
cmake .
make
````

### 3. Run

```shell
./bin/time_hash_map
或
./bin/benchmark ./data/Maven.jar.updates.txt
```
