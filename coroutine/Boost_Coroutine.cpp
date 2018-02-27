// Boost_Coroutine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <boost/coroutine/all.hpp>

/*********************************************************
FunctionName:PushDataToMainContext
FunctionDesc:�ǶԳƵ�Э�� ������
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void PushDataToMainContext()
{
	using namespace boost::coroutines;
	using namespace std;

	try
	{
		//������һ��Э�̵�
		asymmetric_coroutine<int>::pull_type Puller(
			[&](boost::coroutines::asymmetric_coroutine<int>::push_type& sink) {
			for (int i = 0; i < 8; ++i)
			{
				//����������
				sink(i);
			}
		});

		//��ȡЭ�̵�״̬ ���״̬������ô��true
		while (Puller)
		{
			
			cout << Puller.get() << endl;
			//switch the execute control
			Puller();
		}

	}
	catch (const detail::forced_unwind&)
	{
		//����rethrow
		throw;
	}
	catch (...)
	{

	}
}

/*********************************************************
FunctionName:PushDatatoCoroutine
FunctionDesc:��Э����������
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void PushDatatoCoroutine()
{
	
	using namespace boost::coroutines;
	//������һ��Э�̵�
	asymmetric_coroutine<int>::push_type Pusher(
		[&](boost::coroutines::asymmetric_coroutine<int>::pull_type& Source) {

		while (Source)
		{
			using namespace std;
			cout << Source.get() << endl;
			Source();
		}
		
	});

	
	for (int i = 1; i <= 9; ++i)
	{
		//enter the coroutine
		Pusher(i);
	}
}


/*********************************************************
FunctionName:SynmmetricCoroutine
FunctionDesc:���ڲ����ĶԳ�Э��
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void SynmmetricCoroutine(const std::vector<int>& Array1, const std::vector<int>& Array2,std::vector<int>& ResultArray)
{
	using namespace boost::coroutines;
	//�����Ϸ����ж�
	if (Array2.empty() || Array1.empty())
	{
		return;
	}


	//��������
	int nIndex1 = 0;
	int nIndex2 = 0;

	//�����Գ�Э��
	symmetric_coroutine<void>::call_type* pCo1 = nullptr;
	symmetric_coroutine<void>::call_type* pCo2 = nullptr;

	auto Coroutine_fun1 = [&](symmetric_coroutine<void>::yield_type& yield)
	{

		while (nIndex1 < Array1.size())
		{
			//�����ǰֵ������һ�����ݱȽϣ����С������һ������
			if (Array1[nIndex1] > Array2[nIndex2])
			{
				//�л�������һ��Э��
				yield(*pCo2);
			}
			else
			{
				ResultArray.push_back(Array1[nIndex1++]);
			}
		}

		//������һ������ȫ�����
		while (nIndex2 < Array2.size())
		{
			ResultArray.push_back(Array2[nIndex2++]);
		}

	};

	auto Coroutine_fun2 = [&](symmetric_coroutine<void>::yield_type& yield)
	{

		while (nIndex2 < Array2.size())
		{
			//�����ǰֵ������һ�����ݱȽϣ����С������һ������
			if (Array2[nIndex2] > Array1[nIndex1])
			{
				//�л�������һ��Э��
				yield(*pCo1);
			}
			else
			{
				ResultArray.push_back(Array2[nIndex2++]);
			}
		}

		//������һ������ȫ�����
		while (nIndex1 < Array1.size())
		{
			ResultArray.push_back(Array1[nIndex1++]);
		}

	};

	symmetric_coroutine<void>::call_type Co_a(Coroutine_fun1);

	pCo1 = &Co_a;

	symmetric_coroutine<void>::call_type Co_b(Coroutine_fun2);

	pCo2 = &Co_b;

	Co_a();

}


int main()
{
	//PushDatatoCoroutine();
	using namespace std;

	vector<int> A = { 23,34,56,64,78 };
	vector<int> B = { 11,22,33,44,55,66 };

	vector<int> Result;

	SynmmetricCoroutine(A, B,Result);

	copy(Result.begin(), Result.end(), std::ostream_iterator<int>(cout, " "));

	cout << endl;


	return 0;
}

