#include <thread>
#include <chrono>
#include <iostream>
#include <future>


class karl {
    public:
    int a , b , c;

    karl(){};
    karl(int i , int j , int k)
    {
        a = i +1;
        b = j +1;
        c = 0;
    };
    void display()
    {
        std::cout << a <<  b << c << std::endl;
    }
    karl operator+(karl ss)
    {
        karl cn(0 , 0 , 0);
        cn.display();
        cn.a = a + ss.a;
        cn.b = b + ss.b;
        cn.c = c + ss.c;
        return cn;
    }
};




int main()
{
    karl rn(1, 2 , 3);
    karl bn(2 , 3 ,5);
    karl sum = rn + bn;
    rn.display();
    bn.display();
    sum.display();

    return 0;
}