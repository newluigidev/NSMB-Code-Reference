#pragma once
#include "nsmb.h" 

//vtable at 02152474 (ov42)
class BobOmb : public StageEntity {

public:
	
	using StateFunction = bool(BobOmb::*)();
	
	//D0: 0214BD54
	//D1: 0214BCEC
	virtual ~BobOmb() {} 
	
	// 0214CFA0
	s32 onCreate() override;
	// 0214C630
	s32 onDestroy() override;
	// 0214C758
	s32 onRender() override;
	// 0214C638
	void pendingDestroy() override;
	// 0214E244
	bool onPrepareResources() override;
	
	// 0214C9EC
	bool mainUpdate() override;
	// 0214D96C
	void updateGrabbed() override;
	// 0214D90C
	void updateState7() override;
	
	// 0214C618
	void updateAnimation() override;
	// 0214DA7C
	void thrown() override;
	// 0214DA60
	void stopped() override;
	// 0214DAE8
	void grabbed() override;
	// 0214DAA0
	void release() override;
	// 0214C63C
	void onBlockHit() override;
	// 0214C5AC
	void onFireballHit() override;
	// 0214C578
	void onGroundPound() override;
	// 0214CAA8
	void entityCollision(ActiveCollider& self, StageActor& actor) override;
	// 0214CBFC
	void damagePlayer(ActiveCollider& self, Player& player) override;
	// 0214D19C
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	
	// 0214DED8
	bool getStunnedTimer() const;
	// 0214D8C8
	void setRotationAndVelocityBasedOnDirection() const;
	// 0214C118
	void kickedOrThrown() const; //called when the bobomb is either in virtual state 32 (thrown) or state 4 (kickedState)
	// 0214BDF8
	void releaseFromPlayerAndExplode() const; /* called when the bobomb needs to explode, 
	so its released from the players hands so that they're not stuck in the carrying state and then switches to explodeState */
	
	// 0214E150
	bool switchState(StateFunction function);
	// 0214E114
	void updateState();
	
	// 0214E018
	bool turnAroundState(); //when colliding with wall during walk cycle, bobomb changes direction 
	// 0214DEFC
	bool walkAfterTurnAroundState(); //switched to by turnAroundState(), the bobomb continues its walk cycle once it has turned around 
	// 0214DD64
	bool stunnedState();
	// 0214DB38
	bool stopSlidingState(); //called at the end of the slide when groundpounded
	// 0214D744
	bool kickedState();
	// 0214D420
	bool explodeState();
	// 0214D344
	bool montyTankState(); //spawns the bobomb as stunned; used in Monty Tank battle
	// 0214D1E0
	bool mainState();
	
	// 0214C3C8
	static void explosionCallback(ActiveCollider& self, ActiveCollider& other);
	
	// 0214E29C
	static bool loadResources();

	static constexpr u16 objectID = 35;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 48;
	
	// 02152460
	static const ActorProfile profile;
	
	// 02151A30
	static const ActiveColliderInfo activeColliderInfo;
	
	// 02151A4C
	static const fx32 explosionBounds[9]; //also used by explodeState
	// 021519E0
	static const fx16 directionBasedXVelocityValues1[2]; //used by turnAroundState
	// 021519E4
	static const fx16 directionBasedXVelocityValues2[2]; //used by setRotationAndVelocityBasedOnDirection
	// 021519E8
	static const fx16 directionBasedYRotationValues[2]; //used by walkAfterTurnAroundState
	// 02151A10
	static const fx16 directionBasedXVelocityValues3[4]; //used by walkingState
	// 02151A20
	static const fx16 directionBasedYVelocityValues[4]; //also used by walkingState
	// 021519DC
	static const u8 tileRelatedIndicies[4]; //used by explodeState
	
	// 021519EC
	static PointSensor topSensor;
	// 021519F8
	static PointSensor bottomSensor;
	// 02151A04
	static PointSensor sideSensor;
	
	
	ModelAnm model;
	
	StateFunction updateFunction;
	
	u32 unk4A4; //is set to the same value as 0x49C in switchState
		
	u32 unk4A8; //is set to the same value as 0x4A0 in switchState
	
	u32 unk4AC; //used as an ID by onRender
	
	u16 explodeTimer; //counts up to 16 when the BobOmb explodes, used to determine when to delete the BobOmb 
	
	u16 stunnedTimer; //starts at 0xF0, counts down - when it reaches 0x3C the red blinking occurs until it is 0
	
	u8 linkedPlayerID; 
	
	bool explodeFlag;
	
	s8 updateStep;
	
	u8 unk4B7; //always set to 0, not sure of its purpose - montyTankState, virt61 and onRender check if its equal to 0 
	
	u8 unk4B8; //used in stunnedState, where it is compared against 0 & is set to 1 (has other behaviors when printed in game) - unsure of its purpose
	
	bool isSelfDestruct; //set to 1 when spawned with the "Self-destruct" behaviors
	
	u8 unk4BA; /*If the spawn behavior is set to "Self-destructs (240 frames)", it is set to 5, and in stunnedState, is subtracted until it equals 0 -
	code shows that if equal to 0 then certain collision behaviors are configured*/
	
	u8 unk4BB; //completely unused
};