# Oblivous-Shuffle

Forked from [this repository](https://github.com/dujiajun/PSU), with an updated libOTe dependency and a streamlined codebase to focus solely on the OSN component.

## Build

```shell
git submodule update --init --recursive 
cd extern/libOTe
# in extern/libOTe
python build.py -- -D ENABLE_RELIC=ON -D ENABLE_NP=ON -D ENABLE_KOS=ON -D ENABLE_IKNP=ON -D ENABLE_SILENTOT=ON
cd ../..
# in PSU ROOT
cmake -B build .
cmake --build build -j
```


## Run OSN

Run OSN with data size 2^20 in 40 threads with cache:

```shell
build/test_osn -size 20 -thread 40 -cache 1
```
