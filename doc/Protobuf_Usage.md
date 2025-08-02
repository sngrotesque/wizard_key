# Protobuf (MSYS2/MinGW64特供版)

### 警告

1. 不要他妈的在 **MSYS2** 中使用包含 **Abseil库** 的Protobuf版本！
2. 对于 **Linux** 和 **Visual Studio** 的环境，请自行编译部署最新版的Protobuf（包含Abseil库）。

### 安装

1. 常规情况
```bash
# 卸载当前版本（如果有）
pacman -R mingw-w64-x86_64-protobuf

# 安装21.12版本（最后一个不依赖Abseil的稳定版）
pacman -U https://archive.archlinux.org/packages/m/mingw-w64-x86_64-protobuf/mingw-w64-x86_64-protobuf-21.12-1-any.pkg.tar.zst

# 验证安装
protoc --version  # 应显示 libprotoc 3.21.12
```

2. 备用方案（当Arch仓库无法使用）
```bash
wget https://github.com/protocolbuffers/protobuf/releases/download/v21.12/protobuf-all-21.12.tar.gz
tar -xzf protobuf-all-21.12.tar.gz
cd protobuf-21.12
./configure --prefix=/mingw64 --disable-absl
make && make install
```

3. 验证安装完整性
```bash
# 检查头文件路径
ls /mingw64/include/google/protobuf/message.h

# 检查库文件
ls /mingw64/lib/libprotobuf.a
```

4. 环境配置（可选）
```bash
# 临时添加编译器和protoc到PATH
export PATH="/mingw64/bin:$PATH"

# 永久生效（添加到~/.bashrc）
echo 'export PATH="/mingw64/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### 使用
1. 根据`.proto`配置重新生成所需的代码文件
```bash
# 删除旧生成的pb.cc/pb.h文件
rm -f packet.pb.*

# 用新版本重新生成
protoc --cpp_out=. packet.proto
```

2. 编译指令
```bash
g++ test.cc packet.pb.cc --std=c++17 -O3 -lprotobuf -o test.exe
```

3. 永久锁定版本
```bash
# 编辑pacman.conf防止自动升级
echo -e "\n# 锁定Protobuf版本\nIgnorePkg = mingw-w64-x86_64-protobuf" | sudo tee -a /etc/pacman.conf

# 验证锁定
pacman -Sy protobuf  # 应提示"已忽略更新"
```

#### 常见问题
1. **链接失败**：确保使用`-lprotobuf`而非`-lprotobuf-lite`
2. **版本冲突**：运行`pacman -Q | grep protobuf`确认无残留旧版本
3. **生成文件过期**：修改.proto后必须重新执行`protoc`
