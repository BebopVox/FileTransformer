#pragma once

#define OPCODE_PUSHAD 0x60
#define OPCODE_POPAD 0x61
#define OPCODE_PUSHFD 0x9C
#define OPCODE_POPFD 0x9D
#define OPCODE_CALL 0xE8
#define OPCODE_JMP 0xE9
#define OPCODE_NOP 0x90

namespace U
{
  class JMPHook
  {
  public:

    enum class JMPTypes
    {
      JMPType_Call = OPCODE_CALL,
      JMPType_JMP = OPCODE_JMP,
    };

    JMPHook(
      const std::string &sHookName,
      void* pJumpStart,
      int iInstructionLength,
      void* pJumpTo,
      JMPTypes JMPType = JMPTypes::JMPType_Call,
      bool bSaveRegistersBeforeCall = true);

    ~JMPHook();

    std::string GetName() const;
    std::string GetOrigInstructPattern() const;

    void* GetJumpStart() const;
    void* GetJumpTo() const;

    int GetOrigInstructLength() const;

    bool Apply();
    bool Restore();

  private:
    std::string m_sHookName;
    std::string m_sOriginInstructPattern;

    JMPTypes m_JMPType;

    int m_iOrigInstructLength;

    void* m_pJumpStart;
    void* m_pJumpTo;
    void* m_pOpCodes;

    bool m_bHooked;
    bool m_bSaveRegisters;
  };
}