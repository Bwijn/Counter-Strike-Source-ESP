#include "includes.h"



RECT screen_size;

void Hack::Init() {
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");

	entList = (EntList*)(client + dwEntityList);

	//Instead of LocalPlayer client.dll + 0x4C6708 2020��10��26�� 13:26:19
	//��ǰ��ҳ�ʼ��

	localEnt = entList->ents[0].ent;

}

void Hack::Update() {
	
	memcpy(&matrix, (PBYTE*)(engine + dwViewMatrix), sizeof(matrix));
}

bool Hack::CheckValidEnt(Ent* ent) {
	if (ent == nullptr)
		return false;
	if (ent == localEnt)
		return false;
	if (ent->health <= 0)
		return false;
	if (ent->bDormant)
		return false;
	return true;
}

// 
 bool Hack::WorldToScreen(Vec3 pos, Vec2& screen, int ScreenWidth, int ScreenHeight) {
	Vec4 clipCoords;




	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];



	if (clipCoords.w > 0.001)
	{
	
		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
		clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
		clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	
		Vec3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);



		return true;
	}

	return false;

}


 //Vec3 Hack::GetBonePos(Ent* ent, int bone) {
	// uintptr_t bonePtr = ent->boneMatrix;
	// Vec3 bonePos;
	// bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	// bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	// bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);
	// return bonePos;
 //}