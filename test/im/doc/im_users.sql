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
    -- 用户对应的盐值，非空
    salt     BYTEA                     NOT NULL,
    -- 用户密码哈希，固定32字节(SHA-256)，非空
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

/*
-- 选择数据
SELECT hash from users where uid = 0;

-- 插入数据
INSERT INTO users (uid, name, salt, hash, status, created)
VALUES 
(
    0,
    'SN-Grotesque',
    E'\\xd3b99434350266dd3270389cb7167e80',
    E'\\x045d2e91b18e81c586984ad83838f092f9491f3f8655a918f27c34eae26e51e5',
    'NORMAL',
    0
),
(
    183898888,
    'test_user',
    E'\\xec1e4d6d87a06d6b10cfb5290bc14ab1',
    E'\\x97a6f01a152846a474c8041905e1227f5cf0170e1b9a40d262a0a151e68cdc8a',
    'NORMAL',
    1755114535
);

-- 更新数据
UPDATE users SET uid = 0 WHERE name = 'SN-Grotesque';

-- 删除数据（不加where会直接删除整个表数据）
DELETE FROM users WHERE uid = 0;

-- 修改列名
ALTER TABLE users RENAME COLUMN created TO create_time;
*/
