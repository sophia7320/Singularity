# sed 速查表

> 基线 GNU sed 4.9+ | 默认 BRE，日常一律 `-E` | 正则语法见 [正则表达式.md](../正则表达式.md) | 原地改文件前先不加 `-i` 看输出

---

## 基本形式

```bash
sed -E 's/foo/bar/' file        # 每行第一个
sed -E 's/foo/bar/g' file       # 全部
sed -n '1,10p' file             # -n 关闭自动打印，只输出指定行
sed -i -E 's/\s+$//' file       # 原地修改
```

| 选项 | 作用 |
|---|---|
| `-n` | 不自动打印模式空间 |
| `-E` / `-r` | ERE |
| `-i[SUFFIX]` | 原地修改，带后缀则备份 `fileSUFFIX` |
| `-s` | 多文件各自独立（行号、`$` 不跨文件） |
| `-z` | 记录以 NUL 分隔，模式空间里能见到换行 |
| `-u` | 不缓冲输出 |
| `--debug` | 打印脚本执行过程 |
| `--sandbox` | 禁用 `e`/`r`/`w`，跑不可信脚本时用 |

`-i` 与 `-E` 顺序无关；`-i` 走临时文件 + rename，会换 inode（硬链接断开）。符号链接默认被替换成普通文件，`--follow-symlinks` 才改链接指向的目标。

---

## 地址

```bash
sed -n '5p'                     # 第 5 行
sed -n '$p'                     # 最后一行
sed -n '1,5p'                   # 1..5
sed -n '5,$p'                   # 5 到结尾
sed -n '/re/p'                  # 匹配行
sed -n '/start/,/end/p'         # 区间（含两端）
sed -n '0,/re/p'                # GNU：从第 1 行起，re 首次命中即止
sed -n '10,+5p'                 # GNU：第 10 行及后 5 行
sed -n '2,~4p'                  # GNU：第 2 行到 4 的倍数
sed -n '1~3p'                   # GNU：1,4,7,...
sed -n '/re/!p'                 # 取反
sed -n '/re/{p;q}'              # 首个匹配后退出
sed -e '5d' -e '10d'            # 多个脚本用 -e
sed -f script.sed file
```

`0,/re/` 与 `1,/re/` 的区别：后者不会在首行就测试结束地址，首行命中时会一直跑到第二次命中。

---

## s 命令

```bash
sed -E 's/a/b/'                 # 每行第 1 个
sed -E 's/a/b/g'                # 每行全部
sed -E 's/a/b/3'                # 每行第 3 个
sed -E 's/a/b/3g'               # GNU：第 3 个起全部
sed -E 's/a/b/I'                # GNU：忽略大小写
sed -E 's/a/b/M'                # GNU：多行模式，^ $ 按行
sed -E 's/a/b/p'                # 配合 -n，只打印发生替换的行
sed -E 's/a/b/w out.txt'        # 写入文件
sed -E 's/date/date +%F/e'      # GNU：执行替换结果并把输出作为替换内容（危险）
```

分隔符可换成任意字符，路径场景省转义：

```bash
sed -E 's#/usr/local#/opt#g' file
sed -E 's|a/b|c/d|' file
```

替换串：

```bash
sed -E 's/(\w+)@(\w+)/\2 at \1/'      # 分组引用
sed -E 's/foo/[&]/'                   # & = 整个匹配
sed -E 's/&/and/g'                    # 字面 & 要写 \&
sed -E 's/.*/\U&/'                    # GNU 大小写：\U 全大写 \L 全小写 \u 首字母大写 \l 首字母小写 \E 结束
sed -E 's/(\w+)/\u\1/'                # 首字母大写
sed -E 's/\\/\\\\/g'                  # 反斜杠加倍
```

---

## 常用动作

```bash
sed '/re/d'                     # 删除匹配行
sed '/re/!d'                    # 只保留匹配行
sed '5d;10d'
sed '$d'
sed '/re/a\追加行'              # GNU 单行写法；a 在匹配行后追加
sed '/re/i\插入行'
sed '/re/c\整行替换'
sed 'y/abc/xyz/'                # 逐字符映射，两串长度必须相等
sed '/re/q'                     # 打印当前行后退出
sed '10q'                       # 打印前 10 行并提前退出
sed '=' file | sed 'N;s/\n/\t/' # 加行号
```

`a`/`i`/`c` 的多行写法：`sed '/re/a\` 换行 `第一行\` 换行 `第二行'`；GNU 的 `a\文本` 单行形式更常用。

---

## 多行与保持空间

```bash
sed 'N;s/\n/ /' file                    # 两行并一行
sed -z 's/\n/,/g' file                  # 整个文件当一条记录，换行替换成逗号
sed -n '/start/{:a;N;/end/!ba;p}' file  # 提取 start..end 块
sed -E '/^$/{N;/^\n$/D}' file           # 压缩连续空行
```

| 命令 | 作用 |
|---|---|
| `n` / `N` | 读下一行到模式空间（`n` 先打印再读，`N` 直接追加） |
| `p` / `P` | 打印模式空间 / 只打印到第一个换行 |
| `d` / `D` | 删除并开始下一轮 / 只删到第一个换行再继续 |
| `h` `H` / `g` `G` | 复制/追加到保持空间 / 从保持空间复制/追加回来 |
| `x` | 交换模式空间与保持空间 |
| `:label` `b` `t` `T` | 标签、跳转、替换成功跳转、替换失败跳转（GNU） |

---

## 原地修改与批量

```bash
sed -i 's/x/y/' file
sed -i.bak -E 's/\s+$//' file           # 留备份 file.bak
sed -i -E '/^\s*#/d; /^\s*$/d' conf     # 去注释与空行
find . -name '*.conf' -exec sed -i 's/a/b/' {} +
sed -i -s 's/a/b/' a.txt b.txt          # 每文件独立行号
sed -i 's/\r$//' file                   # 去 CRLF 的 CR
sed -i '1s/^\xEF\xBB\xBF//' file        # 去 UTF-8 BOM
```

批量替换先 dry-run：`grep -rIl 'old' . | xargs -r sed -n 's/old/new/p'`。

---

## 常用组合

```bash
# 去首尾空白
sed -E 's/^\s+|\s+$//g' file

# 每行加前缀
sed 's/^/PREFIX /' file

# 只改第 5 行
sed '5s/a/b/' file

# 删除最后一行
sed '$d' file

# 取标记之间（含标记）
sed -n '/BEGIN/,/END/p' file

# 取标记之间（不含标记）
sed -n '/BEGIN/,/END/{/BEGIN/d; /END/d; p}' file

# 大文件取前 10 行
sed '10q' file

# 把空行变成分隔符
sed -E 's/^$/- - -/' file
```

---

## 坑

- `s///` 默认只换每行第一个，忘了 `g` 是最常见错误。
- BRE 下 `+ ? | ( ) { }` 都是字面量，要么转义，要么统一 `-E`。
- 替换串里 `&` 是「整个匹配」，要字面 `&` 写 `\&`。
- `\d` 不存在，用 `[0-9]` 或 `[[:digit:]]`；`\s \w \b` 是 GNU 扩展。
- 普通模式下 sed 按行处理，模式里看不到 `\n`；跨行用 `N` 或 `-z`。
- `sed -n '1,10p'` 会读完整文件；要提前退出用 `sed '10q'`。
- `sed -i` 换 inode，硬链接会断；符号链接默认被替换成本身，加 `--follow-symlinks` 才改目标。
- 在管道里 `-i` 无意义（没有文件名），只能改输出。
- 地址与命令之间不能有空格：`sed -n '1,5 p'` 报错。
