# Libpq 17 文档（超简化版）

这是[Chapter 32. libpq - C 库](http://www.postgres.cn/docs/17/libpq.html)文档的超简化版本，只保留[WukPsql.hh](../WukPsql.hh)中会用上的内容。

 -  [连接](#连接)  
    - [PQconnectdb](#pqconnectdb)
 -  [状态](#状态)
    - [PQstatus](#pqstatus)
 -  [命令执行函数](#命令执行函数)
    - [PQexec](#pqexec)
    - [PQexecParams](#pqexecparams)
    - [PQprepare](#pqprepare)
    - [PQexecPrepared](#pqexecprepared)
    - [PQdescribePrepared](#pqdescribeprepared)
    - [PQdescribePortal](#pqdescribeportal)
    - [PQclosePrepared](#pqcloseprepared)
    - [PQclosePortal](#pqcloseportal)
    - 
    - [PQresultStatus](#pqresultstatus)
    - [PQresStatus](#pqresstatus)
    - [PQresultErrorMessage](#pqresulterrormessage)
    - [PQresultVerboseErrorMessage](#pqresultverboseerrormessage)
    - [PQresultErrorField](#pqresulterrorfield)
    - [PQclear](#pqclear)
    - [PQntuples](#pqntuples)
    - [PQnfields](#pqnfields)
    - [PQfname](#pqfname)
    - [PQfnumber](#pqfnumber)
    - [PQftable](#pqftable)
    - [PQftablecol](#pqftablecol)
    - [PQfformat](#pqfformat)
    - [PQftype](#pqftype)
    - [PQfmod](#pqfmod)
    - [PQfsize](#pqfsize)
    - [PQbinaryTuples](#pqbinarytuples)
    - [PQgetvalue](#pqgetvalue)
    - [PQgetisnull](#pqgetisnull)
    - [PQgetlength](#pqgetlength)
    - [PQnparams](#pqnparams)
    - [PQparamtype](#pqparamtype)
    - [PQprint](#pqprint)
 -  [杂项函数]()
    - [PQcopyResult]()

## 连接
[文档链接](http://www.postgres.cn/docs/17/libpq-connect.html)

### PQconnectdb
建立与数据库服务器的新连接。
```cpp
PGconn *PQconnectdb(
    const char *conninfo
);
```

> 此函数使用从字符串conninfo中获取的参数打开一个新的数据库连接。  
> 传入的字符串可以为空，以使用所有默认参数，或者它可以包含一个或多个由空白分隔的参数设置，也可以包含一个URI。  
> 详情请参见[参数关键词](connect_params.md)。

### PQfinish
关闭与服务器的连接。同时释放PGconn对象使用的内存。
```cpp
void PQfinish(
    PGconn *conn
);
```
> 注意，即使服务器连接尝试失败（如PQstatus所示），应用程序也应调用 PQfinish释放PGconn对象所使用的内存。  
> 在调用PQfinish之后，PGconn指针不得再次使用。

### PQreset
重置与服务器的通信通道。
```cpp
void PQreset(
    PGconn *conn
);
```

> 此函数将关闭与服务器的连接，并尝试使用之前所有相同的参数 建立一个新的连接。
> 如果工作连接丢失，这可能对错误恢复很有用。


## 状态
[文档链接](http://www.postgres.cn/docs/17/libpq-status.html)

### PQstatus
返回连接的状态。
```cpp
ConnStatusType PQstatus(
    const PGconn *conn
);
```

> 状态可以是多个值之一。

> 然而，在异步连接过程之外， 只有两个状态会被看到：CONNECTION_OK 和 CONNECTION_BAD。  
> 与数据库的良好连接 状态为 CONNECTION_OK。连接失败的尝试 由状态 CONNECTION_BAD 表示。  
> 通常， OK 状态会一直保持，直到 PQfinish， 但通信故障可能会导致状态提前变为 CONNECTION_BAD。  
> 在这种情况下，应用程序 可以尝试通过调用 PQreset 来恢复。

| Name                         | Description |
| ---                          | --- |
| CONNECTION_OK                | 连接成功 |
| CONNECTION_BAD               | 连接失败 |
| CONNECTION_STARTED           | 正在等待连接 |
| CONNECTION_MADE              | 连接正常；等待发送 |
| CONNECTION_AWAITING_RESPONSE | 正在等待来自数据库主进程的响应 |
| CONNECTION_AUTH_OK           | 已接收身份验证；等待后端启动 |
| CONNECTION_SETENV            | 此状态不再使用 |
| CONNECTION_SSL_STARTUP       | 正在执行SSL握手 |
| CONNECTION_NEEDED            | 内部状态：需要connect() |
| CONNECTION_CHECK_WRITABLE    | 正在检查会话是否为读写 |
| CONNECTION_CONSUME           | 消耗任何额外的消息 |
| CONNECTION_GSS_STARTUP       | 正在协商GSSAPI |
| CONNECTION_CHECK_TARGET      | 内部状态：正在检查目标服务器属性 |
| CONNECTION_CHECK_STANDBY     | 正在检查服务器是否处于待机模式 |
| CONNECTION_ALLOCATED         | 正在等待连接尝试启动 |

### PQerrorMessage
返回最近一次对连接操作产生的错误信息。
```cpp
char *PQerrorMessage(
    const PGconn *conn
);
```

> 几乎所有libpq函数在失败时都会为 PQerrorMessage 设置一条消息。  
> 请注意，根据libpq的约定，非空的 PQerrorMessage 结果可能由多行组成， 并且会包含一个结尾的换行符。调用者不应直接释放该结果。  
> 当关联的PGconn句柄被传递给 PQfinish时，它将被释放。结果字符串 不应期望在对PGconn结构的操作之间保持不变。

## 命令执行函数
[文档链接](http://www.postgres.cn/docs/17/libpq-exec.html)

### PQexec
提交一个命令给服务器并且等待结果。
```cpp
PGresult *PQexec(
    PGconn *conn,
    const char *command
);
```

> 返回一个PGresult指针或者可能是一个空指针。  
> 除了内存不足的情况或者由于严重错误无法将命令发送给服务器之外，一般都会返回一个非空指针。  
> PQresultStatus函数应当被调用来检查返回值是否代表错误（包括空指针的值，它会返回PGRES_FATAL_ERROR）。  
> 用 PQerrorMessage 可得到关于那些错误的详细信息。

> 命令字符串可以包括多个 SQL 命令（用分号分隔）。  
> 在一次PQexec调用中被发送的多个查询会在一个事务中处理，除非其中有显式的BEGIN/COMMIT命令将该查询字符串划分成
> 多个事务（服务器如何处理多查询字符串的更多细节请参考[Section 53.2.2.1](http://www.postgres.cn/docs/17/protocol-flow.html#PROTOCOL-FLOW-MULTI-STATEMENT)）。  
> 但是注意，返回的PGresult结构只描述该字符串中被执行的最后一个命令的结果。  
> 如果一个命令失败，该字符串的处理会在它那里停止并且返回的PGresult会描述错误情况。

### PQexecParams
提交一个命令给服务器并且等待结果，它可以在 SQL 命令文本之外独立地传递参数。
```cpp
PGresult *PQexecParams(
    // 要在其中发送命令的连接对象。
    PGconn *conn,

    // 要执行的SQL命令字符串。
    // 如果使用了参数，它们在该命令字符串中被引用为$1、$2等。
    const char *command,

    // 提供的参数数量。
    /* 它是数组paramTypes、paramValues、paramLengths和paramFormats的长度（当nParams为
     * 零时，数组指针可以是NULL）。
     */
    int nParams,

    // 通过 OID 指定要赋予给参数符号的数据类型。
    /* 如果paramTypes为NULL或者该数组中任何特定元素为零，服务器会用
     * 对待未知类型文字串的方式为参数符号推测一种数据类型。
     */
    const Oid *paramTypes,

    // 指定参数的实际值。
    /* 这个数组中的一个空指针表示对应的参数为空，否则该指针指向
     * 一个以零终止的文本字符串（用于文本格式）或者以服务器所期待
     * 格式的二进制数据（用于二进制格式）。
     */
    const char *const *paramValues,

    // 指定二进制格式参数的实际数据长度。
    // 它对空参数和文本格式参数被忽略。当没有二进制参数时，该数组指针可以为空。
    const int *paramLengths,

    // 指定参数是否为文本（在参数相应的数组项中放一个零）或二进制（在参数相应的数组项中放一个一）。
    // 如果该数组指针为空，那么所有参数都会被假定为文本串。
    const int *paramFormats,

    /* 指定零来得到文本格式的结果，或者指定一来得到二进制格式
     * 的结果（目前没有规定要求以不同格式得到不同的结果列，
     * 尽管在底层协议中这是可以实现的）。
     */
    int resultFormat
);
```

> PQexecParams相对于PQexec的主要优点是参数值可以从命令串中分离，因此避免了冗长的书写、容易发生错误的引用以及转义。  

> 和PQexec不同，PQexecParams至多允许在给定串中出现一个 SQL 命令（其中可以有分号，但是不能有超过一个非空命令）。  
> 这是底层协议的一个限制，但是有助于抵抗 SQL 注入攻击。

### PQprepare
提交请求以使用给定参数创建一个预处理语句，并等待完成。
```cpp
PGresult *PQprepare(
    // 要在其中发送命令的连接对象。
    PGconn *conn,

    // 语句的名字（自定义，相当于一个标识符）
    const char *stmtName,

    // 执行的SQL指令
    const char *query,

    // 提供的参数数量。
    int nParams,

    // 通过 OID 指定要赋予给参数符号的数据类型。
    const Oid *paramTypes
);
```

> PQprepare 创建一个预处理语句，以便稍后使用 PQexecPrepared 执行。  
> 此功能允许命令被重复执行，而无需每次都进行解析和规划；详见 [PREPARE](http://www.postgres.cn/docs/17/sql-prepare.html)。  

> 该函数从 query 字符串创建一个名为 stmtName 的预处理语句， 该字符串必须包含单个 SQL 命令。  
> stmtName 可以是 "" 来创建一个无名语句， 在这种情况下，任何预先存在的无名语句都会被自动替换；否则，如果当前会话中已定义该语句名，则会报错。  
> 如果使用了参数，则在查询中以 $1、$2 等形式引用它们。 nParams 是预先指定类型的参数数量，类型存储在数组 paramTypes[] 中。  
> （当 nParams 为零时，数组指针可以是 NULL。）paramTypes[] 通过 OID 指定分配给参数符号的数据类型。  
> 如果 paramTypes 是 NULL， 或数组中的某个元素为零，服务器会以未类型化的字面字符串方式为参数符号分配数据类型。  
> 此外，查询可以使用编号高于 nParams 的参数符号；这些符号的数据类型也会被推断。  
> （参见 PQdescribePrepared 了解如何查询推断的数据类型。）  

> 与 PQexec 类似，结果通常是一个 PGresult 对象， 其内容指示服务器端的成功或失败。空结果表示内存不足或无法发送命令。  
> 使用 PQerrorMessage 获取有关此类错误的更多信息。  

> 用于 PQexecPrepared 的预处理语句也可以通过执行 SQL PREPARE 语句来创建。

### PQexecPrepared
发送一个请求来执行一个带有给定参数的预处理语句，并等待结果。
```cpp
PGresult *PQexecPrepared(
    PGconn *conn,
    const char *stmtName,
    int nParams,
    const char * const *paramValues,
    const int *paramLengths,
    const int *paramFormats,
    int resultFormat
);
```

> PQexecPrepared类似于PQexecParams， 但要执行的命令是通过指定先前准备好的语句来指定，而不是提供查询字符串。  
> 此功能允许重复使用的命令只被解析和计划一次，而不是每次执行时都要进行。 该语句必须在当前会话中先前准备好。

> 参数与PQexecParams相同，只是给出了预处理语句的名称而不是查询字符串， 并且paramTypes[]参数不存在（因为在创建预处理语句时已确定了参数类型）。

### PQdescribePrepared
提交请求以获取有关指定准备好的语句的信息，并等待完成。
```cpp
PGresult *PQdescribePrepared(
    PGconn *conn,
    const char *stmtName
);
```

> PQdescribePrepared允许应用程序获取关于先前准备的语句的信息。

> stmtName可以是""或NULL来引用 未命名的语句，否则必须是现有准备好的语句的名称。  
> 成功时，返回一个 状态为PGRES_COMMAND_OK的PGresult。  
> 函数PQnparams和 PQparamtype可以应用于此 PGresult以获取有关准备语句参数的信息，函数PQnfields、  
> PQfname、 PQftype等提供有关语句的结果列（如果有）的信息。

### PQdescribePortal
提交请求以获取有关指定门户的信息，并等待完成。
```cpp
PGresult *PQdescribePortal(
    PGconn *conn,
    const char *portalName
);
```

> PQdescribePortal允许应用程序获取有关先前创建的portal的信息。  
> (libpq不直接提供对portals的访问，但您可以使用此函数检查使用DECLARE CURSOR SQL命令创建的游标的属性。)

> portalName可以是""或NULL来引用未命名的portal， 否则必须是现有portal的名称。  
> 成功时，将返回一个带有状态PGRES_COMMAND_OK的PGresult。  
> 函数PQnfields、PQfname、PQftype等可应用于 PGresult，以获取有关portal的结果列（如果有）的信息。

### PQclosePrepared
提交请求以关闭指定的预处理语句，并等待完成。
```cpp
PGresult *PQclosePrepared(
    PGconn *conn,
    const char *stmtName
);
```

> PQclosePrepared 允许应用程序关闭先前准备好的语句。关闭语句会释放服务器上 与其相关的所有资源，并允许重新使用其名称。

> stmtName 可以是 "" 或 NULL，用于引用未命名的语句。如果不存在该名称的语句， 也没关系，在这种情况下操作不会执行任何操作。  
> 成功时，会返回一个 PGresult，其状态为 PGRES_COMMAND_OK。

### PQclosePortal
提交请求以关闭指定的门户，并等待完成。
```cpp
PGresult *PQclosePortal(
    PGconn *conn,
    const char *portalName
);
```

> PQclosePortal 允许应用程序触发关闭先前创建的门户。 关闭门户会释放服务器上所有相关资源，并允许重用其名称。  
> (libpq 不提供对门户的直接访问，但您可以使用此函数关闭 通过 DECLARE CURSOR SQL 命令创建的游标。)

> portalName 可以是 "" 或 NULL，用于引用未命名的门户。如果不存在具有此名称的门户， 也没关系，在这种情况下操作不会执行任何操作。  
> 成功时，将返回状态为 PGRES_COMMAND_OK 的 PGresult。

---

PGresult 结构封装了服务器返回的结果。  
libpq 应用程序开发人员应该小心维护 PGresult 抽象。  
使用下面的访问器函数来获取 PGresult 的内容。  
避免直接引用 PGresult 结构的字段，因为它们在未来可能会发生变化。

### PQresultStatus
返回命令的结果状态。
```cpp
ExecStatusType PQresultStatus(
    const PGresult *res
);
```

PQresultStatus 可以返回以下值之一。

| Name                   | Description |
| ---                    | --- |
| PGRES_EMPTY_QUERY      | 发送到服务器的字符串是空的。 |
| PGRES_COMMAND_OK       | 成功完成一个不返回数据的命令。 |
| PGRES_TUPLES_OK        | 成功完成返回数据的命令（例如SELECT或SHOW）。 |
| PGRES_COPY_OUT         | 从服务器复制数据传输已开始。 |
| PGRES_COPY_IN          | 开始进行复制（到服务器）的数据传输。 |
| PGRES_BAD_RESPONSE     | 服务器的响应无法被理解。 |
| PGRES_NONFATAL_ERROR   | 发生了一个非致命错误（通知或警告）。 |
| PGRES_FATAL_ERROR      | 发生了致命错误。 |
| PGRES_COPY_BOTH        | 开始进行数据传输（从服务器复制到服务器）。此功能目前仅用于流式复制， 因此在普通应用程序中不应出现此状态。 |
| PGRES_SINGLE_TUPLE     | PGresult包含当前命令的单个结果元组。 仅当查询选择了单行模式时才会出现此状态 （请参阅[Section 32.6](http://www.postgres.cn/docs/17/libpq-single-row-mode.html)）。 |
| PGRES_TUPLES_CHUNK     | 结构体PGresult包含当前命令的多个结果元组。 该状态仅在查询选择了分块模式时出现 （参见[Section 32.6](http://www.postgres.cn/docs/17/libpq-single-row-mode.html)）。 元组数量不会超过传递给 PQsetChunkedRowsMode的限制。 |
| PGRES_PIPELINE_SYNC    | PGresult表示流水线模式中的一个同步点，由 PQpipelineSync或 PQsendPipelineSync请求。 该状态仅在选择流水线模式时出现。 |
| PGRES_PIPELINE_ABORTED | PGresult表示接收到服务器错误的管道。 必须重复调用PQgetResult，每次调用都会返回此状态码， 直到当前管道结束，此时它将返回PGRES_PIPELINE_SYNC， 然后可以恢复正常处理。 |

> 如果结果状态是PGRES_TUPLES_OK、 PGRES_SINGLE_TUPLE或 PGRES_TUPLES_CHUNK，则下面描述的函数可用于检索查询返回的行。

> 注意，即使SELECT命令检索到零行，仍然显示 PGRES_TUPLES_OK。  
> PGRES_COMMAND_OK用于永远不会返回行的命令（如 INSERT或UPDATE，且没有 RETURNING子句等）。  
> PGRES_EMPTY_QUERY的响应可能表示客户端软件中的一个错误。

> 状态PGRES_NONFATAL_ERROR的结果永远不会直接由 PQexec或其他查询执行函数返回；
> 此类结果 会被传递给通知处理器（参见[Section 32.13](http://www.postgres.cn/docs/17/libpq-notice-processing.html)）。

### PQresStatus
将由PQresultStatus返回的枚举类型转换为描述状态代码的 字符串常量。调用者不应释放结果。
```cpp
char *PQresStatus(
    ExecStatusType status
);
```

### PQresultErrorMessage
返回与命令相关的错误消息，如果没有错误，则返回一个空字符串。
```cpp
char *PQresultErrorMessage(
    const PGresult *res
);
```

> 如果存在错误，返回的字符串将包含一个尾随的换行符。调用者不应直接 释放结果。当关联的PGresult句柄被传递给 PQclear时，它将被释放。

> 紧接着调用PQexec或 PQgetResult之后， 连接上的 PQerrorMessage 将返回与 结果上的PQresultErrorMessage相同的字符串。  
> 然而，一个PGresult会保留其错误信息直到被销毁， 而连接的错误信息会在后续操作完成时发生变化。  
> 当您想了解与特定PGresult相关的状态时， 请使用PQresultErrorMessage； 当您想了解连接上最新操作的状态时， 请使用 PQerrorMessage 。

### PQresultVerboseErrorMessage
返回与PGresult对象相关的错误消息的重新格式化版本。
```cpp
char *PQresultVerboseErrorMessage(
    const PGresult *res,
    PGVerbosity verbosity,
    PGContextVisibility show_context
);
```

> 在某些情况下，客户端可能希望获取先前报告的错误的更详细版本。  
> PQresultVerboseErrorMessage通过计算在生成给定 的PGresult时，如果连接的指定详细设置生效，PQresultErrorMessage将会生成的消息来满足这一需求。  
> 如果PGresult不是错误结果，则会报告 “PGresult 不是错误结果”。返回的字符串包括一个尾随换行符。

> 与大多数其他用于从PGresult中提取数据的函数不同， 此函数的结果是一个新分配的字符串。调用者必须在字符串不再需要时， 使用PQfreemem()释放它。

> 如果内存不足，可能会返回NULL。

### PQresultErrorField
返回错误报告的单个字段。
```cpp
char *PQresultErrorField(
    const PGresult *res,
    int fieldcode
);
```

> fieldcode 是一个错误字段标识符；请参阅下面列出的符号。 如果 PGresult 不是错误或警告结果，或者不包含指定字段， 则返回 NULL。  
> 字段值通常不会包含尾随换行符。 调用者不应直接释放结果。当关联的 PGresult 句柄被传递给 PQclear 时，它将被释放。

以下字段代码可用：

| Name                          | Description |
| ---                           | --- |
| PG_DIAG_SEVERITY              | 严重性；字段内容为ERROR、FATAL或 PANIC（在错误消息中），或者WARNING、 NOTICE、DEBUG、INFO或 LOG（在通知消息中），或者是这些之一的本地化翻译。始终存在。 |
| PG_DIAG_SEVERITY_NONLOCALIZED | 严重性；字段内容为ERROR、 FATAL或PANIC（在错误消息中）， 或WARNING、NOTICE、 DEBUG、INFO或LOG （在通知消息中）。这与PG_DIAG_SEVERITY字段完全相同， 只是内容从未被本地化。此字段仅出现在由 PostgreSQL 9.6及更高版本生成的报告中。 |
| PG_DIAG_SQLSTATE              | 错误的SQLSTATE代码。SQLSTATE代码标识发生的错误类型；前端应用程序可以 使用它来执行特定操作（例如错误处理）以响应特定的数据库错误。 有关可能的SQLSTATE代码的列表，请参见Appendix A。此字段不可本地化，并且始终存在。 |
| PG_DIAG_MESSAGE_PRIMARY       | 主要的人类可读错误信息（通常为一行）。始终存在。 |
| PG_DIAG_MESSAGE_DETAIL        | 详情：一个可选的次要错误信息，提供有关问题的更多 细节。可能会占用多行。 |
| PG_DIAG_MESSAGE_HINT          | 提示：关于如何解决问题的可选建议。 这与细节不同，因为它提供建议（可能不合适），而不是硬性事实。 可能会占用多行。 |
| PG_DIAG_STATEMENT_POSITION    | 包含一个十进制整数的字符串，表示错误光标位置，作为原始语句字符串 的索引。第一个字符的索引为1，位置以字符而非字节为单位测量。 |
| PG_DIAG_INTERNAL_POSITION     | 这与PG_DIAG_STATEMENT_POSITION字段的定义相同，但当光标位置 指向一个内部生成的命令而不是客户端提交的命令时使用。每当此字段出现时， PG_DIAG_INTERNAL_QUERY字段也会始终出现。 |
| PG_DIAG_INTERNAL_QUERY        | 内部生成的命令失败的文本。例如，这可能是由PL/pgSQL函数发出的 SQL查询。 |
| PG_DIAG_CONTEXT               | 指示错误发生的上下文。目前，这包括活动过程语言函数和内部生成查询的调用堆栈回溯。 跟踪信息每行一个条目，最近的条目在最前面。 |
| PG_DIAG_SCHEMA_NAME           | 如果错误与特定的数据库对象相关，则包含该对象的模式名称（如果有）。 |
| PG_DIAG_TABLE_NAME            | 如果错误与特定表相关，则为该表的名称。（请参阅架构名称字段以获取 表的架构名称。） |
| PG_DIAG_COLUMN_NAME           | 如果错误与特定的表列相关，则为列的名称。（请参阅模式和表名字段以 确定表。） |
| PG_DIAG_DATATYPE_NAME         | 如果错误与特定数据类型相关，则为该数据类型的名称。（请参阅架构名称字段以获取 数据类型的架构名称。） |
| PG_DIAG_CONSTRAINT_NAME       | 如果错误与特定约束相关，则为约束的名称。请参阅上面列出的字段， 以获取相关的表或域。（在此情况下，索引被视为约束，即使它们不是 使用约束语法创建的。） |
| PG_DIAG_SOURCE_FILE           | 错误报告所在源代码位置的文件名。 |
| PG_DIAG_SOURCE_LINE           | 错误报告所在源代码位置的行号。 |
| PG_DIAG_SOURCE_FUNCTION       | 报告错误的源代码函数的名称。 |

> 架构名称、表名称、列名称、数据类型名称和约束名称字段仅适用于有限 类型的错误；请参见[Appendix A](http://www.postgres.cn/docs/17/errcodes-appendix.html)。不要假设这些 字段中的任何一个的存在就能保证另一个字段的存在。核心错误来源遵循 上述的相互关系，但用户定义的函数可能以其他方式使用这些字段。同样， 不要假设这些字段表示当前数据库中的现有对象。
> 客户端负责格式化显示的信息以满足其需求；特别是，它应该根据需要 换行。错误消息字段中出现的换行符应被视为段落分隔符，而不是 换行符。
> 由libpq内部生成的错误将具有严重性和主要消息， 但通常没有其他字段。
> 请注意，错误字段仅可从PGresult对象中获取， 而不能从PGconn对象中获取；没有 PQerrorField函数。

### PQclear
释放与PGresult相关联的存储。每个命令结果在不再需要时， 都应该通过PQclear释放。
```cpp
void PQclear(
    PGresult *res
);
```

> 如果参数是NULL指针，则不执行任何操作。

> 您可以保留一个PGresult对象，只要您需要它；它不会在您发 出新命令时消失，甚至在您关闭连接时也不会消失。  
> 要删除它，您必须调用 PQclear。未能这样做将导致您的应用程序中出现内存泄漏。

### PQntuples
返回查询结果中的行（元组）数（注意，PGresult对象被限制为不超过INT_MAX行，因此一个int结果就足够了）。
```cpp
int PQntuples(
    const PGresult *res
);
```

### PQnfields
返回查询结果中每一行的列（域）数。
```cpp
int PQnfields(
    const PGresult *res
);
```

### PQfname
返回与给定列号相关联的列名。列号从 0 开始。调用者不应该直接释放该结果。它将在相关的PGresult句柄被传递给PQclear之后被释放。
```cpp
char *PQfname(
    const PGresult *res,
    int column_number
);
```

> 如果列号超出范围，将返回NULL。

### PQfnumber
返回与给定列名相关联的列号。
```cpp
int PQfnumber(
    const PGresult *res,
    const char *column_name
);
```

> 如果给定的名字不匹配任何列，将返回 -1。

> 给定的名称被视作一个 SQL 命令中的一个标识符，也就是说，除非被双引号引用，它是小写形式的。例如，给定一个 SQL 命令：
> ```sql
> SELECT 1 AS FOO, 2 AS "BAR";
> ```
> 我们将得到结果：
> ```text
> PQfname(res, 0)              foo
> PQfname(res, 1)              BAR
> PQfnumber(res, "FOO")        0
> PQfnumber(res, "foo")        0
> PQfnumber(res, "BAR")        -1
> PQfnumber(res, "\"BAR\"")    1
> ```

### PQftable
返回给定列从中取出的表的 OID。列号从 0 开始。
```cpp
Oid PQftable(
    const PGresult *res,
    int column_number
);
```
> 如果列号超出范围或者指定的列不是对一个表列的简单引用时，返回InvalidOid。
> 你可以查询系统表pg_class来确定究竟是哪个表被引用。

> 当你包括libpq头文件，类型oid以及常数InvalidOid将被定义。它们将都是某种整数类型。

### PQftablecol
返回构成指定查询结果列的列（在其表中）的列号。查询结果列号从 0 开始，但是表列具有非零编号。
```cpp
int PQftablecol(
    const PGresult *res,
    int column_number
);
```
> 如果列号超出范围或者指定的列不是对一个表列的简单引用时，返回零。

### PQfformat
返回指示给定列格式的格式编码。列号从 0 开始。
```cpp
int PQfformat(
    const PGresult *res,
    int column_number
);
```

> 格式代码零指示文本数据表示，而格式代码一表示二进制表示（其他代码被保留用于未来的定义）。

### PQftype
返回与给定列号相关联的数据类型。被返回的整数是该类型的内部 OID 号。列号从 0 开始。
```cpp
Oid PQftype(
    const PGresult *res,
    int column_number
);
```

> 你可以查询系统表pg_type来得到多个数据类型的名字和属性。  
> 内建数据类型的OID被定义在PostgreSQL安装的include目录中的catalog/pg_type_d.h文件中。

### PQfmod
返回与给定列号相关联的列的修饰符类型。列号从 0 开始。
```cpp
int PQfmod(
    const PGresult *res,
    int column_number
);
```

> 修饰符值的解释是与类型相关的，它们通常指示精度或尺寸限制。值 -1 被用来指示“没有信息可用”。  
> 大部分的数据类型不适用修饰符，在那种情况中值总是 -1。

### PQfsize
返回与给定列号相关的列的尺寸（以字节计）。列号从 0 开始。
```cpp
int PQfsize(
    const PGresult *res,
    int column_number
);
```

> PQfsize返回在一个数据库行中为这个列分配的空间，换句话说是服务器对该数据类型的内部表示
> 的尺寸（因此，它对客户端并不是真地非常有用）。一个负值指示该数据类型是变长的。

### PQbinaryTuples
如果PGresult包含二进制数据，返回 1。如果包含的是文本数据，返回 0。
```cpp
int PQbinaryTuples(
    const PGresult *res
);
```

> 这个函数已经被废弃（除了与COPY一起使用），因为一个单一PGresult可以在某些列中包含文本
> 数据而且在另一些列中包含二进制数据。  
> PQfformat要更好。只有结果的所有列是二进制（格式 1）时PQbinaryTuples才返回 1。

### PQgetvalue
返回一个PGresult的一行的单一域值。行和列号从 0 开始。调用者不应该直接释放该结果。  
它将在相关的PGresult句柄被传递给PQclear之后被释放。
```cpp
char *PQgetvalue(
    const PGresult *res,
    int row_number,
    int column_number
);
```

> 对于文本格式的数据，PQgetvalue返回的值是该域值的一种空值结束的字符串表示。  
> 对于二进制格式的数据，该值是由该数据类型的typsend和typreceive函数决定的二进制
> 表示（在这种情况下该值实际上也跟随着一个零字节，但是这通常没有用处，因为该值很可能包含嵌入的空）。

> 如果该域值为空，则返回一个空串。关于区分空值和空字符串值请见PQgetisnull。

> PQgetvalue返回的指针指向作为PGresult结构一部分的存储。我们不应该修改它指向的数据，并且如果要在超过PGresult结构本身的生命期之外使用它，我们必须显式地把该数据拷贝到其他存储中。

### PQgetisnull
测试一个域是否为空值。行号和列号从 0 开始。
```cpp
int PQgetisnull(
    const PGresult *res,
    int row_number,
    int column_number
);
```

> 如果该域是空，这个函数返回 1。  
> 如果它包含一个非空值，则返回 0（注意PQgetvalue将为一个空域返回一个空串，不是一个空指针）。

### PQgetlength
返回一个域值的真实长度，以字节计。行号和列号从 0 开始。
```cpp
int PQgetlength(
    const PGresult *res,
    int row_number,
    int column_number
);
```
> 这是特定数据值的真实数据长度，也就是PQgetvalue指向的对象的尺寸。  
> 对于文本数据格式，这和strlen()相同。  
> 对于二进制格式这是基本信息。  
> 注意我们不应该依赖于PQfsize来得到真值的数据长度。

### PQnparams
返回一个预备语句的参数数量。
```cpp
int PQnparams(
    const PGresult *res
);
```

> 此函数仅在检查PQdescribePrepared的结果时有用。  
> 对于其他类型的结果，它将返回零。

### PQparamtype
返回所指示的语句参数的数据类型。参数号从 0 开始。
```cpp
Oid PQparamtype(
    const PGresult *res,
    int param_number
);
```

> 此函数仅在检查PQdescribePrepared的结果时有用。  
> 对于其他类型的结果，它将返回零。

### PQprint
将所有的行打印到指定的输出流，以及有选择地将列名打印到指定的输出流。
```cpp
void PQprint(
    FILE *fout, /* 输出流 */
    const PGresult *res,
    const PQprintOpt *po
);

typedef struct {
    pqbool  header;      /* 打印输出域标题和行计数 */
    pqbool  align;       /* 填充对齐域 */
    pqbool  standard;    /* 旧的格式 */
    pqbool  html3;       /* 输出 HTML 表格 */
    pqbool  expanded;    /* 扩展表格 */
    pqbool  pager;       /* 如果必要为输出使用页 */
    char    *fieldSep;   /* 域分隔符 */
    char    *tableOpt;   /* 用于 HTML 表格元素的属性 */
    char    *caption;    /* HTML 表格标题 */
    char    **fieldName; /* 替换域名称的空终止数组 */
} PQprintOpt;
```

> 这个函数以前被psql用来打印查询结果，但是现在不是这样了。  
> 注意它假定所有的数据都是文本格式。

## 杂项函数
[文档链接](http://postgres.cn/docs/17/libpq-misc.html)

### PQcopyResult
为一个PGresult对象创建一个拷贝。 这个拷贝不会以任何方式链接到源结果，并且当该拷贝不再需要时，必须调用PQclear进行清理。如果函数失败，返回NULL。
```cpp
PGresult *PQcopyResult(
    // 源PGresult对象
    const PGresult *src,

    /* PG_COPYRES_ATTRS          指定复制源结果的属性（列定义）
     * PG_COPYRES_TUPLES         指定复制源结果的元组（这也意味着复制属性）
     * PG_COPYRES_NOTICEHOOKS    指定复制源结果的通知钩子
     * PG_COPYRES_EVENTS         指定复制源结果的事件（但不复制与源相关的任何实例数据）
     */
    int flags
);
```
> 这不是为了制作一个精确的副本。

> 返回的结果总是放在PGRES_TUPLES_OK状态中，并且不复制源中的任何错误消息（但是会复制命令状态字符串）。  
> flags参数确定要复制的其他内容，它是几个标志的按位或。
