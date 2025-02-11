
.. _program_listing_file_rootex_core_resource_files_custom_material_resource_file.h:

Program Listing for File custom_material_resource_file.h
========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_rootex_core_resource_files_custom_material_resource_file.h>` (``rootex\core\resource_files\custom_material_resource_file.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "common/common.h"
   
   #include "renderer/shader.h"
   #include "material_resource_file.h"
   
   class CustomMaterialResourceFile : public MaterialResourceFile
   {
   private:
       static inline Microsoft::WRL::ComPtr<ID3D11SamplerState> s_Sampler;
       Vector<float> customConstantBuffers;
       Vector<TYPES_OF_BUFFERS> typeOfCustomConstantBuffers;
   
       CustomMaterialData m_MaterialData;
   
       Ptr<Shader> m_Shader;
       Microsoft::WRL::ComPtr<ID3D11Buffer> m_PSCB;
       Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSCB;
   
       void pushPSTexture(Ref<ImageResourceFile> texture);
       void setPSTexture(const String& newtexturePath, int position);
       void popPSTexture();
   
       void pushVSTexture(Ref<ImageResourceFile> texture);
       void setVSTexture(const String& newtexturePath, int position);
       void popVSTexture();
   
   public:
       static inline const String s_DefaultCustomVSPath = "rootex/core/renderer/shaders/custom_vertex_shader.hlsl";
       static inline const String s_DefaultCustomPSPath = "rootex/core/renderer/shaders/custom_pixel_shader.hlsl";
   
       static void Load();
       static void Destroy();
   
       explicit CustomMaterialResourceFile(const FilePath& path);
       ~CustomMaterialResourceFile() = default;
   
       void setShaders(const String& vertexShader, const String& pixelShader);
       void setVS(const String& vertexShader);
       void setPS(const String& pixelShader);
       void recompileShaders();
   
       const Shader* getShader() const override { return m_Shader.get(); };
       Vector<Ref<GPUTexture>> getTextures() const override;
   
       void bindShader() override;
       void bindTextures() override;
       void bindSamplers() override;
       void bindVSCB() override;
       void bindPSCB() override;
   
       JSON::json getJSON() const override;
       ID3D11ShaderResourceView* getPreview() const override;
   
       void reimport() override;
       bool save() override;
       void draw() override;
       void drawTextureSlots(const char* label, Vector<Ref<ImageResourceFile>>& textures);
   
       float getFloat(int index);
       Vector3 getFloat3(int index);
       Color getColor(int index);
   
       bool setFloat(int index, float value);
       bool setFloat3(int index, Vector3 value);
       bool setColor(int index, Color value);
   };
