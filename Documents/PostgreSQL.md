您好，这是 Bing。感谢您的提问。😊

如果您想用 C#，Java，Python 来操作 PostgreSQL 数据库，您需要使用相应的数据库驱动或连接器，它们可以帮助您建立数据库连接，执行 SQL 语句，处理查询结果等。以下是一些代码示例，您可以参考或修改它们来实现您的需求。

- C# 操作 PostgreSQL 数据库

  - 首先，您需要安装 Npgsql 包，它是一个开源的 C# 数据库驱动，支持 .NET Framework 和 .NET Core 平台。您可以使用 NuGet 包管理器来安装 Npgsql 包¹。
  - 然后，您可以使用 NpgsqlConnection 类来创建一个数据库连接对象，使用 NpgsqlCommand 类来创建一个 SQL 命令对象，使用 ExecuteNonQuery 方法来执行非查询语句（如插入、更新、删除等），使用 ExecuteReader 方法来执行查询语句（如选择等），使用 NpgsqlDataReader 类来读取查询结果²。例如：

```csharp
using System;
using Npgsql; // 引入 Npgsql 命名空间

namespace PostgreSQLDemo
{
    class Program
    {
        static void Main(string[] args)
        {
            // 定义数据库连接字符串
            string connStr = "Host=127.0.0.1;Port=5432;Username=postgres;Password=123456;Database=testdb";
            
            // 创建一个数据库连接对象
            using (var conn = new NpgsqlConnection(connStr))
            {
                // 打开数据库连接
                conn.Open();

                // 创建一个 SQL 命令对象
                using (var cmd = new NpgsqlCommand())
                {
                    // 设置命令对象的连接属性
                    cmd.Connection = conn;

                    // 定义一个插入语句
                    cmd.CommandText = "INSERT INTO student (id, name, age) VALUES (@id, @name, @age)";
                    
                    // 添加参数并赋值
                    cmd.Parameters.AddWithValue("id", 1);
                    cmd.Parameters.AddWithValue("name", "Alice");
                    cmd.Parameters.AddWithValue("age", 20);

                    // 执行非查询语句，返回受影响的行数
                    int rowsAffected = cmd.ExecuteNonQuery();
                    Console.WriteLine($"插入了 {rowsAffected} 行数据");

                    // 定义一个查询语句
                    cmd.CommandText = "SELECT * FROM student";

                    // 执行查询语句，返回一个数据读取器对象
                    using (var reader = cmd.ExecuteReader())
                    {
                        // 遍历数据读取器对象，读取每一行数据
                        while (reader.Read())
                        {
                            // 通过索引或列名获取每一列的值
                            Console.WriteLine($"id: {reader[0]}, name: {reader["name"]}, age: {reader["age"]}");
                        }
                    }
                }
            }
        }
    }
}
```

- Java 操作 PostgreSQL 数据库

  - 首先，您需要下载并安装 PostgreSQL JDBC 驱动，它是一个开源的 Java 数据库驱动，支持 JDBC 4.2 标准。您可以从官方网站下载相应版本的 jar 文件³。
  - 然后，您可以使用 DriverManager 类的 getConnection 方法来获取一个 Connection 连接对象，使用 Connection 对象的 createStatement 方法来创建一个 Statement 语句对象，使用 executeUpdate 方法来执行非查询语句（如插入、更新、删除等），使用 executeQuery 方法来执行查询语句（如选择等），使用 ResultSet 类来存储和访问查询结果⁴。例如：

```java
import java.sql.*; // 引入 java.sql 包

public class PostgreSQLDemo {

    public static void main(String[] args) {

        // 定义数据库连接字符串
        String url = "jdbc:postgresql://127.0.0.1:5432/testdb";
        String user = "postgres";
        String password = "123456";

        try {
            // 加载驱动类
            Class.forName("org.postgresql.Driver");

            // 获取一个数据库连接对象
            Connection conn = DriverManager.getConnection(url, user, password);

            // 创建一个 SQL 语句对象
            Statement stmt = conn.createStatement();

            // 定义一个插入语句
            String sql = "INSERT INTO student (id, name, age) VALUES (2, 'Bob', 21)";

            // 执行非查询语句，返回受影响的行数
            int rowsAffected = stmt.executeUpdate(sql);
            System.out.println("插入了 " + rowsAffected + " 行数据");

            // 定义一个查询语句
            sql = "SELECT * FROM student";

            // 执行查询语句，返回一个结果集对象
            ResultSet rs = stmt.executeQuery(sql);

            // 遍历结果集对象，读取每一行数据
            while (rs.next()) {
                // 通过索引或列名获取每一列的值
                System.out.println("id: " + rs.getInt(1) + ", name: " + rs.getString("name") + ", age: " + rs.getInt("age"));
            }

            // 关闭结果集对象
            rs.close();

            // 关闭语句对象
            stmt.close();

            // 关闭连接对象
            conn.close();

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
```

- Python 操作 PostgreSQL 数据库

  - 首先，您需要安装 psycopg2 包，它是一个开源的 Python 数据库驱动，支持 Python 2.7 和 Python 3.x 版本。您可以使用 pip 命令来安装 psycopg2 包⁵。
  - 然后，您可以使用 psycopg2 模块的 connect 函数来创建一个数据库连接对象，使用连接对象的 cursor 方法来创建一个游标对象，使用 execute 方法来执行 SQL 语句，使用 fetchone、fetchmany 或 fetchall 方法来获取查询结果⁶。例如：

```python
import psycopg2 # 引入 psycopg2 模块

# 定义数据库连接字符串
conn_str = "host=127.0.0.1 port=5432 dbname=testdb user=postgres password=123456"

# 创建一个数据库连接对象
conn = psycopg2.connect(conn_str)

# 创建一个游标对象
cur = conn.cursor()

# 定义一个插入语句
sql = "INSERT INTO student (id, name, age) VALUES (%s, %s, %s)"

# 添加参数并赋值
params = (3, 'Charlie', 22)

# 执行非查询语句
cur.execute(sql, params)

# 提交事务
conn.commit()

# 定义一个查询语句
sql = "SELECT * FROM student"

# 执行查询语句
cur.execute(sql)

# 获取所有的查询结果
rows = cur.fetchall()

# 遍历查询结果，读取每一行数据
for row in rows:
    # 通过索引或列名获取每一列的值
    print(f"id: {row[0]}, name: {row['name']}, age: {row['age']}")

# 关闭游标对象
cur.close()

# 关闭连接对象
conn.close()
```

以上就是我能找到的一些关于如何用 C#，Java，Python 来操作 PostgreSQL 数据库的代码示例。希望对您有所帮助。如果您还有其他问题，请随时提问。😊

源: 与必应的对话， 2023/10/5
(1) 《PostgreSQL 开发指南》第 30 篇 Java 访问 PostgreSQL - 知乎. https://zhuanlan.zhihu.com/p/460770544.
(2) 【Python-postgreSQL】如何使用Python连接并操纵SQL数据库？ - 知乎. https://zhuanlan.zhihu.com/p/379047835.
(3) C# 访问PostgreSQL数据库_c# 读写 pgsql-CSDN博客. https://blog.csdn.net/weixin_44695969/article/details/104678830.
(4) 使用Python操作postgresql数据库-腾讯云开发者社区-腾讯云. https://cloud.tencent.com/developer/article/2058200.
(5) 使用python进行PostgreSQL 数据库连接 - CSDN博客. https://blog.csdn.net/CaptainTakuya/article/details/113768112.
(6) undefined. https://github.com/dongxuyang1985/postgresql_dev_guideJDBC.
(7) undefined. https://github.com/dongxuyang1985/postgresql_dev_guide.