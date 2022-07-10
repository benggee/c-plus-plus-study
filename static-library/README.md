# 关于 Conan 环境的安装说明

以下只针对 Windows 环境，使用其他环境的同学无须阅读本文档。

由于 libcurl 的构建脚本现在已经禁用了 cmake，因此同学们在 MinGW 环境下使用 conan 安装 libcurl 依赖时会遇到错误，因此同学们直接下载我构建好的包即可，地址如下：

> 链接：https://share.weiyun.com/h6tFSjc1 密码：y3a2qp

文件下载之后，找到 C:\Users\【你的用户名】\.conan\data，如果这个目录不存在，就手动创建一下，把下载的压缩包解压到其中即可，得到的目录结果如下：

```
data
├── libcurl
│     ├── 7.70.0
│     ├── 7.71.0
│     └── 7.72.0
├── openssl
│     └── 1.1.1k
├── sqlite3
│     └── 3.32.3
├── tinycthreadpool
│     └── 1.0
└── zlib
      └── 1.2.11
```