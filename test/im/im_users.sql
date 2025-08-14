-- 注意此SQL为PostgreSQL专用，不一定兼容其他数据库。

-- 创建数据库
CREATE DATABASE im;

-- 创建数据表
CREATE TABLE users (
    -- 用户UID，兼容int64_t，非空且唯一
    uid BIGINT NOT NULL PRIMARY KEY,

    -- 用户名，最大32字符，非空
    name VARCHAR(32) NOT NULL,

    -- 用户对应的盐值（暂时使用Hex显示），后续考虑是否使用BYTEA类型），非空
    salt CHAR(32) NOT NULL,

    -- 用户密码哈希，固定32字节(SHA-256)，非空
    pwd_hash CHAR(64) NOT NULL,

    -- 用户创建时间，UNIX时间戳(整数)，非空
    created DOUBLE PRECISION NOT NULL,

    -- 用户账号状态，仅允许正常使用/已注销，非空
    active BOOLEAN NOT NULL
);

-- 插入数据
INSERT INTO users (uid, name, salt, pwd_hash, created, active)
VALUES 
    (0, 'SN-Grotesque',
    'd3b99434350266dd3270389cb7167e80',
    '045d2e91b18e81c586984ad83838f092f9491f3f8655a918f27c34eae26e51e5',
    1755114362,
    TRUE),
    (183898888, 'test_user',
    'ec1e4d6d87a06d6b10cfb5290bc14ab1',
    '97a6f01a152846a474c8041905e1227f5cf0170e1b9a40d262a0a151e68cdc8a',
    1755114535,
    TRUE);
