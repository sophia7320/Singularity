# Docker Compose

> 最后更新: 2026-05-12

---

## 完整配置示例

```yaml
services:
  mysql:
    image: mysql:8.0
    container_name: my-mysql
    environment:
      MYSQL_ROOT_PASSWORD: root123
      MYSQL_DATABASE: mydb
    ports:
      - "3307:3306"
    volumes:
      - mysql-data:/var/lib/mysql
      - ./sql:/docker-entrypoint-initdb.d
    healthcheck:
      test: ["CMD", "mysqladmin", "ping", "-h", "localhost"]
      interval: 5s
      timeout: 5s
      retries: 10

  redis:
    image: redis:7-alpine
    container_name: my-redis
    ports:
      - "6380:6379"
    healthcheck:
      test: ["CMD", "redis-cli", "ping"]
      interval: 5s
      timeout: 5s
      retries: 10

  app:
    build:
      context: ./backend
      dockerfile: Dockerfile
    container_name: my-app
    ports:
      - "8080:8080"
    environment:
      SPRING_PROFILES_ACTIVE: dev
      SPRING_DATASOURCE_URL: jdbc:mysql://mysql:3306/mydb
      SPRING_DATASOURCE_USERNAME: root
      SPRING_DATASOURCE_PASSWORD: root123
      SPRING_DATA_REDIS_HOST: redis
    depends_on:
      mysql:
        condition: service_healthy
      redis:
        condition: service_healthy

volumes:
  mysql-data:
```

---

## 核心命令

### 启动

```bash
docker compose up -d
```

> 启动所有服务

---

```bash
docker compose up -d --build
```

> 强制重新构建镜像

### 停止

```bash
docker compose down
```

> 停止并清理

---

```bash
docker compose down -v
```

> 同时删除 volumes（数据会丢！）

### 查看

```bash
docker compose ps
```

### 日志

```bash
docker compose logs -f app
```

> 实时

### 重启

```bash
docker compose restart app
```

### 进入

```bash
docker compose exec app sh
```

---

## 关键概念

### depends_on vs healthcheck

```yaml
depends_on:
  mysql:
    condition: service_healthy   # 等 MySQL 真正可用，而不只是容器启动
```

> 没有 `condition: service_healthy`，`depends_on` 只等容器启动（≠ 服务就绪）。

### 端口映射

```
"宿主机端口:容器端口"

"3307:3306" → localhost:3307 → 容器:3306
容器间通信用服务名 mysql:3306
```

### 环境变量覆盖

`docker-compose.yml` 中 `environment` 的变量优先级高于 `application.yml`，可用于覆盖 Spring Boot 配置。
