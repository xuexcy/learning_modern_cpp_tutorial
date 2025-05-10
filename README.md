# learning_modern_cpp_tutorial
ref: https://github.com/changkun/modern-cpp-tutorial/tree/master
## Build And Run
- `env_variable.sh`: 设置环境变量，如 cmake 工作目录、可执行文件存放的 bin 文件夹目录等
- `build.sh`: 编译所有 src/*.cc
- `run.sh`: 运行所有编译后的可执行文件，并将结果输出到 stdout 目录
- `build_one.sh`: 编译 src 目录下的一个 .cc 文件，具体编译哪一个文件取决于 `env_variable.sh` 中的 `default_execute_file` 环境变量。 这个脚本主要是自己修改、测试某个 .cc 代码的时候使用，该文件里没有调用 cmake，如果编译的相关依赖发生改变，请重新执行 `build.sh` 生成新的 makefile 文件
- `run_one.sh`: 执行一个编译结果，同 `build_one.sh` 一样，具体哪个取决于 `env_variable.sh` 中的 `default_execute_file`
- `src/*.cc`: 各个 main 文件

## 现代 C++ 教程
1. 迈向现代 C++
2. 语言可用性的强化
3. 语言运行期的强化
4. 容器
5. 智能指针与内存管理
6. 正则表达式
7. 并行与并发
8. 文件系统
9. 其他杂项
10. 展望: C++20 简介
