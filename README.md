### threapool

#### 1.线程池项目编译
```shell
mkdir build
sh autobuild.sh
```

### 2.线程池项目内存泄漏检查
```shell
valgrind --tool=memcheck --leak-check=full --show-reachable=yes --trace-children=yes ./bin/test
```

