# Private Set Union

## Build

```shell
git submodule update --init --recursive 
cd extern/libOTe
python build.py --setup --boost --relic
```
The first build will failed, change `extern/libOTe/cryptoTools/thirdparty/relic/src/md/blake2.h`

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

