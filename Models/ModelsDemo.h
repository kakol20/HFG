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
#include "Wolf.h"
#include "Texture.h"
#include "Animation.h"
#include "Collision.h"
#include "GameObject.h"
#include "PlayerChar.h"

enum GameStates {PLAY_INTRO, START_MENU, RUN, PAUSED, INTRO_MOVIE_REPLAY,SELECTION,POST_MATCH,THANKS};
//enum Animations { IDLE, WALK, ATTACK, DEATH, DAMAGED };
enum PauseMenuSelection {RETURN, FPS, PLAY_MOVIE, QUIT, CHA_SELEC};
//this Enum has been declared in PlayerChar.h

enum MainMenu { PLAY, OPTIONS, CREDITS, EXIT};
enum Animations { IDLE, WALK, ATTACK, HITTED,DIE };



#define PAUSE_MENU_ITEMS 4
#define MAIN_MENU_ITEMS 4

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
//=========== MAIN MENU ==========
inline MainMenu operator++(MainMenu &eDOW, int)
{
	const MainMenu ePrev = eDOW;
	const int i = static_cast<int>(eDOW);
	eDOW = static_cast<MainMenu>((i + 1) % MAIN_MENU_ITEMS);
	return ePrev;
}
inline MainMenu operator--(MainMenu &eDOW, int)
{
	const MainMenu ePrev = eDOW;
	const int i = static_cast<int>(eDOW);
	if (i > 0)
	{
		eDOW = static_cast<MainMenu>(i - 1);
	}
	else
	{
		eDOW = static_cast<MainMenu>(MAIN_MENU_ITEMS - 1);
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
		float m_z2 = -10.0f;
		float m_x1 = 16.0f;
		float m_y1 = 0.0f;
		float m_z1 = -10.0f;

		// QUAD
		ID3D11Buffer * vertexBufferQuad_;
		//ID3D11ShaderResourceView* quadColorMap_;
		ID3D11ShaderResourceView* soilColorMap_;

		float Tank3Speed;
		float wait;
		float attack_time1;
		float attack_time2;
		bool attack = false;
		bool movement = false;
		bool Imputs = false;

		// Player 1 animations switch
		int AnimatioState = 0;
		int PrevAnimState = 0;
		// Player 2 animations switch
		int AnimatioState2 = 0;
		int PrevAnimState2 = 0;

        ID3D11ShaderResourceView* colorMap1_;
		ID3D11ShaderResourceView* colorMap2_;

		ID3D11SamplerState* colorMapSampler_;
		ID3D11ShaderResourceView* textColorMap_;
		ID3D11ShaderResourceView* terrainColorMap_;
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
		Characters P1charSelection;
		Characters P2charSelection;
		MainMenu MMSelection;
		bool displayFPS;

		// ---------- Game Objects ----------
		Mesh SkyBoxMesh;
		Texture SkyBoxTexture;
		GameObject SkyBox;

		Mesh DomeMesh;
		Texture DomeTexture;
		GameObject DomeObj;

		Mesh DoorMesh;
		Texture DoorTexture;
		GameObject Door;

		Mesh CabinetMesh;
		Texture CabinetTexture;
		GameObject Cabinet;

		Mesh ContainerMesh;
		Texture ContainerTexture;
		GameObject Container;

		Mesh CanisterMesh;
		Texture CanisterTexture;
		GameObject Canister;

		Mesh HangarMesh;
		Texture HangarTexture;
		GameObject Hangar;
		
		// ---------- Players ----------
		Player Player1;
		Player Player2;
		//========== Here to create every mesh & texture for the characters

		Mesh Player1Mesh;
		Texture Player1Texture;

		Mesh Player2Mesh;
		Texture Player2Texture;

		Mesh Zombie_M;
		Texture Zombie_T;
		Animation ZombieWalk;
		Animation ZombieIdle;
		Animation ZombieDeath;
		Animation ZombieAttack;
		Animation ZombieDamaged;


		Mesh Alien_M;
		Texture Alien_T;

		Animation AlienWalk;
		Animation AlienIdle;
		Animation AlienDeath;
		Animation AlienAttack;
		Animation AlienDamaged;

		Mesh Skinny_M;
		Texture Skinny_T;

		Animation SkinnyWalk;
		Animation SkinnyIdle;
		Animation SkinnyDeath;
		Animation SkinnyAttack;
		Animation SkinnyDamaged;

		Mesh Pravez_M;
		Texture Pravez_T;
		// ???

		Mesh Kremit_M;
		Texture Kremit_T;

		Animation KremitWalk;
		Animation KremitIdle;
		Animation KremitDeath;
		Animation KremitAttack;
		Animation KremitDamaged;

		Mesh Wolf_M;
		Texture Wolf_T;

		Animation WolfWalk;
		Animation WolfIdle;
		Animation WolfDeath;
		Animation WolfAttack;
		Animation WolfDamaged;

		Mesh Robot_M;
		Texture Robot_T;

		Animation RobotWalk;
		Animation RobotIdle;
		Animation RobotDeath;
		Animation RobotAttack;
		Animation RobotDamaged;



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