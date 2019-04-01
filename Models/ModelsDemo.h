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
#include"Wolf.h"
#include "Collision.h"

enum GameStates {PLAY_INTRO, START_MENU, RUN, PAUSED, INTRO_MOVIE_REPLAY,SELECTION};

enum PauseMenuSelection {RETURN, FPS, PLAY_MOVIE, QUIT, CHA_SELEC};
//this Enum has been declared in PlayerChar.h



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
//=========== PROPOSAL ==========







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
		ID3D11PixelShader* textureMapPS_;
		ID3D11VertexShader* textureMapVS_;
		ID3D11PixelShader* textTextureMapPS_;
		ID3D11VertexShader* textTextureMapVS_;

		ID3D11Buffer* vertexBuffer1_;
		ID3D11Buffer* vertexBuffer2_;
		ID3D11InputLayout* inputLayout_;
		ID3D11Buffer* textVertexBuffer_;
		ID3D11Buffer* vertexBufferTerrain_;
		ID3D11InputLayout* textInputLayout_;
		//================= MODIFIED STUFF ================
		int totalVerts1_;
		int totalVerts2_;
		float m_x2 = 6.0f;
		float m_y2 = 0.0f;
		float m_y1 = 0.0f;
		float m_x1 = 16.0f;
		float m_z2 = -10.0f;
		float m_z1 = -10.0f;

		float wait;
		float Tank3Speed;
		float attack_time1;
		float attack_time2;

        ID3D11ShaderResourceView* colorMap1_;
		ID3D11ShaderResourceView* colorMap2_;

		ID3D11SamplerState* colorMapSampler_;
		ID3D11ShaderResourceView* textColorMap_;
		ID3D11ShaderResourceView* terrainColorMap_;
		//ID3D11SamplerState* terrainColorMapSampler_;

		ID3D11SamplerState* textColorMapSampler_;

		XMMATRIX projMatrix_;
		ID3D11Buffer* viewCB_;
		ID3D11Buffer* projCB_;
		ID3D11Buffer* worldCB_;
		ID3D11Buffer* camPosCB_;

        //ArcCamera camera_;
		FirstPersonCamera camera_;

        XINPUT_STATE controller1State_;
        XINPUT_STATE prevController1State_;

		bool displayFPS;
		GameStates gameState_;
		Characters P1charSelection;
		Characters P2charSelection;
		PauseMenuSelection pauseMenuSelection;

		// ---------- Game Objects ----------
		Mesh SkyBoxMesh;
		Texture SkyBoxTexture;
		GameObject SkyBox;

		Mesh DomeMesh;
		Texture DomeTexture;
		GameObject DomeObj;
		
		// ---------- Players ----------
		Player Player1;
		Player Player2;
		//========== Here to create every mesh & texture for the characters

		Mesh Player1Mesh;
		Texture Player1Texture;

		Mesh Player2Mesh;
		Texture Player2Texture;

		

		// ---------- CHARACTERS ----------
		Mesh Kremit_M;
		Texture Kremit_T;
		Mesh * KremitWalk[8];
		Mesh * KremitIdle[8];
		Mesh * KremitDeath[8];
		Mesh * KremitAttack[8];
		Mesh * KremitAttacked[8];

		Mesh Wolf_M;
		Texture Wolf_T;
		Mesh * WolfWalk[8];
		Mesh * WolfIdle[8];
		Mesh * WolfDeath[8];
		Mesh * WolfAttack[8];
		Mesh * WolfAttacked[8];

		Mesh Robot_M;
		Texture Robot_T;
		Mesh * RobotWalk[8];
		Mesh * RobotIdle[8];
		Mesh * RobotDeath[8];
		Mesh * RobotAttack[8];
		Mesh * RobotAttacked[8];

		Mesh Zombie_M;
		Texture Zombie_T;
		Mesh * ZombieWalk[8];
		Mesh * ZombieIdle[8];
		Mesh * ZombieDeath[8];
		Mesh * ZombieAttack[8];
		Mesh * ZombieAttacked[8];

		Mesh Alien_M;
		Texture Alien_T;
		Mesh * AlienWalk[8];
		Mesh * AlienIdle[8];
		Mesh * AlienDeath[8];
		Mesh * AlienAttack[8];
		Mesh * AlienAttacked[8];

		Mesh Skinny_M;
		Texture Skinny_T;
		Mesh * SkinnyWalk[8];
		Mesh * SkinnyIdle[8];
		Mesh * SkinnyDeath[8];
		Mesh * SkinnyAttack[8];
		Mesh * SkinnyAttacked[8];

		Mesh Pravez_M;
		Texture Pravez_T;
		// ???????

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