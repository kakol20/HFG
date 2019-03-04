/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#ifndef _MODELS_DEMO_H_
#define _MODELS_DEMO_H_

#include"Dx11DemoBase.h"
#include"ArcCamera.h"
#include "FirstPersonCamera.h"
#include<XInput.h>
#include <string>

#include "Mesh.h"
#include "Texture.h"

#include "GameObject.h"
#include "Animation.h"

#include "PlayerChar.h"
#include "Collision.h"

enum GameStates {PLAY_INTRO, START_MENU, RUN, PAUSED, INTRO_MOVIE_REPLAY};

enum PauseMenuSelection {RETURN, FPS, PLAY_MOVIE, QUIT};
#define PAUSE_MENU_ITEMS 4

inline PauseMenuSelection operator++(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   eDOW = static_cast<PauseMenuSelection>((i + 1) % PAUSE_MENU_ITEMS);
   return ePrev;
}
inline PauseMenuSelection operator--(PauseMenuSelection &eDOW, int)
{
   const PauseMenuSelection ePrev = eDOW;
   const int i = static_cast<int>(eDOW);
   if(i > 0)
   {
		eDOW = static_cast<PauseMenuSelection>(i - 1);
   }
   else
   {
	   eDOW = static_cast<PauseMenuSelection>(PAUSE_MENU_ITEMS - 1);
   }
   return ePrev;
}

class ModelsDemo : public Dx11DemoBase
{
    public:
        ModelsDemo( );
        virtual ~ModelsDemo( );

        bool LoadContent( );
        void UnloadContent( );

		void TurnZBufferOn();
		void TurnZBufferOff();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		bool DrawString( char* message, float startX, float startY );
        void Update( float dt );
        void Render( );

		void SetGameState(GameStates state);
		GameStates GetGameState();

    private:
		Collision collision;
        ID3D11VertexShader* textureMapVS_;
        ID3D11PixelShader* textureMapPS_;
		ID3D11VertexShader* textTextureMapVS_;
        ID3D11PixelShader* textTextureMapPS_;

        ID3D11InputLayout* inputLayout_;
		ID3D11InputLayout* textInputLayout_;
        ID3D11Buffer* vertexBuffer1_;
		ID3D11Buffer* vertexBuffer2_;
		ID3D11Buffer* textVertexBuffer_;
		ID3D11Buffer* vertexBufferTerrain_;
		//================= MODIFIED STUFF ================
        int totalVerts1_;
		int totalVerts2_;
		float m_x2 = 6.0f;
		float m_y2 = 0.0f;
		float m_z2 = -10.0f;
		float m_x1 = 16.0f;
		float m_y1 = 0.0f;
		float m_z1 = -10.0f;

		float Tank3Speed;

        ID3D11ShaderResourceView* colorMap1_;
		ID3D11ShaderResourceView* colorMap2_;

		ID3D11ShaderResourceView* terrainColorMap_;
		ID3D11ShaderResourceView* textColorMap_;
        ID3D11SamplerState* colorMapSampler_;
		//ID3D11SamplerState* terrainColorMapSampler_;

		ID3D11SamplerState* textColorMapSampler_;

        ID3D11Buffer* viewCB_;
        ID3D11Buffer* projCB_;
        ID3D11Buffer* worldCB_;
        ID3D11Buffer* camPosCB_;
        XMMATRIX projMatrix_;

        //ArcCamera camera_;
		FirstPersonCamera camera_;

        XINPUT_STATE controller1State_;
        XINPUT_STATE prevController1State_;

		GameStates gameState_;
		PauseMenuSelection pauseMenuSelection;
		bool displayFPS;

		Mesh model1Mesh;
		Texture model1Texture;

		Mesh model2Mesh;
		Texture model2Texture;

		GameObject models[3];
		
		// ---------- Players ----------
		Mesh Player1Mesh;
		Texture Player1Texture;
		Player Player1;

		Mesh Player2Mesh;
		Texture Player2Texture;
		Player Player2;

		// ---------- CHARACTERS ----------
		Mesh * KremitWalk[8];
		Texture KremitTexture;

		//////////////time globals///////////////////
public:
		//double countsPerSecond_ = 0.0;
		//__int64 CounterStart_ = 0;

		//int frameCount_ = 0;
		int fps_;

		//__int64 frameTimeOld_ = 0;
		double frameTime_;
};

#endif