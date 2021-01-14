#include "DirectGraphics.h"

struct XFile{
	DWORD MaterialNum;
	LPD3DXMESH Meshed;
	LPD3DXBUFFER Materials;
	LPDIRECT3DTEXTURE9* Textures;
};
XFile g_XFile;

LPDIRECT3D9 g_Interface = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

LPCWSTR g_TextureNameList[] = {
	TEXT( "Sprite/Picasso.bmp" )
};

bool InitDirectGraphics(HWND window_handle) {
	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr) {
		return false; // 初期化失敗
	}

	// デバイスの情報を設定する構造体
	D3DPRESENT_PARAMETERS parameters;

	// データを0で初期化
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	parameters.BackBufferCount = 1;
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	parameters.Windowed = true;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window_handle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING |
		D3DCREATE_MULTITHREADED,
		&parameters,
		&g_Device
	);

	if (FAILED(result)) {
		return false;
	}

	SetUpProjection();

	return true;

}

void ReleaseDirectGraphics() {
	if (g_Device != nullptr) {
		g_Device->Release();
		g_Device = nullptr;
	}

	if (g_Interface != nullptr) {
		g_Interface->Release();
		g_Interface = nullptr;
	}
}

void StartDrawing() {
	g_Device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_Device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	g_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	g_Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	g_Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_Device->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_Device->SetRenderState( D3DRS_ZENABLE, TRUE );

	g_Device->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 200, 200, 200 ),
		1.0f,
		0
	);

	g_Device->BeginScene();
}

void FinishDrawing() {

	g_Device->EndScene();

	g_Device->Present( nullptr, nullptr, nullptr, nullptr );
}

void DrawPorigon_Triangle() {
	CustomVertex vertices[] = {
		{ 300, 100, 0, 1, 0xff0000},
		{ 400, 200, 0, 1, 0xff0000},
		{ 200, 200, 0, 1, 0xff0000},
	};

	g_Device->SetFVF( FVF_2D_VERTEX );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		1,
		vertices,
		sizeof( CustomVertex )
	);
}

void DrawPorigon_Rect() {
	CustomVertex vertices[] = {
		{ 300, 300, 0, 1, 0x00ffff},
		{ 500, 300, 0, 1, 0x0088ff},
		{ 500, 400, 0, 1, 0x00ff88},
		{ 300, 400, 0, 1, 0x008888}
	};

	g_Device->SetFVF( FVF_2D_VERTEX );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof( CustomVertex )
	);
}

bool LoadTexture( TextureID tex_id ){
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,
		g_TextureNameList[tex_id],
		&g_Textures[tex_id]
	);

	if ( FAILED( hr ) ){
		return false;
	}

	return true;
}

void ReleaseTexture(){
	for ( int i = 0; i < TextureID::TexIDMax; i++ ){
		if ( g_Textures[i] != nullptr ){
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

void DrawTexture( TextureID tex_id ){
	if ( g_Textures[tex_id] == nullptr ){
		return;
	}

	CustomVertex vertices[] = {
		{   0,   0, 0, 1, 0xffffffff, 0.0f, 0.0f},
		{ 100,   0, 0, 1, 0x80ffffff, 1.0f, 0.0f},
		{ 100, 100, 0, 1, 0x80ffffff, 1.0f, 1.0f},
		{   0, 100, 0, 1, 0x00ffffff, 0.0f, 1.0f}
	};

	g_Device->SetFVF( FVF_2D_VERTEX_TEXTURE );

	g_Device->SetTexture( 0, g_Textures[tex_id] );

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertices,
		sizeof( CustomVertex )
	);
}

void SetUpView(){
	D3DXMATRIX matrix;
	D3DXVECTOR3 cameraPos = D3DXVECTOR3( 0.0f, 0.0f, -200.0f );
	D3DXVECTOR3 eyePos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 upVec = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	D3DXMatrixLookAtLH(
		&matrix,
		&cameraPos,
		&eyePos,
		&upVec
	);

	g_Device->SetTransform( D3DTS_VIEW, &matrix );
}

void SetUpProjection(){
	D3DXMATRIX matrix;
	float aspect = 640.0f / 480.0f;

	D3DXMatrixPerspectiveFovLH(
		&matrix,
		D3DXToRadian( 60.0f ),
		aspect,
		0.1f,
		1000.0f
	);

	g_Device->SetTransform( D3DTS_PROJECTION, &matrix );
}

void Draw3DPorigon( CustomVertex3D top1, CustomVertex3D top2, CustomVertex3D top3 ){
	g_Device->SetRenderState( D3DRS_LIGHTING, FALSE );

	CustomVertex3D vertices[] {
		top1,
		top2,
		top3
	};

	g_Device->SetFVF( FVF_3D_VERTEX );

	g_Device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, vertices, sizeof( CustomVertex3D ) );
}

bool LoadXFile( LPCWSTR fileName_ ){
	HRESULT hr = D3DXLoadMeshFromX(
		fileName_,
		D3DXMESH_SYSTEMMEM,
		g_Device,
		nullptr,
		&g_XFile.Materials,
		nullptr,
		&g_XFile.MaterialNum,
		&g_XFile.Meshed
	);

	if( FAILED( hr ) ) return false;

	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];

	D3DXMATERIAL* materials = ( D3DXMATERIAL* ) g_XFile.Materials->GetBufferPointer();

	for( int i = 0; i < g_XFile.MaterialNum; i++ ){
		if( materials[i].pTextureFilename != nullptr ){
			LPSTR charFileName = materials[i].pTextureFilename;

			wchar_t wtext[256];
			size_t ret;
			mbstowcs_s( &ret, wtext, charFileName, strlen( charFileName ) + 1 );
			LPWSTR fileName = wtext;

			HRESULT hr = D3DXCreateTextureFromFile(
				g_Device,
				fileName,
				&g_XFile.Textures[i]
			);

			if( FAILED( hr ) ) return false;
		}
		else{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void ReleaseXFile(){
	for( int i = 0; i < g_XFile.MaterialNum; i++ ){
		if( g_XFile.Textures[i] != nullptr ){
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	delete[] g_XFile.Textures;

	if( g_XFile.Materials != nullptr ){
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}

	if( g_XFile.Meshed != nullptr ){
		g_XFile.Meshed->Release();
		g_XFile.Meshed = nullptr;
	}
}

void DrawXFile( float posX_, float posY_, float posZ_, float radX_, float radY_, float radZ_, float scaleX_, float scaleY_, float scaleZ_ ){
	float posX = posX_;
	float posY = posY_;
	float posZ = posZ_;

	float scaleX = scaleX_;
	float scaleY = scaleY_;
	float scaleZ = scaleZ_;

	float radX = D3DXToRadian( radX_ );
	float radY = D3DXToRadian( radY_ );
	float radZ = D3DXToRadian( radZ_ );

	D3DXMATRIX matrix;
	D3DXMATRIX scaleMatrix;
	D3DXMATRIX transMatrix;

	D3DXMATRIX rotateXMatrix;
	D3DXMATRIX rotateYMatrix;
	D3DXMATRIX rotateZMatrix;

	D3DXMatrixIdentity( &matrix );

	// 拡縮 * 回転
	D3DXMatrixScaling( &scaleMatrix, scaleX, scaleY, scaleZ );

	D3DXMatrixRotationX( &rotateXMatrix, radX );
	D3DXMatrixRotationY( &rotateYMatrix, radY );
	D3DXMatrixRotationZ( &rotateZMatrix, radZ );

	// = 移動
	D3DXMatrixTranslation( &transMatrix, posX, posY, posZ );

	matrix *= scaleMatrix;

	matrix *= rotateXMatrix;
	matrix *= rotateYMatrix;
	matrix *= rotateZMatrix;

	matrix *= transMatrix;

	g_Device->SetTransform( D3DTS_WORLD, &matrix );

	D3DXMATERIAL* materials = ( D3DXMATERIAL* ) g_XFile.Materials->GetBufferPointer();

	for( DWORD i = 0; i < g_XFile.MaterialNum; i++ ){
		g_Device->SetMaterial( &materials[i].MatD3D );
		g_Device->SetTexture( 0, g_XFile.Textures[i] );
		g_XFile.Meshed->DrawSubset( i );
	}
}
