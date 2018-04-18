#pragma once

namespace U
{
  namespace VMTS
  {
    namespace D3D
    {
      enum
      {
        QueryInterface,
        AddRef,
        Release,
        TestCooperativeLevel,
        GetAvailableTextureMem,
        EvictManagedResources,
        GetDirect3D,
        GetDeviceCaps,
        GetDisplayMode,
        GetCreationParameters,
        SetCursorProperties,
        SetCursorPosition,
        ShowCursor,
        CreateAdditionalSwapChain,
        GetSwapChain,
        GetNumberOfSwapChains,
        Reset,
        Present,
        GetBackBuffer,
        GetRasterStatus,
        SetDialogBoxMode,
        SetGammaRamp,
        GetGammaRamp,
        CreateTexture,
        CreateVolumeTexture,
        CreateCubeTexture,
        CreateVertexBuffer,
        CreateIndexBuffer,
        CreateRenderTarget,
        CreateDepthStencilSurface,
        UpdateSurface,
        UpdateTexture,
        GetRenderTargetData,
        GetFrontBufferData,
        StretchRect,
        ColorFill,
        CreateOffscreenPlainSurface,
        SetRenderTarget,
        GetRenderTarget,
        SetDepthStencilSurface,
        GetDepthStencilSurface,
        BeginScene,
        EndScene,
        Clear,
        SetTransform,
        GetTransform,
        MultiplyTransform,
        SetViewport,
        GetViewport,
        SetMaterial,
        GetMaterial,
        SetLight,
        GetLight,
        LightEnable,
        GetLightEnable,
        SetClipPlane,
        GetClipPlane,
        SetRenderState,
        GetRenderState,
        CreateStateBlock,
        BeginStateBlock,
        EndStateBlock,
        SetClipStatus,
        GetClipStatus,
        GetTexture,
        SetTexture,
        GetTextureStageState,
        SetTextureStageState,
        GetSamplerState,
        SetSamplerState,
        ValidateDevice,
        SetPaletteEntries,
        GetPaletteEntries,
        SetCurrentTexturePalette,
        GetCurrentTexturePalette,
        SetScissorRect,
        GetScissorRect,
        SetSoftwareVertexProcessing,
        GetSoftwareVertexProcessing,
        SetNPatchMode,
        GetNPatchMode,
        DrawPrimitive,
        DrawIndexedPrimitive,
        DrawPrimitiveUP,
        DrawIndexedPrimitiveUP,
        ProcessVertices,
        CreateVertexDeclaration,
        SetVertexDeclaration,
        GetVertexDeclaration,
        SetFVF,
        GetFVF,
        CreateVertexShader,
        SetVertexShader,
        GetVertexShader,
        SetVertexShaderConstantF,
        GetVertexShaderConstantF,
        SetVertexShaderConstantI,
        GetVertexShaderConstantI,
        SetVertexShaderConstantB,
        GetVertexShaderConstantB,
        SetStreamSource,
        GetStreamSource,
        SetStreamSourceFreq,
        GetStreamSourceFreq,
        SetIndices,
        GetIndices,
        CreatePixelShader,
        SetPixelShader,
        GetPixelShader,
        SetPixelShaderConstantF,
        GetPixelShaderConstantF,
        SetPixelShaderConstantI,
        GetPixelShaderConstantI,
        SetPixelShaderConstantB,
        GetPixelShaderConstantB,
        DrawRectPatch,
        DrawTriPatch,
        DeletePatch,
        CreateQuery
      };
    }
  }

  class VMTHook
  {
  public:
    VMTHook(
      const std::string &sHookName,
      void* pVMTObject,
      int iFnIndex,
      void* pHkFunction);

    ~VMTHook();

    std::string GetName() const;

    void* GetVMTObject() const;
    template<class T> T GetVMTObject() const
    {
      return (T)GetVMTObject();
    }

    void* GetOriginFn() const;
    template<class T> T GetOriginFn() const
    {
      return (T)GetOriginFn();
    }

    void* GetHookFn() const;
    template<class T> T GetHookFn() const
    {
      return (T)GetHookFn();
    }


    int GetFnIndex() const;

    bool SetName(const std::string &sNewName);
    bool SetVMTObject(void* pNewVMTObject);
    bool SetOriginFn(void* pNewOriginFn);
    bool SetHookFn(void* pNewHookFn);
    bool SetFnIndex(int iNewFnIndex);

    bool Apply();
    bool Restore();

    bool IsVirtualMethodSpoofed();
    bool IsHooked();

  private:
    std::string m_sHookName;

    void* m_pVMTObject;
    void* m_pOriginFn;
    void* m_pHookFn;

    int m_iFnIndex;

    bool m_bHooked;
  };
}