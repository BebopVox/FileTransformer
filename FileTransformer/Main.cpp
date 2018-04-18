#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>

#if defined(_DEBUG)
#if defined(_USRDLL)
#pragma comment(lib, "Utils/Lib/Dynamic/Utils_debug.lib")
#else
#pragma comment(lib, "Utils/Lib/Static/Utils_debug.lib")
#endif
#else
#if defined(_USRDLL)
#pragma comment(lib, "Utils/Lib/Dynamic/Utils_release.lib")
#else
#pragma comment(lib, "Utils/Lib/Static/Utils_release.lib")
#endif
#endif
#include "Utils/Include/Utils.h"

using namespace std;

#define PARTS(x, y) (x / y + (x % y == 0 ? 0 : 1))

#define B(x) (x)
#define KB(x) (x * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

#include "CFG.h"

bool TransformFile(const string &sFullFileName)
{
  string sFileName = U::GetFileName(sFullFileName);

  WIN32_FILE_ATTRIBUTE_DATA fInfo;
  GetFileAttributesExA(sFullFileName.c_str(), GetFileExInfoStandard, &fInfo);

  if (fInfo.nFileSizeLow < 1)
  {
    cout << "Source file \'" << sFileName << +"\' is empty!" << endl;
    return false;
  }

  ifstream srcFile(sFullFileName.c_str(), ios::binary);
  if (!srcFile.is_open())
  {
    cout << "Failed to open source file: \'" << sFullFileName << "\'" << endl;
    return false;
  }

  cout << "Transforming file \'" << sFileName << "\'..." << endl;

  DWORD dwPartsCount = PARTS(fInfo.nFileSizeLow, PART_SIZE);
  DWORD dwTempPartSize = fInfo.nFileSizeLow / dwPartsCount;

  for (DWORD i = 0, dwWrittenBytes = 0; i < dwPartsCount; i++)
  {
    if (i + 1 >= dwPartsCount)
    {
      dwTempPartSize += fInfo.nFileSizeLow - dwTempPartSize * (i + 1);
    }

    string sFullPartFile =
      sFullFileName +
      string(PART_EXT) +
      PART_INFO_SEPARATOR +
      to_string(i) +
      PART_INFO_SEPARATOR +
      to_string(dwPartsCount);

    ofstream dstFile(sFullPartFile.c_str(), ios::binary);
    if (!dstFile.is_open())
    {
      cout << "Failed to open destination file: \'" << sFullPartFile << "\'" << endl;
      return false;
    }

    char* mem = new char[dwTempPartSize];
    srcFile.read(mem, dwTempPartSize);
    dstFile.write(mem, dwTempPartSize);
    delete[] mem;

    dwWrittenBytes += dwTempPartSize;

    cout << "\tPart \'" << U::GetFileName(sFullPartFile) << "\' created! (" << dwTempPartSize << " bytes)" << endl;

    dstFile.close();
  }
  srcFile.close();

  string sCmd = string(XS("echo off & del \"")) + string(sFullFileName) + XS("\"");
  system(sCmd.c_str());

  cout << "Parts created: " << dwPartsCount << endl;
  return true;
}

bool AssembleFile(const string &sFullFileName)
{
  string sInputFileName = U::GetFileName(sFullFileName);
  string sInputFilePath = U::GetFilePath(sFullFileName);
  string sTransformedFileName = sInputFileName.substr(0, sInputFileName.find(PART_EXT));
  string sTransformedFileNameWithPath = sInputFilePath + "\\" + sTransformedFileName;

  cout << "Detected part(s) of transformed file \'" << sTransformedFileName << "\'..." << endl;
  cout << "Assembling to file \'" << sTransformedFileName << "\'..." << endl;

  vector<string> vFileInfo = U::Split(sInputFileName, PART_INFO_SEPARATOR);

  string sInputPartFileName = vFileInfo[0];
  DWORD dwPartsCount = stoi(vFileInfo[vFileInfo.size() - 1]);

  ofstream dstFile(sTransformedFileNameWithPath.c_str(), ios::binary);
  if (!dstFile.is_open())
  {
    cout << "Failed to create assembling file: \'" << sTransformedFileNameWithPath << "\'" << endl;
    return false;
  }

  for (DWORD i = 0; i < dwPartsCount; i++)
  {
    string sTempPartFileNameWithPath =
      sInputFilePath + "\\" +
      sInputPartFileName +
      PART_INFO_SEPARATOR +
      to_string(i) +
      PART_INFO_SEPARATOR +
      to_string(dwPartsCount);

    WIN32_FILE_ATTRIBUTE_DATA fInfo;
    GetFileAttributesExA(sTempPartFileNameWithPath.c_str(), GetFileExInfoStandard, &fInfo);

    if (fInfo.nFileSizeLow < 1)
    {
      cout << "Part file \'" << sTempPartFileNameWithPath << +"\' is empty!" << endl;
      return false;
    }

    ifstream srcPart(sTempPartFileNameWithPath.c_str(), ios::binary);
    if (!srcPart.is_open())
    {
      cout << "Failed to open part file: \'" << sTempPartFileNameWithPath << "\'" << endl;
      return false;
    }

    char* mem = new char[fInfo.nFileSizeLow];
    srcPart.read(mem, fInfo.nFileSizeLow);
    dstFile.write(mem, fInfo.nFileSizeLow);
    delete[] mem;

    srcPart.close();

    string sCmd = string(XS("echo off & del \"")) + string(sTempPartFileNameWithPath) + XS("\"");
    system(sCmd.c_str());
  }

  dstFile.close();

  return true;
}

int main(int iArgCount, char** pszArgs)
{
  if (iArgCount >= 2)
  {
    string sFullFileName(pszArgs[1]);
    string sFileName = U::GetFileName(sFullFileName);
    string sFilePath = U::GetFilePath(sFullFileName);

    if (sFileName.find(string(PART_EXT) + PART_INFO_SEPARATOR) == string::npos)
    {
      if (TransformFile(sFullFileName))
      {
        cout << "File successful transformated!" << endl;
      }
    }
    else
    {
      if (AssembleFile(sFullFileName))
      {
        cout << "File successful assembled!" << endl;
      }
    }
  }
  else
  {
    cout << "Drag & drop any big size file onto this program..." << endl;
  }
  system("pause");

  return 0;
}
