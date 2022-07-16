#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
//此API由HalfPeopleStudio所有
//此 API 歸 HalfPeopleStudio 所有
//This API is owned by HalfPeopleStudio
//このAPIはHalfPeopleStudioが所有しています
//API นี้เป็นเจ้าของโดย HalfPeopleStudio
//API ini dimiliki oleh HalfPeopleStudio
//Cette API appartient à HalfPeopleStudio

namespace HalfPeopleStudio
{

	class HalfSaveV1
	{
	public:
		HalfSaveV1(const char* FilePath,int Password)
		{
			Path = FilePath;
			Pw = Password;
		}
		~HalfSaveV1()
		{

		}
		void Save(std::string Name, std::string SaveData)
		{
			std::string CacheFilePath = std::string(Path).append(".HalfSaveCacheFile");
			std::ifstream file(Path);
			std::ofstream CacheFile(CacheFilePath,std::ios::app);
			if (!file)
			{
				file.close();
				AddNewLineSave(Name, SaveData);
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;
			bool HaveSet = false;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					srand(Pw);
					std::string Save ;
					Name.append(";'/");
					//std::fstream fs(Path, std::ios::out | std::ios::in );
					for (size_t i = 0; i < Name.size(); i++)
					{
						Save.append(std::to_string((int)Name[i])).append(" ");
					}
					for (size_t i = 0; i < SaveData.size(); i++)
					{
						Save.append(std::to_string((int)SaveData[i])).append(" ");
					}
					CacheFile << Save <<"\n";
					HaveSet = true;
				}
				else
				{
					CacheFile << SaveLine <<"\n";
				}
			}
			file.close();
			if (!HaveSet)
			{
				AddNewLineSave(Name, SaveData);
			}
			CacheFile.close();

			remove(Path);
			rename(CacheFilePath.c_str(), Path);

//			std::ofstream file2(Path);
//			std::ifstream CacheFile2(CacheFilePath);
//			file2 << CacheFile2;
		}
		bool Load(std::string Name, std::string* SaveData)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					file.close();
					*SaveData = LoadLineSaveData;
					return true;
				}
			}
			file.close();
			return -2;
		}

		int Exist(std::string Name)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;
			int i = 0;
			while (std::getline(file,SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave,&LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					file.close();
					return i;
				}
				i = i + SaveLine.size();
			}
			file.close();
			return -2;
		}
		bool Exist()
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}
			file.close();
		}

	private:
		const char* Path;
		int Pw;

		void LoadLine(std::string Data, std::string* Name)
		{
			std::string a;
			LoadLine(Data, Name, &a);
		}

		void LoadLine(std::string Data,std::string * Name , std::string * OutData)
		{
			if (Data.size()>3)
			{
				char SaveLine[2048];
				int pos;
				char Text;
				int i = 0;
				do
				{
					pos = Data.find(" ");
					SaveLine[i] = (char)(atoi(Data.substr(0, pos).c_str()));
					Data = Data.substr(pos + 1,1000000);
					i++;
	
				} while (Data.length() > 4);
				pos = Data.rfind(" ");
				SaveLine[i] = (char)(atoi(Data.substr(0, pos).c_str()));
				SaveLine[i + 1] = NULL;
				std::string LineData(SaveLine);
				//delete(SaveLine);
				pos = LineData.find(";'/");
				*Name = LineData.substr(0, pos);
				*OutData = LineData.substr(pos + 3, 10000000);
				
				//std::cout << "\nName :" << *Name;
				//std::cout << "\nData :" << *OutData;
			}

		}

		void AddNewLineSave(std::string Name,std::string SaveData)
		{
			std::ofstream file(Path, std::ios::app);

			srand(Pw);
			std::string Save;
			Name.append(";'/");

			for (size_t i = 0; i < Name.size(); i++)
			{
				Save.append(std::to_string((int)Name[i])).append(" ");
			}
			for (size_t i = 0; i < SaveData.size(); i++)
			{
				Save.append(std::to_string((int)SaveData[i])).append(" ");
			}

			std::cout << "\n" << Save;
			file << Save << "\n";
			file.close();
		}
	};

	std::string StringArrayToString(std::vector<std::string> Array[])
	{
		std::string SaveString = ":array&*";
		int ArrayEndPos = Array->size();
		for (size_t i = 0; i < ArrayEndPos; i++)
		{
			SaveString.append(Array->at(i));

			if (i!=ArrayEndPos-1)
				SaveString.append("<H.A.R>");
		}

		return SaveString;
	}
	std::vector<std::string>* StringToStringArray(std::string InData)
	{
		int pos = 0;
		std::vector<std::string> SaveArray;
		InData = InData.substr(pos + 8, 1000000);

		do
		{
			pos = InData.find("<H.A.R>");
			SaveArray.push_back(InData.substr(0, pos));
			InData = InData.substr(pos + 7, 1000000);

		} while (InData.length() > 4);
		SaveArray.push_back(InData);
		return new std::vector<std::string>(SaveArray);
	}
	bool IsArray(std::string Data)
	{
		return (Data.find(":array&*") < 10000);
	}

	class HalfSaveV2
	{
	public:
		HalfSaveV2(const char* FilePath, int Password)
		{
			Path = FilePath;
			Pw = Password;
		}
		~HalfSaveV2()
		{

		}
		void Save(std::string Name, std::string SaveData)
		{
			std::string CacheFilePath = std::string(Path).append(".HalfSaveCacheFile");
			std::ifstream file(Path);
			std::ofstream CacheFile(CacheFilePath, std::ios::app);
			if (!file)
			{
				file.close();
				CacheFile.close();
				AddNewLineSave(Name, SaveData);
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;
			bool HaveSet = false;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					srand(Pw);
					std::string Save;
					Name.append(";'/");
					//std::fstream fs(Path, std::ios::out | std::ios::in );
					for (size_t i = 0; i < Name.size(); i++)
					{
						Save.append(std::to_string(((int)Name[i])+rand())).append(" ");
					}
					for (size_t i = 0; i < SaveData.size(); i++)
					{
						Save.append(std::to_string(((int)SaveData[i])+rand())).append(" ");
					}
					CacheFile << Save << "\n";
					HaveSet = true;
				}
				else
				{
					CacheFile << SaveLine << "\n";
				}
			}
			file.close();
			CacheFile.close();
			if (!HaveSet)
			{
				AddNewLineSave(Name, SaveData);
			}
			else
			{
				remove(Path);
				rename(CacheFilePath.c_str(), Path);
			}



			//			std::ofstream file2(Path);
			//			std::ifstream CacheFile2(CacheFilePath);
			//			file2 << CacheFile2;
		}
		bool Load(std::string Name, std::string* SaveData)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					file.close();
					*SaveData = LoadLineSaveData;
					return true;
				}
			}
			file.close();
			return -2;
		}

		int Exist(std::string Name)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;
			int i = 0;
			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);

				if (Name == LoadLineNameSave)
				{
					file.close();
					return i;
				}
				i = i + SaveLine.size();
			}
			file.close();
			return -2;
		}
		bool Exist()
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return -1;
			}
			file.close();
		}

		void LoadAllName(std::vector<std::string> * NameList)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return ;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);
				NameList->push_back(LoadLineNameSave);
			}
			file.close();
			return ;
		}
		void LoadAllName(std::vector<std::string>* NameList,std::vector<std::string>* DataList)
		{
			std::ifstream file(Path);
			if (!file)
			{
				file.close();
				return;
			}

			std::string SaveLine;
			std::string LoadLineNameSave;
			std::string LoadLineSaveData;

			while (std::getline(file, SaveLine))
			{
				LoadLine(SaveLine, &LoadLineNameSave, &LoadLineSaveData);
				NameList->push_back(LoadLineNameSave);
				DataList->push_back(LoadLineSaveData);
			}
			file.close();
			return;
		}
	private:
		const char* Path;
		int Pw;

		void LoadLine(std::string Data, std::string* Name)
		{
			std::string a;
			LoadLine(Data, Name, &a);
		}

		void LoadLine(std::string Data, std::string* Name, std::string* OutData)
		{
			srand(Pw);
			if (Data.size() > 3)
			{
				char SaveLine[2048];
				int pos;
				char Text;
				int i = 0;
				do
				{
					pos = Data.find(" ");
					SaveLine[i] = (char)(atoi(Data.substr(0, pos).c_str())-rand());
					Data = Data.substr(pos + 1, 1000000);
					i++;

				} while (Data.length() > 4);
				pos = Data.rfind(" ");
				SaveLine[i] = (char)(atoi(Data.substr(0, pos).c_str())-rand());
				SaveLine[i + 1] = NULL;
				std::string LineData(SaveLine);
				//delete(SaveLine);
				pos = LineData.find(";'/");
				*Name = LineData.substr(0, pos);
				*OutData = LineData.substr(pos + 3, 10000000);

				//std::cout << "\nName :" << *Name;
				//std::cout << "\nData :" << *OutData;
			}

		}

		void AddNewLineSave(std::string Name, std::string SaveData)
		{
			std::ofstream file(Path, std::ios::app);

			srand(Pw);


			std::string Save;
			Name.append(";'/");

			for (size_t i = 0; i < Name.size(); i++)
			{
				Save.append(std::to_string(((int)Name[i])+rand())).append(" ");
			}
			for (size_t i = 0; i < SaveData.size(); i++)
			{
				Save.append(std::to_string(((int)SaveData[i])+rand())).append(" ");
			}

			//std::cout << "\n" << Save;
			file << Save << "\n";
			file.close();
		}
	};
}