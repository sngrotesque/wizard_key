# C#的知识

### 类
```cs
// https://learn.microsoft.com/zh-cn/dotnet/csharp/programming-guide/classes-and-structs/access-modifiers
// 定义一个正常类
public class Human {
    public string name;
    public void sleep() {
        Console.WriteLine($"{this.name} sleep.");
    }
}

// 定义一个基类（接口），可实例化
public class Human {
    public string name;
    public virtual void sleep(); // 虚函数
}

// 定义一个抽象类（接口），不可实例化
public abstract class Human {
    public string name;
    public virtual void sleep(); // 虚函数
}

// 定义一个派生类
public class Tom : Human {
    this.name = "Tom";
    public override void sleep() {
        Console.WriteLine($"{this.name} sleep.");
    }
}

// internal类
// 同一程序集中的任何代码都可以访问该类型或成员，但其他程序集中的代码不可以。
// 换句话说，internal 类型或成员可以从属于同一编译的代码中访问。
internal class Human {..}
class Human {...}
```
