# Arch Linux 软件安装

---

## 1. 代理 (Clash Verge Rev)

```bash
sudo pacman -S clash-verge-rev
```

> 注意：2.4.4 版本有 TUN 安装 bug，建议使用 2.4.5 及以上。

离线安装：

```bash
sudo pacman -U clash*****.pkg.tar.zst
```

---

## 2. 文本编辑器 (Sublime Text 4)

```bash
yay -S sublime-text
```

### 汉化

1. 打开 Sublime Text
2. `Ctrl+Shift+P` → 输入 `install`
3. 稍等片刻（可能需重试多次）
4. 弹出插件框后输入 `chinese`，安装第一个中文描述插件

---

## 3. AUR 助手 (Paru)

```bash
git clone https://aur.archlinux.org/paru.git
```

```bash
cd paru
```

```bash
makepkg -si
```

---

## 4. 任务栏 (DMS - Dank Material Shell)

一键安装（可在国内网络加载界面后开代理下载）：

```bash
curl -fsSL https://install.danklinux.com | sh
```

> 手动安装参考：<https://danklinux.com/docs/dankmaterialshell/installation>

自启动：

```bash
systemctl --user enable dms.service
```

### 图标

```bash
paru -S qt6ct-kde
```

```bash
sudo pacman -S --needed papirus-icon-theme breeze-icons adwaita-icon-theme
```

验证图标设置：

```bash
echo $QT_QPA_PLATFORMTHEME
```

重启 DMS 使图标生效：

```bash
dms restart
```

### 输入法图标缺失修复

编辑 `/etc/environment`：

```bash
sudo nano /etc/environment
```

添加：

```ini
QT_QPA_PLATFORMTHEME=qt6ct
```

然后重启。

---

## 5. Niri 桌面配置

编辑 `~/.config/niri/config.kdl`：

```bash
nano ~/.config/niri/config.kdl
```

在开头加入：

```kdl
spawn-at-startup "bash" "-c" "wl-paste --watch cliphist store &"
spawn-at-startup "/usr/lib/polkit-kde-authentication-agent-1"

environment {
  XDG_CURRENT_DESKTOP "niri"
  QT_QPA_PLATFORM "wayland"
  ELECTRON_OZONE_PLATFORM_HINT "auto"
  QT_QPA_PLATFORMTHEME "qt6ct"
  QT_QPA_PLATFORMTHEME_QT6 "qt6ct"
}

layer-rule {
    match namespace="^quickshell$"
    place-within-backdrop true
}

layer-rule {
    match namespace="dms:blurwallpaper"
    place-within-backdrop true
}

window-rule {
    match app-id=r#"^org\.gnome\."#
    draw-border-with-background false
    geometry-corner-radius 12
    clip-to-geometry true
}

window-rule {
    match app-id=r#"^org\.wezfurlong\.wezterm$"#
    match app-id="Alacritty"
    match app-id="zen"
    match app-id="com.mitchellh.ghostty"
    match app-id="kitty"
    draw-border-with-background false
}

window-rule {
    match is-active=false
    opacity 0.97
}

window-rule {
    geometry-corner-radius 12
    clip-to-geometry true
}
```

在 `layout { }` 块中增加背景色：

```kdl
layout {
    background-color "transparent"
}
```

---

## 6. Thunar 终端集成

```bash
nano ~/.config/xfce4/helpers.rc
```

写入：

```ini
TerminalEmulator=alacritty
TerminalEmulatorDismissed=true
```

---

## 7. 输入法词库

安装搜狗词库：

```bash
yay -S fcitx5-pinyin-sougou-dict
```

---

## 8. 截图工具 (Flameshot)

```bash
sudo pacman -S flameshot grim
```

编辑配置：

```bash
nano ~/.config/flameshot/flameshot.ini
```

追加：

```ini
disabledGrimWarning=true
useGrimAdapter=true
```

### Niri 快捷键

编辑 `~/.config/niri/config.kdl`：

```bash
nano ~/.config/niri/config.kdl
```

在 `binds { }` 块中加入：

```kdl
Mod+A { spawn-sh "flameshot gui"; }
Mod+S { spawn-sh "flameshot gui -d 2000"; }
```

---

## 9. SDDM 主题

```bash
git clone -b main --depth=1 https://github.com/uiriansan/SilentSDDM && cd SilentSDDM && ./install.sh
```

> 样式预览：<https://github.com/uiriansan/SilentSDDM>

切换样式：

```bash
sudo nano /usr/share/sddm/themes/silent/metadata.desktop
```

将 `ConfigFile` 指向所需样式，例如：

```ini
ConfigFile=configs/rei.conf
```

---

## 10. 应用切换器 (Fuzzel) 主题

```bash
git clone https://github.com/dracula/fuzzel.git
```

克隆后将 `fuzzel` 文件夹移动到 `~/.config/` 即可。

---

## 11. 浏览器

Google Chrome：

```bash
yay -S google-chrome
```

Chromium：

```bash
sudo pacman -S chromium
```

Firefox：

```bash
sudo pacman -S firefox
```

---

## 12. 常用软件

LocalSend —— 局域网文件传输：

```bash
yay -S localsend-bin
```

KCalc —— 科学计算器：

```bash
sudo pacman -S kcalc
```

YesPlayMusic —— 第三方网易云播放器：

```bash
yay -S yesplaymusic
```

go-musicfox —— 终端音乐播放器：

```bash
yay -S go-musicfox
```

> 启动命令：`musicfox`
> 快捷键参考：<https://github.com/go-musicfox/go-musicfox/blob/master/README.md#%E5%BF%AB%E6%8D%B7%E9%94%AE>

Feem —— 局域网传输（需 snap）：

```bash
git clone https://aur.archlinux.org/snapd.git
```

```bash
cd snapd
```

```bash
makepkg -si
```

```bash
sudo systemctl enable --now snapd.socket
```

```bash
sudo ln -s /var/lib/snapd/snap /snap
```

```bash
sudo snap install feem
```

---

## 参考资源

- [Arch Linux 简明指南](https://arch.icekylin.online/)
