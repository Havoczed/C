#include<iostream>
#include <vector>
using namespace std;
int main()
{
	vector<int> tree;
	int num,A,B = -1,apple,cont = 0,maxn = 0;

	while (1) 
    {
		cin >> num;
		tree.push_back(num);
		if(cin.get() == '\n')
        	break;
	}
	int n = tree.size();                      //输入不定长的数作为数组

	for (int i = 0; i < n; i++)    
	{
		A = tree[i];               
		apple = 1;
		for (int j = i + 1; j < n; j++)
		{
			if (tree[j] != A)
			{
				B = tree[j];       
				break;
			}                                 //确定A和B两个篮子的数
		}
		if (B == -1)
			apple = n - i;                    //对于找不到B篮子的可以直接得到apple的结果      
		else
		{
			for (int j = i + 1; j < n; j++)
			{
				if (tree[j] != A && tree[j] != B)
					break;                    //只能装两个篮子，否则直接跳出循环
				else
					apple++;
			}
		}
		if (maxn < apple)                     //每次循环均比较选出最大值
			maxn = apple;
	}
	cout << maxn<<endl;
	return 0;
}