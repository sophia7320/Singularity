# FlavorCreator Docker 专用命令

> 最后更新: 2026-05-12

---

## 构建镜像

```bash
cd /home/wdy/Desktop/FlavorCreator
docker build -t flavor-creator -f backend/Dockerfile backend
```

## 启动全套服务

```bash
docker compose up -d
```

## 强制重建启动

```bash
docker compose up -d --build
```

## 查看状态

```bash
docker compose ps
```

## 实时日志

```bash
docker compose logs -f app
```

## 停止

```bash
docker compose down
```

## 推送到腾讯云

```bash
docker tag flavor-creator:latest ccr.ccs.tencentyun.com/<ns>/flavor-creator:v1
```

---

```bash
docker push ccr.ccs.tencentyun.com/<ns>/flavor-creator:v1
```

## 环境变量清单（云托管）

| 变量 | 说明 |
|------|------|
| `SPRING_PROFILES_ACTIVE` | `cloud` |
| `DB_URL` | CloudBase MySQL 内网地址 |
| `DB_USERNAME` | 数据库用户 |
| `DB_PASSWORD` | 数据库密码 |
| `REDIS_HOST` | CloudBase Redis 地址 |
| `REDIS_PORT` | `6379` |
| `WX_APP_ID` | 小程序 AppID |
| `WX_SECRET` | 小程序 Secret |
| `JWT_SECRET` | JWT 签名密钥 |
| `COS_SECRET_ID` | COS API 密钥 ID |
| `COS_SECRET_KEY` | COS API 密钥 Key |
| `COS_REGION` | `ap-shanghai` |
| `COS_BUCKET` | COS 桶名 |
