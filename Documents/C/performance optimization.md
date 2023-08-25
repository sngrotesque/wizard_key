# C编译优化

### 如果不指定优化标志，gcc会产生可调试的代码
### 启用优化后，gcc会改变程序的结构，让代码变小或者运行得更快

-O0
> 不优化

-O, -O1
> 在不影响编译速度的情况下，尽量采取一些优化算法降低代码大小和加快代码运行的速度，并启用以下优化选项。

-O2
> 会牺牲一定的编译速度，除了执行O1的所有编译优化之外，还会采取几乎所有的目标配置支持的优化算法，用来提高目标代码的运行速度。

-O3
> 除了执行O2所有的优化选项之外，一般还会采用很多向量化算法，提高程序的并行度，并利用CPU的流水线和cache来加快运行速度。


fauto-inc-dec
> > 地址访问时，合并地址访问指令和地址的递增和递减操作

fbranch-count-reg
> 在计数寄存器中运行递减和分支指令，而不是单独运行递减指令，并与零比较，然后再根据结果进行分支

combine-stack-adjustments
> 跟踪堆栈的push和pop，并尝试找到合并它们的方法

fcompare-elim
> 判断计算结果的flag寄存器，如果可以，用flag寄存器的结果来代替现式地比较操作

fcprop-registers
> 通过寄存器存有的值，通过计算变量的依赖，减少从内存中读取该变量的值，通过拷贝传播，来减少变量的拷贝

-fdce
> 消除死代码

-fdefer-pop
> 程序一般在函数返回时，会pop出栈中元素，而编译器，会合并多个函数调用，并一次性pop出相关的元素

-fdelayed-branch
> 尝试对指令重新排序，来利用延迟分支指令后可用的指令槽

-fdse
> 对死代码进行消除

-fforward-propagate
> 该过程尝试将两条指令组合起来，并检查结果是否可以简化。如果循环展开处于活动状态，则执行两次循环，并在循环展开后安排第二次循环。

-fguess-branch-probability
> 分支预测

-fif-conversion2
> 利用条件执行把if条件转化为无分支等价代码

-fif-conversion
> 利用moves, min, max, set, abs等手段把if条件转化为无分支等价代码

-finline-functions-called-once
> 把静态函数转为inline函数

-fipa-pure-const
> 发现函数是纯函数还是常量函数

-fipa-profile
> 对cold(仅调用一次的函数)函数进行优化

-fipa-reference
> 启用全局变量引用分析，即分析全局变量在不同编译单元中的引用情况，以确定哪些全局变量可以被视为局部变量，从而减少对全局变量的引用和修改，提高性能和安全性。（属于-O2 -O3的一部分）

-fmerge-constants
> 合并常数计算

-fmove-loop-invariants
> 优化循环的不变量

-freorder-blocks
> 重排代码块

-fshrink-wrap
> 函数的开场白仅在使用前才初始化，而不是在函数开始的时候做

-fshrink-wrap-separate
> 把函数的开场白和结束语分开，尽在使用的时候才初始化

-fsplit-wide-types
> 对于某些需要占用多个寄存器的变量，独立的申请寄存器

-fssa-backprop
> 在定义链上传播相关使用的信息，来简化定义

-fssa-phiopt
> 优化条件代码

-fstore-merging
> 合并存储

-ftree-bit-ccp
> 在tree上传播稀疏条件位常量，并对齐传播指针

-ftree-ccp
> 在tree上传播稀疏条件常量，并对齐传播指针

-ftree-ch
> 启用循环头复制，即在循环开始之前复制循环头，以减少循环中的分支跳转。

-ftree-coalesce-vars
> 启用变量合并，即将多个具有相同作用域和类型的变量合并为一个变量，以减少内存使用和寄存器压力。

-ftree-copy-prop
> 启用拷贝传播，即将赋值语句中的右值替换为左值，以消除不必要的拷贝操作。

-ftree-dce
> 启用死代码消除，即删除不可能执行到的代码，以减少代码大小和提高性能。

-ftree-dominator-opts
> 启用支配树优化，即基于支配树结构对代码进行一系列的优化，如常量传播、条件传播、死代码消除等。

-ftree-dse
> 启用死存储消除，即删除对不会被使用的内存位置的写操作，以减少内存访问和提高性能。

-ftree-forwprop
> 启用前向传播，即将赋值语句中的左值替换为右值，以消除不必要的中间变量。

-ftree-fre
> 启用前向冗余消除，即将已知值替换为冗余计算的结果，以减少运算量和提高性能。

-ftree-phiprop
> 启用phi节点传播，即将SSA形式中的phi节点替换为常量或单一变量，以简化控制流和数据流。

-ftree-sink
> 启用语句下沉，即将只在某个分支中执行的语句移动到该分支内部，以减少分支外部的语句数量和执行次数。

-ftree-slsr
> 启用基于强度规约的线性扫描寄存器分配算法，即将复杂的寄存器操作替换为简单的寄存器操作，以提高寄存器利用率和性能。

-ftree-sra
> 启用标量替换聚合物，即将结构体或数组等聚合类型的变量替换为其成员或元素等标量类型的变量，以减少内存访问和提高性能。

-ftree-pta
> 启用基于树形结构的指针分析算法，即对指针变量进行别名分析、可达性分析、偏移量分析等，以辅助其他优化选项进行更精确的优化。

-ftree-ter
> 启用尾递归消除，即将尾递归调用转换为迭代循环，以减少函数调用开销和栈空间占用。

-funit-at-a-time
> 启用单元编译模式，即将每个编译单元（源文件）作为一个整体进行优化，而不是按照函数进行优化。这样可以进行更多的跨函数优化，如函数内联、死函数消除等。

-fcaller-saves
-fcrossjumping
-fcse-follow-jumps
-fcse-skip-blocks

-fthread-jumps
判断是否有double jump，并进行优化
-falign-functions
把函数的开始地址对齐到2的幂
-falign-jumps
把分支的起点地址对齐到2的幂
-falign-loops
把循环的开始地址对齐到2的幂
-falign-labels
把标签的地址对齐到2的幂
-fdelete-null-pointer-checks
检查是否存在delete空指针
-fdevirtualize -fdevirtualize-speculatively
用子类虚函数覆盖虚函数函数指针
-fexpensive-optimizations
-fgcse -fgcse-lm
全局的公用子表达式优化
-fhoist-adjacent-loads
-finline-small-functions
把小函数优化为内联函数
-findirect-inlining
-fipa-cp
过程间常数传播
-fipa-cp-alignment
-fipa-bit-cp
-fipa-sra
过程间优化，删除未使用的参数，将值传递优化为引用传递
-fipa-icf
-fisolate-erroneous-paths-dereference
检测由于取消空指针而触发错误或未定义行为的路径，将这些路径隔离
-flra-remat
通过上下文敏感，重用变量
-foptimize-sibling-calls
优化同级和尾部递归调用
-foptimize-strlen
优化strlen函数
-fpartial-inlining
部分内联函数
-fpeephole2
机器相关优化
-freorder-blocks-algorithm=stc
块内代码重排优化
-freorder-blocks-and-partition -freorder-functions
重排代码
-frerun-cse-after-loop
重新运行公共表达式减少在循环后
-fsched-interblock -fsched-spec
在不同块间进行重排指令
-fschedule-insns -fschedule-insns2
-fstrict-aliasing
类型严格转换检查
-fstrict-overflow
检查类型是否溢出
-ftree-builtin-call-dce
-ftree-switch-conversion -ftree-tail-merge
-fcode-hoisting
-ftree-pre
-ftree-vrp
-fipa-ra
函数调用者，如果某些寄存器没有用到，则可以不需要保存这些寄存器的值

-finline-functions
采用一些启发式算法对函数进行内联

-funswitch-loops
找到循环的不变条件，并移到循环外部

-fpredictive-commoning
在循环间重用计算

-fgcse-after-reload
执行全局的共同子表达式消除

-ftree-loop-vectorize　
向量化优化

-ftree-loop-distribute-patterns
把loop的某些部分转化为标准函数，例如memset

-fsplit-paths
-ftree-slp-vectorize
基本块向量化

-fvect-cost-model
-ftree-partial-pre
部分冗余代码消除

-fpeel-loops
-fipa-cp-clone
过程间调用函数克隆，让常量传播更有效

-Os
目的是尽量减少目标代码的大小，这对存储容量很小的设备尤为重要

-falign-functions
对齐

-falign-jumps
对齐

-falign-loops
对齐

-falign-labels
对齐

-freorder-blocks
重排

-freorder-blocks-algorithm=stc
重排

-freorder-blocks-and-partition
重排

-fprefetch-loop-arrays
预取

-Ofast
除了启用所有O3的优化选项之外，还会针对某些语言启用部分优化

-Og
目的是保留较好的可调试信息的前提下提供合理的优化

GCC常用选项
-E: 预处理
-S: 编译
-c: 编译 汇编 不链接
-g: 包含调试信息
-I: 指定include头文件目录
-o: 输出编译文件
-w: 忽略所有警告
-Werror: 不区分警告和错误，遇到任何警告都停止编译
-Wall: 开启大部分告警提示
-Wshadow: 作用域相关存在变量同名时发出告警
-Wextra: 对所有合法但值得怀疑的表达式发出告警

