# Arch Linux 安装指南

---

## 方式一：脚本安装

```bash
archinstall
```

交互式脚本，按提示操作即可。

---

## 方式二：手动安装

支持 **ext4** 或 **btrfs** 文件系统。

### 1. 联网

禁用 reflector 服务（避免自动换源失败）：

```bash
systemctl stop reflector.service
```

解锁被内核屏蔽的 WiFi 固件：

```bash
rfkill unblock wifi
```

进入 iwctl 连接模式：

```bash
iwctl
```

列出网卡名（假设为 wlan0，后续需替换为实际名称）：

```bash
device list
```

扫描网络（正常无输出）：

```bash
station wlan0 scan
```

列出可用网络：

```bash
station wlan0 get-networks
```

连接 WiFi（输入密码后回车）：

```bash
station wlan0 connect <WiFi名称>
```

退出连接模式：

```bash
exit
```

测试网络连通性（Ctrl+C 退出）：

```bash
ping bing.com
```

---

### 2. 时间同步

```bash
timedatectl
```

---

### 3. 换软件源

使用中国镜像：

```bash
reflector --country China --save /etc/pacman.d/mirrorlist
```

检查源列表：

```bash
cat /etc/pacman.d/mirrorlist
```

生效源：

```bash
sudo pacman -Sy
```

---

### 4. 分区

查看硬盘信息：

```bash
lsblk
```

使用 cfdisk 分区（替换 nvme0n1 为实际硬盘名）：

```bash
cfdisk /dev/nvme0n1
```

建议分区方案：

| 分区 | 类型        | 大小建议     |
|------|------------|-------------|
| p1   | EFI (启动)  | 512M ~ 1G   |
| p2   | Swap (交换) | 内存大小以上   |
| p3   | 根目录       | 剩余空间     |

---

### 5. 格式化 & 挂载

#### 方案 A：ext4

格式化 EFI 分区：

```bash
mkfs.fat -F32 /dev/nvme0n1p1
```

格式化 Swap 分区：

```bash
mkswap /dev/nvme0n1p2
```

格式化根目录：

```bash
mkfs.ext4 /dev/nvme0n1p3
```

挂载根目录：

```bash
mount /dev/nvme0n1p3 /mnt
```

```bash
mkdir -p /mnt/boot
```

```bash
mount /dev/nvme0n1p1 /mnt/boot
```

启用 swap：

```bash
swapon /dev/nvme0n1p2
```

复查分区与 swap：

```bash
df -h
```

```bash
free -h
```

#### 方案 B：btrfs（推荐）

格式化 EFI 分区：

```bash
mkfs.fat -F32 /dev/nvme0n1p1
```

格式化根目录为 btrfs：

```bash
mkfs.btrfs /dev/nvme0n1p3
```

挂载 btrfs 根分区：

```bash
mount /dev/nvme0n1p3 /mnt
```

创建 btrfs 子卷：

```bash
btrfs subvolume create /mnt/@
```

```bash
btrfs subvolume create /mnt/@home
```

```bash
btrfs subvolume create /mnt/@snapshots
```

```bash
btrfs subvolume create /mnt/@log
```

卸载根分区：

```bash
umount /mnt
```

挂载子卷（compress=zstd 启用透明压缩，noatime 减少写入）：

```bash
mount -o subvol=@,compress=zstd,noatime /dev/nvme0n1p3 /mnt
```

```bash
mkdir -p /mnt/{boot,home,.snapshots,var/log}
```

```bash
mount -o subvol=@home,compress=zstd,noatime /dev/nvme0n1p3 /mnt/home
```

```bash
mount -o subvol=@snapshots,compress=zstd,noatime /dev/nvme0n1p3 /mnt/.snapshots
```

```bash
mount -o subvol=@log,compress=zstd,noatime /dev/nvme0n1p3 /mnt/var/log
```

```bash
mkdir -p /mnt/boot
```

```bash
mount /dev/nvme0n1p1 /mnt/boot
```

启用 swap：

```bash
swapon /dev/nvme0n1p2
```

复查：

```bash
df -h
```

```bash
free -h
```

---

### 6. 安装基础系统

使用 lts 长期内核，注重稳定性（btrfs 用户末尾追加 `btrfs-progs`）：

```bash
pacstrap -K /mnt base base-devel linux-firmware linux-lts linux-lts-headers intel-ucode
```

挂载信息写入系统：

```bash
genfstab -U /mnt >> /mnt/etc/fstab
```

复查 fstab：

```bash
cat /mnt/etc/fstab
```

进入新系统：

```bash
arch-chroot /mnt
```

---

### 7. 基础配置

安装文本编辑器与网络管理：

```bash
pacman -S nano networkmanager
```

开机自启网络：

```bash
systemctl enable NetworkManager
```

设置 root 密码：

```bash
passwd root
```

---

### 8. 建立启动引导 (GRUB)

安装 GRUB（双系统需追加 `os-prober`）：

```bash
pacman -S grub efibootmgr
```

安装引导：

```bash
grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=Arch
```

> 正常输出：`Installation finished. No error reported.`

编辑 GRUB 配置文件：

```bash
nano /etc/default/grub
```

修改 `GRUB_CMDLINE_LINUX_DEFAULT` 行：

```ini
GRUB_CMDLINE_LINUX_DEFAULT="loglevel=5 nowatchdog"
```

- 去掉 `quiet` 参数
- `loglevel` 改为 `5`（便于排错）
- 加入 `nowatchdog`（显著提高开关机速度）

生成 GRUB 配置文件：

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

退出安装环境：

```bash
exit
```

卸载新分区：

```bash
umount -R /mnt
```

重启：

```bash
reboot
```

---

### 9. 联网（重启后）

使用 nmtui 配置网络：

```bash
nmtui
```

检查网络连通性：

```bash
ping bing.com
```

---

### 10. 新建用户

> root 用户默认隐藏，安装图形界面需要创建普通用户。

添加用户（例：fufumi）：

```bash
useradd -m -G wheel -s /bin/bash fufumi
```

设置用户密码：

```bash
passwd fufumi
```

设置 sudo 权限：

```bash
EDITOR=nano visudo
```

取消下面这行的注释：

```ini
#%wheel ALL=(ALL:ALL) ALL
```

---

### 11. 安装图形桌面

#### 方案 A：Niri 桌面（轻量平铺式 Wayland）

安装基础桌面（选项时先输入 2，再输入 1）：

```bash
pacman -S sddm niri alacritty fuzzel thunar xwayland-satellite egl-wayland
```

#### 方案 B：KDE Plasma 桌面（功能完整）

```bash
pacman -S --needed sddm xorg plasma-meta konsole kate dolphin ark sudo plasma-workspace xdg-desktop-portal egl-wayland
```

设置 SDDM 自启动：

```bash
systemctl enable sddm
```

---

### 12. 显卡驱动

> 建议装完桌面后立即安装，避免重启黑屏。
> 若黑屏，按 `Ctrl+Alt+F2~F6` 进入 TTY 终端补救。

#### NVIDIA 显卡（dkms 版本，推荐）

```bash
sudo pacman -S nvidia-dkms nvidia-utils nvidia-settings
```

#### NVIDIA 显卡（普通版本）

```bash
sudo pacman -S nvidia nvidia-utils nvidia-settings
```

#### Intel 核显

```bash
sudo pacman -S --needed mesa lib32-mesa vulkan-intel lib32-vulkan-intel
```

最后重启：

```bash
reboot
```

---

## 附录：虚拟机 (VMware)

检查是否 UEFI 启动（正常输出 32 或 64）：

```bash
cat /sys/firmware/efi/fw_platform_size
```

### VMware Tools

```bash
sudo pacman -Sy open-vm-tools
```

```bash
sudo systemctl enable --now vmtoolsd.service
```

```bash
sudo systemctl enable --now vmware-vmblock-fuse.service
```

### SSH 配置

检查 SSH 状态：

```bash
sudo systemctl status sshd
```

启动 SSH：

```bash
sudo systemctl start sshd
```

编辑 SSH 配置：

```bash
sudo nano /etc/ssh/sshd_config
```

```ini
PermitRootLogin yes
PasswordAuthentication yes
```

重启 SSH 服务：

```bash
sudo systemctl restart sshd
```

设置 root 密码并查看 IP：

```bash
passwd
```

```bash
ip a
```

### PowerShell 连接

```powershell
ssh root@<Arch的IP>
```

若出现 `WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!` 警告：

```powershell
ssh-keygen -R <目标IP>
```

### SSH 安装（如未安装）

```bash
pacman -S openssh
```

```bash
systemctl start sshd
```
