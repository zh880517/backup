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
	int iKey = arry[iBegin];//�õ�һ����Ϊkey

	while (iBegin < iEnd)
	{
		//������ҵ�һ����keyС��ֵ������
		while (iBegin < iEnd && arry[iEnd] >= iKey)
			--iEnd;

		arry[iBegin] = arry[iEnd];

		while (iBegin < iEnd && arry[iBegin] <= iKey)
			++iBegin;

		arry[iEnd] = arry[iBegin];
	}
	arry[iBegin] = iKey;/*�����¼��λ*/
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
	//Qsort(a, 0, 8);/*����ԭ�ĵ���������Ҫ��1�����ڴ�Խ��*/
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
		std::cout << a[i] << " ";
	}
	return 0;
}