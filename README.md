# SkipList-KV
SkipList实现KV存储

基于 跳表 实现的 KV 存储引擎，使用 C++ 实现。
基本功能如下：

"-----------------SkipList Test-----------------" 

"Enter command: (1) insert, (2) search,  (3) delete " 

"               (4) update, (5) display, (6) dump "   

"               (7) load,   (8) clear,   (9) help: "  

"                           (0) size               "  

"-----------------------------------------------" 

在KV-Value-2中，受到前两个项目的启发，我将原本的KV存储操作改进为了上面这种，命令行式的执行命令。

然后又将这种方式，分别改写成了TCP/UDP连接的模式，在客户端输入要执行的命令，在服务器端进行相应的KV存储操作。
