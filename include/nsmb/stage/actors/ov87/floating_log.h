#pragma once
#include "nsmb.h"

//vtable at 0217F624 (ov87)

class FloatingLog : public StageEntity {

public: 
	
	//D0: 0217DB60
	//D1: 0217DBEC
	virtual ~FloatingLog() {}
	
	// 0217DF68
	s32 onCreate() override;
	// 0217DF0C
	s32 onDestroy() override;
	// 0217DE10
	s32 onRender() override;
	// 0217DF08
	void pendingDestroy() override;
	// 0217DF30
	bool onPrepareResources() override;
	
	// 0217DCA4
	bool mainUpdate() override;
	
	// 0217DC80
	static bool loadResources();

	static constexpr u16 objectID = 227;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 190;
	
	// 0217F600
	static const ActorProfile profile;
	
	// 0217F60C
	static const RotatingPlatformInfo rotatingPlatformInfo;
	
	Model model;
	
	RotatingPlatform platform;
	
	u16 rotationSpeed;
	
	u16 unk4F6;
	
	bool isPlayerOn;
	
	u8 unk4F9;
	
	u8 unk4FA;
	
	u8 unk4FB;
	
}
NTR_SIZE_GUARD(FloatingLog, 0x4FC);