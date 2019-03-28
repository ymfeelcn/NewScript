/*
	{"","文件",""},
	{"WriteINI","向配置文件里写一个键值(小节名,键名,写入内容,INI文件路径)","向配置文件里写一个键值,小节名$1,键名$2,内容$3,ini文件路径$4"},
	{"ReadINI","从配置文件里读一个键值(小节名,键名,INI文件路径)[读出内容]","从配置文件里读一个键值,小节名$1,键名$2,ini文件路径$3"},
	{"CopyFile","拷贝文件(目标文件,存放路径)","拷贝文件,目标文件$1,存放路径$2"},
	{"IsFileExit","判断一个文件是否存在(目标文件)[真假]","判断$1文件是否存在"},
	{"DeleteFile","删除文件(目标文件)","删除文件$1"},
	{"MoveFile","移动文件(目标文件,存放路径)","移动文件,从$1到$2"},
	{"CreateFolder","建立文件夹(创建文件夹路径)","建立文件夹,路径为$1"},
	{"DeleteFolder","删除文件夹(目标文件)","删除$1文件夹"},
	{"ReNameFile","重命名文件(源文件,目标文件)","重命名文件$1为$2"},
	{"SetAttrib","设置文件属性(目标文件,读/写)","根据$2设置$1文件属性"},
	{"SetDate","设置文件日期和时间(目标文件,日期和时间)","设置$1的日期和时间为$2"},
//	{"ReadFile","读文本内容(目标文件路径)[文本内容]","读$1的文本内容"},
//	{"WriteFile","写文本内容(目标文件路径,写入内容)","向$1写入内容为$2的文本内容"},
	{"GetFileLength","得到文本内容长度(文本内容)[内容长度]","得到$1文本内容的长度"},
	{"ReadFileEx","读文本内容(目标文件路径)[文本内容]","读$1的文本内容"},
	{"WriteFileEx","写文本内容(目标文件路径,写入内容)","向$1写入内容为$2的文本内容"},
	{"SelectFile","弹出选择文件的对话框，并得到用户选择的文件"},
	{"SelectDirectory","弹出选择文件夹的对话框，并得到用户选择的文件夹"},
	{"SeekFile","设置文件的当前读写位置(文件句柄,读写位置)","设置文件的当前读写位置,文件句柄$1,读写位置$2"},
	{"ReadLine","从文件当前的读写位置读一行内容(文件句柄)","从文件当前的读写位置读一行内容,文件句柄为$1"},
	{"WriteLine","写文本内容(目标文件路径,写入内容)","向$1写入内容为$2的文本内容"},
	{"ReadFile","从文件当前的读写位置读取一定长度的内容(文件句柄,读取长度)","从文件当前的读写位置读取一定长度的内容,文件句柄$1,读取长度$2"},
	{"WriteFile","往文件当前读写位置写入一个字符串(文件句柄,写入内容)","往文件当前读写位置写入一个字符串,文件句柄为$1,写入内容为$2"},
	{"OpenFile","打开一个文件，以备读写使用(文件全路径)","文件全路劲为S1"},
	{"CloseFile","关闭一个已经打开的文件(文件句柄)","文件句柄为$1"},
	{"ExistFile","判断文件是否存在","判断$1文件是否存在"},
*/

//删除节点(小节名,INI文件路径)	
static long DeleteINI(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszINIFilePath)
{
	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, NULL, lpszINIFilePath);
}
//向配置文件里写一个键值(小节名,键名,写入内容,INI文件路径)	
static long WriteINI(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszBuffer, LPCTSTR lpszINIFilePath)
{
	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, lpszBuffer, lpszINIFilePath);
}
//从配置文件里读一个键值(小节名,键名,INI文件路径)[读出内容]
static CString ReadINI(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszINIFilePath)
{
	CString strResult;
	char lpszBuffer[4096] = { 0 };
	GetPrivateProfileString(lpszSectionName, lpszKeyName, "", lpszBuffer, sizeof(lpszBuffer), lpszINIFilePath);
	strResult = lpszBuffer;		//复制到lpszRetVal,,因此这个插件命令将返回一个字符串
	return strResult;
}

static DWORD GetProfileSectionNames(CStringArray &strArray, CString chFileName)
{
	int nAllSectionNamesMaxSize = 1024;
	char *pszSectionNames = new char[nAllSectionNamesMaxSize];
	DWORD dwCopied = 0;
	dwCopied = ::GetPrivateProfileSectionNames(pszSectionNames, nAllSectionNamesMaxSize, chFileName);

	strArray.RemoveAll();

	char *pSection = pszSectionNames;
	do
	{
		CString szSection(pSection);
		if (szSection.GetLength() < 1)
		{
			delete[] pszSectionNames;
			return dwCopied;
		}
		strArray.Add(szSection);

		pSection = pSection + szSection.GetLength() + 1; // next section name     
	} while (pSection && pSection < pszSectionNames + nAllSectionNamesMaxSize);

	delete[] pszSectionNames;
	return dwCopied;
}

static CString GetProfileSectionNames(CString chFileName)
{
	int nAllSectionNamesMaxSize = 1024;
	char *pszSectionNames = new char[nAllSectionNamesMaxSize];
	DWORD dwCopied = 0;
	dwCopied = ::GetPrivateProfileSectionNames(pszSectionNames, nAllSectionNamesMaxSize, chFileName);

	CString strRet = "";

	char *pSection = pszSectionNames;
	do
	{
		CString szSection(pSection);
		if (szSection.GetLength() < 1)
		{
			break;
		}
		strRet += szSection + "|";

		pSection = pSection + szSection.GetLength() + 1; // next section name     
	} while (pSection && pSection < pszSectionNames + nAllSectionNamesMaxSize);

	delete[] pszSectionNames;
	if (strRet != "")
	{
		strRet = strRet.Left(strRet.GetLength() - 1);
	}
	return strRet;
}

//拷贝文件(目标文件,存放路径)	
static long CopyFile(LPCTSTR lpszSourceFile, LPCTSTR lpszTargetFile)
{
	return ::CopyFile(lpszSourceFile, lpszTargetFile, FALSE);
}
//判断一个文件是否存在(目标文件)[真假]
static BOOL IsFileExit(LPCTSTR lpszSourceFile)
{
	DWORD dwFileAttr = GetFileAttributes(lpszSourceFile);//只有一个字符串参数，可以不用分割参数了

	if ((int)dwFileAttr < 0)			//出错或者没有找到
		return FALSE;
	else if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)		//是文件夹
		return FALSE;
	else						//是文件
		return TRUE;
}
//删除文件(目标文件)	
static long MyDeleteFile(LPCTSTR FileName)
{
	return ::DeleteFile(FileName);
}
//移动文件(目标文件,存放路径)	
static long MyMoveFile(LPCTSTR lpszSourceFile, LPCTSTR lpszTargetFile)
{
	return ::MoveFile(lpszSourceFile, lpszTargetFile);
}
//建立文件夹(创建文件夹路径)	
static long CreateFolder(LPCTSTR lpszDirectory)
{
	return _mkdir(lpszDirectory);
}
//删除文件夹(目标文件)
static long DeleteFolder(LPCTSTR lpszDirectory)
{
	return _rmdir(lpszDirectory);
}
//重命名文件(原文件$1,目标文件$2)	
static long ReNameFile(LPCTSTR oldName, LPCTSTR newName)
{
	return rename(oldName, newName);
}
//设置文件属性(目标文件,读/写)
static BOOL SetAttrib(LPCTSTR lpszFilePath, BOOL Sign)
{
	CFileStatus rStatus;
	CFile::GetStatus(lpszFilePath, rStatus);//获得文件的属性设置
	if (Sign == 0)
	{
		rStatus.m_attribute = rStatus.m_attribute & 0x3E;   //如果文件为只读的，将只读属性去掉
	}
	else
		rStatus.m_attribute = rStatus.m_attribute | 0x01;   //如果文件为读的，将只读属性钩上
	CFile::SetStatus(lpszFilePath, rStatus);//更改文件的属性设置
	return TRUE;
}
//设置文件日期和时间(目标文件,日期和时间)
static long SetDate(LPCTSTR lpszFileName, LPCTSTR lpLastWriteTime)
{
	//	CString lpLastWriteTime="1999-09-09 07:20:20",lpszFileName="C://黄明发.txt";
		// TODO: Add your control notification handler code here
	int len = ((CString)lpLastWriteTime).GetLength();
	int index0 = ((CString)lpLastWriteTime).Find('-', 0);
	int year = atoi(((CString)lpLastWriteTime).Left(index0));
	CString sTemp0 = ((CString)lpLastWriteTime).Right(len - index0 - 1);
	int index1 = sTemp0.Find('-', 0);
	int month = atoi(sTemp0.Left(index1));
	CString sTemp1 = ((CString)lpLastWriteTime).Right(len - index0 - index1 - 2);
	int index2 = sTemp1.Find(' ', 0);
	int day = atoi(sTemp1.Left(index2));
	CString sTmep2 = ((CString)lpLastWriteTime).Right(len - index0 - index1 - index2 - 3);
	int index3 = sTmep2.Find(':', 0);
	int hour = atoi(sTmep2.Left(index3));
	CString sTmep3 = ((CString)lpLastWriteTime).Right(len - index0 - index1 - index2 - index3 - 4);
	int index4 = sTmep3.Find(':', 0);
	int min = atoi(sTmep3.Left(index4));

	CString sTmep4 = ((CString)lpLastWriteTime).Right(len - index0 - index1 - index2 - index3 - index4 - 5);
	int second = atoi(sTmep4);
	SYSTEMTIME systime;

	if (hour >= 12)
	{
		systime.wDay = day;
		systime.wHour = hour - 8;
	}
	if (12 >= hour && hour >= 8)
	{
		systime.wDay = day;
		systime.wHour = hour - 8;
	}
	if (hour < 8)
	{
		systime.wDay = day - 1;
		systime.wHour = hour + 16;
	}
	systime.wYear = year;
	systime.wMonth = month;
	systime.wMinute = min;
	systime.wDayOfWeek = 0;
	systime.wMilliseconds = 0;
	systime.wSecond = second;
	FILETIME lastWriteTimeme;
	int a = SystemTimeToFileTime(&systime, &lastWriteTimeme);
	char buffer[1024] = { 0 };
	HFILE  handle = ::OpenFile(lpszFileName, (LPOFSTRUCT)buffer, OF_WRITE);
	if (handle == HFILE_ERROR)
	{
		return FALSE;
	}
	SetFileTime((HANDLE)handle, &lastWriteTimeme, &lastWriteTimeme, &lastWriteTimeme);
	_lclose(handle);
	return TRUE;
}

//得到文本内容长度(文本内容)[内容长度]
static long GetFileLength(LPCTSTR lpszFileName)
{
	CFile   file;   BOOL sign;
	sign = file.Open(lpszFileName, CFile::modeRead);
	if (sign == 0)
	{
		//	MessageBox("打开文件失败");
		return FALSE;
	}
	int filelength = (int)file.GetLength();
	//	CString str;
	//	str.Format("文件长度=%d",filelength);
	//	MessageBox(str);
	file.Close();
	return filelength;
}

//弹出选择文件的对话框，并得到用户选择的文件
static CString SelectFile()
{
	CString strResult;

	char lpszBuffer[4096] = { 0 };
	OPENFILENAME sOpenFileDialogInfo = { 0 };
	sOpenFileDialogInfo.lStructSize = sizeof(sOpenFileDialogInfo);
	sOpenFileDialogInfo.lpstrFile = lpszBuffer;
	sOpenFileDialogInfo.nMaxFile = sizeof(lpszBuffer);
	sOpenFileDialogInfo.Flags = 6148;
	if (GetOpenFileName(&sOpenFileDialogInfo) == 0)//如果选择了"取消"
		return NULL;
	strResult = lpszBuffer;
	return strResult;
}

//弹出选择文件夹的对话框，并得到用户选择的文件夹
static CString SelectDirectory()
{
	CString lpszRetVal;
	char str[4096] = { 0 };
	BROWSEINFO sBrowseDirectoryInfo = { 0 };
	LPITEMIDLIST lpSelectDirectoryReturn = NULL;
	sBrowseDirectoryInfo.lpszTitle = "";
	sBrowseDirectoryInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	lpSelectDirectoryReturn = SHBrowseForFolder(&sBrowseDirectoryInfo);

	if (lpSelectDirectoryReturn == NULL)//没有作出选择（如选择了“取消”）
		return NULL;
	SHGetPathFromIDList(lpSelectDirectoryReturn, str);//这里会修改str,因此这个插件命令将返回一个字符串
	lpszRetVal = str;
	return lpszRetVal;
}

//设置文件的当前读写位置
static long SeekFile(long lpszHandle, long lpszOffset)
{
	SetFilePointer((HANDLE)lpszHandle, lpszOffset, NULL, FILE_BEGIN);
	return 0;
}

//从文件当前的读写位置读一行内容
static BSTR ReadLine(long hFile)
{
	CString strResult;

	DWORD dwNumberOfBytesRead;
	char lpszCurrentReadPos[4096];

	//windows api居然没有读取一行的函数，只好自己写了。效率可能有点低，将就着用吧
	int i = 0;
	do
	{
		::ReadFile((HANDLE)hFile, lpszCurrentReadPos + i, 1, &dwNumberOfBytesRead, NULL);
		if (*(lpszCurrentReadPos + i) == '\n')//读到行末
			break;
		i++;
	} while (dwNumberOfBytesRead);
	*(lpszCurrentReadPos + i) = 0;//加上字符串结束标示
	strResult = lpszCurrentReadPos;
	return strResult.AllocSysString();
}

//同WriteFile，但会多写入一个回车换行符号
static long WriteLine(long lpszHandle, LPCTSTR lpszWriteContent)
{
	DWORD dwNumberOfBytesWrite;
	strcat((char*)lpszWriteContent, "\r\n");//和WriteFile的唯一区别是后面多了回车换行符
	return ::WriteFile((HANDLE)lpszHandle, lpszWriteContent, strlen(lpszWriteContent), &dwNumberOfBytesWrite, NULL);
}
//读文本内容(目标文件路径)[文本内容]
static CString ReadFileEx(LPCTSTR lpszFileName, CString strSplit = "|")
{
	CStdioFile myFile; CString sTemp;
	CString ReadFileString;

	if (myFile.Open(lpszFileName, CFile::modeRead) == TRUE)
	{
		while (myFile.ReadString(ReadFileString) != FALSE)
		{
			ReadFileString.Replace("\r\n", "");
			ReadFileString.Replace("\r", "");
			ReadFileString.Replace("\n", "");
			if (ReadFileString != "")
			{
				sTemp += ReadFileString + strSplit;
			}
		}
	}
	return sTemp;
}
//写文本内容(目标文件路径,写入内容)
static long WriteFileEx(LPCTSTR lpszFileName, LPCTSTR lpszBeWrite)
{
	CStdioFile   myFile;//CString sTemp;    
	CString   WriteFileString, ReadFileString;
	if (myFile.Open(lpszFileName, CFile::modeReadWrite) == TRUE)
	{
		while (myFile.ReadString(ReadFileString) != FALSE)
		{
		}
	}
	else
	{
		myFile.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite);
	}
	WriteFileString = lpszBeWrite;
	myFile.WriteString(WriteFileString + "\r\n");
	return TRUE;
}
//从文件当前的读写位置读取一定长度的内容
static CString ReadFile(long lpszHandle, long lpszReadLength)
{
	CString strResult;
	DWORD dwNumberOfBytesRead;
	char str[4096];
	::ReadFile((HANDLE)lpszHandle, str, lpszReadLength, &dwNumberOfBytesRead, NULL);
	str[dwNumberOfBytesRead] = 0;   //加上字符串结束标示
	strResult = str;
	return strResult;
}
//往文件当前读写位置写入一个字符串
static long WriteFile(long lpszHandle, LPCTSTR lpszWriteContent)
{
	DWORD dwNumberOfBytesWrite;

	return ::WriteFile((HANDLE)lpszHandle, lpszWriteContent, strlen(lpszWriteContent), &dwNumberOfBytesWrite, NULL);
}
//判断文件或文件夹是否存在
static long ExistFile(LPCTSTR FileAttr)
{
	DWORD dwFileAttr = GetFileAttributes(FileAttr);//只有一个字符串参数，可以不用分割参数了

	if ((int)dwFileAttr < 0)			//出错或者没有找到
		return 0;
	else if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)		//是文件夹
		return 2;
	else						//是文件
		return 1;
}
//打开一个文件，以备读写使用

static long OpenFile(LPCTSTR FileName)
{
	return (long)CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
}

//关闭一个已经打开的文件。文件关闭后，其句柄不再有效
static long CloseFile(long hwd)
{
	return CloseHandle((HANDLE)hwd);
}
