#include "pch.h"
#include"Firefox_decrypt.h"
#define MY_BUFSIZE 128 // Arbitrary initial value.
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define Key32READ  KEY_READ
#define Key64READ  KEY_READ | KEY_WOW64_64KEY
// Dynamic allocation will be used.
constexpr auto Get_failed = "get_InstallationPath Failed";

BOOL IsWow64()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL bIsWow64 = FALSE;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(("kernel32")), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}

// ͨ��ע����ȡ����İ�װ·��
string getInstallationPath(){
	REGSAM KEY_T;
	//�ж�64λ������ע���Ĳ���
	if (IsWow64() ==TRUE)
	{		
		KEY_T = Key64READ;
	}
	else
	{
		KEY_T = Key32READ;
	}

	string foxpath;
	HKEY hfKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Mozilla\\Mozilla Firefox"),//���������ĸ��ӽ�
		0,
		KEY_T,
		&hfKey) == ERROR_SUCCESS
		)
	{
		foxpath = QueryKey(hfKey);
	}
	RegCloseKey(hfKey);

	string foxTpath = "SOFTWARE\\Mozilla\\Mozilla Firefox\\";
	foxTpath += foxpath;
	foxTpath += "\\Main";
	HKEY hKey;
	TCHAR szProductType[MY_BUFSIZE];
	DWORD dwBufLen = MY_BUFSIZE;
	LONG lRet;
	// �����Ǵ�ע���, ֻ�д򿪺��������������
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, // Ҫ�򿪵ĸ���
		TEXT(foxTpath.c_str()), // Ҫ�򿪵����Ӽ�������汾67.0.4 (x64 zh-CN)��
		0, // ���һ��ҪΪ0
		KEY_T,// ָ���򿪷�ʽ,��Ϊ��,32λ����Ҫ��ȡ64λ��ע�����Ҫ�ڴ򿪼�ʱ����Ӳ���KEY_WOW64_64KEY		
		&hKey); // �������ؾ��
	if (lRet == ERROR_SUCCESS) // �ж��Ƿ�򿪳ɹ�
	{
		// ��ע���ɹ�
		// ��ʼ��ѯ
		lRet = RegQueryValueEx(hKey, // ��ע���ʱ���صľ��
			TEXT("Install Directory"), //Ҫ��ѯ������,�����װĿ¼��¼������
			NULL, // һ��ΪNULL����0
			NULL,
			(LPBYTE)szProductType, // Ŀ¼��������
			&dwBufLen);
		if (lRet == ERROR_SUCCESS) // �ж��Ƿ��ѯ�ɹ�
		{
			RegCloseKey(hKey);
			return (char*)szProductType;
		}
		else
		{
			printf("��ð�װĿ¼ʧ��\n");
			return Get_failed;
		}
	}
	else {
		printf("��ע���ʧ��\n");
		return Get_failed;
	}
}

// ͨ������ע����ȡ��װ·��
string QueryKey(HKEY hKey)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
	DWORD    cchClassName = MAX_PATH;  // size of class string
	DWORD    cSubKeys = 0;               // number of subkeys
	DWORD    cbMaxSubKey;              // longest subkey size
	DWORD    cchMaxClass;              // longest class string
	DWORD    cValues;              // number of values for key
	DWORD    cchMaxValue;          // longest value name
	DWORD    cbMaxValueData;       // longest value data
	DWORD    cbSecurityDescriptor; // size of security descriptor
	FILETIME ftLastWriteTime;      // last write time

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	// Get the class name and the value count.
	retCode = RegQueryInfoKey(
		hKey,                    // key handle
		achClass,                // buffer for class name
		&cchClassName,           // size of class string
		NULL,                    // reserved
		&cSubKeys,               // number of subkeys
		&cbMaxSubKey,            // longest subkey size
		&cchMaxClass,            // longest class string
		&cValues,                // number of values for this key
		&cchMaxValue,            // longest value name
		&cbMaxValueData,         // longest value data
		&cbSecurityDescriptor,   // security descriptor
		&ftLastWriteTime);       // last write time

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		printf("\nNumber of subkeys: %d\n", cSubKeys);

		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				&ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				printf(TEXT("(%d) %s\n"), i + 1, achKey);
				return achKey;
			}
		}
	}

	// Enumerate the key values.
	//�����ӽ��µļ�ֵ
// 	if (cValues)
// 	{
// 		printf("\nNumber of values: %d\n", cValues);
// 
// 		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
// 		{
// 			cchValue = MAX_VALUE_NAME;
// 			achValue[0] = '\0';
// 			retCode = RegEnumValue(hKey, i,
// 				achValue,
// 				&cchValue,
// 				NULL,
// 				NULL,
// 				NULL,
// 				NULL);
// 
// 			if (retCode == ERROR_SUCCESS)
// 			{
// 				printf(TEXT("(%d) %s\n"), i + 1, achValue);
// 			}
// 		}
// 	}
}
