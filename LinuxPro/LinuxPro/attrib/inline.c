
inline int max (int,int) __attribute__((always_inline));
inline int max(int x,int y)
{
           if(x>y)
                      return x;
           else
                      return y;
}

int main()
{
	int a=10,b=20;
	max(a,b);
}

