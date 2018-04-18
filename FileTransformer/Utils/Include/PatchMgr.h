#pragma once

#include <map>

namespace U
{
  class PatchMgr
  {
  public:
    ~PatchMgr();

    static PatchMgr* Instance();

    static bool RegPatch(
      const std::string &sPatchName,
      void* pDestination,
      const std::string &sNewBytes);

    static bool UnregPatch(const std::string &sPatchName);

    static bool ApplyPatch(const std::string &sPatchName);
    static bool RestorePatch(const std::string &sPatchName);

    static Patch* GetPatch(const std::string &sPatchName);

    static std::map<std::string, Patch*> GetPatches();

  private:
    PatchMgr();

    static PatchMgr* sm_pInstance;

    std::map<std::string, Patch*> m_mapPatches;
  };
}


