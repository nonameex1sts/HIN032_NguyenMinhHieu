#pragma once
#include "../Utilities/utilities.h"
#include "Math.h"
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "Object.h"

#define PLAY			0
#define EXIT			1
#define HELP			2
#define CREDIT			3
#define BGM				4
#define NOBGM				5
#define BACK			6
#define PREV_PAGE		7
#define NEXT_PAGE		8
#define PAUSE			9
#define RESUME			10
#define RESET			11
#define LEVEL_SELECT	12
#define MENU			13
#define IMAGE			14

class Button : public Object {
private:
	//Model* pModel;
	//Texture* pTexture;
	//Camera* pCamera;
	//Shaders* pShader;
	//Matrix mt_world;
	int iType;
	bool isActive;

public:
	Button();
	Button(Model* model, Texture* texture, Camera* camera, Shaders* shader, Vector3 position, Vector3 rotation, Vector3 scale, int iType, int isActive);
	void Update();
	void Key();
	void MouseClick(int x, int y);
	void MouseMove();
	void Move();
	void Rotate();
	bool getActive();
	~Button();
};