# 系统优化 & 工具配置

---

## 1. DNS 设置

编辑 resolv.conf：

```bash
sudo nano /etc/resolv.conf
```

删除已有条目，加入：

```
nameserver 223.5.5.5
nameserver 114.114.115.115
nameserver 1.1.1.1
```

### 防止被覆盖

若路由器自动覆盖 `/etc/resolv.conf`，加不可变标志：

```bash
sudo chattr +i /etc/resolv.conf
```

需要编辑时先解锁：

```bash
sudo chattr -i /etc/resolv.conf
```

---

## 2. 视频播放 & 录屏

mpv 播放器：

```bash
sudo pacman -S mpv
```

OBS Studio：

```bash
sudo pacman -S obs-studio
```

---

## 3. 文件系统支持

NTFS / exFAT 格式支持：

```bash
sudo pacman -S ntfs-3g exfat-utils
```

---

## 4. JDK 安装 (Amazon Corretto 21)

```bash
yay -S amazon-corretto-21-bin
```

查看已安装 Java：

```bash
archlinux-java status
```

设置默认版本：

```bash
sudo archlinux-java set <JAVA_ENV_NAME>
```

> 官网：<https://aws.amazon.com/cn/corretto/>

---

## 5. 休眠到硬盘

> 前提：已设置 swap 交换分区。

查看 swap 分区 UUID：

```bash
lsblk -o name,mountpoint,size,uuid
```

编辑 GRUB 配置：

```bash
sudo nano /etc/default/grub
```

在 `GRUB_CMDLINE_LINUX_DEFAULT` 值中添加（替换为实际 UUID）：

```
resume=UUID=4c2d3cf9-995a-4fea-8bc2-e54847159b91
```

生效 GRUB 配置：

```bash
sudo grub-mkconfig -o /boot/grub/grub.cfg
```

编辑 mkinitcpio 配置：

```bash
sudo nano /etc/mkinitcpio.conf
```

在 `HOOKS` 行添加 `resume`（置于 `udev` 之后）。

> 若使用 btrfs 文件系统，还需确保 `HOOKS` 中包含 `btrfs`（置于 `block` 之后、`filesystems` 之前）。

重新生成 initramfs：

```bash
sudo mkinitcpio -P
```

执行休眠：

```bash
systemctl hibernate
```

---

## 6. 缩短关机等待时间

编辑配置：

```bash
sudo nano /etc/systemd/system.conf
```

取消注释并修改：

```ini
DefaultTimeoutStopSec=10s
```

重载配置：

```bash
sudo systemctl daemon-reload
```

---

## 7. BBR 网络优化

查看当前拥塞控制算法：

```bash
sysctl net.ipv4.tcp_congestion_control
```

加载 BBR 模块：

```bash
sudo modprobe tcp_bbr
```

新建配置文件：

```bash
sudo nano /etc/sysctl.d/30-bbr.conf
```

写入：

```ini
net.core.default_qdisc = cake
net.ipv4.tcp_congestion_control = bbr
```

---

## 8. IPv6 隐私拓展

查看网卡名（三种方式任选）：

```bash
ls /sys/class/net
```

```bash
ip link
```

```bash
iw dev
```

新建配置文件：

```bash
sudo nano /etc/sysctl.d/40-ipv6.conf
```

将 `wlan0` 替换为实际网卡名后写入：

```ini
net.ipv6.conf.all.use_tempaddr = 2
net.ipv6.conf.default.use_tempaddr = 2
net.ipv6.conf.wlan0.use_tempaddr = 2
```

> 参考：<https://wiki.archlinuxcn.org/zh-tw/IPv6>

---

## 9. 随机 MAC 地址

新建配置文件：

```bash
sudo nano /etc/NetworkManager/conf.d/rand_mac.conf
```

写入：

```ini
[device-mac-randomization]
wifi.scan-rand-mac-address=yes

[connection-mac-randomization]
ethernet.cloned-mac-address=stable
wifi.cloned-mac-address=stable
```

重启 NetworkManager 生效：

```bash
sudo systemctl restart NetworkManager
```

---

## 10. 时间同步

编辑 timesyncd 配置：

```bash
sudo nano /etc/systemd/timesyncd.conf
```

修改 NTP 服务器：

```ini
NTP=cn.ntp.org.cn ntp1.nim.ac.cn
```

重启服务：

```bash
sudo systemctl restart systemd-timesyncd.service
```

查看同步状态：

```bash
timedatectl timesync-status
```
