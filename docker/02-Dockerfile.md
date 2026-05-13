# Dockerfile 详解

> 最后更新: 2026-05-12

---

## 指令速查表

| 指令 | 作用 | 示例 |
|------|------|------|
| `FROM` | 指定基础镜像 | `FROM eclipse-temurin:17-jre-alpine` |
| `AS` | 给阶段命名（多阶段构建） | `FROM maven:3.9 AS builder` |
| `WORKDIR` | 设置工作目录 | `WORKDIR /app` |
| `COPY` | 复制文件进镜像 | `COPY pom.xml .` |
| `COPY --from` | 从其他阶段复制 | `COPY --from=builder /build/target/*.jar app.jar` |
| `RUN` | 构建时执行命令 | `RUN mvn clean package -DskipTests` |
| `EXPOSE` | 声明端口（文档作用） | `EXPOSE 8080` |
| `CMD` | 容器启动命令 | `CMD ["java", "-jar", "app.jar"]` |
| `ENV` | 设置环境变量 | `ENV JAVA_OPTS="-Xmx512m"` |
| `ARG` | 构建参数 | `ARG JAR_FILE=target/*.jar` |
| `ENTRYPOINT` | 入口点（与 CMD 组合） | `ENTRYPOINT ["java"]` |

---

## 完整示例（Spring Boot 多阶段构建）

```dockerfile
# ============ 阶段一：构建 ============
FROM maven:3.9-eclipse-temurin-17-alpine AS builder
WORKDIR /build
COPY pom.xml .
COPY src ./src
RUN mvn clean package -DskipTests -B -q

# ============ 阶段二：运行 ============
FROM eclipse-temurin:17-jre-alpine
WORKDIR /app
COPY --from=builder /build/target/*.jar app.jar
RUN mkdir -p /var/log/flavor-creator
EXPOSE 8080
CMD ["java", "-jar", "app.jar"]
```

> `eclipse-temurin:17-jre-alpine` 只有 ~180MB，`maven:3.9-eclipse-temurin-17-alpine` ~450MB。  
> 最终镜像只包含 JRE + JAR，约 200MB。

---

## .dockerignore

排除不必要文件，减小构建上下文，加速 `docker build`：

```dockerignore
target/
.git/
.idea/
*.iml
*.md
docs/
script/
node_modules/
.DS_Store
mvnw
mvnw.cmd
.mvn/
```
