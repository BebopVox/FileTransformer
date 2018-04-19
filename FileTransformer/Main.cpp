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

#include "CFG.h"

void DelFile(const string &sFullFileNameWithPath)
{
  string sCmd = string(XS("echo off & del \"")) + string(sFullFileNameWithPath) + XS("\"");
  std::system(sCmd.c_str());
}

ifstream::pos_type GetFileSize(const string &FullFileNameWithPath)
{
  ifstream in(FullFileNameWithPath.c_str(), ifstream::ate | ifstream::binary);
  ifstream::pos_type size = in.tellg();
  in.close();
  return size;
}

bool TransformFile(const string &sFullFileName)
{
  string sFileName = U::GetFileName(sFullFileName);

  long long unsigned int lluiFileSize = GetFileSize(sFullFileName);

  if (lluiFileSize < 1)
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

  DWORD dwPartsCount = (DWORD)PARTS(lluiFileSize, PART_SIZE);
  DWORD dwTempPartSize = (DWORD)(lluiFileSize / dwPartsCount);

  for (long long unsigned int i = 0, lluiWrittenBytes = 0; i < dwPartsCount; i++)
  {
    if (i + 1 >= dwPartsCount)
    {
      dwTempPartSize += (DWORD)(lluiFileSize - dwTempPartSize * (i + 1));
    }

    string sFullPartFile =
      sFullFileName +
      string(PART_EXT) +
      PART_INFO_SEPARATOR +
      to_string(i + 1) +
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
    dstFile.close();

    lluiWrittenBytes += dwTempPartSize;

    cout << "\tPart \'" << U::GetFileName(sFullPartFile) << "\' created! (" << dwTempPartSize << " bytes)" << endl;
  }
  srcFile.close();

  DelFile(sFullFileName);

  cout << "Parts created: " << dwPartsCount << endl;
  return true;
}

bool AssembleFile(const string &sFullFileName)
{
  string sInputFileName = U::GetFileName(sFullFileName);
  string sInputFilePath = U::GetFilePath(sFullFileName);
  string sTransformedFileName = sInputFileName.substr(0, sInputFileName.find(PART_EXT));
  string sTransformedFileNameWithPath = sInputFilePath + "\\" + sTransformedFileName;

  cout << "Assembling to file \'" << sTransformedFileName << "\'..." << endl;

  vector<string> vFileInfo = U::Split(sInputFileName, PART_INFO_SEPARATOR);

  string sInputPartFileName = vFileInfo[0];
  long long unsigned int lluiPartsCount = stoi(vFileInfo[vFileInfo.size() - 1]);

  ofstream dstFile(sTransformedFileNameWithPath.c_str(), ios::binary);
  if (!dstFile.is_open())
  {
    cout << "Failed to create assembling file: \'" << sTransformedFileNameWithPath << "\'" << endl;
    return false;
  }

  vector<string> vPartsFullPathes;

  for (long long unsigned int i = 0; i < lluiPartsCount; i++)
  {
    string sTempPartFileNameWithPath =
      sInputFilePath + "\\" +
      sInputPartFileName +
      PART_INFO_SEPARATOR +
      to_string(i + 1) +
      PART_INFO_SEPARATOR +
      to_string(lluiPartsCount);

    DWORD dwPartSize = (DWORD)GetFileSize(sTempPartFileNameWithPath);

    if (dwPartSize < 1)
    {
      cout << "Part file \'" << sTempPartFileNameWithPath << +"\' is empty!" << endl;
      dstFile.close();
      return false;
    }

    ifstream srcPart(sTempPartFileNameWithPath.c_str(), ios::binary);
    if (!srcPart.is_open())
    {
      cout << "Failed to open part file: \'" << sTempPartFileNameWithPath << "\'" << endl;
      srcPart.close();
      dstFile.close();
      DelFile(sTransformedFileNameWithPath);
      return false;
    }

    char* mem = new char[dwPartSize];
    srcPart.read(mem, dwPartSize);
    dstFile.write(mem, dwPartSize);
    delete[] mem;
    srcPart.close();

    vPartsFullPathes.push_back(sTempPartFileNameWithPath);
    cout << "\tPart \'" << U::GetFileName(sTempPartFileNameWithPath) << "\' assembled! (" << dwPartSize << " bytes)" << endl;
  }

  dstFile.close();

  for (int i = 0; i < (int)vPartsFullPathes.size(); i++)
  {
    DelFile(vPartsFullPathes[i]);
  }

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
      cout << "Transforming file \'" << sFileName << "\'..." << endl;
      if (TransformFile(sFullFileName))
      {
        cout << "File successful transformated!" << endl;
      }
      else
      {
        cout << "File transforming failed!" << endl;
      }
    }
    else
    {
      string sTransformedFileName = sFileName.substr(0, sFileName.find(PART_EXT));
      cout << "Detected part(s) of transformed file \'" << sTransformedFileName << "\'..." << endl;
      if (AssembleFile(sFullFileName))
      {
        cout << "File successful assembled!" << endl;
      }
      else
      {
        cout << "File assembling failed!" << endl;
      }
    }
  }
  else
  {
    cout << "Drag & drop any big size file onto this program..." << endl;
  }
  std::system("pause");

  return 0;
}
