#pragma once
#include "nsmb.h" 

//vtable at 0215246C (ov42)
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
	// 0214E234
	bool onPrepareResources() override;
	
	// 0214C9EC
	bool mainUpdate() override;
	// 0214D96C
	void updateCarried() override;
	// 0214D90C
	void updateDropped() override;
	
	// 0214C618
	void updateAnimation() override;
	// 0214DA7C
	void thrown() override;
	// 0214DA64
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
	void setDirectionalValues() const;
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
	bool turnState(); //when colliding with wall during walk cycle, bobomb changes direction 
	// 0214DEDC
	bool walkAfterTurnAroundState(); //switched to by turnAroundState(), the bobomb continues its walk cycle once it has turned around 
	// 0214DD64
	bool stunnedState();
	// 0214DB38
	bool stopSlideState(); //called at the end of the slide when groundpounded
	// 0214D744
	bool kickedState();
	// 0214D420
	bool explodeState();
	// 0214D344
	bool stunnedSpawnState(); //spawns the bobomb as stunned; used in Monty Tank battle
	// 0214D1E0
	bool mainState();
	
	// 0214C3C8
	static void activeCallback(ActiveCollider& self, ActiveCollider& other);
	
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
	// 021519DC
	static const fx16 directionBasedXVels1[2]; //used by turnAroundState
	// 021519D4
	static const fx16 directionBasedXVels2[2]; //used by setRotationAndVelocityBasedOnDirection
	// 021519E8
	static const fx16 directionBasedYRots[2]; //used by walkAfterTurnAroundState
	// 02151A10
	static const fx16 directionBasedXVels3[4]; //used by walkingState
	// 02151A20
	static const fx16 directionBasedYVels[4]; //also used by walkingState
	// 021519DC
	static const u8 spawnEffectType[4]; //used by explodeState
	
	// 021519EC
	static PointSensor topSensor;
	// 021519F8
	static PointSensor bottomSensor;
	// 02151A04
	static PointSensor sideSensor;
	
	
	ModelAnm model;
	
	StateFunction updateFunction;
	
	StateFunction prevUpdateFunction;
	
	u32 montyTankGUID;
	
	u16 explodeTimer; //counts up to 16 when the BobOmb explodes, used to determine when to delete the BobOmb 
	
	u16 stunnedTimer; //starts at 0xF0, counts down - when it reaches 0x3C the red blinking occurs until it is 0
	
	u8 playerID;
	
	bool explodeFlag;
	
	s8 updateStep;
	
	bool attachToMontyTank; 
	
	bool isStunnedFromStomp; //used in stunnedState
	
	bool isSelfDestruct; //set to 1 when spawned with the "Self-destruct" behaviors
	
	u8 collisionlessTimer; /*If the spawn behavior is set to "Self-destructs (240 frames)", it is set to 5, and in stunnedState, is subtracted until it equals 0 -
	code shows that if equal to 0 then certain collision behaviors are configured*/
};
NTR_SIZE_GUARD(BobOmb, 0x4BC);