#pragma once

#include <map>

namespace U
{
  class VMTHookMgr
  {
  public:
    ~VMTHookMgr();

    static VMTHookMgr* Instance();

    static bool RegHook(
      const std::string &sHookName,
      void* pVMTObject,
      int iFnIndex,
      void* pHkFunction);

    static bool UnregHook(const std::string &sHookName);

    static bool ApplyHook(const std::string &sHookName);
    static bool RestoreHook(const std::string &sHookName);

    static VMTHook* GetHook(const std::string &sHookName);

    static std::map<std::string, VMTHook*> GetHooks();

  private:
    VMTHookMgr();

    static VMTHookMgr* sm_pInstance;

    std::map<std::string, VMTHook*> m_mapHooks;
  };
}