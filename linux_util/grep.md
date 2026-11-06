# grep 速查表

> 基线 GNU grep 3.x | 正则语法见 [正则表达式.md](../正则表达式.md) | 交互式搜索优先 `rg`，脚本/管道用 `grep`

---

## 模式类型

```bash
grep 'a.b' file          # BRE（默认）：. * ^ $ [] 生效，+ ? | () {} 需转义
grep -E 'a|b+' file      # ERE：+ ? | () {} 直接写，日常首选
grep -P '(?<=id=)\d+' f  # PCRE：环视、\d、\K 可用；GNU grep 需编译带 PCRE
grep -F 'a.b' file       # 固定串，不解析正则，最快
grep -G 'a\{2\}' file    # 显式 BRE
```

`\d` 只有 `-P` 有；GNU 扩展的 `\w \W \b \B \s \S \< \>` 在 BRE/ERE 下也能用。

---

## 选择与过滤

```bash
grep -i 'error' file                 # 忽略大小写
grep -w 'cat' file                   # 整词：不匹配 category
grep -x 'exact line' file            # 整行完全匹配
grep -v '^#' file                    # 反选
grep -e 'foo' -e 'bar' file          # 多模式（OR）
grep -f patterns.txt file            # 从文件读模式，一行一个
grep -E 'foo|bar' file
```

`-e` 能安全写「以 `-` 开头的模式」：`grep -e '-foo' file`；也可以用 `--` 结束选项。

---

## 输出控制

```bash
grep -n 'pat' file                   # 行号
grep -H 'pat' file                   # 强制显示文件名；-h 反过来隐藏
grep -c 'pat' file                   # 匹配行数（不是匹配次数）
grep -o 'pat' file                   # 只输出匹配部分，每处一行
grep -l 'pat' *.log                  # 只列有匹配的文件名；-L 列没有的
grep -q 'pat' file                   # 静默，靠退出码判断
grep -m 5 'pat' file                 # 命中 5 行即停，大文件提速明显
grep -b 'pat' file                   # 字节偏移
grep -A3 -B2 -C2 'pat' file          # 后 3 / 前 2 / 前后各 2 行
grep --color=auto 'pat' file
grep -T 'pat' file                   # 对齐制表符
```

---

## 递归与文件筛选

```bash
grep -r 'TODO' .                                   # -r 只跟随命令行上给的符号链接
grep -R 'TODO' .                                   # -R 跟随所有符号链接，可能绕圈
grep -r --include='*.py' --exclude='test_*' 'TODO' src/
grep -r --exclude-dir='.git' --exclude-dir=node_modules 'TODO' .
grep -r -I 'TODO' .                                # 跳过二进制文件
grep -r -s 'TODO' /etc 2>/dev/null                 # -s 抑制不可读文件的报错
```

`--include` / `--exclude` 匹配的是文件名（basename），不是路径；`--exclude-dir` 匹配目录名。

---

## 二进制与特殊输入

```bash
grep -a 'pat' binary            # 把二进制当文本
grep -I 'pat' binary            # 跳过二进制（等价 --binary-files=without-match）
grep --binary-files=text ...
grep -z 'pat' file              # 记录以 NUL 分隔（处理 find -print0 / xargs -0 的数据）
grep -lZ 'pat' ./* | xargs -0 ...   # 文件名用 NUL 分隔输出
grep -U 'pat' file              # 不做 CRLF 转换（Windows 场景）
```

默认遇到二进制只打印 `Binary file ... matches`；`-a` 与 `-I` 是两种相反的处理。

---

## 性能

- 能字面量就 `-F`，其次 `-E`，`-P` 最慢（回溯引擎）。
- ASCII 场景加 `LC_ALL=C grep ...`，避开多字节 locale 的开销；处理中文别加。
- 先缩小范围：`--include` / `--exclude-dir`，或 `find ... -print0 | xargs -0 grep`。
- 模式别以 `.*` 开头；需要「存在性」判断用 `-q` 或 `-m1` 提前退出。
- 大目录递归用 `rg`：并行、默认尊重 `.gitignore`、自动跳过二进制。
- 反复搜同一批文件，考虑 `git grep`（走索引）或建倒排索引。

---

## 退出码与脚本

| 退出码 | 含义 |
|---|---|
| 0 | 至少一行匹配 |
| 1 | 无匹配 |
| 2 | 出错（文件不存在、正则非法等） |

```bash
if grep -q 'pat' file; then ...; fi
grep -q 'pat' file || echo 'not found'      # set -e 下必须显式兜底
grep -c 'pat' file || true                  # 无匹配时 -c 仍打印 0，但退出码是 1
```

`grep -c` 无匹配会输出 `0` 且返回 1，管道里别被 `set -o pipefail` 坑到。

---

## 常用组合

```bash
grep -rniE 'error|warn' --include='*.log' /var/log | head
grep -oP '\d+' file | sort -n | uniq -c | sort -rn | head
grep -v -e '^#' -e '^$' config.ini            # 去注释与空行
grep -n 'pat' file | cut -d: -f1              # 只要行号
grep -c '' file                               # 行数（空模式匹配每行）
ps aux | grep '[n]ginx'                       # 方括号避免匹配 grep 自身
grep -rIl 'pat' . | xargs -r sed -i 's/pat/new/g'   # 先筛文件再改
```

`grep -rIl` = 递归 + 跳过二进制 + 只输出文件名，改文件前的标准前置。
