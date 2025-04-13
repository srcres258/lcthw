#include <stdio.h>

int main()
{
    // 使用一些高级的占位符
    int num = 42;
    double pi = 3.14159265358979323846;
    char str[] = "Hello";

    printf("整数：%d\n", num);
    printf("浮点数：%.2f\n", pi);
    printf("十六进制：0x%X\n", num);
    printf("字符串：%10s\n", str);
    printf("指针：%p\n", &num);

    printf("打印空字符串：%s\n", "");

    return 0;
}