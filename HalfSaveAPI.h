#pragma once
#include <fstream>
#include <iostream>
#include <string>


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