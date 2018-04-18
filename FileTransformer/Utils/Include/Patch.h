#pragma once

namespace U
{
  class Patch
  {
  public:
    Patch(
      const std::string &sPatchName,
      void* pDestination,
      const std::string &sNewBytes);

    ~Patch();

    void* GetDestination() const;

    std::string GetName() const;

    DWORD GetCountOldBytes() const;
    DWORD GetCountNewBytes() const;

    bool SetDestination(void* pNewDestination);
    bool SetName(const std::string &sNewName);
    bool SetOldBytes(const std::string &sOldBytes);
    bool SetNewBytes(const std::string &sNewBytes);

    bool Apply();
    bool Restore();

  private:
    std::string m_sPatchName;
    void* m_pDestination;

    std::string m_sOldBytesPattern;
    DWORD m_dwCountOldBytes;

    std::string m_sNewBytesPattern;
    DWORD m_dwCountNewBytes;

    bool m_bPatched;
  };
}

