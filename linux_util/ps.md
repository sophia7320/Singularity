# ps 速查表

> 基线 procps-ng 4.x（Linux） | `ps` 是快照，持续观察用 `top` / `pidstat` | 示例 PID `1234`

---

## 三种语法

`ps` 同时吃三套写法，混用时以各自规则解析：

```bash
ps aux        # BSD：USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
ps -ef        # UNIX：UID PID PPID C STIME TTY TIME CMD
ps -eF        # UNIX 全量 + PPID/SZ/RSS/PSR
ps -ely       # 加 PRI/NI/WCHAN
ps -eo pid,cmd --sort=-pcpu   # GNU 长选项
```

默认只显示「当前 euid + 当前终端」的进程；`aux` / `-ef` 才是全量。BSD 选项里 `a` 放开「只看自己」，`x` 放开「必须有终端」，`u` 换用户格式；两个都加才是所有进程。

---

## 选择进程

```bash
ps -p 1234,5678 -o pid,stat,etime,cmd
ps --ppid 1                       # 某进程的子进程
ps -u alice                       # 有效用户（EUID）
ps -U alice                       # 真实用户（RUID）
ps -C nginx                       # 按可执行名（comm）匹配，不是命令行
ps -t pts/0                       # 按终端
ps -q 1234                        # quick：只读指定 PID，不做额外过滤/排序/树
ps -N -u root                     # 反选，等价 --deselect
ps -e -o pid,cmd | grep -v '\['
```

`-C` 匹配内核里的可执行名，`./nginx -c foo.conf` 和 `/usr/bin/nginx` 都算 `nginx`；要按参数找用 `pgrep -af` 或 `ps -eo args | grep`。

---

## 自定义列

```bash
ps -eo pid,ppid,user,stat,pcpu,pmem,rss,etime,args --sort=-pcpu | head
ps -o pid,ni,pri,rtprio,psr,cmd -p 1234
ps -o pid,stat,wchan:32,cmd -p 1234        # 卡在哪个内核函数
ps -o pid,lstart,etime,time,cmd -p 1234    # 启动时刻 / 已运行 / 累计 CPU
ps -o pid,rss,vsz,nlwp -p 1234
ps -o pid= -C nginx                        # 只要 PID，无表头
ps -o comm= -p 1234
ps -o cgroup -p 1234                       # 对应 systemd 单元/容器
```

列宽可加 `:N` 截断或 `=标题` 改名：`ps -o wchan:20=WAIT,cmd=COMMAND`。

常用列含义：

| 列 | 含义 |
|---|---|
| `pid` `ppid` `pgid` `sid` | 进程 / 父 / 进程组 / 会话 |
| `comm` | 内核记录的可执行名，不是完整命令行 |
| `args` `cmd` | 完整命令行（含参数） |
| `pcpu` | 生命周期平均 CPU：CPU 时间 / 已运行时间 |
| `pmem` | 常驻内存占物理内存百分比 |
| `rss` | 常驻物理内存（KB），含共享页 |
| `vsz` | 虚拟地址空间（KB），基本无参考价值 |
| `etime` / `etimes` | 已运行时长（格式化 / 秒） |
| `time` | 累计 CPU 时间，不是墙钟 |
| `lstart` / `start` | 完整启动时刻 / 当日时刻 |
| `stat` | 状态码，见下 |
| `wchan` | 睡眠中的内核等待点 |
| `psr` | 当前所在 CPU |
| `nlwp` | 线程数 |

---

## 排序、树、线程

```bash
ps -eo pid,pcpu,pmem,cmd --sort=-pcpu,-pmem
ps -eo pid,cmd --sort=start_time
ps -ejH                           # 树
ps --forest -e                    # 树（长选项）
ps auxf                           # BSD 树
ps -eLf                           # 线程：LWP / NLWP
ps -T -p 1234                     # 线程：SPID TID ...
ps -eLo pid,tid,psr,pcpu,comm --sort=-pcpu | head
```

`--sort` 的 `-` 为降序、`+` 为升序；多键逗号分隔。`-L` 与 `-T` 都列线程，`-T` 的 SPID 列即 TID。

---

## STAT 状态码

| 码 | 含义 |
|---|---|
| `R` | 运行或可运行（在运行队列） |
| `S` | 可中断睡眠（等事件） |
| `D` | 不可中断睡眠，通常卡 I/O，kill 不掉 |
| `T` | 被作业控制信号停止 |
| `t` | 被调试器停止 |
| `Z` | 僵尸：已退出，父进程未回收 |
| `I` | 空闲内核线程 |
| `X` | 已死（正常看不到） |
| `W` | 换页（2.6 后无意义） |

BSD 格式与 `stat` 列还会附加：

| 附加码 | 含义 |
|---|---|
| `<` | 高优先级（nice 值为负） |
| `N` | 低优先级（nice 值为正） |
| `L` | 有页面锁定在内存 |
| `s` | 会话首进程 |
| `l` | 多线程 |
| `+` | 位于前台进程组 |

`Ss` = 可中断睡眠 + 会话首进程；`Rsl` = 运行中 + 会话首进程 + 多线程。

---

## 常用组合

```bash
# 内存 Top 10
ps -eo pid,rss,pmem,comm --sort=-rss | head -11

# 按进程名汇总 RSS（注意共享页会重复计）
ps -eo comm,rss --no-headers | awk '{a[$1]+=$2} END{for(k in a) printf "%8d KB  %s\n", a[k], k}' | sort -rn | head

# 僵尸进程
ps -eo stat,pid,ppid,cmd | awk '$1 ~ /^Z/'

# 某进程的线程与 CPU 分布
ps -T -p 1234 -o spid,psr,pcpu,stat,comm

# 父子关系定位
ps -o pid,ppid,stat,cmd -p 1234 --ppid 1234

# 持续观察（top 更合适，这里只是示例）
watch -n1 'ps -eo pid,pcpu,pmem,cmd --sort=-pcpu | head'

# 进程树里找 systemd 单元
ps -eo pid,comm,cgroup --sort=pid | grep -i nginx
```

---

## 坑

- `%CPU`（BSD）与 `C`（UNIX）都是**生命周期平均**：`cputime/realtime`，后者取整；跑久了的进程会显得很低。瞬时占用看 `top`、`pidstat 1`。
- `ps` 每次都是快照，抓瞬时峰值要么循环采样，要么上 `pidstat`。
- `RSS` 含共享库与共享内存，多个进程相加会重复；精确值用 `smem` 的 PSS。
- `VSZ` 是虚地址空间，几十 GB 很正常，不代表真占内存。
- `comm` ≠ 命令行，`-C` 也只匹配 `comm`；完整参数用 `args`。
- 容器里 `ps` 受 PID namespace 限制，只能看到容器内进程；宿主机定位用 `ps -eo pid,cgroup`。
- `ps -p` 找不到进程时退出码为 1，脚本里注意 `set -e`。
- `ps aux` 与 `ps -ef` 的 `%CPU`/`C` 列同源，都是生命周期平均，只是整数与小数之差。
