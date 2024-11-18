
硬件环境：Intel(R) Xeon(R) Platinum 8255C @ 2.5G Hz (阿里云轻量级服务器), 双核, L1 Cache 32KB, 2GB 内存。

系统：Ubuntu Server 20.04 - 64bit

编译器：gcc 13.1.0

测试结果：

```text
---------------------- benchmark_all_hashmaps (iters = 10000000) ----------------------

std::unordered_map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                  9.08 ns  lf=0.825
serial_find_random                  82.85 ns  lf=0.825
serial_find_failed                  16.49 ns  lf=0.825
serial_find_empty                    1.64 ns  lf=0.000

random_find_serial                  61.66 ns  lf=0.825
random_find_random                  91.76 ns  lf=0.825
random_find_failed                 174.37 ns  lf=0.825
random_find_empty                    1.64 ns  lf=0.000

serial_insert                       54.67 ns  lf=0.825    477.6 MB
serial_insert_predicted             40.75 ns  lf=0.966    477.6 MB
serial_insert_replace               23.74 ns  lf=0.825    439.4 MB

serial_emplace                      56.52 ns  lf=0.825    439.4 MB
serial_emplace_predicted            41.54 ns  lf=0.966    439.4 MB
serial_emplace_replace              24.88 ns  lf=0.825    439.4 MB

serial operator []                  54.35 ns  lf=0.825    439.4 MB
serial operator [] predicted        40.45 ns  lf=0.966    439.4 MB
serial operator [] replace          10.55 ns  lf=0.825    439.4 MB

random_insert                      304.78 ns  lf=0.825    439.4 MB
random_insert_predicted            132.70 ns  lf=0.966    439.4 MB
random_insert_replace              181.77 ns  lf=0.825    439.4 MB

random_emplace                     302.66 ns  lf=0.825    439.4 MB
random_emplace_predicted           134.43 ns  lf=0.966    439.4 MB
random_emplace_replace             184.91 ns  lf=0.825    439.4 MB

random operator []                 305.37 ns  lf=0.825    439.4 MB
random operator [] predicted       136.20 ns  lf=0.966    439.4 MB
random operator [] replace         155.58 ns  lf=0.825    439.4 MB

serial_erase                        26.54 ns  lf=0.000    439.4 MB
serial_erase_failed                 16.74 ns  lf=0.825    439.4 MB
serial_toggle                       41.60 ns  lf=0.000    439.4 MB
serial_emplace - iterate             4.10 ns  lf=0.825    439.4 MB

random_erase                       217.62 ns  lf=0.000    439.4 MB
random_erase_failed                184.77 ns  lf=0.825    439.4 MB
random_toggle                       41.48 ns  lf=0.000    439.4 MB
random_emplace - iterate            37.68 ns  lf=0.825    439.4 MB

stress_hash_function: map_size = 256  stride = 1   : 29.86 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 30.00 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 29.95 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 29.94 ns/insertion

jstd::robin_hash_map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                  1.75 ns  lf=0.298
serial_find_random                  20.14 ns  lf=0.298
serial_find_failed                   2.36 ns  lf=0.298
serial_find_empty                    1.61 ns  lf=0.000

random_find_serial                   1.82 ns  lf=0.298
random_find_random                  19.88 ns  lf=0.298
random_find_failed                   7.80 ns  lf=0.298
random_find_empty                    7.66 ns  lf=0.000

serial_insert                       18.96 ns  lf=0.298    118.6 MB (+76.1 MB)
serial_insert_predicted              9.51 ns  lf=0.298    118.6 MB (+76.1 MB)
serial_insert_replace                4.52 ns  lf=0.298    194.7 MB

serial_emplace                      21.15 ns  lf=0.298    118.6 MB (+76.1 MB)
serial_emplace_predicted            11.67 ns  lf=0.298    118.6 MB (+76.1 MB)
serial_emplace_replace               4.63 ns  lf=0.298    194.7 MB

serial operator []                  19.33 ns  lf=0.298    118.6 MB (+76.1 MB)
serial operator [] predicted         9.92 ns  lf=0.298    118.6 MB (+76.1 MB)
serial operator [] replace           4.74 ns  lf=0.298    194.7 MB

random_insert                       59.74 ns  lf=0.298    118.6 MB (+76.3 MB)
random_insert_predicted             47.81 ns  lf=0.298    118.6 MB (+76.1 MB)
random_insert_replace               36.84 ns  lf=0.298    194.9 MB

random_emplace                      63.68 ns  lf=0.298    118.6 MB (+76.3 MB)
random_emplace_predicted            50.81 ns  lf=0.298    118.6 MB (+76.1 MB)
random_emplace_replace              37.17 ns  lf=0.298    194.9 MB

random operator []                  60.18 ns  lf=0.298    118.6 MB (+76.3 MB)
random operator [] predicted        49.40 ns  lf=0.298    118.6 MB (+76.1 MB)
random operator [] replace          36.15 ns  lf=0.298    194.9 MB

serial_erase                         2.78 ns  lf=0.000    194.7 MB
serial_erase_failed                  0.96 ns  lf=0.298    194.7 MB
serial_toggle                        8.58 ns  lf=0.000    118.6 MB
serial_emplace - iterate             2.40 ns  lf=0.298    194.7 MB

random_erase                        28.17 ns  lf=0.000    194.9 MB
random_erase_failed                  4.58 ns  lf=0.298    194.9 MB
random_toggle                        8.65 ns  lf=0.000    118.6 MB
random_emplace - iterate             3.01 ns  lf=0.298    194.9 MB

stress_hash_function: map_size = 256  stride = 1   : 3.58 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 49.45 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 3.46 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 277.79 ns/insertion

ska::flat_hash_map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                 23.79 ns  lf=0.298
serial_find_random                  20.42 ns  lf=0.298
serial_find_failed                  19.38 ns  lf=0.298
serial_find_empty                   19.27 ns  lf=0.000

random_find_serial                  24.16 ns  lf=0.298
random_find_random                  20.39 ns  lf=0.298
random_find_failed                  17.62 ns  lf=0.298
random_find_empty                   17.81 ns  lf=0.000

serial_insert                       97.73 ns  lf=0.298    104.3 MB (+384.0 MB)
serial_insert_predicted             73.49 ns  lf=0.298    104.3 MB (+384.0 MB)
serial_insert_replace               26.36 ns  lf=0.298    488.3 MB

serial_emplace                     104.38 ns  lf=0.298    104.3 MB (+384.0 MB)
serial_emplace_predicted            80.81 ns  lf=0.298    104.3 MB (+384.0 MB)
serial_emplace_replace              26.79 ns  lf=0.298    488.3 MB

serial operator []                  98.65 ns  lf=0.298    104.3 MB (+384.0 MB)
serial operator [] predicted        74.11 ns  lf=0.298    104.3 MB (+384.0 MB)
serial operator [] replace          25.56 ns  lf=0.298    488.3 MB

random_insert                      110.13 ns  lf=0.298    104.3 MB (+384.0 MB)
random_insert_predicted             76.69 ns  lf=0.298    104.3 MB (+384.0 MB)
random_insert_replace               24.25 ns  lf=0.298    488.3 MB

random_emplace                     114.88 ns  lf=0.298    104.3 MB (+384.0 MB)
random_emplace_predicted            83.01 ns  lf=0.298    104.3 MB (+384.0 MB)
random_emplace_replace              19.96 ns  lf=0.298    488.3 MB

random operator []                 107.85 ns  lf=0.298    104.3 MB (+384.0 MB)
random operator [] predicted        75.63 ns  lf=0.298    104.3 MB (+384.0 MB)
random operator [] replace          26.59 ns  lf=0.298    488.3 MB

serial_erase                        27.50 ns  lf=0.000    488.3 MB
serial_erase_failed                 22.59 ns  lf=0.298    488.3 MB
serial_toggle                        8.89 ns  lf=0.000    104.3 MB
serial_emplace - iterate             4.44 ns  lf=0.298    488.3 MB

random_erase                        30.86 ns  lf=0.000    488.3 MB
random_erase_failed                 19.82 ns  lf=0.298    488.3 MB
random_toggle                        9.00 ns  lf=0.000    104.3 MB
random_emplace - iterate             4.50 ns  lf=0.298    488.3 MB

stress_hash_function: map_size = 256  stride = 1   : 10.36 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 10.69 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 10.90 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 10.55 ns/insertion

absl::flat_hash_map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                 30.91 ns  lf=0.596
serial_find_random                  30.43 ns  lf=0.596
serial_find_failed                  18.42 ns  lf=0.596
serial_find_empty                    3.33 ns  lf=0.000

random_find_serial                  30.48 ns  lf=0.596
random_find_random                  30.83 ns  lf=0.596
random_find_failed                  21.71 ns  lf=0.596
random_find_empty                    3.65 ns  lf=0.000

serial_insert                      164.50 ns  lf=0.596    104.4 MB (+144.0 MB)
serial_insert_predicted            207.01 ns  lf=0.596    104.4 MB (+143.9 MB)
serial_insert_replace               72.29 ns  lf=0.596    248.4 MB

serial_emplace                     157.02 ns  lf=0.596    104.4 MB (+144.0 MB)
serial_emplace_predicted           201.31 ns  lf=0.596    104.4 MB (+143.9 MB)
serial_emplace_replace              67.43 ns  lf=0.596    248.4 MB

serial operator []                 164.45 ns  lf=0.596    104.4 MB (+144.0 MB)
serial operator [] predicted       209.82 ns  lf=0.596    104.4 MB (+143.9 MB)
serial operator [] replace         231.20 ns  lf=0.596    248.4 MB

random_insert                      137.48 ns  lf=0.596    104.4 MB (+144.0 MB)
random_insert_predicted            168.21 ns  lf=0.596    104.4 MB (+143.9 MB)
random_insert_replace               74.07 ns  lf=0.596    248.4 MB

random_emplace                     130.05 ns  lf=0.596    104.4 MB (+144.0 MB)
random_emplace_predicted           163.36 ns  lf=0.596    104.4 MB (+143.9 MB)
random_emplace_replace              44.88 ns  lf=0.596    248.4 MB

random operator []                 135.39 ns  lf=0.596    104.4 MB (+144.0 MB)
random operator [] predicted       169.26 ns  lf=0.596    104.4 MB (+143.9 MB)
random operator [] replace         184.88 ns  lf=0.596    248.4 MB

serial_erase                       185.70 ns  lf=0.000    248.4 MB
serial_erase_failed                 18.00 ns  lf=0.596    248.4 MB
serial_toggle                       27.69 ns  lf=0.000    104.4 MB
serial_emplace - iterate             5.04 ns  lf=0.596    248.4 MB

random_erase                       156.83 ns  lf=0.000    248.4 MB
random_erase_failed                 18.06 ns  lf=0.596    248.4 MB
random_toggle                       27.85 ns  lf=0.000    104.4 MB
random_emplace - iterate             5.06 ns  lf=0.596    248.4 MB

stress_hash_function: map_size = 256  stride = 1   : 23.66 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 18.57 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 84.97 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 10.56 ns/insertion

tsl::robin_map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                  1.78 ns  lf=0.298
serial_find_random                  15.04 ns  lf=0.298
serial_find_failed                   1.28 ns  lf=0.298
serial_find_empty                    1.41 ns  lf=0.000

random_find_serial                   1.74 ns  lf=0.298
random_find_random                  14.86 ns  lf=0.298
random_find_failed                  13.02 ns  lf=0.298
random_find_empty                   12.41 ns  lf=0.000

serial_insert                       55.35 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_predicted             31.44 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_replace                4.08 ns  lf=0.298    488.4 MB

serial_emplace                      55.44 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_predicted            31.45 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_replace               4.08 ns  lf=0.298    488.4 MB

serial operator []                  55.48 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] predicted        31.54 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] replace           3.96 ns  lf=0.298    488.4 MB

random_insert                      100.45 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_predicted             69.99 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_replace               30.03 ns  lf=0.298    488.4 MB

random_emplace                      99.85 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_predicted            69.98 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_replace              30.66 ns  lf=0.298    488.4 MB

random operator []                 100.50 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] predicted        70.80 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] replace          33.71 ns  lf=0.298    488.4 MB

serial_erase                         2.48 ns  lf=0.000    488.4 MB
serial_erase_failed                  2.11 ns  lf=0.298    488.4 MB
serial_toggle                        8.26 ns  lf=0.000    104.4 MB
serial_emplace - iterate             4.24 ns  lf=0.298    488.4 MB

random_erase                        30.56 ns  lf=0.000    488.4 MB
random_erase_failed                 12.81 ns  lf=0.298    488.4 MB
random_toggle                        8.66 ns  lf=0.000    104.4 MB
random_emplace - iterate             4.24 ns  lf=0.298    488.4 MB

stress_hash_function: map_size = 256  stride = 1   : 7.66 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 109.23 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 7.63 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 380.63 ns/insertion

ankerl::unordered_dense::map<K, V> (4 byte objects, 8 byte ValueType, 10000000 iterations):

serial_find_success                 21.09 ns  lf=0.596
serial_find_random                  36.95 ns  lf=0.596
serial_find_failed                  35.74 ns  lf=0.596
serial_find_empty                    1.95 ns  lf=0.000

random_find_serial                  37.41 ns  lf=0.596
random_find_random                  37.12 ns  lf=0.596
random_find_failed                  35.17 ns  lf=0.596
random_find_empty                    1.94 ns  lf=0.000

serial_insert                       94.48 ns  lf=0.596    104.4 MB (+204.1 MB)
serial_insert_predicted             60.96 ns  lf=0.596    104.4 MB (+204.1 MB)
serial_insert_replace               40.57 ns  lf=0.596    308.6 MB

serial_emplace                      93.61 ns  lf=0.596    104.4 MB (+204.1 MB)
serial_emplace_predicted            60.36 ns  lf=0.596    104.4 MB (+204.1 MB)
serial_emplace_replace              40.76 ns  lf=0.596    308.6 MB

serial operator []                  89.68 ns  lf=0.596    104.4 MB (+204.1 MB)
serial operator [] predicted        53.74 ns  lf=0.596    104.4 MB (+204.1 MB)
serial operator [] replace         117.53 ns  lf=0.596    308.6 MB

random_insert                       99.94 ns  lf=0.596    104.4 MB (+204.1 MB)
random_insert_predicted             58.95 ns  lf=0.596    104.4 MB (+204.1 MB)
random_insert_replace               40.31 ns  lf=0.596    308.6 MB

random_emplace                     102.29 ns  lf=0.596    104.4 MB (+204.1 MB)
random_emplace_predicted            60.10 ns  lf=0.596    104.4 MB (+204.1 MB)
random_emplace_replace              39.39 ns  lf=0.596    308.6 MB

random operator []                  95.32 ns  lf=0.596    104.4 MB (+204.1 MB)
random operator [] predicted        52.56 ns  lf=0.596    104.4 MB (+204.1 MB)
random operator [] replace         109.23 ns  lf=0.596    308.6 MB

serial_erase                       137.75 ns  lf=0.000    308.6 MB
serial_erase_failed                 31.09 ns  lf=0.596    308.6 MB
serial_toggle                       12.02 ns  lf=0.000    104.4 MB
serial_emplace - iterate             0.63 ns  lf=0.596    308.6 MB

random_erase                       134.04 ns  lf=0.000    308.6 MB
random_erase_failed                 33.69 ns  lf=0.596    308.6 MB
random_toggle                       12.26 ns  lf=0.000    104.4 MB
random_emplace - iterate             0.63 ns  lf=0.596    308.6 MB

stress_hash_function: map_size = 256  stride = 1   : 5.63 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 5.64 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 5.61 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 5.69 ns/insertion

std::unordered_map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                  9.06 ns  lf=0.838
serial_find_random                  80.47 ns  lf=0.838
serial_find_failed                  15.72 ns  lf=0.838
serial_find_empty                    1.27 ns  lf=0.000

random_find_serial                  58.49 ns  lf=0.838
random_find_random                  85.39 ns  lf=0.838
random_find_failed                 167.89 ns  lf=0.838
random_find_empty                    1.54 ns  lf=0.000

serial_insert                       54.18 ns  lf=0.838    278.3 MB
serial_insert_predicted             39.84 ns  lf=0.981    278.3 MB
serial_insert_replace               23.03 ns  lf=0.838    278.3 MB

serial_emplace                      55.62 ns  lf=0.838    278.3 MB
serial_emplace_predicted            42.13 ns  lf=0.981    278.3 MB
serial_emplace_replace              23.87 ns  lf=0.838    278.3 MB

serial operator []                  53.94 ns  lf=0.838    278.3 MB
serial operator [] predicted        40.38 ns  lf=0.981    278.3 MB
serial operator [] replace          10.43 ns  lf=0.838    278.3 MB

random_insert                      267.25 ns  lf=0.838    278.3 MB
random_insert_predicted            123.89 ns  lf=0.981    278.3 MB
random_insert_replace              149.35 ns  lf=0.838    278.3 MB

random_emplace                     269.65 ns  lf=0.838    278.3 MB
random_emplace_predicted           122.35 ns  lf=0.981    278.3 MB
random_emplace_replace             149.96 ns  lf=0.838    278.3 MB

random operator []                 285.89 ns  lf=0.838    278.3 MB
random operator [] predicted       125.83 ns  lf=0.981    278.3 MB
random operator [] replace         136.16 ns  lf=0.838    278.3 MB

serial_erase                        26.92 ns  lf=0.000    278.3 MB
serial_erase_failed                 17.28 ns  lf=0.838    278.3 MB
serial_toggle                       42.00 ns  lf=0.000    278.3 MB
serial_emplace - iterate             4.23 ns  lf=0.838    278.3 MB

random_erase                       187.01 ns  lf=0.000    278.3 MB
random_erase_failed                174.32 ns  lf=0.838    278.3 MB
random_toggle                       41.78 ns  lf=0.000    278.3 MB
random_emplace - iterate            32.96 ns  lf=0.838    278.3 MB

stress_hash_function: map_size = 256  stride = 1   : 29.76 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 29.73 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 29.81 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 29.84 ns/insertion

jstd::robin_hash_map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                  2.50 ns  lf=0.298
serial_find_random                  19.65 ns  lf=0.298
serial_find_failed                   2.37 ns  lf=0.298
serial_find_empty                    1.60 ns  lf=0.000

random_find_serial                   2.40 ns  lf=0.298
random_find_random                  19.18 ns  lf=0.298
random_find_failed                   5.56 ns  lf=0.298
random_find_empty                    6.82 ns  lf=0.000

serial_insert                       32.08 ns  lf=0.298     80.6 MB (+76.1 MB)
serial_insert_predicted             13.55 ns  lf=0.298     80.6 MB (+76.1 MB)
serial_insert_replace                4.52 ns  lf=0.298    156.7 MB

serial_emplace                      32.39 ns  lf=0.298     80.6 MB (+76.1 MB)
serial_emplace_predicted            13.90 ns  lf=0.298     80.6 MB (+76.1 MB)
serial_emplace_replace               4.67 ns  lf=0.298    156.7 MB

serial operator []                  32.59 ns  lf=0.298     80.6 MB (+76.1 MB)
serial operator [] predicted        14.16 ns  lf=0.298     80.6 MB (+76.1 MB)
serial operator [] replace           4.90 ns  lf=0.298    156.7 MB

random_insert                       78.12 ns  lf=0.298     80.6 MB (+76.3 MB)
random_insert_predicted             50.29 ns  lf=0.298     80.6 MB (+76.1 MB)
random_insert_replace               32.74 ns  lf=0.298    156.8 MB

random_emplace                      84.12 ns  lf=0.298     80.6 MB (+76.3 MB)
random_emplace_predicted            54.51 ns  lf=0.298     80.6 MB (+76.1 MB)
random_emplace_replace              33.95 ns  lf=0.298    156.8 MB

random operator []                  79.73 ns  lf=0.298     80.6 MB (+76.3 MB)
random operator [] predicted        51.95 ns  lf=0.298     80.6 MB (+76.1 MB)
random operator [] replace          39.17 ns  lf=0.298    156.8 MB

serial_erase                         3.58 ns  lf=0.000    156.7 MB
serial_erase_failed                  0.96 ns  lf=0.298    156.7 MB
serial_toggle                        8.58 ns  lf=0.000     80.6 MB
serial_emplace - iterate             2.82 ns  lf=0.298    156.7 MB

random_erase                        27.74 ns  lf=0.000    156.8 MB
random_erase_failed                  5.08 ns  lf=0.298    156.8 MB
random_toggle                        8.97 ns  lf=0.000     80.6 MB
random_emplace - iterate             3.67 ns  lf=0.298    156.8 MB

stress_hash_function: map_size = 256  stride = 1   : 3.90 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 52.07 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 3.78 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 295.44 ns/insertion

ska::flat_hash_map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                 23.88 ns  lf=0.298
serial_find_random                  20.61 ns  lf=0.298
serial_find_failed                  19.32 ns  lf=0.298
serial_find_empty                   19.14 ns  lf=0.000

random_find_serial                  23.80 ns  lf=0.298
random_find_random                  20.78 ns  lf=0.298
random_find_failed                  18.35 ns  lf=0.298
random_find_empty                   17.53 ns  lf=0.000

serial_insert                      148.94 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_predicted            100.92 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_replace               26.78 ns  lf=0.298    488.4 MB

serial_emplace                     155.48 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_predicted           108.33 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_replace              26.73 ns  lf=0.298    488.4 MB

serial operator []                 149.60 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] predicted       101.48 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] replace          30.74 ns  lf=0.298    488.4 MB

random_insert                      158.01 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_predicted            103.24 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_replace               24.61 ns  lf=0.298    488.4 MB

random_emplace                     163.61 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_predicted           109.62 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_replace              20.69 ns  lf=0.298    488.4 MB

random operator []                 156.30 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] predicted       101.81 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] replace          29.05 ns  lf=0.298    488.4 MB

serial_erase                        28.15 ns  lf=0.000    488.4 MB
serial_erase_failed                 22.82 ns  lf=0.298    488.4 MB
serial_toggle                        8.87 ns  lf=0.000    104.4 MB
serial_emplace - iterate             7.03 ns  lf=0.298    488.4 MB

random_erase                        34.47 ns  lf=0.000    488.4 MB
random_erase_failed                 21.25 ns  lf=0.298    488.4 MB
random_toggle                        8.83 ns  lf=0.000    104.4 MB
random_emplace - iterate             7.06 ns  lf=0.298    488.4 MB

stress_hash_function: map_size = 256  stride = 1   : 10.55 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 10.96 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 11.07 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 10.73 ns/insertion

absl::flat_hash_map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                 29.17 ns  lf=0.596
serial_find_random                  27.25 ns  lf=0.596
serial_find_failed                  17.77 ns  lf=0.596
serial_find_empty                    3.49 ns  lf=0.000

random_find_serial                  28.98 ns  lf=0.596
random_find_random                  27.42 ns  lf=0.596
random_find_failed                  20.03 ns  lf=0.596
random_find_empty                    3.56 ns  lf=0.000

serial_insert                      140.24 ns  lf=0.596    104.4 MB (+136.0 MB)
serial_insert_predicted            204.15 ns  lf=0.596    104.4 MB (+135.9 MB)
serial_insert_replace               62.76 ns  lf=0.596    240.4 MB

serial_emplace                     144.20 ns  lf=0.596    104.4 MB (+136.0 MB)
serial_emplace_predicted           189.77 ns  lf=0.596    104.4 MB (+135.9 MB)
serial_emplace_replace              61.50 ns  lf=0.596    240.4 MB

serial operator []                 148.48 ns  lf=0.596    104.4 MB (+136.0 MB)
serial operator [] predicted       199.69 ns  lf=0.596    104.4 MB (+135.9 MB)
serial operator [] replace         195.95 ns  lf=0.596    240.4 MB

random_insert                      120.21 ns  lf=0.596    104.4 MB (+136.0 MB)
random_insert_predicted            132.46 ns  lf=0.596    104.4 MB (+135.9 MB)
random_insert_replace               64.59 ns  lf=0.596    240.4 MB

random_emplace                     122.14 ns  lf=0.596    104.4 MB (+136.0 MB)
random_emplace_predicted           130.19 ns  lf=0.596    104.4 MB (+135.9 MB)
random_emplace_replace              43.16 ns  lf=0.596    240.4 MB

random operator []                 124.32 ns  lf=0.596    104.4 MB (+136.0 MB)
random operator [] predicted       143.28 ns  lf=0.596    104.4 MB (+135.9 MB)
random operator [] replace         157.57 ns  lf=0.596    240.4 MB

serial_erase                       172.76 ns  lf=0.000    240.4 MB
serial_erase_failed                 16.79 ns  lf=0.596    240.4 MB
serial_toggle                       27.60 ns  lf=0.000    104.4 MB
serial_emplace - iterate             5.36 ns  lf=0.596    240.4 MB

random_erase                       125.34 ns  lf=0.000    240.4 MB
random_erase_failed                 16.74 ns  lf=0.596    240.4 MB
random_toggle                       27.91 ns  lf=0.000    104.4 MB
random_emplace - iterate             5.20 ns  lf=0.596    240.4 MB

stress_hash_function: map_size = 256  stride = 1   : 23.52 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 7.55 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 84.72 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 10.85 ns/insertion

tsl::robin_map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                  2.99 ns  lf=0.298
serial_find_random                  16.47 ns  lf=0.298
serial_find_failed                   2.20 ns  lf=0.298
serial_find_empty                    2.24 ns  lf=0.000

random_find_serial                   2.85 ns  lf=0.298
random_find_random                  16.22 ns  lf=0.298
random_find_failed                  13.68 ns  lf=0.298
random_find_empty                   13.14 ns  lf=0.000

serial_insert                      100.70 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_predicted             54.90 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_insert_replace                3.58 ns  lf=0.298    488.4 MB

serial_emplace                     100.82 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_predicted            54.91 ns  lf=0.298    104.4 MB (+384.0 MB)
serial_emplace_replace               3.56 ns  lf=0.298    488.4 MB

serial operator []                 101.12 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] predicted        55.31 ns  lf=0.298    104.4 MB (+384.0 MB)
serial operator [] replace           3.75 ns  lf=0.298    488.4 MB

random_insert                      143.68 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_predicted             91.90 ns  lf=0.298    104.4 MB (+384.0 MB)
random_insert_replace               26.36 ns  lf=0.298    488.4 MB

random_emplace                     144.29 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_predicted            91.96 ns  lf=0.298    104.4 MB (+384.0 MB)
random_emplace_replace              23.95 ns  lf=0.298    488.4 MB

random operator []                 143.92 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] predicted        92.53 ns  lf=0.298    104.4 MB (+384.0 MB)
random operator [] replace          28.13 ns  lf=0.298    488.4 MB

serial_erase                         3.24 ns  lf=0.000    488.4 MB
serial_erase_failed                  3.10 ns  lf=0.298    488.4 MB
serial_toggle                        6.98 ns  lf=0.000    104.4 MB
serial_emplace - iterate             7.39 ns  lf=0.298    488.4 MB

random_erase                        32.05 ns  lf=0.000    488.4 MB
random_erase_failed                 13.77 ns  lf=0.298    488.4 MB
random_toggle                        7.80 ns  lf=0.000    104.4 MB
random_emplace - iterate             7.33 ns  lf=0.298    488.4 MB

stress_hash_function: map_size = 256  stride = 1   : 7.84 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 109.68 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 7.78 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 381.91 ns/insertion

ankerl::unordered_dense::map<K, V> (8 byte objects, 16 byte ValueType, 5000000 iterations):

serial_find_success                 22.02 ns  lf=0.596
serial_find_random                  35.55 ns  lf=0.596
serial_find_failed                  32.61 ns  lf=0.596
serial_find_empty                    1.93 ns  lf=0.000

random_find_serial                  35.97 ns  lf=0.596
random_find_random                  35.73 ns  lf=0.596
random_find_failed                  31.26 ns  lf=0.596
random_find_empty                    2.33 ns  lf=0.000

serial_insert                       98.08 ns  lf=0.596    104.4 MB (+140.1 MB)
serial_insert_predicted             67.89 ns  lf=0.596    104.4 MB (+140.1 MB)
serial_insert_replace               39.09 ns  lf=0.596    244.6 MB

serial_emplace                      96.02 ns  lf=0.596    104.4 MB (+140.1 MB)
serial_emplace_predicted            67.20 ns  lf=0.596    104.4 MB (+140.1 MB)
serial_emplace_replace              39.31 ns  lf=0.596    244.6 MB

serial operator []                  92.52 ns  lf=0.596    104.4 MB (+140.1 MB)
serial operator [] predicted        61.30 ns  lf=0.596    104.4 MB (+140.1 MB)
serial operator [] replace         110.98 ns  lf=0.596    244.6 MB

random_insert                      101.95 ns  lf=0.596    104.4 MB (+140.1 MB)
random_insert_predicted             66.69 ns  lf=0.596    104.4 MB (+140.1 MB)
random_insert_replace               39.41 ns  lf=0.596    244.6 MB

random_emplace                     100.76 ns  lf=0.596    104.4 MB (+140.1 MB)
random_emplace_predicted            67.02 ns  lf=0.596    104.4 MB (+140.1 MB)
random_emplace_replace              28.03 ns  lf=0.596    244.6 MB

random operator []                  97.36 ns  lf=0.596    104.4 MB (+140.1 MB)
random operator [] predicted        59.80 ns  lf=0.596    104.4 MB (+140.1 MB)
random operator [] replace          94.52 ns  lf=0.596    244.6 MB

serial_erase                       124.84 ns  lf=0.000    244.6 MB
serial_erase_failed                 28.09 ns  lf=0.596    244.6 MB
serial_toggle                       11.96 ns  lf=0.000    104.4 MB
serial_emplace - iterate             1.24 ns  lf=0.596    244.6 MB

random_erase                       117.66 ns  lf=0.000    244.6 MB
random_erase_failed                 29.62 ns  lf=0.596    244.6 MB
random_toggle                       12.26 ns  lf=0.000    104.4 MB
random_emplace - iterate             1.26 ns  lf=0.596    244.6 MB

stress_hash_function: map_size = 256  stride = 1   : 5.65 ns/insertion
stress_hash_function: map_size = 256  stride = 256 : 5.64 ns/insertion
stress_hash_function: map_size = 1024 stride = 1   : 5.64 ns/insertion
stress_hash_function: map_size = 1024 stride = 1024: 5.59 ns/insertion

std::unordered_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                304.41 ns  lf=0.877
random_find_success                348.47 ns  lf=0.877
find_failed                        298.75 ns  lf=0.877
find_empty                           2.86 ns  lf=0.000

insert                             847.28 ns  lf=0.877    276.7 MB
insert_predicted                   671.59 ns  lf=0.949    276.7 MB
insert_replace                     323.76 ns  lf=0.877    276.7 MB

emplace                            848.62 ns  lf=0.877    276.7 MB
emplace_predicted                  666.66 ns  lf=0.949    276.7 MB
emplace_replace                    495.22 ns  lf=0.877    276.7 MB

try_emplace                        831.48 ns  lf=0.877    276.7 MB
try_emplace_predicted              662.80 ns  lf=0.949    276.7 MB
try_emplace_replace                564.56 ns  lf=0.877    276.7 MB

operator []                        848.88 ns  lf=0.877    276.7 MB
operator [] predicted              673.18 ns  lf=0.949    276.7 MB
operator [] replace                423.18 ns  lf=0.877    276.7 MB

serial_erase                       548.17 ns  lf=0.000    276.7 MB
random_erase                       653.05 ns  lf=0.000    276.7 MB
erase_failed                       282.31 ns  lf=0.877    276.7 MB

toggle                             115.50 ns  lf=0.000    276.7 MB
iterate                            111.33 ns  lf=0.877    276.7 MB

jstd::robin_hash_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                113.67 ns  lf=0.298
random_find_success                186.29 ns  lf=0.298
find_failed                         64.63 ns  lf=0.298
find_empty                          57.50 ns  lf=0.000

insert                             570.13 ns  lf=0.298    300.8 MB (+37.9 MB)
insert_predicted                   332.41 ns  lf=0.298    300.8 MB (+37.9 MB)
insert_replace                     237.30 ns  lf=0.298    338.7 MB

emplace                            583.52 ns  lf=0.298    300.8 MB (+37.9 MB)
emplace_predicted                  341.95 ns  lf=0.298    300.8 MB (+37.9 MB)
emplace_replace                    237.33 ns  lf=0.298    338.7 MB

try_emplace                        544.87 ns  lf=0.298    300.8 MB (+37.9 MB)
try_emplace_predicted              321.19 ns  lf=0.298    300.8 MB (+37.9 MB)
try_emplace_replace                202.62 ns  lf=0.298    338.7 MB

operator []                        551.89 ns  lf=0.298    300.8 MB (+37.9 MB)
operator [] predicted              332.77 ns  lf=0.298    300.8 MB (+37.9 MB)
operator [] replace                159.76 ns  lf=0.298    338.7 MB

serial_erase                       241.11 ns  lf=0.000    338.7 MB
random_erase                       433.58 ns  lf=0.000    338.7 MB
erase_failed                        63.89 ns  lf=0.298    338.7 MB

toggle                              87.16 ns  lf=0.000    300.8 MB
iterate                              4.98 ns  lf=0.298    338.7 MB

ska::flat_hash_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                137.04 ns  lf=0.298
random_find_success                153.90 ns  lf=0.298
find_failed                         97.75 ns  lf=0.298
find_empty                          75.69 ns  lf=0.000

insert                             921.09 ns  lf=0.298    303.8 MB (+144.0 MB)
insert_predicted                   601.58 ns  lf=0.298    303.8 MB (+144.0 MB)
insert_replace                     147.45 ns  lf=0.298    447.8 MB

emplace                            923.13 ns  lf=0.298    303.8 MB (+144.0 MB)
emplace_predicted                  598.31 ns  lf=0.298    303.8 MB (+144.0 MB)
emplace_replace                    148.11 ns  lf=0.298    447.8 MB

try_emplace                        918.67 ns  lf=0.298    303.8 MB (+144.0 MB)
try_emplace_predicted              602.56 ns  lf=0.298    303.8 MB (+144.0 MB)
try_emplace_replace                164.62 ns  lf=0.298    447.8 MB

operator []                        915.73 ns  lf=0.298    303.8 MB (+144.0 MB)
operator [] predicted              602.75 ns  lf=0.298    303.8 MB (+144.0 MB)
operator [] replace                206.32 ns  lf=0.298    447.8 MB

serial_erase                       235.82 ns  lf=0.000    447.8 MB
random_erase                       288.51 ns  lf=0.000    447.8 MB
erase_failed                        89.20 ns  lf=0.298    447.8 MB

toggle                              87.22 ns  lf=0.000    303.8 MB
iterate                             24.16 ns  lf=0.298    447.8 MB

emhash8::HashMap<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                114.07 ns  lf=0.596
random_find_success                203.93 ns  lf=0.596
find_failed                         62.40 ns  lf=0.596
find_empty                          48.46 ns  lf=0.000

insert                             556.31 ns  lf=0.596    321.6 MB (+37.9 MB)
insert_predicted                   364.44 ns  lf=0.596    321.6 MB (+37.9 MB)
insert_replace                     173.68 ns  lf=0.596    359.5 MB

emplace                            569.57 ns  lf=0.596    321.6 MB (+37.9 MB)
emplace_predicted                  376.74 ns  lf=0.596    321.6 MB (+37.9 MB)
emplace_replace                    179.41 ns  lf=0.596    359.5 MB

try_emplace                        600.46 ns  lf=0.596    321.6 MB (+37.9 MB)
try_emplace_predicted              392.27 ns  lf=0.596    321.6 MB (+37.9 MB)
try_emplace_replace                252.83 ns  lf=0.596    359.5 MB

operator []                        591.43 ns  lf=0.596    321.6 MB (+37.9 MB)
operator [] predicted              391.02 ns  lf=0.596    321.6 MB (+37.9 MB)
operator [] replace                192.39 ns  lf=0.596    359.5 MB

serial_erase                       233.56 ns  lf=0.000    359.5 MB
random_erase                       408.28 ns  lf=0.000    359.5 MB
erase_failed                        60.56 ns  lf=0.596    359.5 MB

toggle                              89.58 ns  lf=0.000    321.6 MB
iterate                              5.04 ns  lf=0.596    359.5 MB

absl::flat_hash_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                230.80 ns  lf=0.596
random_find_success                253.89 ns  lf=0.596
find_failed                        110.61 ns  lf=0.596
find_empty                          98.72 ns  lf=0.000

insert                             827.08 ns  lf=0.596    321.6 MB (+65.0 MB)
insert_predicted                   452.17 ns  lf=0.596    321.6 MB (+65.0 MB)
insert_replace                     259.56 ns  lf=0.596    386.6 MB

emplace                            837.94 ns  lf=0.596    321.6 MB (+65.0 MB)
emplace_predicted                  453.08 ns  lf=0.596    321.6 MB (+65.0 MB)
emplace_replace                    263.75 ns  lf=0.596    386.6 MB

try_emplace                        844.81 ns  lf=0.596    321.6 MB (+65.0 MB)
try_emplace_predicted              455.13 ns  lf=0.596    321.6 MB (+65.0 MB)
try_emplace_replace                276.28 ns  lf=0.596    386.6 MB

operator []                        848.00 ns  lf=0.596    321.6 MB (+65.0 MB)
operator [] predicted              468.85 ns  lf=0.596    321.6 MB (+65.0 MB)
operator [] replace                310.36 ns  lf=0.596    386.6 MB

serial_erase                       339.47 ns  lf=0.000    386.6 MB
random_erase                       383.27 ns  lf=0.000    386.6 MB
erase_failed                       111.34 ns  lf=0.596    386.6 MB

toggle                             219.64 ns  lf=0.000    321.6 MB
iterate                              8.64 ns  lf=0.596    386.6 MB

tsl::robin_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                137.66 ns  lf=0.298
random_find_success                152.69 ns  lf=0.298
find_failed                         84.61 ns  lf=0.298
find_empty                          64.28 ns  lf=0.000

insert                             799.91 ns  lf=0.298    321.6 MB (+144.0 MB)
insert_predicted                   530.71 ns  lf=0.298    321.6 MB (+144.0 MB)
insert_replace                     156.86 ns  lf=0.298    465.6 MB

emplace                            819.40 ns  lf=0.298    321.6 MB (+144.0 MB)
emplace_predicted                  547.61 ns  lf=0.298    321.6 MB (+144.0 MB)
emplace_replace                    286.09 ns  lf=0.298    465.6 MB

try_emplace                        821.42 ns  lf=0.298    321.6 MB (+144.0 MB)
try_emplace_predicted              550.94 ns  lf=0.298    321.6 MB (+144.0 MB)
try_emplace_replace                318.35 ns  lf=0.298    465.6 MB

operator []                        805.83 ns  lf=0.298    321.6 MB (+144.0 MB)
operator [] predicted              543.19 ns  lf=0.298    321.6 MB (+144.0 MB)
operator [] replace                208.13 ns  lf=0.298    465.6 MB

serial_erase                       244.36 ns  lf=0.000    465.6 MB
random_erase                       293.20 ns  lf=0.000    465.6 MB
erase_failed                        84.01 ns  lf=0.298    465.6 MB

toggle                              88.16 ns  lf=0.000    321.6 MB
iterate                             24.62 ns  lf=0.298    465.6 MB

robin_hood::unordered_map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                123.53 ns  lf=0.596
random_find_success                191.38 ns  lf=0.596
find_failed                         45.33 ns  lf=0.596
find_empty                          44.03 ns  lf=0.000

insert                             488.31 ns  lf=0.596    321.6 MB
insert_predicted                   283.82 ns  lf=0.596    321.6 MB
insert_replace                     244.06 ns  lf=0.596    321.6 MB

emplace                            492.58 ns  lf=0.596    321.6 MB
emplace_predicted                  281.34 ns  lf=0.596    321.6 MB
emplace_replace                    245.70 ns  lf=0.596    321.6 MB

try_emplace                        489.96 ns  lf=0.596    321.6 MB
try_emplace_predicted              293.96 ns  lf=0.596    321.6 MB
try_emplace_replace                370.16 ns  lf=0.596    321.6 MB

operator []                        490.98 ns  lf=0.596    321.6 MB
operator [] predicted              285.52 ns  lf=0.596    321.6 MB
operator [] replace                160.41 ns  lf=0.596    321.6 MB

serial_erase                       194.26 ns  lf=0.000    321.6 MB
random_erase                       367.20 ns  lf=0.000    321.6 MB
erase_failed                        43.32 ns  lf=0.596    321.6 MB

toggle                              84.36 ns  lf=0.000    321.6 MB
iterate                             14.80 ns  lf=0.596    321.6 MB

ankerl::unordered_dense::map<K, V> (32 byte objects, 64 byte ValueType, 625000 iterations):

serial_find_success                103.51 ns  lf=0.596
random_find_success                173.40 ns  lf=0.596
find_failed                         53.01 ns  lf=0.596
find_empty                           2.53 ns  lf=0.000

insert                             477.39 ns  lf=0.596    321.6 MB (+37.9 MB)
insert_predicted                   388.97 ns  lf=0.596    321.6 MB (+37.9 MB)
insert_replace                     239.37 ns  lf=0.596    359.5 MB

emplace                            585.83 ns  lf=0.596    321.6 MB (+37.9 MB)
emplace_predicted                  381.32 ns  lf=0.596    321.6 MB (+37.9 MB)
emplace_replace                    234.66 ns  lf=0.596    359.5 MB

try_emplace                        584.87 ns  lf=0.596    321.6 MB (+37.9 MB)
try_emplace_predicted              386.22 ns  lf=0.596    321.6 MB (+37.9 MB)
try_emplace_replace                365.08 ns  lf=0.596    359.5 MB

operator []                        581.21 ns  lf=0.596    321.6 MB (+37.9 MB)
operator [] predicted              383.87 ns  lf=0.596    321.6 MB (+37.9 MB)
operator [] replace                148.77 ns  lf=0.596    359.5 MB

serial_erase                       240.94 ns  lf=0.000    359.5 MB
random_erase                       448.79 ns  lf=0.000    359.5 MB
erase_failed                        50.10 ns  lf=0.596    359.5 MB

toggle                              75.81 ns  lf=0.000    321.6 MB
iterate                              5.04 ns  lf=0.596    359.5 MB

-----------------------------------------------------------------------------
```
