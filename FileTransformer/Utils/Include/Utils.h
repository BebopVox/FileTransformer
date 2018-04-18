#pragma once

#include <Windows.h>
#include <string>
#include <array>
#include <vector>
#include <Psapi.h>
#include <chrono>
#include <d3d9.h>

#include "Patch.h"
#include "PatchMgr.h"
#include "VMTHook.h"
#include "VMTHookMgr.h"
#include "JMPHook.h"
#include "JMPHookMgr.h"

#ifndef UTILS_LIB_VER
#define UTILS_LIB_VER 9.5.3.1
#endif // UTILS_LIB_VER

#ifndef XS
#define XS(str) (U::XorString::XSClass<sizeof(str) - 1> \
(str, std::make_index_sequence<sizeof(str) - 1>()).DecryptAll())
#endif // XS

#ifndef CONCAT
#ifndef CONCAT_IMPLEMENT
#define CONCAT_IMPLEMENT(x, y) x##y
#endif // CONCAT_IMPLEMENT
#define CONCAT(x, y) CONCAT_IMPLEMENT(x, y)
#endif // CONCAT

namespace U
{
  extern bool bDebug;

  enum class ValueTypes
  {
    UnknownValue,
    IntValue,
    FloatValue,
    StringValue
  };

  bool World2Screen(int iScreenSizeWidth, int iScreenHeight,
    float &flOrigin_X, float &flOrigin_Y, float &flOrigin_Z,
    float &flScreen_X, float &flScreen_Y, float* pflMatrix);

  std::chrono::duration<long long, std::ratio_multiply
    <std::ratio<100i64, 1i64>, std::nano>> Time();

  template<typename T> // T = std::chrono::milliseconds
  T Time()
  {
    return std::chrono::duration_cast<T>(Time());
  }

  std::string GetFilePath(const std::string &sInput);
  std::string GetFileName(const std::string &sInput);
  std::string ToString(int iNumber);

  WORD GetWindowsVersionWord();
  BOOL IsWin8OrHigher();
  BOOL IsVistaOrHigher();

  PVOID GetPEB();
  PVOID GetPEB64();

  int GetHeapFlagsOffset(bool x64);
  int GetHeapForceFlagsOffset(bool x64);

  IDirect3DDevice9* GetD3D();

  HMODULE GetModHandle(const std::string &moduleName);
  MODULEINFO GetModInfo(const std::string &moduleName);
  MODULEINFO GetModInfo(HMODULE hModule);

  ValueTypes GetValueType(const std::string &sInput);
  bool IsFloat(const std::string &sInput);
  bool IsInt(const std::string &sInput);

  void CopyToClipboard(const std::string &sInput);

  void OpenConsole(const std::string &consoleTitle);
  void CloseConsole();

  void HWID(std::string &sInput, int iHWIDGenerateDelayMs = 1000);

  bool GetFilePattern(const std::string &sFilePath, std::string &sFilePattern);
  std::string sha256(std::string input);

  std::string U16S_S8(const std::u16string &u16str);
  std::u16string S8_U16S(const std::string &str8);
  std::wstring S8_WS(const std::string &str8);
  std::string WS_S8(const std::wstring &wstr);

  std::vector<int> FindSeparatorIndexes(
    const std::string &str,
    const std::string &separator);

  std::vector<std::string> Split(
    const std::string &str,
    const std::string &separator);

  bool GeneratePattern(void* pDestination, DWORD size, std::string& pattern);

  void HexStringToPattern(
    const std::string &hexString,
    std::string &pattern,
    DWORD length);

  void PatternToHexString(
    const std::string &pattern,
    std::string &hexString);

  void PatternToHexString(
    const std::string &pattern,
    std::string &hexString,
    std::string &mask);

  std::string ByteToHex(unsigned char byte);

  DWORD CountPatternBytes(const std::string &pattern);

  void* FindPattern(
    const std::string &sModule,
    const std::string &pattern,
    const std::string &patternName = "",
    int iFinalOffset = 0);

  template<class T> T FindPattern(
    const std::string &sModule,
    const std::string &pattern,
    const std::string &patternName,
    int iFinalOffset)
  {
    return (T)FindPattern(sModule, pattern, patternName, iFinalOffset);
  }

  void* FindPattern(
    HMODULE hModule,
    const std::string &pattern,
    const std::string &patternName = "",
    int iFinalOffset = 0);

  template<class T> T FindPattern(
    HMODULE hModule,
    const std::string &pattern,
    const std::string &patternName,
    int iFinalOffset)
  {
    return (T)FindPattern(hModule, pattern, patternName, iFinalOffset);
  }

  void* FindPattern(
    MODULEINFO mInfo,
    const std::string &pattern,
    const std::string &patternName = "",
    int iFinalOffset = 0);

  template<class T> T FindPattern(
    MODULEINFO mInfo,
    const std::string &pattern,
    const std::string &patternName,
    int iFinalOffset)
  {
    return (T)FindPattern(mInfo, pattern, patternName, iFinalOffset);
  }

  void* FindIFace(void* pFnFactory, const std::string &sIFaceName);
  template <typename T> T FindIFace(void* pFnFactory, const std::string &name)
  {
    return (T)FindIFace(pFnFactory, name);
  }

  float Calc2dDistance(float point1[2], float point2[2]);
  float Calc3dDistance(float point1[3], float point2[3]);

  double Calc2dDistance(double point1[2], double point2[2]);
  double Calc3dDistance(double point1[3], double point2[3]);

  namespace XorString
  {
    constexpr auto compileTime =
      (__TIME__[7] - '0') +
      (__TIME__[6] - '0') * 10 +
      (__TIME__[4] - '0') * 60 +
      (__TIME__[3] - '0') * 600 +
      (__TIME__[1] - '0') * 3600 +
      (__TIME__[0] - '0') * 36000;

    template <size_t size>
    class XSClass
    {
    public:
      template <size_t... IdxSequence>
      constexpr __forceinline XSClass(
        const char* str, std::index_sequence<IdxSequence...>)
        : m_cKey(static_cast<char>(compileTime % 125 + 1)),
          m_Encrypted { Encrypt(str[IdxSequence])... }
      {

      }

      __forceinline decltype(auto) DecryptAll()
      {
        for (size_t i = 0; i < size; i++)
        {
          m_Encrypted[i] = Encrypt(m_Encrypted[i]);
        }
        m_Encrypted[size] = '\0';
        return m_Encrypted.data();
      }

    private:
      const char m_cKey;
      std::array<char, size + 1> m_Encrypted;

      constexpr char Encrypt(char symbol) const
      {
        return symbol ^ m_cKey;
      }
    }; // XSClass
  }
}


