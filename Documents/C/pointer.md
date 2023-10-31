# 指针

```c
char *p;
char **ptr = &p;
printf("%p, %p", p, &p);
/*
* &p 为指针的地址；
* p  为指针指向的数据的地址
* ptr 指向 p 的地址，*ptr 指向为 p 指向的数据的地址
* &p   address: 0x7ffc1d738f38
* &ptr address: 0x7ffc1d738f30
* p    address: 0x559b8ab422a0
* ptr  address: 0x7ffc1d738f38
* *ptr address: 0x559b8ab422a0
*/

uint32_t arr[4] = {0x41, 0x42, 0x43, 0x44};
uint8_t *p[4] = {
    (uint8_t *)&arr[0],
    (uint8_t *)&arr[1],
    (uint8_t *)&arr[2],
    (uint8_t *)&arr[3]
};
```
### 行视图
| method    | address         |  method    | address        | method     | address        | method     | address        |
| :---      | :---            | :---       | :---           | :---       |  :---          | :---       | :---           |
| p[0]      | 0x7ffe60a0f120  | p[1]       | 0x7ffe60a0f124 | p[2]       | 0x7ffe60a0f128 | p[3]       | 0x7ffe60a0f12c |
| &p[0]     | 0x7ffe60a0f100  | &p[1]      | 0x7ffe60a0f108 | &p[2]      | 0x7ffe60a0f110 | &p[3]      | 0x7ffe60a0f118 |
| &((*p)[0])| 0x7ffe60a0f120  | &((*p)[1]) | 0x7ffe60a0f121 | &((*p)[2]) | 0x7ffe60a0f122 | &((*p)[3]) | 0x7ffe60a0f123 |
| &(*p[0])  | 0x7ffe60a0f120  | &(*p[1])   | 0x7ffe60a0f124 | &(*p[2])   | 0x7ffe60a0f128 | &(*p[3])   | 0x7ffe60a0f12c |

### 列视图
| method    | address        | method   | address        | method     | address        | method   | address        |
| :---      | :---           | :---     | :---           | :---       |  :---          | :---     | :---           |
| p[0]      | 0x7ffe60a0f120 | &p[0]    | 0x7ffe60a0f100 | &((*p)[0]) | 0x7ffe60a0f120 | &(*p[0]) | 0x7ffe60a0f120 |
| p[1]      | 0x7ffe60a0f124 | &p[1]    | 0x7ffe60a0f108 | &((*p)[1]) | 0x7ffe60a0f121 | &(*p[1]) | 0x7ffe60a0f124 |
| p[2]      | 0x7ffe60a0f128 | &p[2]    | 0x7ffe60a0f110 | &((*p)[2]) | 0x7ffe60a0f122 | &(*p[2]) | 0x7ffe60a0f128 |
| p[3]      | 0x7ffe60a0f12c | &p[3]    | 0x7ffe60a0f118 | &((*p)[3]) | 0x7ffe60a0f123 | &(*p[3]) | 0x7ffe60a0f12c |
