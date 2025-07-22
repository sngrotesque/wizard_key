# 🔐 The Wizard's Universal Key

## ⚠️ 注意事项

自 `v0.9` 起，FEA（Fast Encryption Algorithm）与 `v0.8` 中的实现完全不同，尽管名称相同，但它们是两个独立的加密算法。

---

## 🧪 v0.9 更新内容

0. 请暂时不要使用此库中包含的加密算法（包括OP4/FAE/SSC），因为它们可能并不是最新版本，可能引入某些风险。

1. 重新以密码学安全标准实现了 **SSC (SN Stream Cipher)** 和 **FEA (Fast Encryption Algorithm)** 加密算法。
   - 💰 如果有人能成功破解 SSC 加密算法，我将奖励人民币 1,000 元。
   - 示例密文（SSC RAW）：`6cRcip3GaBKiu/O2l9Iu3phYCxKKoLd6vyX2PQXiqAeU4kOCBo9csBjsa4hVGhl=`。

2. 修复了 FEA 加密算法的严重缺陷：
   - 问题：由于密钥扩展函数存在缺陷，密钥的每个字节的高两位未被有效利用。
   - 结果：密钥空间从理论上的 2<sup>256</sup> 降至实际的 2<sup>192</sup>，严重影响安全性。
   - 修复：将密钥空间恢复至 2<sup>256</sup>。

3. 设计了 **OP4 (four fundamental operations)** 加密算法，以用作未来的密码算法（替代 SSC 和 FEA）。
   - 它支持高并行化，可使用SIMD高度优化性能（摆脱 SSC / FEA 的自引用依赖导致的性能问题）。
   - 采用了独特的 **192-bit** 分组大小和 **8** 轮加密，如此也能保证超高安全性。
