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
FunctionDesc:非对称的协程 拉数据
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
		//定义了一个协程的
		asymmetric_coroutine<int>::pull_type Puller(
			[&](boost::coroutines::asymmetric_coroutine<int>::push_type& sink) {
			for (int i = 0; i < 8; ++i)
			{
				//进行推数据
				sink(i);
			}
		});

		//获取协程的状态 如果状态正常那么是true
		while (Puller)
		{
			
			cout << Puller.get() << endl;
			//switch the execute control
			Puller();
		}

	}
	catch (const detail::forced_unwind&)
	{
		//重新rethrow
		throw;
	}
	catch (...)
	{

	}
}

/*********************************************************
FunctionName:PushDatatoCoroutine
FunctionDesc:向协程推送数据
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void PushDatatoCoroutine()
{
	
	using namespace boost::coroutines;
	//定义了一个协程的
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
FunctionDesc:用于并发的对称协程
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void SynmmetricCoroutine(const std::vector<int>& Array1, const std::vector<int>& Array2,std::vector<int>& ResultArray)
{
	using namespace boost::coroutines;
	//参数合法性判断
	if (Array2.empty() || Array1.empty())
	{
		return;
	}


	//建立索引
	int nIndex1 = 0;
	int nIndex2 = 0;

	//建立对称协程
	symmetric_coroutine<void>::call_type* pCo1 = nullptr;
	symmetric_coroutine<void>::call_type* pCo2 = nullptr;

	auto Coroutine_fun1 = [&](symmetric_coroutine<void>::yield_type& yield)
	{

		while (nIndex1 < Array1.size())
		{
			//如果当前值与另外一个数据比较，如果小于另外一个数据
			if (Array1[nIndex1] > Array2[nIndex2])
			{
				//切换到另外一个协程
				yield(*pCo2);
			}
			else
			{
				ResultArray.push_back(Array1[nIndex1++]);
			}
		}

		//把另外一个数组全部添加
		while (nIndex2 < Array2.size())
		{
			ResultArray.push_back(Array2[nIndex2++]);
		}

	};

	auto Coroutine_fun2 = [&](symmetric_coroutine<void>::yield_type& yield)
	{

		while (nIndex2 < Array2.size())
		{
			//如果当前值与另外一个数据比较，如果小于另外一个数据
			if (Array2[nIndex2] > Array1[nIndex1])
			{
				//切换到另外一个协程
				yield(*pCo1);
			}
			else
			{
				ResultArray.push_back(Array2[nIndex2++]);
			}
		}

		//把另外一个数组全部添加
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

