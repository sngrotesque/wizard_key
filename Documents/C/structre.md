# 结构体

### 不同的声明方法

`A_Structre`是一个结构体类型的别名，它使用了`typedef`关键字，它的原始类型是一个匿名的结构体，它有一个`char`指针类型的成员a。如果要定义一个`A_Structre`类型的变量，只需要写`A_Structre x;`即可。
```c
typedef struct {
    char *a;
} A_Structre;
```

`B_Structre`是一个结构体类型，它没有使用`typedef`关键字，它有一个`char`指针类型的成员b。如果要定义一个`B_Structre`类型的变量，需要写`struct B_Structre x;`，不能省略`struct`关键字。
```c
struct B_Structre {
    char *b;
};
```

`C_Structre`是一个结构体类型的别名，它使用了`typedef`关键字，它的原始类型是一个有名字的结构体，它有一个`char`指针类型的成员c。如果要定义一个`C_Structre`类型的变量，只需要写`C_Structre x;`即可。但是，如果要使用原始类型，也可以写`struct C_Structre x;`。
```c
typedef struct C_Structre {
    char *c;
};
```

`D_Structre`是一个结构体类型的别名，它使用了`typedef`关键字，它的原始类型是一个有名字的结构体，它有一个`char`指针类型的成员d。如果要定义一个`D_Structre`类型的变量，只需要写`D_Structre x;`即可。但是，如果要使用原始类型，也可以写`struct D_Structre x;`。这个结构体和`C_Structre`结构体没有本质区别，只是写法不同。
```c
typedef struct D_Structre {
    char *d
} D_Structre;
```

`E_Structre_t`是一个结构体变量，它没有使用`typedef`关键字，它是由`E_Structre`类型的结构体定义的，它有一个`char`指针类型的成员e。如果要访问这个变量的成员，需要写`E_Structre_t.e;`。如果要定义另一个`E_Structre`类型的变量，需要写`struct E_Structre x;`。
```c
struct E_Structre {
    char *e;
} E_Structre_t;
```

`F_Structre`是一个结构体变量，也是一个结构体类型的别名，它没有使用`typedef`关键字，但是在定义变量的同时给结构体起了别名，它有一个`char`指针类型的成员f。如果要访问这个变量的成员，需要写`F_Structre.f;`。如果要定义另一个`F_Structre`类型的变量，可以写`F_Structre x`;或者`struct F_Structre x;`。
```c
struct F_Structre {
    char *f;
} F_Structre;
```

