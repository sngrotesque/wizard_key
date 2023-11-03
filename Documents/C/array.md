# C Array

### one-dimensional array
```cpp
#define X 4

void test()
{
    uint8_t array[X];
    array[3] = 4;
    cout << array[3] << endl;
}
```

### Two-dimensional array
```cpp
#define X 4
#define Y 4

void test()
{
    uint8_t array[Y][X];
    uint8_t pos[Y * X];
    array[3][1] = 4;
    memcpy(pos, array, sizeof(array));
    std::cout << pos[(3 * X) + 1] << std::endl;
}
```

### three dimensional array
```cpp
#define X 4
#define Y 4
#define Z 4

void test()
{
    uint8_t array[Z][Y][X];
    uint8_t pos[X * Y * Z];

    array[2][3][2] = 4;
    memcpy(pos, array, sizeof(array));

    // Z轴大小必须是Z轴长度*Y轴长度的结果
    // 假设三维数组为array[4][4][4];
    // array[(pos_Z * 16) + (pos_Y * 4) + pos_X];
    std::cout << pos[(2 * (Z*Y)) + (3 * Y) + 2] << std::endl;
}
```
