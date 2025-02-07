#include "pch.h"
#include "PixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Device.h"
#include "Vertex.h"
#include "Defines.h"

#include <iostream>
#include <string>

PixelShader::PixelShader(const std::shared_ptr<Device>& device, const std::wstring& filename,
	const std::string& entryPoint, const D3D_SHADER_MACRO* macro)
{
	// TODO: Binary 파일을 읽는 것과 hlsl 파일을 읽는 것으로 함수를 분리하는게 좋을 듯 하다.
	// -> 기껏 매크로 작성해서 인자로 넣었는데 hlsl 파일 안읽어질시에 바이너리 파일 읽히면 매크로가 적용이 안되어서 어리둥절할 수가 있다.
	// -> + 진입점 설정해 두어도 hlsl 컴파일 실패하면 바이너리 파일을 읽어버려서 적용이 안됨.. 등등..

	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	DWORD shaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;

	std::wstring basePath;
#ifdef _DEBUG
	shaderFlag |= D3DCOMPILE_DEBUG;
	shaderFlag |= D3DCOMPILE_SKIP_OPTIMIZATION;
	basePath = L"..\\Renderer\\";
	//basePath = L"..\\Data\\HLSL\\";
#else
	basePath = L"..\\Data\\HLSL\\";
	//basePath = L"..\\Renderer\\";
#endif

	const std::string& shaderModel = "ps_5_0";
	const std::wstring hlslFileBasePath = basePath;
	const std::wstring hlslFileExtension = L".hlsl";
	const std::wstring binaryFileExtension = L".cso";

	std::wstring filePath = hlslFileBasePath + filename + hlslFileExtension;

	// 컴파일 된 것이 있다면 그걸 쓴다.
	if (FAILED(D3DCompileFromFile(
		filePath.c_str(),
		macro,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(),
		shaderModel.c_str(),
		shaderFlag,
		0,
		&blob,
		&blob)))
	{
		filePath = filename + binaryFileExtension;
		hr = D3DReadFileToBlob(filePath.c_str(), &blob);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"PS Load Fail", nullptr, 0);
		return;
	}

	HR_CHECK(device->Get()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_PS));
}


void PixelShader::Release()
{
	m_PS.Reset();
}
