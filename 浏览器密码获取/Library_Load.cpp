#include "pch.h"
#include"Firefox_decrypt.h"
// ��̬���ؿ��ļ� 
HMODULE loadLibrary(string installationPath)
{
	string nsspath = installationPath;
	nsspath += "\\nss3.dll";
 	const char nssLibraryName[] = "nss3.dll";	// ���ض�̬���ӿ��ļ�nss.dll
 	SetCurrentDirectory(installationPath.c_str());	// �л���nss.dll�ļ�����Ŀ¼

	HMODULE nssLib = LoadLibrary(nssLibraryName);	// ����nss.dll�ļ�
	if (nssLib == NULL) 
	{	// ģ��������Ϊ�գ���̬����ʧ��
		printf("Library couldnt loaded!.. %d\n", GetLastError());
	}
	return	nssLib;	// ����ģ����
}