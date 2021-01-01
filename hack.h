#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}
#include <stdint.h>
#define ABS(x) ((x < 0) ? (-x) : (x))

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

//class Ent {
//public:
//	union {
//		// isDormant
//		DEFINE_MEMBER_N(bool, isDormant, 0xED);
//		// iHealth
//		DEFINE_MEMBER_N(int, iHealth, 0x100);
//		// vecOrigin
//		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);
//		// iTeamNum
//		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
//	};
//};

//class EntListObj {
//public:
//	Ent* ent;
//	char padding[12];
//};
//
//class EntList {
//public:
//	EntListObj ents[32];
//};







//------------------------------------------------------------------------------------------------------------

class EntListObj
{
public:
	class Ent* ent; //0x0000
	char pad_0004[12]; //0x0004
}; //Size: 0x0010

class EntList
{
public:
	class EntListObj ents[32]; //0x0000
}; //Size: 0x0200

class Ent
{
public:
	char pad_0000[84]; //0x0000
	int32_t clientId; //0x0054
	char pad_0058[60]; //0x0058
	int32_t health; //0x0094
	char pad_0098[4]; //0x0098
	int32_t team; //0x009C
	char pad_00A0[216]; //0x00A0
	int32_t state; //0x0178
	char pad_017C[2]; //0x017C
	int8_t bDormant; //0x017E
	char pad_017F[225]; //0x017F
	Vec3 pos; //0x0260
	Vec3 angle; //0x026C
}; //Size: 0x0278

class Hack {
public:
	uintptr_t dwEntityList = 0x4D3904; // dwBaseEntity = 0x4D3904; // client.dll 2020��10��26�� 01:14:51
	//uintptr_t dwViewMatrix = 0x5AD688;// 3x4MV
	uintptr_t dwViewMatrix = 0x5AD648;// 4x4 MV
	//uintptr_t dwViewMatrix = 0x5A78EC; //2020年11月4日 09:07:38

	uintptr_t engine;
	uintptr_t client;

	uintptr_t localEntAddr =0x4C6708;// 自己的偏移
	Ent* localEnt;

	EntList* entList;
	 float	matrix[16];



	 ID3DXLine* LineL;

	 Vec2 crosshair2D;
	 int crosshairSize = 4;

	 void Init();
	 void Update();
	 bool CheckValidEnt(Ent* ent);
	 bool WorldToScreen(Vec3 pos, Vec2& screen, int ScreenWidth, int ScreenHeight);
	 Vec3 GetBonePos(Ent* ent, int bone);
};


