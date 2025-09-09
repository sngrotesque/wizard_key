Protobuf（Protocol Buffers）支持多种数据类型，主要分为**标量类型**和**结构化类型**。下面是一个完整的分类和说明：

---

## 🧮 标量数据类型（Scalar Types）

这些是最常用的基本类型，类似于 C++ 或 Java 中的原始类型：

| `.proto` 类型 | 描述 | 编码方式 | 对应 C++ 类型 |
|---------------|------|----------|----------------|
| `double`      | 64 位浮点数 | 定长 8 字节 | `double` |
| `float`       | 32 位浮点数 | 定长 4 字节 | `float` |
| `int32`       | 有符号整数 | Varint | `int32` |
| `int64`       | 有符号整数 | Varint | `int64` |
| `uint32`      | 无符号整数 | Varint | `uint32` |
| `uint64`      | 无符号整数 | Varint | `uint64` |
| `sint32`      | 有符号整数（ZigZag 编码）| Varint | `int32` |
| `sint64`      | 有符号整数（ZigZag 编码）| Varint | `int64` |
| `fixed32`     | 无符号整数 | 定长 4 字节 | `uint32` |
| `fixed64`     | 无符号整数 | 定长 8 字节 | `uint64` |
| `sfixed32`    | 有符号整数 | 定长 4 字节 | `int32` |
| `sfixed64`    | 有符号整数 | 定长 8 字节 | `int64` |
| `bool`        | 布尔值 | Varint | `bool` |
| `string`      | UTF-8 字符串 | Length-delimited | `std::string` |
| `bytes`       | 原始字节序列 | Length-delimited | `std::string` |

## 🧩 结构化类型（Complex Types）

这些类型用于构建更复杂的数据结构：

- `message`：嵌套结构体，类似 C++ 的类或 struct
- `enum`：枚举类型，值为整数
- `repeated`：数组或列表（可以是标量或 message 类型）
- `map<key, value>`：键值对映射（key 通常为标量类型）
- `oneof`：联合体，只能设置其中一个字段
- `Any`：可以存储任意类型的 Protobuf 消息（需导入 `google/protobuf/any.proto`）

## 🧠 编码方式简述

| 编码方式         | 用途                     | 特点 |
|------------------|--------------------------|------|
| Varint           | 整数类型（默认）         | 可变长度，节省空间 |
| ZigZag + Varint  | `sint32/sint64`          | 对负数更高效 |
| Fixed-length     | `fixed32/fixed64` 等     | 定长，适合大数 |
| Length-delimited | `string`, `bytes`, `message` | 先写长度，再写内容 |

如果你想深入探索某种类型的编码细节（比如 `sint64` 的 ZigZag 编码），我可以帮你写出编码/解码函数，或者分析它的空间效率。你也可以看看这篇文章对各种类型的使用场景做了很好的总结：[SegmentFault 的 Protobuf 类型解析](https://segmentfault.com/a/1190000038778850)。

---

## 📜 基础关键字

| 关键字 | 作用 | 示例 |
|--------|------|------|
| `syntax` | 指定语法版本（proto2 或 proto3） | `syntax = "proto3";` |
| `package` | 指定生成代码的命名空间 | `package wuk.net;` |
| `import` | 引入其他 `.proto` 文件 | `import "google/protobuf/timestamp.proto";` |
| `option` | 设置编译器选项 | `option java_package = "com.example";` |

## 🧩 数据结构定义

| 关键字 | 作用 | 示例 |
|--------|------|------|
| `message` | 定义结构体类型 | `message WukPacket { ... }` |
| `enum` | 定义枚举类型 | `enum MessageType { NONE = 0; FILE = 1; }` |
| `repeated` | 定义数组字段 | `repeated string tags = 3;` |
| `map` | 定义键值对字段 | `map<string, int32> scores = 4;` |
| `oneof` | 定义互斥字段 | `oneof payload { string text = 1; bytes image = 2; }` |

## 🔧 字段修饰符（proto3）

| 修饰符 | 说明 | 示例 |
|--------|------|------|
| `optional` | 可选字段（proto3 支持） | `optional string nickname = 5;` |
| `required` | 必填字段（仅 proto2 支持） | `required int32 id = 1;` |
| `extensions` | 定义扩展区间（proto2） | `extensions 100 to 199;` |
| `extend` | 扩展已有类型（proto2） | `extend Person { optional string email = 100; }` |

## 🧠 服务与 RPC（用于 gRPC）

| 关键字 | 作用 | 示例 |
|--------|------|------|
| `service` | 定义服务接口 | `service ChatService { ... }` |
| `rpc` | 定义远程方法 | `rpc SendMessage (Message) returns (Ack);` |

## 🧬 高级结构示例

```proto
syntax = "proto3";
package wuk.net;

message User {
  uint64 id = 1;
  string name = 2;
  optional string nickname = 3;
  repeated string tags = 4;
  map<string, int32> scores = 5;
  oneof contact {
    string email = 6;
    string phone = 7;
  }
}

enum Status {
  ONLINE = 0;
  OFFLINE = 1;
  AWAY = 2;
}

service UserService {
  rpc GetUser (UserRequest) returns (UserResponse);
}
```
