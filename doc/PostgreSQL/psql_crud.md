以下是 PostgreSQL 中基本的增删改查（CRUD）SQL 语句的作用和使用方法，结合了标准语法和实际应用场景：

---

### **1. 插入数据（INSERT）**
**作用**：向表中添加新记录。  
**语法**：
```sql
INSERT INTO 表名 (列1, 列2, ...) VALUES (值1, 值2, ...);
```
**示例**：
- 插入单条记录：
  ```sql
  INSERT INTO users (name, age) VALUES ('Alice', 25);
  ```
- 插入多条记录（批量插入）：
  ```sql
  INSERT INTO users (name, age) VALUES 
    ('Bob', 30),
    ('Charlie', 22);
  ```
- 插入并返回生成的主键（`RETURNING` 子句）：
  ```sql
  INSERT INTO users (name, age) VALUES ('David', 28) RETURNING id;
  ```
**注意事项**：  
- 若省略列名，需按表定义的列顺序提供所有值（不推荐，易出错）。  
- 支持默认值和 `DEFAULT` 关键字（如 `JOIN_DATE = DEFAULT`）。

---

### **2. 查询数据（SELECT）**
**作用**：从表中检索数据。  
**语法**：
```sql
SELECT 列1, 列2, ... FROM 表名 [WHERE 条件] [ORDER BY 列名] [LIMIT 数量];
```
**示例**：
- 查询所有列：
  ```sql
  SELECT * FROM users;
  ```
- 条件查询（`WHERE`）：
  ```sql
  SELECT name, age FROM users WHERE age > 25;
  ```
- 排序（`ORDER BY`）和分页（`LIMIT/OFFSET`）：
  ```sql
  SELECT * FROM users ORDER BY age DESC LIMIT 10 OFFSET 5; -- 第2页，每页10条
  ```
- 聚合与分组（`GROUP BY`）：
  ```sql
  SELECT department, AVG(salary) FROM employees GROUP BY department;
  ```
**高级功能**：  
- 支持 `JOIN` 关联查询、窗口函数（如 `RANK()`）和 JSON 字段操作。

---

### **3. 更新数据（UPDATE）**
**作用**：修改表中已有记录。  
**语法**：
```sql
UPDATE 表名 SET 列1=值1, 列2=值2 WHERE 条件;
```
**示例**：
- 更新单条记录：
  ```sql
  UPDATE users SET age = 26 WHERE name = 'Alice';
  ```
- 基于子查询更新：
  ```sql
  UPDATE orders SET status = 'completed' 
  WHERE id IN (SELECT order_id FROM payments WHERE paid = true);
  ```
**注意事项**：  
- **必须使用 `WHERE`**，否则会更新全表数据（高危操作）。  
- 支持 `RETURNING` 返回更新后的记录。

---

### **4. 删除数据（DELETE）**
**作用**：从表中移除记录。  
**语法**：
```sql
DELETE FROM 表名 WHERE 条件;
```
**示例**：
- 删除特定记录：
  ```sql
  DELETE FROM users WHERE age < 18;
  ```
- 级联删除（需外键约束支持）：
  ```sql
  DELETE FROM departments WHERE dept_id = 101 CASCADE;
  ```
**替代方案**：  
- 清空表（更快但不可回滚）：
  ```sql
  TRUNCATE TABLE users;
  ```
**注意事项**：  
- 无 `WHERE` 会删除全表数据（生产环境慎用）。  
- 建议先使用 `SELECT` 验证条件。

---

### **5. 综合示例**
**场景**：管理员工表 `employees`：
```sql
-- 创建表
CREATE TABLE employees (
  id SERIAL PRIMARY KEY,
  name VARCHAR(50) NOT NULL,
  department VARCHAR(50),
  salary NUMERIC(10,2)
);

-- 插入数据
INSERT INTO employees (name, department, salary) VALUES 
  ('John', 'HR', 5000),
  ('Jane', 'IT', 7000);

-- 查询IT部门员工
SELECT * FROM employees WHERE department = 'IT';

-- 加薪10%
UPDATE employees SET salary = salary * 1.1 WHERE department = 'HR';

-- 删除离职员工
DELETE FROM employees WHERE name = 'John';
```

---

### **总结**
- **插入**：`INSERT` 需注意列名与值匹配，推荐批量插入提升性能。  
- **查询**：`SELECT` 支持复杂条件、聚合和连接，避免 `SELECT *`。  
- **更新/删除**：务必指定 `WHERE`，事务中操作更安全。  
- **扩展功能**：PostgreSQL 支持 JSON 操作、全文搜索等高级特性。  

更多实践可参考 PostgreSQL 官方文档或相关教程。