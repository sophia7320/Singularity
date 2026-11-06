# Arch Linux 下 MySQL 的安装与配置

## 两个选择

Arch 官方仓库没有 Oracle MySQL：2013 年起 MariaDB 是默认实现，`mariadb` 提供 `mysql`，两者互斥。

| | MariaDB（官方仓库） | Oracle MySQL（AUR） |
|---|---|---|
| 包 | `mariadb` / `mariadb-lts`（13.0.2，extra） | `mysql` 9.7.1（AUR，Muflone 维护） |
| 服务名 | `mariadb.service` | `mysqld.service` |
| 主配置 | `/etc/my.cnf` + `/etc/my.cnf.d/*.cnf` | `/etc/mysql/my.cnf` |
| 数据目录 | `/var/lib/mysql` | `/var/lib/mysql` |
| socket | `/run/mysqld/mysqld.sock` | `/run/mysqld/mysqld.sock` |
| 初始化 | `mariadb-install-db` | `mysqld --initialize` |
| root 认证 | `unix_socket`（`sudo mariadb` 即可） | 随机临时密码（`caching_sha2_password`） |

## 方案 A：MariaDB（推荐）

```bash
sudo pacman -Syu --needed mariadb
# 必须先初始化，再启动服务
sudo mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql
sudo systemctl enable --now mariadb
sudo mariadb-secure-installation   # 删匿名用户/测试库，禁 root 远程登录
```

验证：

```bash
systemctl status mariadb --no-pager
sudo mariadb -e "SELECT VERSION(); SELECT user,host,plugin FROM mysql.user;"
ss -ltn | grep 3306
```

`root` 走 `unix_socket` 认证，`mariadb -u root -p` 可能失败，用 `sudo mariadb`。

建业务库/账号（最小权限）：

```sql
CREATE DATABASE appdb CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
CREATE USER 'app'@'localhost' IDENTIFIED BY '强密码';
GRANT SELECT,INSERT,UPDATE,DELETE ON appdb.* TO 'app'@'localhost';
FLUSH PRIVILEGES;
```

## 方案 B：Oracle MySQL（AUR）

```bash
sudo pacman -S --needed base-devel git
# 与 mariadb 冲突，先移除；有数据先 mariadb-dump 备份
sudo pacman -Rns mariadb mariadb-clients mariadb-libs
yay -S mysql   # 源码编译，耗时长、吃内存/磁盘
```

初始化（datadir 必须为空；临时 root 密码输出到 stderr，务必截住）：

```bash
sudo mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql 2>&1 \
  | tee /tmp/mysql-init.log
grep 'temporary password' /tmp/mysql-init.log

sudo systemctl enable --now mysqld
mysql -u root -p   # 粘贴临时密码
mysql_secure_installation
```

本地/虚拟机图快可用 `--initialize-insecure`（root 空密码），随后立刻 `mysql_secure_installation`。

## 配置

MariaDB 写 `/etc/my.cnf.d/server.cnf`，MySQL 写 `/etc/mysql/my.cnf` 的 `[mysqld]`：

```ini
[mysqld]
bind-address = 127.0.0.1          # 只监听本机
innodb_buffer_pool_size = 2G      # 专用机 50–70% 内存，共享机 10%
innodb_flush_log_at_trx_commit = 1
max_connections = 200
skip-name-resolve                 # 按 IP 授权，省 DNS 反查
# skip-networking                 # 极端：只走 unix socket
```

改完 `sudo systemctl restart mariadb`（或 `mysqld`）。字符集两个包都已是 utf8mb4（MySQL 包编译参数 `-DDEFAULT_CHARSET=utf8mb4 -DDEFAULT_COLLATION=utf8mb4_unicode_ci`），用 `SHOW VARIABLES LIKE 'character_set_server';` 核对。

时区表（要用 `CONVERT_TZ` 才需要）：

```bash
mariadb-tzinfo-to-sql /usr/share/zoneinfo | sudo mariadb mysql
```

远程访问（非必要不开，优先 SSH 隧道/VPN）：`sudo ufw allow 3306/tcp`，并确认 `bind-address` 不是 127.0.0.1，授权 `'app'@'10.0.0.%'`。

## 排错

```bash
sudo journalctl -u mariadb -n 50 --no-pager   # 或 -u mysqld
sudo ss -ltnp | grep 3306
ls -ld /var/lib/mysql /run/mysqld             # 属主应为 mysql:mysql
```

常见坑：

- 启动即退：忘了 `mariadb-install-db` / `--initialize`，或 datadir 非空。
- `Can't connect to local server through socket`：socket 路径不一致，显式 `-S /run/mysqld/mysqld.sock` 或 `-h 127.0.0.1 -P 3306`。
- MariaDB 大版本升级后跑 `sudo mariadb-upgrade -u root -p`；MySQL 8.0.16+ 自动升级数据字典。
- Btrfs 上数据目录 CoW 由包内 tmpfiles 自动关闭（`+C`）；ZFS 需另查。
- 完全不碰系统：`docker run -d --name mysql -e MYSQL_ROOT_PASSWORD=... -p 127.0.0.1:3306:3306 -v mysql:/var/lib/mysql mysql:9`
