#include "Process.h"

Process::Process(HWND hWnd) : m_hWnd(hWnd)
{

}

Process::~Process()
{

}

void Process::Initialize()
{
	RECT tempsize{};
	GetClientRect(m_hWnd, &tempsize);
	float Width = static_cast<float>(tempsize.right - tempsize.left);
	float Height = static_cast<float>(tempsize.bottom - tempsize.top);
	float ratio = Width / Height;


	m_cameraManager = new CameraManager(ratio);
	m_cameraManager->Initialize();
	m_cameraManager->SetCurCameraPos({ 0, 1, -3 });

	m_timeManager = new TimeManager();
	m_timeManager->Initialize();

	m_graphicsEngine = CreateGraphics(m_hWnd);
	m_graphicsEngine->Initialize();
	m_graphicsEngine->DebugRenderONOFF(true);

	m_inputManager = InputManager::GetInstance();
	m_inputManager->Initialize(m_hWnd);

	Scene();
}

void Process::Update()
{

	m_timeManager->Update(m_timeManager->DeltaTime());
	double FPS = m_timeManager->FPS();
	ui::TextInfo fps = m_UIs[1];
	fps.Text = std::format(L"FPS : {:.1f}", FPS);
	m_graphicsEngine->UpdateTextObject(fps.uid, fps);

	if (InputManager::GetInstance()->IsKeyDown('I'))
	{
		static bool use = true;
		use = !use;
		m_graphicsEngine->IBLONOFF(use);
	}

	if (InputManager::GetInstance()->IsKeyDown(VK_F1))
	{
		static bool use = true;
		use = !use;
		m_graphicsEngine->DebugRenderONOFF(use);
	}

	if (InputManager::GetInstance()->IsKeyPress(VK_MBUTTON))
	{
		static double rotation = 0.0f;

		std::shared_ptr<RenderData> testmodel = m_models[0];

		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(InputManager::GetInstance()->GetDeltMouseX()));
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(InputManager::GetInstance()->GetDeltMouseY()));

		testmodel->world._11 = cosf(rotation);		testmodel->world._13 = sinf(rotation);
		testmodel->world._31 = -sinf(rotation);		testmodel->world._33 = cosf(rotation);
		rotation += 2 * 0.5f * m_timeManager->DeltaTime();
	}
	InputManager::GetInstance()->Update();

	m_cameraManager->Update(m_timeManager->DeltaTime());
	DirectX::SimpleMath::Matrix view = m_cameraManager->View();
	DirectX::SimpleMath::Matrix proj = m_cameraManager->Proj();
	DirectX::SimpleMath::Matrix ortho = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Vector3 pos = m_cameraManager->GetCamerPos();
	ui::TextInfo camerapos = m_UIs[1];
	camerapos.Text = std::format(L"ī�޶� ��ġ : {:.1f}, {:.1f}, {:.1f}", (double)pos.x, (double)pos.y, (double)pos.z);
	m_graphicsEngine->UpdateTextObject(camerapos.uid, camerapos);



	m_graphicsEngine->CulingUpdate();
	m_graphicsEngine->AnimationUpdate(m_timeManager->DeltaTime());
	m_graphicsEngine->Update(m_timeManager->DeltaTime());
	m_graphicsEngine->EndUpdate(m_timeManager->DeltaTime());
}

void Process::Render()
{
	m_graphicsEngine->BeginRender();
	m_graphicsEngine->Render(m_timeManager->DeltaTime());
	m_graphicsEngine->EndRender();
}

void Process::Finalize()
{
	for (auto info : m_UIs)
	{
		m_graphicsEngine->DeleteTextObject(info.uid);
	}

	m_graphicsEngine->Finalize();
	DestroyGraphics(m_graphicsEngine);

}

void Process::OnResize(HWND hWnd, bool isFullScreen)
{
	m_graphicsEngine->OnResize(m_hWnd, isFullScreen);
}

void Process::Scene()
{
	///��1
	std::shared_ptr<RenderData> testmodel = std::make_shared<RenderData>();
	testmodel->EntityID = 1;
	//testmodel->FBX = L"monkey.fbx"; //�̸����� � ���� �ҷ����� ����
	testmodel->FBX = L"pbrtest.fbx"; //�̸����� � ���� �ҷ����� ����
	testmodel->world = DirectX::SimpleMath::Matrix::Identity;
	testmodel->world._42 = 1;
	testmodel->offset = { 0,0 };
	testmodel->lightmapindex = 0;
	testmodel->scale = 1;
	testmodel->tiling = { 0,0 };
	testmodel->punchEffect = false;
	testmodel->isSkinned = false;	//���� �ִϸ��̼��� ������ �ִ°�?
	testmodel->isPlay = false;		//���� �ִϸ��̼��� �����ϴ°�?
	testmodel->color = DirectX::XMFLOAT4{ 0,0,0,0 };
	testmodel->preAni = 0;
	testmodel->curAni = 0;
	testmodel->duration = 0;

	double testmodelanimationtime = 0;
	if (testmodel->isSkinned && testmodel->isPlay)
	{
		testmodelanimationtime = m_graphicsEngine->GetDuration(testmodel->FBX, 0);

	}
	m_graphicsEngine->AddRenderModel(testmodel);
	m_models.push_back(testmodel);

	///��2
	std::shared_ptr<RenderData> pureMetal = std::make_shared<RenderData>();
	pureMetal->EntityID = 2;
	pureMetal->FBX = L"pbrtest.fbx";
	//pureMetal->FBX = L"monkey.fbx";
	pureMetal->world = DirectX::SimpleMath::Matrix::Identity;
	pureMetal->world._41 = 2;
	pureMetal->world._42 = 1;
	pureMetal->offset = { 0,0 };
	pureMetal->lightmapindex = 0;
	pureMetal->scale = 1;
	pureMetal->tiling = { 0,0 };
	pureMetal->punchEffect = false;
	pureMetal->isSkinned = false;	//���� �ִϸ��̼��� ������ �ִ°�?
	pureMetal->isPlay = false;		//���� �ִϸ��̼��� �����ϴ°�?
	pureMetal->color = DirectX::XMFLOAT4{ 0,0,0,0 };
	pureMetal->preAni = 0;
	pureMetal->curAni = 0;
	pureMetal->duration = 0;

	double pureMetalanimationtime = 0;
	if (pureMetal->isSkinned && pureMetal->isPlay)
	{
		pureMetalanimationtime = m_graphicsEngine->GetDuration(pureMetal->FBX, 0);
	}
	m_graphicsEngine->AddRenderModel(pureMetal);
	m_models.push_back(pureMetal);

	LightData dir;
	dir.direction = DirectX::XMFLOAT3(0, -1, 1);
	dir.type = static_cast<float>(LightType::Direction);
	m_graphicsEngine->AddLight(1001, LightType::Direction, dir);


	///����� ��ο�
	debug::GridInfo grid;
	grid.Color = { 1,1,1,1 };
	grid.GridSize = 100;
	grid.XAxis = { 1,0,0 };
	grid.YAxis = { 0,0,1 };
	grid.Origin = { 0,0,0 };
	grid.XDivs = 100;
	grid.YDivs = 100;

	debug::RayInfo XAxis;
	XAxis.Color = { 1,0,0,1 };
	XAxis.Direction = { 10,0,0 };
	XAxis.Normalize = false;
	XAxis.Origin.y = 0.01;

	debug::RayInfo YAxis;
	YAxis.Color = { 0,1,0,1 };
	YAxis.Direction = { 0,10,0 };
	YAxis.Normalize = false;

	debug::RayInfo ZAxis;
	ZAxis.Color = { 0,0,1,1 };
	ZAxis.Direction = { 0,0,10 };
	ZAxis.Normalize = false;
	ZAxis.Origin.y = 0.01;

	m_graphicsEngine->DrawGrid(grid);
	m_graphicsEngine->DrawRay(XAxis);
	m_graphicsEngine->DrawRay(YAxis);
	m_graphicsEngine->DrawRay(ZAxis);



	ui::TextInfo text;
	text.uid = 111;
	text.Color = { 0,1,0,1 };
	text.PosXPercent = 10;
	text.PosYPercent = 3;
	text.Text = L"ī�޶� �̵� : WASD\nī�޶� ȸ�� : ���콺 ��Ŭ�� �̵�";
	text.Scale = 0.3f;
	m_graphicsEngine->CreateTextObject(text.uid, text);
	m_UIs.push_back(text);


	ui::TextInfo camerapos;
	camerapos.uid = 112;
	camerapos.Color = { 1,0,0,1 };
	camerapos.PosXPercent = 9;
	camerapos.PosYPercent = 10;
	camerapos.Text = std::format(L"ī�޶� ��ġ : {}, {}, {}", 0, 0, 0);
	camerapos.Scale = 0.3f;
	m_graphicsEngine->CreateTextObject(camerapos.uid, camerapos);
	m_UIs.push_back(camerapos);

	ui::TextInfo fps;
	fps.uid = 113;
	fps.Color = { 1,1,0,1 };
	fps.PosXPercent = 4;
	fps.PosYPercent = 15;
	fps.Text = std::format(L"FPS : {}", 0);
	fps.Scale = 0.3f;
	m_graphicsEngine->CreateTextObject(fps.uid, fps);
	m_UIs.push_back(fps);

}