# C#的知识

### 类
```cs
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
```
