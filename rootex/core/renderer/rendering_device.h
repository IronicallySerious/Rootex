#pragma once

#include "common/common.h"

#include <d3d11.h>

#include <d3dcompiler.h>
#include <string>

#include "resource_file.h"

#include "vendor/DirectXTK/Inc/SpriteBatch.h"
#include "vendor/DirectXTK/Inc/SpriteFont.h"

/// The boss of all rendering, all DirectX API calls requiring the Device or Context go through this
class RenderingDevice
{
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;

	/// Texture to render the game into when the Editor is launched
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_RenderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetBackBufferView;
	/// Quirks of Editor and Game rendering differences
	ID3D11RenderTargetView** m_CurrentRenderTarget;
	/// Quirks of Editor and Game rendering differences
	ID3D11RenderTargetView** m_UnboundRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_RenderTextureShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_OldSkyDepthStencilState;
	UINT m_StencilRef;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_NewSkyDepthStencilState;

	/// DirectXTK batch font renderer data structure
	Ref<DirectX::SpriteBatch> m_FontBatch;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RSState;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11Debug> m_Debug;
	bool m_MSAA;
	unsigned int m_4XMSQuality;

	RenderingDevice();
	RenderingDevice(RenderingDevice&) = delete;
	~RenderingDevice();

	/// Should only be called by Window class
	void swapBuffers();

	friend class Window;

#ifdef ROOTEX_EDITOR
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getContext();
	friend class Editor;
#endif // ROOTEX_EDITOR

public:
	static RenderingDevice* GetSingleton();

	void initialize(HWND hWnd, int width, int height, bool MSAA);

	void enableSkyDepthStencilState();
	void disableSkyDepthStencilState();
	Microsoft::WRL::ComPtr<ID3D11Buffer> createVertexBuffer(D3D11_BUFFER_DESC* vbd, D3D11_SUBRESOURCE_DATA* vsd, const UINT* stride, const UINT* offset);
	Microsoft::WRL::ComPtr<ID3D11Buffer> createIndexBuffer(D3D11_BUFFER_DESC* ibd, D3D11_SUBRESOURCE_DATA* isd, DXGI_FORMAT format);
	void createVSModelConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd);
	void createVSModelInverseConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd);
	void createVSViewConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd);
	void createVSProjectionConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd);
	void createPSConstantBuffer(D3D11_BUFFER_DESC* cbd, D3D11_SUBRESOURCE_DATA* csd, UINT offset);
	Microsoft::WRL::ComPtr<ID3D11PixelShader> createPixelShader(ID3DBlob* blob);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> createVertexShader(ID3DBlob* blob);
	Microsoft::WRL::ComPtr<ID3D11InputLayout> createVertexLayout(ID3DBlob* vertexShaderBlob, const D3D11_INPUT_ELEMENT_DESC* ied, UINT size);
	
	Ref<DirectX::SpriteFont> createFont(FileBuffer* fontFileBuffer);
	/// To hold shader blobs loaded from the compiled shader files
	Microsoft::WRL::ComPtr<ID3DBlob> createBlob(LPCWSTR path);
	/// To render the game onto a texture in case of Editor
	void createRenderTextureTarget(int width, int height);
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> createTexture(ImageResourceFile* imageRes);
	Microsoft::WRL::ComPtr<ID3D11SamplerState> createSamplerState();

	void bind(ID3D11Buffer* vertexBuffer, const unsigned int* stride, const unsigned int* offset);
	void bind(ID3D11Buffer* indexBuffer, DXGI_FORMAT format);
	void bind(ID3D11VertexShader* vertexShader);
	void bind(ID3D11PixelShader* pixelShader);
	void bind(ID3D11InputLayout* inputLayout);
	
	/// Binds textures used in Pixel Shader
	void setInPixelShader(unsigned int slot, unsigned int number, ID3D11ShaderResourceView* texture);
	/// Binds sampler used in sampling textures in Pixel Shader
	void setInPixelShader(ID3D11SamplerState* samplerState);

	void unbindShaderResources();

	void setRasterizerState();
	void setDepthStencilState();

	void setTextureRenderTarget();
	/// Faking Editor rendering
	void setBackBufferRenderTarget();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> getRenderTextureShaderResourceView();
	Ref<DirectX::SpriteBatch> getUIBatch();

	void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY pt);
	void setViewport(const D3D11_VIEWPORT* vp);
	
	/// The last boss, draws Triangles
	void drawIndexed(UINT number);
	void beginDrawUI();
	void endDrawUI();
	void clearCurrentRenderTarget(float r, float g, float b);
	void clearUnboundRenderTarget(float r, float g, float b);
};
