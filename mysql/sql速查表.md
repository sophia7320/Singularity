# SQL 速查表

> 方言 MySQL 8.x | 示例统一用 `app` 库、`user` / `orders` 表 | 按功能分组，组内按使用频率排列

---

## 库操作

```sql
CREATE DATABASE app DEFAULT CHARSET utf8mb4 COLLATE utf8mb4_0900_ai_ci;
DROP DATABASE IF EXISTS app;
USE app;
SHOW DATABASES;
SHOW CREATE DATABASE app;
```

字符集统一 `utf8mb4`（`utf8` 是只有三字节的残缺别名）。

---

## 表操作

建表：

```sql
CREATE TABLE user (
  id         BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
  name       VARCHAR(32)     NOT NULL,
  phone      VARCHAR(20)     NOT NULL DEFAULT '',
  status     TINYINT UNSIGNED NOT NULL DEFAULT 1,
  created_at DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
  UNIQUE KEY uk_phone (phone),
  KEY idx_created (created_at)
) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4;
```

结构变更：

```sql
ALTER TABLE user ADD COLUMN age TINYINT UNSIGNED NOT NULL DEFAULT 0 AFTER name;  -- 8.0 尾部加列走 INSTANT，大表秒级
ALTER TABLE user MODIFY COLUMN phone VARCHAR(20) NOT NULL DEFAULT '';           -- 类型收窄要重建表，先试 ALGORITHM=INPLACE, LOCK=NONE
ALTER TABLE user RENAME COLUMN phone TO mobile;
ALTER TABLE user DROP COLUMN age;
RENAME TABLE user TO member;
```

索引：

```sql
ALTER TABLE orders ADD INDEX idx_user_created (user_id, created_at);  -- 联合索引最左前缀：等值列在前，范围列在后
ALTER TABLE orders ADD UNIQUE KEY uk_order_no (order_no);
SHOW INDEX FROM orders;

-- 撤销索引（与上面二选一）
ALTER TABLE orders DROP INDEX idx_user_created;
```

查看与清理：

```sql
DESC user;                       -- 等价 SHOW COLUMNS FROM user;
-- 8.4+ 非交互（-e/管道）需加 --commands 才认 \G
SHOW CREATE TABLE user\G
SHOW TABLES LIKE 'order%';
CREATE TABLE user_bak LIKE user; -- 连数据：INSERT INTO user_bak SELECT * FROM user;
DROP TABLE IF EXISTS log_2025;
TRUNCATE TABLE log;              -- DDL，隐式提交、不可回滚、重置自增；要能回滚用 DELETE
```

---

## 数据操作

### 查询

主键/条件/集合：

```sql
SELECT * FROM user WHERE id = 1;                                  -- 主键或唯一索引命中，最快路径
SELECT id, name FROM user WHERE status = 1 AND age >= 18;         -- 只取需要的列，SELECT * 放大回表和网络开销
SELECT * FROM user WHERE id IN (1, 2, 3)
  AND created_at BETWEEN '2026-01-01' AND '2026-12-31';
SELECT * FROM user WHERE deleted_at IS NULL;                      -- = NULL 恒为 UNKNOWN
SELECT * FROM user WHERE name LIKE '张%';                         -- 前置 % 无法走索引
SELECT DISTINCT city FROM user;
SELECT EXISTS(SELECT 1 FROM user WHERE phone = '13800000000');    -- 判断存在比 COUNT 便宜
```

分页：

```sql
SELECT * FROM user ORDER BY id DESC LIMIT 20 OFFSET 40;
-- 深分页改游标，OFFSET 越大越慢
SELECT * FROM user WHERE id < 1000 ORDER BY id DESC LIMIT 20;
```

聚合与分组：

```sql
SELECT COUNT(*) FROM user WHERE status = 1;                       -- 无条件 COUNT 走最小索引全扫，大表很慢
SELECT SUM(amount), AVG(amount), MAX(created_at) FROM orders;
SELECT status, COUNT(*) AS cnt FROM orders GROUP BY status;
SELECT user_id, COUNT(*) AS cnt FROM orders GROUP BY user_id HAVING cnt > 3;
SELECT user_id, GROUP_CONCAT(id ORDER BY id) FROM orders GROUP BY user_id;  -- 默认截断 1024 字节，调 group_concat_max_len
```

关联：

```sql
SELECT o.id, u.name FROM orders o JOIN user u ON o.user_id = u.id;
-- 找孤儿行：ON 放连接条件，WHERE 放结果过滤，写反会退化成内连接
SELECT u.id FROM user u LEFT JOIN orders o ON o.user_id = u.id WHERE o.id IS NULL;
SELECT * FROM user u WHERE EXISTS (SELECT 1 FROM orders o WHERE o.user_id = u.id);
```

窗口与 CTE：

```sql
-- 取每组最新一条：外层再 WHERE rn = 1
SELECT id, ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY id DESC) AS rn FROM orders;
WITH t AS (
  SELECT user_id, SUM(amount) AS s FROM orders GROUP BY user_id
) SELECT * FROM t WHERE s > 100;
```

时间范围：

```sql
SELECT * FROM orders WHERE created_at >= DATE_SUB(NOW(), INTERVAL 7 DAY);
-- 别对索引列套函数（DATE(created_at) >= ...），会失效
```

### 写入

```sql
-- 插入：显式写列名，表结构变更时不炸；单批 500~1000 行，过大触发 max_allowed_packet
INSERT INTO user (name, phone) VALUES ('张三', '13800000000'), ('李四', '13900000000');

-- 冲突即更新：8.0.19+ 行别名写法，旧写法 VALUES(name) 自 8.0.20 起废弃
INSERT INTO user (id, name) VALUES (1, '张三') AS new
  ON DUPLICATE KEY UPDATE name = new.name;

-- 更新：漏写 WHERE 就是全表更新，SET sql_safe_updates = 1 可兜底
UPDATE user SET status = 0, updated_at = NOW() WHERE id = 1;
UPDATE orders o JOIN user u ON o.user_id = u.id SET o.city = u.city;

-- 删除：大表分批，避免长事务和大范围锁
DELETE FROM user WHERE id = 1;
DELETE FROM user WHERE created_at < '2025-01-01' LIMIT 1000;
```

---

## 事务与锁

```sql
START TRANSACTION;
UPDATE user SET status = 0 WHERE id = 1;
COMMIT;                                   -- 或 ROLLBACK; 三者必须同一连接，中途 DDL 会隐式提交

SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
SELECT @@transaction_isolation;

SELECT * FROM user WHERE id = 1 FOR UPDATE;   -- 必须在事务内；条件没走索引会退化成表锁

SELECT * FROM performance_schema.data_lock_waits;  -- 5.7 查 information_schema.innodb_locks
```

---

## 账号与权限

```sql
CREATE USER 'app'@'%' IDENTIFIED BY 'Str0ngPwd!';
GRANT SELECT, INSERT, UPDATE, DELETE ON app.* TO 'app'@'%';
REVOKE DELETE ON app.* FROM 'app'@'%';
ALTER USER 'app'@'%' IDENTIFIED BY 'NewPwd!';
SHOW GRANTS FOR 'app'@'%';
FLUSH PRIVILEGES;   -- 只有直接改 mysql.user 表才需要，GRANT 本身即时生效
```

---

## 备份与导入导出

```bash
# 整库：--single-transaction 不锁表，InnoDB 一致快照
mysqldump -u root -p --single-transaction --routines --triggers app > app.sql
# 导入到非从库若报 GTID_PURGED 冲突，导出时加 --set-gtid-purged=OFF

mysqldump -u root -p app orders > orders.sql   # 单表
mysql -u root -p app < app.sql                 # 导入

# 客户端内导入
# SOURCE /data/app.sql;
```

---

## 运维排查

```sql
SHOW FULL PROCESSLIST;                  -- Time 大且 Command=Sleep 多为连接池闲置
KILL 12345;                             -- 断连接：KILL CONNECTION 12345; 只断语句：KILL QUERY
SHOW VARIABLES LIKE 'max_connections';
SHOW STATUS LIKE 'Threads_connected';
SELECT VERSION();
-- 看 LATEST DETECTED DEADLOCK 与 TRANSACTIONS 段
SHOW ENGINE INNODB STATUS\G
-- 8.0.22+；旧版 SHOW SLAVE STATUS\G
SHOW REPLICA STATUS\G
```

执行计划与统计：

```sql
EXPLAIN ANALYZE SELECT * FROM orders WHERE user_id = 1;   -- 8.0.18+ 带真实耗时；type=ALL、rows 远大于结果数要警惕
ANALYZE TABLE orders;                                     -- 更新统计信息；碎片多再 OPTIMIZE TABLE（会重建表）
SET GLOBAL slow_query_log = ON;                           -- 配套 long_query_time = 1，路径见 slow_query_log_file

SELECT table_name, ROUND(data_length / 1024 / 1024, 2) AS mb
FROM information_schema.tables
WHERE table_schema = 'app'
ORDER BY data_length DESC;
```
