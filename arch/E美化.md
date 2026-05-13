# Arch Linux 美化定制

---

## 1. 字体配置

### 安装 Maple Mono 字体

```bash
paru -S ttf-maplemononormalnl-nf-cn woff2-maplemononl
```

刷新字体缓存：

```bash
fc-cache -fv
```

### 字体配置文件

创建目录：

```bash
mkdir -p ~/.config/fontconfig && nano ~/.config/fontconfig/fonts.conf
```

写入：

```xml
<?xml version="1.0"?>
<!DOCTYPE fontconfig SYSTEM "urn:fontconfig:fonts.dtd">
<fontconfig>
  <!-- 1. 全局渲染设置 -->
  <match target="font">
    <edit name="antialias" mode="assign">
      <bool>true</bool>
    </edit>
    <edit name="hinting" mode="assign">
      <bool>true</bool>
    </edit>
    <edit name="hintstyle" mode="assign">
      <const>hintmedium</const>
    </edit>
    <edit name="rgba" mode="assign">
      <const>rgb</const>
    </edit>
    <edit name="autohint" mode="assign">
      <bool>false</bool>
    </edit>
  </match>

  <!-- 2. 默认字体族 -->
  <match target="pattern">
    <test qual="any" name="family">
      <string>Maple Mono Normal NL NF CN</string>
    </test>
    <edit name="family" mode="prepend" binding="strong">
      <string>Noto Sans</string>
      <string>DejaVu Sans</string>
    </edit>
  </match>

  <match target="pattern">
    <test qual="any" name="family">
      <string>Maple Mono Normal NL NF CN</string>
    </test>
    <edit name="family" mode="prepend" binding="strong">
      <string>Noto Serif</string>
      <string>DejaVu Serif</string>
    </edit>
  </match>

  <match target="pattern">
    <test qual="any" name="family">
      <string>JetBrains Mono</string>
    </test>
    <edit name="family" mode="prepend" binding="strong">
      <string>JetBrains Mono</string>
      <string>Source Code Pro</string>
    </edit>
  </match>

  <!-- 3. 字体替换规则 -->
  <match target="pattern">
    <test name="family" qual="any">
      <string>Arial</string>
    </test>
    <edit name="family" mode="replace">
      <string>Noto Sans</string>
    </edit>
  </match>
</fontconfig>
```

### 常用查询命令

查询字体引用名：

```bash
fc-list | grep "JetBrains Mono"
```

仅显示支持中文的字体：

```bash
fc-list :lang=zh
```

### 推荐字体资源

| 字体 | 地址 |
|------|------|
| Nerd Fonts | <https://www.nerdfonts.com/#features> |
| 霞鹜文楷 | <https://github.com/lxgw/Lxgwwenkai-screen> |
| Maple Font | <https://github.com/subframe7536/maple-font> |

> 参考：<https://geek-blogs.com/blog/arch-linux-font/>

---

## 2. 终端美化 (Zsh + Powerlevel10k)

### 安装

```bash
sudo pacman -S --needed oh-my-zsh-git zsh zsh-autosuggestions zsh-syntax-highlighting fontconfig
```

复制默认配置：

```bash
cp /usr/share/oh-my-zsh/zshrc ~/.zshrc
```

### 配置 ~/.zshrc

编辑配置文件：

```bash
nano ~/.zshrc
```

写入主题与插件配置：

```bash
ZSH_THEME="powerlevel10k/powerlevel10k"

source $ZSH/oh-my-zsh.sh
source /usr/share/zsh/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh
source /usr/share/zsh/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh

plugins=(
  git
  zsh-autosuggestions
  zsh-syntax-highlighting
)
```

### 安装 Powerlevel10k 字体

创建临时目录并下载字体：

```bash
mkdir -p ~/fontstext && cd ~/fontstext
```

```bash
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Regular.ttf &&
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold.ttf &&
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Italic.ttf &&
wget https://github.com/romkatv/powerlevel10k-media/raw/master/MesloLGS%20NF%20Bold%20Italic.ttf
```

移动字体到系统目录：

```bash
sudo mkdir -p /usr/share/fonts/truetype/MesloLGS && sudo mv *.ttf /usr/share/fonts/truetype/MesloLGS
```

刷新字体缓存：

```bash
fc-cache -fv
```

测试字体图标是否正常显示：

```bash
echo $'\uf115'
```

### 切换默认 Shell

```bash
chsh -s /bin/zsh
```

进入终端配置：

```bash
zsh
```

### 如提示 theme not found

```bash
sudo git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/themes/powerlevel10k
```

再次进入终端进行美化配置：

```bash
zsh
```

查看当前 Shell：

```bash
echo $SHELL
```

重新配置 Powerlevel10k：

```bash
p10k configure
```

---

## 3. Zsh 语法高亮自定义

编辑 `~/.zshrc`，在末尾追加以下内容：

```bash
nano ~/.zshrc
```

```bash
# 异步计算降低延迟
ZSH_HIGHLIGHT_MAX_BUFFER_LEN=1024

# 双击 Tab 逐词接受建议
bindkey '\t\t' forward-word

# pacman 自定义命令颜色
ZSH_HIGHLIGHT_STYLES[arg0]="fg=#21e6ff,bold"
ZSH_HIGHLIGHT_STYLES[precommand]="fg=#1ec5da"
ZSH_HIGHLIGHT_STYLES[path]="fg=#89d881,underline,bold"
ZSH_HIGHLIGHT_STYLES[commandseparator]="fg=#f2ff56,bold"

# 选项颜色
ZSH_HIGHLIGHT_STYLES[single-hyphen-option]="fg=#ed42cf,bold"
ZSH_HIGHLIGHT_STYLES[double-hyphen-option]="fg=#29fabb,bold"

# 错误提示
ZSH_HIGHLIGHT_STYLES[unknown-token]="fg=red,bold,underline"

# 字符串
ZSH_HIGHLIGHT_STYLES[single-quoted-argument]="fg=cyan"
ZSH_HIGHLIGHT_STYLES[double-quoted-argument]="fg=cyan"

# 括号层级颜色
ZSH_HIGHLIGHT_STYLES[bracket-level-1]="fg=blue,bold"
ZSH_HIGHLIGHT_STYLES[bracket-level-2]="fg=green,bold"
ZSH_HIGHLIGHT_STYLES[bracket-level-3]="fg=orange,bold"
ZSH_HIGHLIGHT_STYLES[bracket-level-4]="fg=yellow,bold"
ZSH_HIGHLIGHT_STYLES[bracket-level-5]="fg=cyan,bold"
ZSH_HIGHLIGHT_STYLES[bracket-level-6]="fg=purple,bold"

# 文件路径
ZSH_HIGHLIGHT_STYLES[autodirectory]="fg=green,bold,underline"
ZSH_HIGHLIGHT_STYLES[path-readonly]="fg=yellow"
```

> 颜色参考：  
> [zsh-syntax-highlighting 文档](https://gitcode.com/gh_mirrors/zs/zsh-syntax-highlighting/blob/5eb677bb0fa9a3e60f0eff031dc13926e093df92/docs/highlighters/main.md)  
> 默认配置位置：`/usr/share/zsh/plugins/zsh-syntax-highlighting/highlighters/main/main-highlighter.zsh`

---

## 4. 鼠标光标

将光标主题文件夹移动到 icons 目录（例：Nier）：

```bash
sudo mv Nier /usr/share/icons
```

用户级配置：

```bash
mkdir -p ~/.icons/default
```

```bash
sudo nano ~/.icons/default/index.theme
```

写入：

```ini
[Icon Theme]
Name=Nier
```

系统级配置：

```bash
sudo nano /usr/share/icons/default/index.theme
```

写入：

```ini
[Icon Theme]
Name=Nier
```

> 详细参考：Arch Wiki 鼠标光标条目。

---

## 5. 罗技鼠标

```bash
sudo pacman -S solaar
```

---

## 6. GRUB 美化

推荐主题：

- [Wuthering GRUB2 Theme](https://github.com/vinceliuice/Wuthering-grub2-themes)
- [StarRail GRUB Theme（Saber）](https://github.com/voidlhf/StarRailGrubThemes)

### 安装示例（Saber 主题）

下载并解压后，移动到主题目录：

```bash
sudo mv Saber /usr/share/grub/themes/
```

编辑 GRUB 配置：

```bash
sudo nano /etc/default/grub
```

末尾添加主题路径：

```ini
GRUB_THEME="/usr/share/grub/themes/Saber/theme.txt"
```

更新 GRUB：

```bash
sudo grub-mkconfig -o /boot/grub/grub.cfg
```
