#pragma once
#include <vector>
#include <string>
#include <iostream>
//Copyright (c) HalfPeopleStudio Corporation. All rights reserved.

//#ifdef HalfPeopleStudio_Var_API_Lib
//#define HalfPeopleStudio_Var_API __declspec(dllexport)
//#else
//#pragma comment(lib,"HalfPeopleStudio_Var.lib")
//#define HalfPeopleStudio_Var_API __declspec(dllimport)
//#endif

//HalfPeopleStudio_Var_API class HVarRoot
//{
//public:
//	HVarRoot(){}
//};

//std::vector<char> HTypeConversion(void* in)
//{
//	std::vector<char> S;
//	S.resize(sizeof(in)/sizeof(char));
//	memcpy(S.data(), &in, sizeof(in));
//	return S;
//}
//
//template <typename T>
//T HRTypeConversion(std::string in)
//{
//	T data;
//	memcpy(data, in.data(),in.size());
//	return data;
//}
//


#if HVar_Integration

namespace Integration
{
	class HVarRoot
	{
	public:

		virtual void AddOffset(){}
		virtual void RemoveAllOffset(){}
		virtual void RemoveRandOffset(){}

	private:

	};

	std::vector<HVarRoot*> AllHVarList;
	
	template <class T>
	class HVar : public HVarRoot
	{
	public:
		HVar()
		{
			Data = new T();
			DataList.push_back(Data);
			AllHVarList.push_back(this);
		}
	
		HVar(T In)
		{
			Data = new T(In);
			DataList.push_back(Data);
			AllHVarList.push_back(this);
		}
	
		void AdvancedAutoOffset()
		{
			AddOffset();
	
			if (RandBool())
			{
				RemoveRandOffset();
			}
			else
			{
				if (RandBool())
				{
					RemoveAllOffset();
				}
			}
		}
	
		void AutoOffset()
		{
			AddOffset();
	
			RemoveAllOffset();
		}
	
		void AddOffset()override
		{
			Data = new T(*Data);
			*DataList.back() = T();
			DataList.push_back(Data);
		}
	
		void RemoveAllOffset()override
		{
			for (size_t i = 0; i < DataList.size() - 1; i++)
			{
				T* Save = DataList.at(i);
				DataList.erase(DataList.begin() + i);
				delete Save;
			}
		}
	
		void RemoveRandOffset()override
		{
			int i = DataList.size() * rand() / (RAND_MAX + 1);
	
			if (DataList.at(i) != Data)
			{
				T* Save = DataList.at(i);
				DataList.erase(DataList.begin() + i);
				delete Save;
			}
		}
	
		T& at()
		{
			return *Data;
		}
		T* data()
		{
			return Data;
		}
	
		~HVar()
		{
			for (size_t i = 0; i < DataList.size(); i++)
			{
				T* Save = DataList.at(i);
				DataList.erase(DataList.begin() + i);
				delete Save;
			}
	
			delete Data;
		}
	
	#if Half_DEBUG
		int DataListIndex()
		{
			return DataList.size();
		}
	#endif // DEBUG
	
	private:
		T* Data;
		std::vector<T*> DataList;
	
		bool RandBool()
		{
			int g = std::rand();
			return (g % 2); // 1 is converted to true and 0 as false
		}
	};


	void UpdataAllHVar()
	{
		for (size_t i = 0; i < AllHVarList.size(); i++)
		{
			AllHVarList.at(i)->RemoveAllOffset();
		}
		AllHVarList.rend();
		for (size_t i = 0; i < AllHVarList.size(); i++)
		{
			AllHVarList.at(i)->AddOffset();
		}
	}
}








#else

template <class T>
class HVar
{
public:
	HVar()
	{
		Data = new T();
		DataList.push_back(Data);
	}
	
	HVar(T In)
	{
		Data = new T(In);
		DataList.push_back(Data);
	}


	void AdvancedAutoOffset()
	{
		AddOffset();

		if (RandBool())
		{
			RemoveRandOffset();
		}
		else
		{
			if (RandBool())
			{
				RemoveAllOffset();
			}
		}
	}

	void AutoOffset()
	{
		AddOffset();

		RemoveAllOffset();
	}

	void AddOffset()
	{
		Data = new T(*Data);
		*DataList.back() = T();
		DataList.push_back(Data);
	}

	void RemoveAllOffset()
	{
		for (size_t i = 0; i < DataList.size()-1; i++)
		{
			T* Save = DataList.at(i);
			DataList.erase(DataList.begin() + i);
			delete Save;
		}
	}

	void RemoveRandOffset()
	{
		int i = DataList.size() * rand() / (RAND_MAX + 1);

		if (DataList.at(i) != Data)
		{
			T* Save = DataList.at(i);
			DataList.erase(DataList.begin() + i);
			delete Save;
		}
	}

	T& at()
	{
		return *Data;
	}
	T* data()
	{
		return Data;
	}

	~HVar()
	{
		for (size_t i = 0; i < DataList.size(); i++)
		{
			T* Save = DataList.at(i);
			DataList.erase(DataList.begin() + i);
			delete Save;
		}

		delete Data;
	}

#if Half_DEBUG
	int DataListIndex()
	{
		return DataList.size();
	}
#endif // DEBUG

private:
	T* Data;
	std::vector<T*> DataList;

	bool RandBool()
	{
		int g = std::rand();
		return (g % 2); // 1 is converted to true and 0 as false
	}
};

#endif // HVar_Integration