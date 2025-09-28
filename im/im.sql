-- 注意此SQL为PostgreSQL专用，不一定兼容其他数据库。

-- 创建数据库
CREATE DATABASE im;

-- 创建枚举类型（用于账号状态）
CREATE TYPE ACCOUNT_STATUS AS ENUM (
    'NORMAL',  -- 正常状态
    'BANNED',  -- 封禁状态
    'MUTED',   -- 禁言状态
    'DELETED'  -- 删除状态
);

-- 创建数据表
CREATE TABLE users (
    -- 用户UID，兼容int64_t，非空且唯一，自账户创建起不再改变。
    uid      BIGINT PRIMARY KEY        NOT NULL,
    -- 用户名，最大32字符，非空且唯一（在业务层判断唯一性，因数据库基本只支持一表一键）
    name     VARCHAR(32) UNIQUE        NOT NULL,
    -- 用户对应的盐值（目前固定为128位），非空
    salt     BYTEA                     NOT NULL,
    -- 用户密码哈希，固定256位（使用PBKDF2通过用户密码和SALT生成），非空
    hash     BYTEA                     NOT NULL,
    -- 枚举字段，表示账号状态
    status   ACCOUNT_STATUS            NOT NULL DEFAULT 'NORMAL',

    -- 用户创建时间，UNIX时间戳(64位浮点数)，非空
    created  DOUBLE PRECISION          NOT NULL DEFAULT 0,
    -- 用户修改时间，如改密码等，非空（默认为创建时间或0）
    modified DOUBLE PRECISION          NOT NULL DEFAULT 0,
    -- 账户最后访问时间，非空（默认为创建时间或0）
    accessed DOUBLE PRECISION          NOT NULL DEFAULT 0,
    -- 账户注销时间，给此值添加实际值时应同时修改active。非空（默认为0）
    deleted  DOUBLE PRECISION          NOT NULL DEFAULT 0
);
