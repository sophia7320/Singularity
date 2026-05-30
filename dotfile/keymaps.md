# Neovim 键位速查表

## 你的自定义键位

| 按键 | 模式 | 来源 | 功能 | 覆盖 LazyVim？ |
|---|---|---|---|---|
| `<leader>w` | n | `keymaps.lua` | 保存文件 | ⚠️ 覆盖 `<leader>w` 窗口组 |
| `<leader>q` | n | `keymaps.lua` | 保存并退出 | ⚠️ 覆盖 `<leader>q` 退出组 |
| `<leader>f` | n, x | `formatter.lua` | 格式化 buffer | ⚠️ 覆盖 `<leader>f` 文件组 |
| `<Tab>` | i | `keymaps.lua` | 智能 Tab（弹出菜单可见时选下一项） | — |
| `gD` | n | `autocmds.lua` | LSP 跳转到声明 | 🔁 和 LazyVim 重复 |
| `gl` | n | `autocmds.lua` | 当前行诊断浮动窗 | — |
| `<leader>th` | n | `autocmds.lua` | 切换 Inlay Hints | — |
| `<leader>e` | n | `editing.lua` | Neo-tree 开关 | — |
| `<leader>E` | n | `editing.lua` | Neo-tree 定位当前文件 | — |
| `<leader>?` | n | `editing.lua` | Buffer 本地键位 | ⚠️ 覆盖同名键 |
| `<leader>yz` | n, v | `yazi.lua` | Yazi 打开当前文件 | — |
| `<leader>cw` | n | `yazi.lua` | Yazi 打开工作目录 | — |
| `<c-up>` | n | `yazi.lua` | 恢复上次 Yazi 会话 | — |

## LazyVim 默认键位

### 导航

| 按键 | 功能 |
|---|---|
| `<C-h/j/k/l>` | 窗口间跳转 |
| `<C-↑/↓/←/→>` | 调整窗口大小 |
| `<S-h>` | 上一个 buffer |
| `<S-l>` | 下一个 buffer |
| `[b` / `]b` | 上一个/下一个 buffer |
| `[q` / `]q` | 上一个/下一个 quickfix |
| `<leader>bb` | 切换到其他 buffer |
| `<leader>bd` | 删除 buffer |
| `<leader>bo` | 删除其他 buffer |
| `<leader>bD` | 删除 buffer 并关闭窗口 |

### 快速移动

| 按键 | 模式 | 功能 |
|---|---|---|
| `s` | n, x, o | Flash 跳转（输入字符，弹出标签，按标签直达） |
| `S` | n, o, x | Flash Treesitter 跳转（函数、类、块等） |
| `r` | o | 远程 Flash（operator-pending 模式） |
| `R` | o, x | Treesitter 搜索 |
| `<C-s>` | cmd | 命令行中切换 Flash 搜索 |
| `<C-space>` | n, o, x | Treesitter 增量选择 |
| `f` + 字符 | n | 跳到当前行下一个指定字符 |
| `F` + 字符 | n | 跳到当前行上一个 |
| `t` + 字符 | n | 跳到下一个指定字符之前 |
| `;` / `,` | n | 重复上次 f/t 正向/反向 |

### 编辑

| 按键 | 模式 | 功能 |
|---|---|---|
| `<A-j>` / `<A-k>` | n, i, v | 移动当前行向下/向上 |
| `<` / `>` | x | 缩进/反缩进（保持选中） |
| `gco` / `gcO` | n | 在下面/上面添加注释行 |
| `<C-s>` | i, x, n, s | 保存文件 |

### LSP

| 按键 | 功能 |
|---|---|
| `gd` | 跳转到定义 |
| `gr` | 查找引用 |
| `gI` | 跳转到实现 |
| `gy` | 跳转到类型定义 |
| `K` | 悬停提示 |
| `gK` / `<C-k>`(i) | 签名帮助 |
| `<leader>cr` | 重命名 |
| `<leader>ca` | 代码操作 |
| `<leader>cl` | LSP 信息 |
| `<leader>co` | 整理 imports |
| `<leader>cR` | 重命名文件 |
| `]]` / `[[` | 下一个/上一个引用 |
| `]d` / `[d` | 下一个/上一个诊断 |
| `]e` / `[e` | 下一个/上一个错误 |
| `]w` / `[w` | 下一个/上一个警告 |

### Git

| 按键 | 功能 |
|---|---|
| `<leader>gg` | Lazygit（项目根） |
| `<leader>gG` | Lazygit（当前目录） |
| `<leader>gl` | Git 日志 |
| `<leader>gb` | Blame 当前行 |
| `<leader>gf` | 当前文件历史 |
| `<leader>gB` | Git Browse 打开 |
| `<leader>gY` | Git Browse 复制链接 |
| `]h` / `[h` | 下一个/上一个 hunk |
| `<leader>ghs` | Stage hunk |
| `<leader>ghr` | Reset hunk |
| `<leader>ghS` | Stage 整个 buffer |
| `<leader>ghR` | Reset 整个 buffer |
| `<leader>ghb` | Blame 行 |
| `<leader>ghB` | Blame 整个 buffer |

### 搜索

| 按键 | 功能 |
|---|---|
| `<leader>sr` | 搜索替换（GrugFar） |

### 终端

| 按键 | 功能 |
|---|---|
| `<C-/>` | 终端 toggle（根目录） |
| `<leader>ft` | 终端（根目录） |
| `<leader>fT` | 终端（当前目录） |

### UI 开关

| 按键 | 功能 |
|---|---|
| `<leader>uf` | 切换自动格式化 |
| `<leader>uF` | 切换自动格式化（强制） |
| `<leader>us` | 切换拼写检查 |
| `<leader>uw` | 切换自动换行 |
| `<leader>uL` | 切换相对行号 |
| `<leader>ud` | 切换诊断显示 |
| `<leader>ul` | 切换行号 |
| `<leader>uc` | 切换 conceallevel |
| `<leader>ub` | 切换暗色背景 |
| `<leader>uD` | 切换 dim 模式 |
| `<leader>ua` | 切换动画 |
| `<leader>ug` | 切换缩进引导线 |
| `<leader>uS` | 切换滚动 |
| `<leader>uh` | 切换 Inlay Hints |
| `<leader>uz` | 切换禅模式 |
| `<leader>uZ` | 切换缩放模式 |
| `<leader>wm` | 同上（缩放） |

### 窗口 & 标签

| 按键 | 功能 |
|---|---|
| `<leader>-` | 水平分屏 |
| `<leader>\|` | 垂直分屏 |
| `<leader>wd` | 删除窗口 |
| `<leader><tab>l` | 最后一个标签 |
| `<leader><tab>f` | 第一个标签 |
| `<leader><tab>o` | 关闭其他标签 |
| `<leader><tab>d` | 关闭标签 |
| `<leader><tab><tab>` | 新建标签 |
| `<leader><tab>]` / `[` | 下一个/上一个标签 |

### Troubles & Diagnostics

| 按键 | 功能 |
|---|---|
| `<leader>xx` | Trouble 诊断面板（toggle） |
| `<leader>xX` | Trouble 当前 buffer 诊断 |
| `<leader>cs` | Trouble 符号面板 |
| `<leader>cS` | Trouble LSP 面板 |
| `<leader>xL` | Trouble 位置列表 |
| `<leader>xQ` | Trouble Quickfix 列表 |
| `<leader>xl` | 位置列表 toggle |
| `<leader>xq` | Quickfix 列表 toggle |

### TODO

| 按键 | 功能 |
|---|---|
| `]t` / `[t` | 下一个/上一个 TODO 注释 |
| `<leader>xt` | Trouble TODO 面板 |
| `<leader>xT` | Trouble TODO/FIX/FIXME 面板 |

### 其他

| 按键 | 功能 |
|---|---|
| `<leader>l` | 打开 Lazy |
| `<leader>L` | LazyVim 更新日志 |
| `<leader>fn` | 新建文件 |
| `<leader>cf` | 格式化（强制） |
| `<leader>qq` | 全部退出 |
| `<leader>ur` | 清除搜索高亮/重绘 |
| `<leader>ui` | 检查光标位置信息 |
| `<leader>uI` | 检查 Treesitter 树 |
| `<localleader>r` | 运行 Lua（仅 lua 文件） |

## DAP 调试

| 按键 | 功能 |
|---|---|
| `<leader>dB` | 切换断点 |
| `<leader>dc` | 启动调试 / 继续 |
| `<leader>di` | 步入 (Step Into) |
| `<leader>do` | 步过 (Step Over) |
| `<leader>dO` | 步出 (Step Out) |
| `<leader>dt` | 终止调试 |
| `<leader>dr` | 打开 REPL |
| `<leader>du` | 开关 DAP UI |

> 记忆：`d` = debug, `c` = continue, `B` = Breakpoint, `o` = step Over, `i` = step Into, `O` = step Out, `t` = terminate, `r` = repl, `u` = ui

## Rust 专属（仅在 .rs 文件）

| 按键 | 功能 |
|---|---|
| `K` | RustLsp 悬停提示 |
| `gd` | 跳转到定义 |
| `<leader>ca` | 代码操作 |
| `<leader>rr` | 运行测试/二进制 |

## Treesitter Textobjects

| 按键 | 模式 | 功能 |
|---|---|---|
| `af` | x, o | 选择外层函数 |
| `if` | x, o | 选择内层函数 |
| `ac` | x, o | 选择外层类 |
| `ic` | x, o | 选择内层类 |
| `]f` | n, x, o | 跳到下一个函数 |
| `[f` | n, x, o | 跳到上一个函数 |
| `]c` | n, x, o | 跳到下一个类 |
| `[c` | n, x, o | 跳到上一个类 |
| `<leader>sn` | n | 交换参数（向下） |
| `<leader>sp` | n | 交换参数（向上） |

## Mini.surround（环绕编辑）

| 按键 | 模式 | 功能 | 例子 |
|---|---|---|---|
| `gsa` | n, v | 添加包围 | `gsaiw"` → `hello` 变为 `"hello"` |
| `gsd` | n | 删除包围 | 光标在 `(hello)` → `hello` |
| `gsr` | n | 替换包围 | 光标在 `"hello"` → `'hello'` |
| `gsf` | n | 查找右侧包围 | |
| `gsF` | n | 查找左侧包围 | |

> 后缀 `l`（last）/ `n`（next）用于指定外层/内层

## Bufferline

| 按键 | 功能 |
|---|---|
| `<leader>bh` | 上一个 buffer |
| `<leader>bl` | 下一个 buffer |
| `<leader>bd` | 关闭 buffer |
| `<leader>bo` | 关闭其他 buffer |
| `<leader>bp` | Buffer 挑选 |
| `<leader>bc` | Buffer 挑选并关闭 |

## 图例

- ⚠️ 你的按键覆盖了 LazyVim 默认
- 🔁 和 LazyVim 重复定义
- n = normal, i = insert, x = visual, o = operator-pending, v = visual
