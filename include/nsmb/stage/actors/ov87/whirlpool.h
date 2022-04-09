#pragma once
#include "nsmb.h"

//vtable at 0217F8A8 (ov87)

class Whirlpool : public StageEntity {

public:
	
	using StateFunction = bool(Whirlpool::*)();
	
	//D0: 0217F2A0
	//D1: 0217F220
	virtual ~Whirlpool() {}
	
	// 0217F0AC
	s32 onCreate() override;
	// 0217F018
	s32 onRender() override;
	// 0217F18C
	bool onPrepareResources() override;
	
	// 0217EFD4
	bool mainUpdate() override;
	
	// 0217EE2C
	void playSoundEffect() const;
	
	// 0217EEE0
	bool switchState(StateFunction function); 
	// 0217EE78
	void updateState();
	
	// 0217EBA4
	bool changeSizeState();
	// 0217EC48
	bool inactiveState();
	// 0217ED44
	bool activeState();
	
	// 0217F388
	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	
	// 0217F480
	bool loadResources() override;
	
	static constexpr u16 objectID = 226;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 189;
	
	// 0217F89C
	static const ActorProfile profile;
	
	// 0217F5D0
	static const fx32 cameraBounds[4];
	
	// 0217F5E0
	static const ActiveColliderInfo activeColliderInfo;
	
	ModelAnm model;
	
	AnimationCtrl texAnim;
	
	StateFunction updateFunction;
	
	StateFunction prevUpdateFunction;
	
	u32 unk4D8;
	
	u16 unk4DA;
	
	s16 changeSizeCounter;
	
	u16 unk4E0;
	
	u16 unk4E2;
	
	s8 updateStep;
	
	bool isInvisibleWhenInactive;
	
	u8 unk4E6;
	
	u8 unk4E7;
}
NTR_SIZE_GUARD(Whirlpool, 0x4E8);