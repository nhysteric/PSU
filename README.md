# Private Set Union

## Build

```shell
git clone https://github.com/dujiajun/PSU
cd PSU

git clone https://github.com/osu-crypto/libOTe
cd libOTe
git checkout 3a40823f0507710193d5b90e6917878853a2f836

git clone https://github.com/ladnir/cryptotools
cd cryptotools
git checkout 4a83de286d05669678364173f9fdfe45a44ddbc6

cd ..
# in extern/libOTe
python build.py --setup --boost --relic
```
First build will failed, change `extern/libOTe/cryptoTools/thirdparty/relic/src/md/blake2.h`

```cpp
// changed line 64:
typedef struct ALIGNME( 64 ) __blake2s_state
// and line 89:
typedef struct ALIGNME( 64 ) __blake2b_state
```

And build again:

```shell
# in extern/libOTe
python build.py --setup --boost --relic
python build.py -- -D ENABLE_RELIC=ON -D ENABLE_NP=ON -D ENABLE_KOS=ON -D ENABLE_IKNP=ON -D ENABLE_SILENTOT=ON
cd ../..
# in PSU ROOT
cmake -B build .
cmake --build build -j 40  
```


## Run OSN

Run OSN with data size 2^20 in 40 threads:

```shell
build/test_osn 20 40
```

