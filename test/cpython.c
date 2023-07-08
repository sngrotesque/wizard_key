/*
这段代码展示了如何在 C 语言中使用 Python 的 `rsa` 库生成 RSA 密钥对并使用公钥加密数据。
下面是对这段代码的逐行解释：

```c
#include <Python.h>
```
这一行代码包含了 Python 的 C API 头文件，它定义了用于在 C 语言中调用 Python 解释器和库的函数和类型。

```c
    Py_Initialize();
```
这里调用 `Py_Initialize()` 函数来初始化 Python 解释器。
这个函数必须在调用任何其他 Python C API 函数之前调用。

```c
    // 导入 rsa 模块
    PyObject *pName = PyUnicode_DecodeFSDefault("rsa");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
```
这几行代码导入了 Python 的 `rsa` 模块。首先，我们使用 `PyUnicode_DecodeFSDefault()` 函数
创建一个表示模块名称的字符串对象。
然后，我们使用 `PyImport_Import()` 函数导入模块，并获取一个指向模块对象的指针。
最后，我们使用 `Py_DECREF()` 函数减少模块名称字符串对象的引用计数。

```c
    if (pModule != NULL) {
        // 获取 rsa.newkeys 函数对象
        PyObject *pFunc = PyObject_GetAttrString(pModule, "newkeys");
        if (pFunc && PyCallable_Check(pFunc)) {
```
这几行代码检查模块是否成功导入，并获取 `rsa.newkeys` 函数对象。
我们使用 `PyObject_GetAttrString()` 函数从模块对象中获取函数对象，并使用 `PyCallable_Check()` 函数
检查它是否可调用。

```c
            // 创建参数对象
            PyObject *pArgs = PyTuple_New(1);
            PyObject *pValue = PyLong_FromLong(2048);
            PyTuple_SetItem(pArgs, 0, pValue);
```
这几行代码创建了一个包含一个参数的元组对象，用于传递给 `rsa.newkeys` 函数。
我们使用 `PyTuple_New()` 函数创建一个新的元组对象，
并使用 `PyLong_FromLong()` 函数创建一个表示整数值的对象。然后，我们使用 `PyTuple_SetItem()` 函数
将整数对象添加到元组中。

```c
            // 调用函数
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
```
这几行代码调用了 `rsa.newkeys` 函数，并获取了返回值。我们使用 `PyObject_CallObject()` 函数调用函数，
并传递参数元组。然后，我们使用 `Py_DECREF()` 函数减少参数元组的引用计数。

```c
            if (pValue != NULL) {
                // 获取密钥对
                PyObject *pPubKey = PyTuple_GetItem(pValue, 0);
                PyObject *pPrivKey = PyTuple_GetItem(pValue, 1);
```
这几行代码检查函数调用是否成功，并从返回值中获取公钥和私钥。由于 `rsa.newkeys` 函数
返回一个包含两个元素的元组，分别表示公钥和私钥，因此我们可以使用 `PyTuple_GetItem()` 函数
从返回值中获取公钥和私钥。

```c
                // 获取 rsa.encrypt 函数对象
                pFunc = PyObject_GetAttrString(pModule, "encrypt");
                if (pFunc && PyCallable_Check(pFunc)) {
                    // 创建参数对象
                    pArgs = PyTuple_New(2);
                    PyTuple_SetItem(pArgs, 0, PyBytes_FromString("Hello, World!"));
                    PyTuple_SetItem(pArgs, 1, pPubKey);
```
这几行代码获取了 `rsa.encrypt` 函数对象，并创建了一个包含两个参数的元组对象，用于传递给该函数。
第一个参数是要加密的数据，第二个参数是公钥。我们使用 `PyObject_GetAttrString()` 函数
从模块对象中获取函数对象，并使用 `PyCallable_Check()` 函数检查它是否可调用。
然后，我们使用 `PyTuple_New()` 和 `PyTuple_SetItem()` 函数创建一个包含两个参数的元组对象。

```c
                    // 调用函数
                    pValue = PyObject_CallObject(pFunc, pArgs);
                    Py_DECREF(pArgs);
```
这几行代码调用了 `rsa.encrypt` 函数，并获取了返回值。
我们使用 `PyObject_CallObject()` 函数调用函数，并传递参数元组。然后，我们使用 `Py_DECREF()` 函数
减少参数元组的引用计数。

```c
                    if (pValue != NULL) {
                        // 输出加密后的数据
                        printf("Encrypted data: %s\n", PyBytes_AsString(pValue));
                        Py_DECREF(pValue);
                    }
```
这几行代码检查函数调用是否成功，并从返回值中获取加密后的数据。
我们使用 `PyBytes_AsString()` 函数将返回值转换为字符串，并输出结果。然后，我们使用 `Py_DECREF()` 函数
减少返回值对象的引用计数。

```c
    Py_FinalizeEx();
```
最后，我们使用 `Py_FinalizeEx()` 函数关闭 Python 解释器并退出程序。
*/
#include <Python.h>

/* GCC命令选项
gcc test\cpython.c -I H:\python3.7\include -L H:\python3.7\libs -lpython37 ^
    -o snCompiled\cpython.exe && snCompiled\cpython.exe
*/

int main()
{
    // 初始化 Python 解释器
    Py_Initialize();

    // 导入 rsa 模块
    PyObject *pName = PyUnicode_DecodeFSDefault("rsa");
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule) {
        // 获取 rsa.newkeys 函数对象
        PyObject *pFunc = PyObject_GetAttrString(pModule, "newkeys");
        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject *pArgs = PyTuple_New(1);
            PyObject *pValue = PyLong_FromLong(2048);
            PyTuple_SetItem(pArgs, 0, pValue);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            if (pValue) {
                PyObject *pPubKey = PyTuple_GetItem(pValue, 0);
                PyObject *pPrivKey = PyTuple_GetItem(pValue, 1);

                pFunc = PyObject_GetAttrString(pModule, "encrypt");
                if (pFunc && PyCallable_Check(pFunc)) {
                    pArgs = PyTuple_New(2);
                    PyTuple_SetItem(pArgs, 0, PyBytes_FromString("Hello, World!"));
                    PyTuple_SetItem(pArgs, 1, pPubKey);

                    // 调用函数
                    pValue = PyObject_CallObject(pFunc, pArgs);
                    Py_DECREF(pArgs);

                    if (pValue) {
                        // 输出加密后的数据
                        uint8_t *cipher = PyBytes_AsString(pValue);
                        size_t cipher_len = PyBytes_Size(pValue);
                        
                        printf("Encrypted data: ");
                        for(int x = 0; x < cipher_len; ++x) {
                            printf("%02x", cipher[x]);
                        }
                        printf("\n");

                        Py_DECREF(pValue);
                    } else {
                        PyErr_Print();
                    }
                } else {
                    PyErr_Print();
                }
                Py_XDECREF(pFunc);
                Py_DECREF(pPubKey);
                Py_DECREF(pPrivKey);
            } else {
                PyErr_Print();
            }
        } else {
            PyErr_Print();
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    // 关闭 Python 解释器
    Py_FinalizeEx();

    return 0;
}
