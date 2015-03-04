#include "header.h"

tm* curTime()
{
	time_t t(time(NULL));
	tm* t_m = new tm;
	localtime_s(t_m, &t);
	return t_m;
}

wstring strDate(tm* curTM)
{
	wstringstream convert;
	convert << (curTM->tm_year + 1900);
	convert << (((curTM->tm_mon + 1) / 10) < 1 ? "0" : "");
	convert << (curTM->tm_mon + 1);
	convert << ((curTM->tm_mday / 10) < 1 ? "0" : "");
	convert << (curTM->tm_mday);
	return convert.str();
}

bool isFile(const wstring& fileName)
{	//проверка существования файла
	long tmp;
	_finddata_t *fileinfo = new _finddata_t;

	int length = WideCharToMultiByte(CP_ACP, 0,
		fileName.c_str(),
		-1, NULL, 0, 0, 0);
	char *path = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		fileName.c_str(),
		-1, path, length, 0, 0);

	long done = _findfirst(path, fileinfo);
	tmp = done;
	_findclose(done);

	delete fileinfo;
	delete[] path;

	if (tmp == -1)
		return 0;
	else
		return 1;
}

void removeDir(const wstring& nameDir)
{
	wstring szDir(nameDir), szMask(nameDir + L"*");

	int length = WideCharToMultiByte(CP_ACP, 0,
		szMask.c_str(),
		-1, NULL, 0, 0, 0);
	char *mask = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		szMask.c_str(),
		-1, mask, length, 0, 0);

	length = WideCharToMultiByte(CP_ACP, 0,
		szDir.c_str(),
		-1, NULL, 0, 0, 0);
	char *dir = new char[length];
	WideCharToMultiByte(CP_ACP, 0,
		szDir.c_str(),
		-1, dir, length, 0, 0);

	_finddata_t *fileinfo = new _finddata_t;

	long done = _findfirst(mask, fileinfo);
	int MayWeWork = done;

	char * fName = NULL;
	while (MayWeWork != -1)
	{
		fName = new char[strlen(fileinfo->name) + 1];
		strcpy_s(fName, strlen(fileinfo->name) + 1, fileinfo->name);
		if (!strcmp(fName, ".") && !strcmp(fName, ".."))
		{
			if (fileinfo->attrib == _A_SUBDIR)
			{
				char *allPath = new char[strlen(dir) + strlen(fName) + 2];
				strcpy_s(allPath, strlen(dir), dir);
				strcat_s(allPath, strlen(fName), fName);
				strcat_s(allPath, 1, "/");
				wchar_t *wRName = new wchar_t[strlen(allPath)+1];

				mbstowcs_s(
					NULL,
					wRName,
					strlen(allPath) + 1,
					allPath,
					strlen(allPath) + 1
					);
				delete allPath;
				removeDir(wRName);
				delete wRName;
			}
			char *allPath = new char[strlen(dir) + strlen(fName) + 1];
			strcpy_s(allPath, strlen(dir), dir);
			strcat_s(allPath, strlen(fName), fName);
			remove(allPath);
			delete allPath;
		}
		MayWeWork = _findnext(done, fileinfo);
		delete fName;
	}
	_findclose(done);

	_rmdir(dir);
	delete dir, fileinfo, mask;
}