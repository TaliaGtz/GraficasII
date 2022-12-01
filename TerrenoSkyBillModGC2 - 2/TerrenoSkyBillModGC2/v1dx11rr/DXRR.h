#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include <dinput.h>
#include "GUI.h"
#include "Text.h"

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	//Mi terreno
	TerrenoRR *terreno;

	//Mi Skydome
	SkyDome *skydome;

	//Mis Billboards
	BillboardRR *billboard;
	BillboardRR* miArbol;
	BillboardRR* Bote;
	BillboardRR* Lamp;
	BillboardRR* Hidrante;
	BillboardRR* ModelColisioner;

	//Mi Camara
	Camara *camara;

	//Mi Texto
	Text* texto;

	//Mis modelos
	ModeloRR* model;
	ModeloRR* coche;
	ModeloRR* zombi;
	ModeloRR* casa;
	ModeloRR* casa2;
	ModeloRR* casaPerro;
	ModeloRR* sign;
	ModeloRR* gasolinera;
	ModeloRR* hangar;
	ModeloRR* edificio;
	ModeloRR* suministros;
	ModeloRR* suministros1;
	ModeloRR* suministros2;
	ModeloRR* suministros3;
	ModeloRR* suministros4;
	

	//Mi GUI
	GUI* gameover;
	GUI* win;

	float izqder;
	float arriaba;
	float vel;
	float segundos;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];

	bool ColisionesSuministros[5] = {false};

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;

	bool camType = true;
	float rotCam;
	float escala = 10.0;
	float extra = 0.2f;
	float moveZ = 25;

	int intPuntos;
	int intSuministrosObtenidos;
	
    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		billCargaFuego();
		segundos = 121;
		intPuntos = 0;
		intSuministrosObtenidos = 0;
		camara = new Camara(D3DXVECTOR3(0,120,70), D3DXVECTOR3(0,40,0), D3DXVECTOR3(0,10,0), Ancho, Alto);
		terreno = new TerrenoRR(1500, 1500, d3dDevice, d3dContext);
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Assets/Skydome/skydome.jpg");
		//Billboards
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);
		miArbol = new BillboardRR(L"Assets/Billboards/arbolSeco.png", L"Assets/Billboards/arbolSecoNormal.png", d3dDevice, d3dContext, 10);
		Bote = new BillboardRR(L"Assets/Billboards/Bote.png", L"Assets/Billboards/BoteNormal.png", d3dDevice, d3dContext, 10);
		Lamp = new BillboardRR(L"Assets/Billboards/lamp.png", L"Assets/Billboards/lampNormal.png", d3dDevice, d3dContext, 10);
		Hidrante = new BillboardRR(L"Assets/Billboards/hydrant.png", L"Assets/Billboards/hydrantNormal.png", d3dDevice, d3dContext, 10);
		ModelColisioner = new BillboardRR(L"Assets/Billboards/Collider.png", L"Assets/Billboards/Collider.png", d3dDevice, d3dContext, 10);
		//Models
		model = new ModeloRR(d3dDevice, d3dContext, "Assets/Cofre/Cofre.obj", L"Assets/Cofre/Cofre-color.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);
		coche = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Truck/Truck2.obj", L"Assets/Modelos/Truck/Truck_diff.jpg", L"Assets/Modelos/Truck/Truck_spec.jpg", 0, 0);
		zombi = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/zombie/Zombi.obj", L"Assets/Modelos/zombie/Zombie.png", L"Assets/Modelos/zombie/Zombie_gloss.png", 0, moveZ);
		casa = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Casa/Construccion1.obj", L"Assets/Modelos/Casa/house_base.png", L"Assets/Modelos/Casa/house_base.png", 0, moveZ);
		casa2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/EdificioRojo/EdificioRojo.obj", L"Assets/Modelos/EdificioRojo/EdRojo.jpg", L"Assets/Modelos/EdificioRojo/EdRojoSpec.png", 0, moveZ);
		casaPerro = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/CasaPerro/CasaPerro.obj", L"Assets/Modelos/CasaPerro/doghouse0908_PBR_BaseColor.png", L"Assets/Modelos/CasaPerro/doghouse0908_PBR_AO.png", 0, moveZ);
		sign = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Sign/Sign.obj", L"Assets/Modelos/Sign/sign_base_1.png", L"Assets/Modelos/Sign/sign_specular.png", 0, moveZ);
		gasolinera = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Gasolinera/gasolinera.obj", L"Assets/Modelos/Gasolinera/gasolinera.jpeg", L"Assets/Modelos/Gasolinera/gasolineraSpec.jpg", 0, moveZ);
		hangar = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Hangares/Hangares.obj", L"Assets/Modelos/Hangares/Hangar.png", L"Assets/Modelos/Hangares/HangarSpec.jpg", 0, moveZ);
		edificio = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Edificio/Edificio.obj", L"Assets/Modelos/Edificio/Edificio.jpeg", L"Assets/Modelos/Edificio/EdificioSpec.jpg", 0, moveZ);
		suministros = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Suministros/Suministros.obj", L"Assets/Modelos/Suministros/BaseColor.png", L"Assets/Modelos/Suministros/SumEspec.png", 45, -126 + moveZ);
		suministros1 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Suministros/Suministros.obj", L"Assets/Modelos/Suministros/BaseColor.png", L"Assets/Modelos/Suministros/SumEspec.png", -51, -69 + moveZ);
		suministros2 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Suministros/Suministros.obj", L"Assets/Modelos/Suministros/BaseColor.png", L"Assets/Modelos/Suministros/SumEspec.png", 62, 32 + moveZ);
		suministros3 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Suministros/Suministros.obj", L"Assets/Modelos/Suministros/BaseColor.png", L"Assets/Modelos/Suministros/SumEspec.png", -53, 85 + moveZ);
		suministros4 = new ModeloRR(d3dDevice, d3dContext, "Assets/Modelos/Suministros/Suministros.obj", L"Assets/Modelos/Suministros/BaseColor.png", L"Assets/Modelos/Suministros/SumEspec.png", 41, 121 + moveZ);

		//camType = true;	//Primera persona = true


		//GUI
		gameover = new GUI(d3dDevice, d3dContext, 1, 1, L"Assets/UI/GameOverFin.png");
		win = new GUI(d3dDevice, d3dContext, 1, 1, L"Assets/UI/Winner.png");
		//Texto
		texto = new Text(d3dDevice, d3dContext, 3.6, 1.2, L"Assets/Texto/font.jpg", XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

		rotCam = 0.0;
	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		rotCam += izqder;
		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if( d3dContext == 0 )
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView( backBufferTarget, clearColor );
		d3dContext->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 4.5;
		camara->posCam3P.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 6;
		camara->UpdateCam(vel, arriaba, izqder, camType);
		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		
		//Billboards
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-5, -85 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-5, -84.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard,true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			22, -66 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			22, -65.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-106, -63 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-106, -62.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-115, -49 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-115, -48.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			37, -62 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			37, -61.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			35, -153 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			35, -152.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			52, -167 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			52, -166.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-92, 36 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-92, 35.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			102, 140 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			102, 139.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);
		Bote->Draw(camara->vista, camara->proyeccion, camara->posCam,
			49, 63 + moveZ, 0, 3, uv1, uv2, uv3, uv4, frameBillboard, false);
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			49, 62.9 + moveZ, 4, 3, uv1, uv2, uv3, uv4, frameBillboard, true);

		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-87, -48 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard,false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-36, -138 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			58, -135 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			110, -60 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			125, -31 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			5, 5 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-76, 26 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-78, 170 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-50, 186 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-60, 153 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			92, 161 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			26, 98 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		miArbol->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-19, 109 + moveZ, 0, 20, uv1, uv2, uv3, uv4, frameBillboard, false);
		
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-73, 39 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-44, 155 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			78, 103 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			79, 5 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			37, 15 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			67, 142 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-59, -31 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			28, -127 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			75, -146 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);
		Lamp->Draw(camara->vista, camara->proyeccion, camara->posCam,
			83, -58 + moveZ, 0, 10, uv1, uv2, uv3, uv4, frameBillboard, false);

		Hidrante->Draw(camara->vista, camara->proyeccion, camara->posCam,
			88, -12 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		Hidrante->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-32, -115 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		Hidrante->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-47, 18 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		Hidrante->Draw(camara->vista, camara->proyeccion, camara->posCam,
			83, 138 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		Hidrante->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-80, 52 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);

		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			23.536, 3.028 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {	
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			37.594, 3.028 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			50.999, 3.028 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11.955, 16.254 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-29.513, -6.526 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			12.436, -55.128 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-9.195, -75.386 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			33.136, -76.779 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-29.513, -6.526 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(14))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			15.128, -80.934 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(7))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			30.222, -58.987 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(7))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			61.905, -44.458 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			104.62, -38.089 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			104.62, -78.295 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			104.62, -121.661 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-51, 134 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-58, 177 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-17, 177 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			24, 177 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			69, 187 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			69, 165 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			113, -164 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			92, -164 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(10))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-1, -135 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(30))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-74, -4 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(30))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			26, 69 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			8, 69 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-8, 69 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-93, -88 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-86, -105 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-77, -123 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(25))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-101, 78 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-125, 78 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-125, 121 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-101, 121 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-125, 161 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-101, 161 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			109, 63 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			133, 63 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			133, 103 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			133, 20 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			109, 103 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}
		ModelColisioner->Draw(camara->vista, camara->proyeccion, camara->posCam,
			109, 20 + moveZ, 0, 2, uv1, uv2, uv3, uv4, frameBillboard, false);
		if (isPointInsideSphere(coche->getPos(), ModelColisioner->getSphere(28))) {
			colisiones();
			intPuntos = intPuntos - 1;
		}


		//Texto 
	

		coche->setPosX(camara->hdveo.x);
		coche->setPosZ(camara->hdveo.z);
		coche->Draw(camara->vista, camara->proyeccion, terreno->Superficie(coche->getPosX(), coche->getPosZ()), camara->posCam, 1.0f, rotCam + XM_PI, 'Y', 2.5 + extra, camType, true);
		zombi->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 0.7 + extra, camType, false);
		casa->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		casa2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		casaPerro->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		sign->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		gasolinera->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		hangar->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
		edificio->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', escala + extra, camType, false);
	
		// Suministro 
		if (ColisionesSuministros[0]) {
			suministros->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 50, camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		else {
			suministros->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		// Suministro 1
		if (ColisionesSuministros[1]) {
			suministros1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 50, camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		else {
			suministros1->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		// Suministro 2
		if (ColisionesSuministros[2]) {
			suministros2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) -50, camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		else {
			suministros2->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		// Suministro 3
		if (ColisionesSuministros[3]) {
			suministros3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20) - 50, camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		else {
			suministros3->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		// Suministro 4
		if (ColisionesSuministros[4]) {
			suministros4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20)-50, camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}
		else {
			suministros4->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camType, false);
		}

	
		// Colisiones con los suministros
		if ((isPointInsideSphere(coche->getPos(), suministros->getSphere(9))) && !ColisionesSuministros[0]) {
			colisiones();
			intPuntos= intPuntos + 1500;	
			intSuministrosObtenidos++;
			ColisionesSuministros[0] = true;
			segundos += 0.05;
		}
		
		if ((isPointInsideSphere(coche->getPos(), suministros1->getSphere(9))) && !ColisionesSuministros[1]) {
			colisiones();
			intPuntos = intPuntos + 1500;
			intSuministrosObtenidos++;
			ColisionesSuministros[1] = true;
			segundos += 0.05;
		}
	
		if ((isPointInsideSphere(coche->getPos(), suministros2->getSphere(9))) && !ColisionesSuministros[2]) {
			colisiones();
			intPuntos = intPuntos + 1500;
			intSuministrosObtenidos++;
			ColisionesSuministros[2] = true;
			segundos += 0.05;
		}
		if ((isPointInsideSphere(coche->getPos(), suministros3->getSphere(9))) && !ColisionesSuministros[3]) {
			colisiones();
			intPuntos = intPuntos + 1500;
			intSuministrosObtenidos++;
			ColisionesSuministros[3] = true;
			segundos += 0.05;
		}
		if ((isPointInsideSphere(coche->getPos(), suministros4->getSphere(9))) && !ColisionesSuministros[4]) {
			
			colisiones();
			intPuntos = intPuntos + 1500;
			intSuministrosObtenidos++;
			ColisionesSuministros[4] = true;
			segundos += 0.05;
		}

		//Puntos
		stringstream puntos;
		puntos << intPuntos;
		string puntosStr = puntos.str();

		intPuntos++;
		TurnOnAlphaBlending();
		//GUI
		texto->DrawText(-0.9, 0.9, "Puntuacion: " + puntosStr, 0.01);
		texto->DrawText(-0.9, 0.8, "Tiempo: " + texto->Time(segundos), 0.01);

		if (segundos <= 0 && intSuministrosObtenidos < 5) {
			gameover->Draw(0, 0);
		}
		if (intSuministrosObtenidos >= 5) {
			win->Draw(0, 0);
		}
		segundos -= 0.005;
	


		swapChain->Present( 1, 0 );
	}

	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}

	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}
	}

	void colisiones() {
		camara->posCam = camara->pastPosCam;
		camara->posCam3P = camara->pastPosCam3P;
	}

};
#endif