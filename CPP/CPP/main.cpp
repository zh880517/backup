#include <iostream>
#include <array>
#include <functional>
union MyUnion
{
	uint8_t ar[4] = {0};
	int32_t iValue;
};
void Qsort(std::array<int, 9> arry, int iLow, int iHigh)
{
	if (iLow >= iHigh)
	{
		return;
	}
	std::cout << "Before : ";
	for (int i = iLow; i < iHigh + 1; ++i)
	{
		std::cout << arry[i] << " ";
	}
	std::cout << std::endl;

	int iBegin = iLow;
	int iEnd = iHigh;
	int iKey = arry[iBegin];//用第一个作为key

	while (iBegin < iEnd)
	{
		//逆向查找第一个比key小的值的索引
		while (iBegin < iEnd && arry[iEnd] >= iKey)
			--iEnd;

		arry[iBegin] = arry[iEnd];

		while (iBegin < iEnd && arry[iBegin] <= iKey)
			++iBegin;

		arry[iEnd] = arry[iBegin];
	}
	arry[iBegin] = iKey;/*枢轴记录到位*/
	std::cout << "After : ";
	for (int i = iLow ; i< iHigh + 1; ++i)
	{
		std::cout << arry[i] << " ";
	}
	std::cout << std::endl;
	Qsort(arry, iLow, iBegin - 1);
	Qsort(arry, iBegin + 1, iHigh);
}
int main()
{
	std::array<int, 9> a = { 57, 72, 28, 96, 33, 24, 68, 59, 59 };
	std::sort(a.begin(), a.end());
	//Qsort(a, 0, 8);/*这里原文第三个参数要减1否则内存越界*/
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		std::cout << a[i] << " ";
	}
	return 0;
}