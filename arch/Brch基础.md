# Arch Linux 基础配置

---

## 1. Thunar 文件管理器

安装 Thunar 及挂载支持：

```bash
sudo pacman -S thunar-volman gvfs
```

```bash
sudo systemctl enable udisks2.service
```

安装 Thunar 插件（归档、缩略图、预览等）：

```bash
sudo pacman -S thunar-volman gvfs thunar-archive-plugin file-roller ffmpegthumbnailer tumbler libgsf webp-pixbuf-loader ristretto
```

---

## 2. 汉化 & 本地化

安装谷歌开源字体及表情：

```bash
sudo pacman -S --needed noto-fonts noto-fonts-cjk noto-fonts-emoji noto-fonts-extra ttf-dejavu ttf-liberation
```

编辑 locale.gen，取消 `en_US.UTF-8` 和 `zh_CN.UTF-8` 前的 `#` 号：

```bash
sudo nano /etc/locale.gen
```

生成 locale：

```bash
sudo locale-gen
```

写入 locale.conf：

```bash
su
```

```bash
echo "LANG=zh_CN.UTF-8" > /etc/locale.conf
```

重启：

```bash
reboot
```

---

## 3. 终端字体 (Alacritty)

安装 Fira Code 字体：

```bash
sudo pacman -S ttf-fira-code
```

创建 alacritty 配置目录：

```bash
mkdir -p ~/.config/alacritty
```

编辑配置文件：

```bash
nano ~/.config/alacritty/alacritty.toml
```

```toml
[font]
normal = { family = "FiraCode", style = "Regular" }
bold = { family = "FiraCode", style = "bold" }
italic = { family = "FiraCode", style = "italic" }
bold_italic = { family = "FiraCode", style = "bold italic" }
size = 12

[general]
import = [
  "~/.config/alacritty/dank-theme.toml"
]

[window]
decorations = "None"
padding = { x = 15, y = 15 }
opacity = 0.9

[scrolling]
history = 3023

[cursor]
style = { shape = "Block", blinking = "On" }
blink_interval = 500
unfocused_hollow = true

[mouse]
hide_when_typing = true

[selection]
save_to_clipboard = true

[bell]
duration = 0

[keyboard]
bindings = [
  { key = "C",       mods = "Control|Shift", action = "Copy"  },
  { key = "V",       mods = "Control|Shift", action = "Paste" },
  { key = "N",       mods = "Control|Shift", action = "SpawnNewInstance" },
  { key = "Equals",  mods = "Control|Shift", action = "IncreaseFontSize" },
  { key = "Minus",   mods = "Control",       action = "DecreaseFontSize" },
  { key = "Key0",    mods = "Control",       action = "ResetFontSize"    },
  { key = "Enter",   mods = "Shift",         chars = "\n" },
]
```

> 参考：
> [FiraCode Linux Instructions](https://github.com/tonsky/FiraCode/wiki/Linux-instructions)
> [51CTO 教程](https://www.51cto.com/article/754779.html)

---

## 4. 时区设置

```bash
su
```

```bash
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
```

```bash
hwclock --systohc
```

---

## 5. 主机名 & Hosts

设置主机名（不要包含特殊字符及空格）：

```bash
sudo nano /etc/hostname
```

编辑 hosts 文件，将 `Arch` 替换为上面设置的主机名：

```bash
sudo nano /etc/hosts
```

```
127.0.0.1   localhost
::1         localhost
127.0.1.1   Arch.localdomain    Arch
```

---

## 6. 音频固件

```bash
sudo pacman -S --needed sof-firmware alsa-firmware alsa-ucm-conf pipewire-pulse pipewire-alsa wireplumber
```

---

## 7. 显卡驱动

### Intel 核显

```bash
sudo pacman -S --needed mesa lib32-mesa vulkan-intel lib32-vulkan-intel
```

### NVIDIA 显卡

dkms 版本（推荐）：

```bash
sudo pacman -S nvidia-dkms nvidia-utils nvidia-settings
```

普通版本：

```bash
sudo pacman -S nvidia nvidia-utils nvidia-settings
```

### 验证驱动

安装驱动后应无输出（nouveau 未被加载）：

```bash
lsmod | grep nouveau
```

---

## 8. 输入法 (fcitx5)

安装输入法及配置工具（提示选择时直接回车）：

```bash
sudo pacman -S --needed fcitx5-im fcitx5-chinese-addons fcitx5-pinyin-zhwiki fcitx5-configtool fcitx5-material-color
```

诊断输入法：

```bash
fcitx5-diagnose
```

编辑环境变量配置：

```bash
sudo nano /etc/profile
```

写入：

```bash
# export GTK_IM_MODULE=fcitx
export XMODIFIERS=@im=fcitx
export QT_IM_MODULE=fcitx
```

> 单行模式：在输入法设置 → 拼音设置 → 启用「在程序中显示预编辑文本」。

---

## 9. 权限授权 (Polkit)

安装授权包：

```bash
sudo pacman -S polkit polkit-kde-agent
```

编辑 Niri 配置文件：

```bash
nano ~/.config/niri/config.kdl
```

添加自启动行：

```kdl
spawn-at-startup "/usr/lib/polkit-kde-authentication-agent-1"
```

---

## 10. 家目录 (xdg-user-dirs)

安装：

```bash
sudo pacman -S xdg-user-dirs
```

生成中文家目录：

```bash
xdg-user-dirs-update
```

生成英文家目录：

```bash
LANG=en_US.UTF-8 xdg-user-dirs-update
```

> 如需修改已有家目录，先删除 `~/.config/user-dirs.locale` 和 `~/.config/user-dirs.dirs`。
> 修改路径：编辑 `~/.config/user-dirs.dirs`。

---

## 11. 双系统引导

编辑 GRUB 配置，取消最后一行的 `#` 号：

```bash
sudo nano /etc/default/grub
```

```ini
GRUB_DISABLE_OS_PROBER=false
```

安装 os-prober：

```bash
sudo pacman -S os-prober
```

切换 root：

```bash
su
```

查看分区：

```bash
lsblk
```

挂载 Windows EFI 分区（替换为实际分区名）：

```bash
mount /dev/nvme0n1p1 /mnt
```

探测系统：

```bash
os-prober
```

重新生成 GRUB 配置：

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

---

## 12. 开启 32 位支持库 & Arch Linux CN 仓库

编辑 pacman 配置：

```bash
sudo nano /etc/pacman.conf
```

### 开启 multilib

取消以下两行的 `#` 号：

```ini
[multilib]
Include = /etc/pacman.d/mirrorlist
```

### 可选优化

在 `[options]` 下取消注释：

```ini
Color
VerbosePkgLists
```

### 添加 Arch Linux CN 源

在文件末尾加入：

```ini
[archlinuxcn]
Server = https://mirrors.bfsu.edu.cn/archlinuxcn/$arch
Server = https://mirrors.ustc.edu.cn/archlinuxcn/$arch
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/$arch
Server = https://mirrors.hit.edu.cn/archlinuxcn/$arch
Server = https://repo.huaweicloud.com/archlinuxcn/$arch
```

安装 archlinuxcn 源密钥：

```bash
sudo pacman -S archlinuxcn-keyring
```

更新系统：

```bash
sudo pacman -Syyu
```

安装 yay（AUR 助手）：

```bash
sudo pacman -S yay
```

---

## 13. 蓝牙

> 推荐使用 Intel 无线网卡。Broadcom/Realtek 在 Linux 下兼容性较差。

安装蓝牙相关包：

```bash
sudo pacman -S bluez bluez-utils bluez-obex
```

开启蓝牙服务并设置自启动：

```bash
sudo systemctl enable --now bluetooth
```

安装蓝牙任务栏管理：

```bash
sudo pacman -S blueman
```
