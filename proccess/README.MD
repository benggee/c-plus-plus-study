## 进程间通信
### 消息队列模型

- msgget()
- msgsnd()
- msgrcv()

创建一个消息队列对象

```
gcc -o msgget msgget.c
./msgget
```

使用ipcs -q 查看消息队列对象

```
ipcs -q

T     ID     KEY        MODE       OWNER    GROUP
Message Queues:
q  65536 0x00043dd6 --rw-rw-rw-    HEADS    staff
```

发送消息

```
gcc -o send msgsnd.c
./send -i 65536 -t 123  -m "hello world"
```

接收消息

```
gcc -o recv msgrcv.c
./recv -i 65536 -i 123 
```

## 信号

查看信号列表

```
man 7 signal
```