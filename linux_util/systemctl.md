# systemctl 速查表

> 基线 systemd 257+（Arch 滚动） | 示例单元 `nginx.service` | 新特性标注引入版本 | 用户级单元加 `--user`

---

## 查看状态

```bash
systemctl status nginx              # Loaded / Active / MainPID / CGroup + 最近日志
systemctl status nginx -l --no-pager   # -l 不截断，--no-pager 不进水less
systemctl status 1234               # 按 PID 反查所属单元
systemctl cat nginx                 # 单元文件 + 所有 drop-in 合并后的内容
systemctl show nginx -p MainPID,ExecStart,Restart,ActiveState   # 机读，脚本用这个
systemctl is-active nginx; systemctl is-enabled nginx; systemctl is-failed nginx
systemctl is-system-running         # running / degraded / maintenance / starting ...
```

`status` 只反映本次运行态，历史失败看 `journalctl -u`。退出码：`is-active` 活跃为 0，`status` 非运行态返回 3。

列表：

```bash
systemctl list-units --type=service --state=running
systemctl list-units --all --no-legend        # 含 inactive；默认只列 active/failed/有任务的
systemctl --failed                            # 失败单元，排障第一站
systemctl list-unit-files 'nginx*'            # 已安装单元文件 + 自启状态
systemctl list-unit-files --state=masked
systemctl list-dependencies nginx             # 依赖树；加 --reverse 看谁依赖它
systemctl list-jobs                           # 卡住时看排队任务
```

单元类型：`service` `socket` `target` `timer` `mount` `automount` `path` `slice` `scope` `device` `swap`。

---

## 生命周期

```bash
systemctl start nginx
systemctl stop nginx
systemctl restart nginx             # 停再起，连接全断
systemctl reload nginx              # 只发 ExecReload，单元没定义就报错
systemctl reload-or-restart nginx   # 优先 reload，不支持则 restart
systemctl try-restart nginx         # 仅在运行时重启，常用于升级脚本
systemctl kill -s HUP nginx         # 按 cgroup 发信号，覆盖单元内所有进程
systemctl kill -s SIGKILL --kill-whom=all nginx
systemctl set-property nginx CPUQuota=50% MemoryMax=1G   # 持久化到 /etc/systemd/system.control
systemctl reset-failed nginx        # 清失败计数，解除 StartLimit 锁定
systemctl clean nginx --what=state  # 243+：清 RuntimeDirectory/StateDirectory 等
```

`start` 卡住通常是依赖或 `ExecStartPre` 在等：`systemctl list-jobs` + `journalctl -u nginx -f`。

---

## 开机自启

```bash
systemctl enable nginx              # 按 [Install] 建符号链接，不启动
systemctl enable --now nginx
systemctl disable --now nginx
systemctl reenable nginx            # 按当前 [Install] 重建链接
systemctl preset nginx              # 按厂商 preset 策略
```

`is-enabled` 返回值：

| 值 | 含义 | 退出码 |
|---|---|---|
| `enabled` | 已启用 | 0 |
| `disabled` | 有 `[Install]` 但未启用 | >0 |
| `static` | 无 `[Install]`，只能被依赖拉起 | 0 |
| `indirect` | 自身未启用，但 `Also=` 或别名指向别的单元 | 0 |
| `alias` | 是别名（符号链接） | 0 |
| `masked` / `masked-runtime` | 屏蔽，任何方式都起不来 | >0 |
| `not-found` | 单元不存在 | 4 |

`static` 不等于「不会开机启动」：被 `WantedBy` 的 target 依赖时照样启动。

屏蔽：

```bash
systemctl mask nginx                # 链到 /dev/null，强于 disable，enable 会报错
systemctl mask --runtime nginx      # 只到下次重启
systemctl unmask nginx
```

---

## 单元文件与修改

优先级从高到低：

```text
/etc/systemd/system/       管理员覆盖
/run/systemd/system/       运行时
/usr/lib/systemd/system/   包管理器
~/.config/systemd/user/    用户单元
```

```bash
systemctl edit nginx                # 生成 /etc/systemd/system/nginx.service.d/override.conf，保存后自动 daemon-reload
systemctl edit --full nginx         # 整份复制到 /etc 再改，升级后不会自动跟进
systemctl revert nginx              # 撤销 edit / mask / set-property
systemctl daemon-reload             # 手改单元文件后必须执行，否则跑的还是旧配置
systemd-analyze verify /etc/systemd/system/foo.service   # 校验语法与依赖
```

drop-in 示例（`override.conf`）：

```ini
[Service]
Environment="APP_ENV=prod"
Restart=always
RestartSec=2
```

`edit` 会自动 reload，手写 drop-in 不会；`enable`/`disable`/`mask` 也会隐式 reload（`--no-reload` 可关）。

---

## 日志

```bash
journalctl -u nginx -f                                  # 跟踪
journalctl -u nginx -b -p warning --since '1 hour ago'
journalctl -u nginx --since '2026-11-01' --until '2026-11-02 03:00'
journalctl -b -1                                        # 上次启动
journalctl -k                                           # 内核
journalctl -o json-pretty _PID=1234                     # 按字段查
journalctl --disk-usage; journalctl --vacuum-size=500M; journalctl --vacuum-time=7d
```

---

## 定时器与套接字

```bash
systemctl list-timers --all                             # NEXT / LAST / 下次触发时间
systemd-analyze calendar 'Mon *-*-* 04:00:00'           # 验证 OnCalendar 表达式
systemctl list-sockets
systemctl list-paths                                    # 254+
```

timer 单元靠同名 service 干活：

```ini
[Timer]
OnCalendar=daily
Persistent=true      # 关机错过的任务开机补跑
```

---

## 目标与系统

```bash
systemctl get-default; systemctl set-default graphical.target
systemctl isolate multi-user.target      # 立即切运行级，会停掉不属于该 target 的单元
systemctl list-units --type=target
systemctl daemon-reexec                  # 重新执行 systemd 自身（升级后）
systemd-analyze blame                    # 开机耗时排序
systemd-analyze critical-chain           # 关键路径
systemctl poweroff / reboot / suspend / hibernate
systemctl soft-reboot                    # 254+：只重启用户空间，内核不动
systemctl freeze / thaw nginx            # 246+：cgroup 冻结
```

---

## 用户级

```bash
systemctl --user status foo
systemctl --user enable --now foo
loginctl enable-linger alice             # 未登录也运行用户单元
systemctl --user daemon-reload
```

用户单元路径 `~/.config/systemd/user/`，日志用 `journalctl --user-unit=foo`。

---

## 排错对照

| 现象 | 处理 |
|---|---|
| `Unit nginx.service not found` | 查 `systemctl list-unit-files \| grep nginx`；注意 `.service` 后缀与路径 |
| `Unit is masked` | `systemctl unmask nginx` |
| `loaded failed failed` | `journalctl -u nginx -b -p err`，再 `systemctl reset-failed` |
| 改了配置不生效 | 忘了 `systemctl daemon-reload` |
| `start` 一直不返回 | `systemctl list-jobs`，看是否有 `start` 任务卡住 |
| 反复重启后不再拉起 | `StartLimitBurst` / `StartLimitIntervalSec` 触发，`reset-failed` |
| 服务起在错误目录/环境 | `systemctl show -p WorkingDirectory,Environment,ExecStart` |
| 找不到进程但单元 active | `systemctl status` 看 CGroup，`ps -o cgroup` 对照 |
