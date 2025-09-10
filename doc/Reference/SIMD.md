

### **一、SSE2 常用类型与函数**
#### **1. 数据类型**
| 类型              | 位宽  | 描述                     |
|-------------------|-------|--------------------------|
| `__m128i`         | 128位 | 通用整数向量（8/16/32/64位） |
| `__m128d`         | 128位 | 双精度浮点向量（2个`double`） |
| `__m128`          | 128位 | 单精度浮点向量（4个`float`，SSE1） |

#### **2. 整数运算**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm_add_epi8/16/32/64`  | `PADDB/W/D/Q` | 整数加法（8/16/32/64位）       |
| `_mm_adds_epi8/16`       | `PADDSB/W`    | 饱和加法（结果钳制到范围）      |
| `_mm_sub_epi8/16/32/64`  | `PSUBB/W/D/Q` | 整数减法                       |
| `_mm_mullo_epi16`        | `PMULLW`      | 低16位乘法（保留16位结果）      |
| `_mm_mulhi_epi16`        | `PMULHW`      | 高16位乘法（保留16位结果）      |
| `_mm_slli_epi16/32/64`   | `PSLLW/D/Q`   | 逻辑左移                       |
| `_mm_srli_epi16/32/64`   | `PSRLW/D/Q`   | 逻辑右移                       |
| `_mm_srai_epi16/32`      | `PSRAW/D`     | 算术右移（保留符号位）           |
| `_mm_and_si128`          | `PAND`        | 按位与                         |
| `_mm_or_si128`           | `POR`         | 按位或                         |
| `_mm_xor_si128`          | `PXOR`        | 按位异或                       |

#### **3. 浮点运算（双精度）**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm_add_pd`             | `ADDPD` | 双精度浮点加法                |
| `_mm_sub_pd`             | `SUBPD` | 双精度浮点减法                |
| `_mm_mul_pd`             | `MULPD` | 双精度浮点乘法                |
| `_mm_div_pd`             | `DIVPD` | 双精度浮点除法                |
| `_mm_sqrt_pd`            | `SQRTPD` | 双精度平方根                  |
| `_mm_max_pd`             | `MAXPD` | 双精度最大值                  |
| `_mm_min_pd`             | `MINPD` | 双精度最小值                  |

#### **4. 内存操作**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm_load_si128`         | `MOVDQA` | 对齐加载（16字节对齐）        |
| `_mm_loadu_si128`        | `MOVDQU` | 未对齐加载                    |
| `_mm_store_si128`        | `MOVDQA` | 对齐存储                      |
| `_mm_storeu_si128`       | `MOVDQU` | 未对齐存储                    |
| `_mm_shuffle_epi32`      | `PSHUFD` | 32位整数重排                  |

### **二、SSE3 常用类型与函数**

#### **1. 数据类型**
> SSE3 没有引入新的数据类型，它继续使用 SSE/SSE2 中的 `__m128`, `__m128i`, `__m128d` 等 128 位类型。

#### **2. 新增整数与浮点运算**

| 函数                          | 指令       | 作用说明                                   |
|-------------------------------|------------|--------------------------------------------|
| `_mm_addsub_ps`               | `ADDSUBPS` | 交替执行加法/减法（单精度）                 |
| `_mm_addsub_pd`               | `ADDSUBPD` | 交替执行加法/减法（双精度）                 |
| `_mm_hadd_ps`                 | `HADDPS`   | 水平加法（横向：相邻元素相加）              |
| `_mm_hadd_pd`                 | `HADDPD`   | 水平加法（双精度）                          |
| `_mm_hsub_ps`                 | `HSUBPS`   | 水平减法（横向：相邻元素相减）              |
| `_mm_hsub_pd`                 | `HSUBPD`   | 水平减法（双精度）                          |
| `_mm_moveldup_ps`            | `MOVSLDUP` | 复制低位元素：将 0/1 → 0/0 和 2/3 → 2/2     |
| `_mm_movehdup_ps`            | `MOVSHDUP` | 复制高位元素：将 0/1 → 1/1 和 2/3 → 3/3     |
| `_mm_loaddup_pd`             | `MOVDDUP`  | 将标量 double 加载并广播复制成两个元素     |

#### **3. 内存访问改进**
| 函数                      | 指令       | 作用说明                                   |
|---------------------------|------------|--------------------------------------------|
| `_mm_loaddup_pd`          | `MOVDDUP`  | 加载一个标量 double 并复制到两个位置       |
| `_mm_moveldup_ps`         | `MOVSLDUP` | 加载并复制低位 float                       |
| `_mm_movehdup_ps`         | `MOVSHDUP` | 加载并复制高位 float                       |

### **三、AVX2 常用类型与函数**
#### **1. 数据类型**
| 类型              | 位宽  | 描述                     |
|-------------------|-------|--------------------------|
| `__m256i`         | 256位 | 通用整数向量（8/16/32/64位） |
| `__m256d`         | 256位 | 双精度浮点向量（4个`double`） |
| `__m256`          | 256位 | 单精度浮点向量（8个`float`）  |

#### **2. 整数运算（扩展SSE2功能）**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm256_add_epi8/16/32/64` | `VPADDB/W/D/Q` | 256位整数加法               |
| `_mm256_sub_epi8/16/32/64` | `VPSUBB/W/D/Q` | 256位整数减法               |
| `_mm256_mullo_epi16`      | `VPMULLW`      | 16位乘法（低16位结果）        |
| `_mm256_mullo_epi32`      | `VPMULLD`      | 32位乘法（完整32位结果）       |
| `_mm256_slli_epi16/32/64` | `VPSLLW/D/Q`   | 256位逻辑左移                 |
| `_mm256_srli_epi16/32/64` | `VPSRLW/D/Q`   | 256位逻辑右移                 |
| `_mm256_and_si256`        | `VPAND`        | 256位按位与                   |
| `_mm256_or_si256`         | `VPOR`         | 256位按位或                   |
| `_mm256_xor_si256`        | `VPXOR`        | 256位按位异或                 |

#### **3. 浮点运算**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm256_add_pd`          | `VADDPD` | 4个双精度浮点加法             |
| `_mm256_add_ps`          | `VADDPS` | 8个单精度浮点加法             |
| `_mm256_mul_pd`          | `VMULPD` | 4个双精度浮点乘法             |
| `_mm256_mul_ps`          | `VMULPS` | 8个单精度浮点乘法             |
| `_mm256_max_pd`          | `VMAXPD` | 4个双精度最大值               |
| `_mm256_min_ps`          | `VMINPS` | 8个单精度最小值               |

#### **4. 高级操作**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm256_permutevar8x32_ps` | `VPERMPS` | 单精度浮点重排（8个lane）    |
| `_mm256_shuffle_epi8`    | `VPSHUFB` | 8位整数重排（灵活字节操作）    |
| `_mm256_broadcastss_ps`  | `VBROADCASTSS` | 广播单精度标量到8个lane      |
| `_mm256_maskload_epi32`  | `VPMASKMOVD` | 条件加载（掩码控制）          |

#### **5. 内存操作**
| 函数                     | 指令   | 作用                          |
|--------------------------|--------|-------------------------------|
| `_mm256_load_si256`      | `VMOVDQA` | 对齐加载（32字节对齐）        |
| `_mm256_loadu_si256`     | `VMOVDQU` | 未对齐加载                    |
| `_mm256_store_si256`     | `VMOVDQA` | 对齐存储                      |
| `_mm256_storeu_si256`    | `VMOVDQU` | 未对齐存储                    |

---

# SIMD (English)
### **1. Common SSE2 Types and Functions**
#### **1. Data Types**
| Type              | Bit Width | Description                                                   |
|-------------------|-----------|---------------------------------------------------------------|
| `__m128i`         | 128-bit   | General-purpose integer vector (8/16/32/64-bit)               |
| `__m128d`         | 128-bit   | Double-precision floating-point vector (2 `double`)           |
| `__m128`          | 128-bit   | Single-precision floating-point vector (4 `float`, from SSE1) |

#### **2. Integer Arithmetic**
| Function                   | Instruction      | Description                                |
|----------------------------|------------------|--------------------------------------------|
| `_mm_add_epi8/16/32/64`    | `PADDB/W/D/Q`    | Integer addition (8/16/32/64-bit)          |
| `_mm_adds_epi8/16`         | `PADDSB/W`       | Saturated addition (clamps result)         |
| `_mm_sub_epi8/16/32/64`    | `PSUBB/W/D/Q`    | Integer subtraction                        |
| `_mm_mullo_epi16`          | `PMULLW`         | Lower 16-bit multiply (returns low bits)   |
| `_mm_mulhi_epi16`          | `PMULHW`         | Higher 16-bit multiply (returns high bits) |
| `_mm_slli_epi16/32/64`     | `PSLLW/D/Q`      | Logical left shift                         |
| `_mm_srli_epi16/32/64`     | `PSRLW/D/Q`      | Logical right shift                        |
| `_mm_srai_epi16/32`        | `PSRAW/D`        | Arithmetic right shift (preserves sign)    |
| `_mm_and_si128`            | `PAND`           | Bitwise AND                                |
| `_mm_or_si128`             | `POR`            | Bitwise OR                                 |
| `_mm_xor_si128`            | `PXOR`           | Bitwise XOR                                |

#### **3. Floating Point (Double Precision)**
| Function           | Instruction | Description                     |
|--------------------|-------------|---------------------------------|
| `_mm_add_pd`       | `ADDPD`     | Double-precision addition       |
| `_mm_sub_pd`       | `SUBPD`     | Double-precision subtraction    |
| `_mm_mul_pd`       | `MULPD`     | Double-precision multiplication |
| `_mm_div_pd`       | `DIVPD`     | Double-precision division       |
| `_mm_sqrt_pd`      | `SQRTPD`    | Double-precision square root    |
| `_mm_max_pd`       | `MAXPD`     | Maximum (double-precision)      |
| `_mm_min_pd`       | `MINPD`     | Minimum (double-precision)      |

#### **4. Memory Operations**
| Function               | Instruction | Description                            |
|------------------------|-------------|----------------------------------------|
| `_mm_load_si128`       | `MOVDQA`    | Aligned load (16-byte aligned)         |
| `_mm_loadu_si128`      | `MOVDQU`    | Unaligned load                         |
| `_mm_store_si128`      | `MOVDQA`    | Aligned store                          |
| `_mm_storeu_si128`     | `MOVDQU`    | Unaligned store                        |
| `_mm_shuffle_epi32`    | `PSHUFD`    | 32-bit integer shuffle                 |

### **2. Common SSE3 Data Types and Functions**
#### **1. Data Types**
> SSE3 does not introduce new data types; it continues to use the 128-bit types from SSE/SSE2, such as `__m128`, `__m128i`, and `__m128d`.

#### **2. New Integer and Floating-Point Operations**

| Function                     | Instruction | Description                                         |
|------------------------------|-------------|-----------------------------------------------------|
| `_mm_addsub_ps`              | `ADDSUBPS`  | Alternating addition/subtraction (single-precision) |
| `_mm_addsub_pd`              | `ADDSUBPD`  | Alternating addition/subtraction (double-precision) |
| `_mm_hadd_ps`                | `HADDPS`    | Horizontal addition (adjacent elements)             |
| `_mm_hadd_pd`                | `HADDPD`    | Horizontal addition (double-precision)              |
| `_mm_hsub_ps`                | `HSUBPS`    | Horizontal subtraction (adjacent elements)          |
| `_mm_hsub_pd`                | `HSUBPD`    | Horizontal subtraction (double-precision)           |
| `_mm_moveldup_ps`            | `MOVSLDUP`  | Duplicate low elements: 0/1 → 0/0, 2/3 → 2/2        |
| `_mm_movehdup_ps`            | `MOVSHDUP`  | Duplicate high elements: 0/1 → 1/1, 2/3 → 3/3       |
| `_mm_loaddup_pd`             | `MOVDDUP`   | Load a scalar double and broadcast to two elements  |

#### **3. Memory Access Improvements**

| Function                     | Instruction | Description                                |
|------------------------------|-------------|--------------------------------------------|
| `_mm_loaddup_pd`             | `MOVDDUP`   | Load a scalar double and duplicate it      |
| `_mm_moveldup_ps`            | `MOVSLDUP`  | Load and duplicate low floats              |
| `_mm_movehdup_ps`            | `MOVSHDUP`  | Load and duplicate high floats             |

### **3. Common AVX2 Types and Functions**
#### **1. Data Types**
| Type              | Bit Width | Description                                         |
|-------------------|-----------|-----------------------------------------------------|
| `__m256i`         | 256-bit   | General-purpose integer vector (8/16/32/64-bit)     |
| `__m256d`         | 256-bit   | Double-precision floating-point vector (4 `double`) |
| `__m256`          | 256-bit   | Single-precision floating-point vector (8 `float`)  |

#### **2. Integer Arithmetic (Extended SSE2)**
| Function                      | Instruction      | Description                           |
|-------------------------------|------------------|---------------------------------------|
| `_mm256_add_epi8/16/32/64`    | `VPADDB/W/D/Q`   | 256-bit integer addition              |
| `_mm256_sub_epi8/16/32/64`    | `VPSUBB/W/D/Q`   | 256-bit integer subtraction           |
| `_mm256_mullo_epi16`          | `VPMULLW`        | 16-bit multiply (low bits result)     |
| `_mm256_mullo_epi32`          | `VPMULLD`        | 32-bit full multiplication            |
| `_mm256_slli_epi16/32/64`     | `VPSLLW/D/Q`     | 256-bit logical left shift            |
| `_mm256_srli_epi16/32/64`     | `VPSRLW/D/Q`     | 256-bit logical right shift           |
| `_mm256_and_si256`            | `VPAND`          | 256-bit bitwise AND                   |
| `_mm256_or_si256`             | `VPOR`           | 256-bit bitwise OR                    |
| `_mm256_xor_si256`            | `VPXOR`          | 256-bit bitwise XOR                   |

#### **3. Floating Point Operations**
| Function               | Instruction | Description                            |
|------------------------|-------------|----------------------------------------|
| `_mm256_add_pd`        | `VADDPD`    | 4-wide double-precision addition       |
| `_mm256_add_ps`        | `VADDPS`    | 8-wide single-precision addition       |
| `_mm256_mul_pd`        | `VMULPD`    | 4-wide double-precision multiplication |
| `_mm256_mul_ps`        | `VMULPS`    | 8-wide single-precision multiplication |
| `_mm256_max_pd`        | `VMAXPD`    | 4-wide double-precision max            |
| `_mm256_min_ps`        | `VMINPS`    | 8-wide single-precision min            |

#### **4. Advanced Operations**
| Function                        | Instruction      | Description                              |
|---------------------------------|------------------|------------------------------------------|
| `_mm256_permutevar8x32_ps`      | `VPERMPS`        | Shuffle 8 single-precision lanes         |
| `_mm256_shuffle_epi8`           | `VPSHUFB`        | Byte-wise integer shuffle                |
| `_mm256_broadcastss_ps`         | `VBROADCASTSS`   | Broadcast single float across 8 lanes    |
| `_mm256_maskload_epi32`         | `VPMASKMOVD`     | Conditional masked load                  |

#### **5. Memory Operations**
| Function               | Instruction | Description                            |
|------------------------|-------------|----------------------------------------|
| `_mm256_load_si256`    | `VMOVDQA`   | Aligned 256-bit load (32-byte aligned) |
| `_mm256_loadu_si256`   | `VMOVDQU`   | Unaligned 256-bit load                 |
| `_mm256_store_si256`   | `VMOVDQA`   | Aligned 256-bit store                  |
| `_mm256_storeu_si256`  | `VMOVDQU`   | Unaligned 256-bit store                |
