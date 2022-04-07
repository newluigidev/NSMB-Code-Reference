#pragma once
#include "nsmb.h" 

//vtable at 0215246C (ov42)
class BobOmb : public StageEntity {

public:
	
	using StateFunction = bool(BobOmb::*)();
	
	//D0: 0214BD4C
	//D1: 0214BCE4
	virtual ~BobOmb() {} 
	
	// 0214CF98
	s32 onCreate() override;
	// 0214C628
	s32 onDestroy() override;
	// 0214C750
	s32 onRender() override;
	// 0214C630
	void pendingDestroy() override;
	// 0214E23C
	bool onPrepareResources() override;
	
	// 0214C9E4
	bool mainUpdate() override;
	// 0214D964
	void updateCarried() override;
	// 0214D904
	void updateDropped() override;
	
	// 0214C610
	void updateAnimation() override;
	// 0214DA74
	void thrown() override;
	// 0214DA58
	void stopped() override;
	// 0214DAE0
	void grabbed() override;
	// 0214DA98
	void release() override;
	// 0214C634
	void onBlockHit() override;
	// 0214C5A4
	void onFireballHit() override;
	// 0214C570
	void onGroundPound() override;
	// 0214CAA0
	void entityCollision(ActiveCollider& self, StageActor& actor) override;
	// 0214CBF4
	void damagePlayer(ActiveCollider& self, Player& player) override;
	// 0214D194
	void defeat(fx32 velX, fx32 velY, fx32 accelY, u8 unk) override;
	
	// 0214DED0
	bool getStunnedTimer() const;
	// 0214D8C0
	void setDirectionalValues() const;
	// 0214C110
	void kickedOrThrown() const; //called when the bobomb is either in virtual state 32 (thrown) or state 4 (kickedState)
	// 0214BDF0
	void releaseFromPlayerAndExplode() const; /* called when the bobomb needs to explode, 
	so its released from the players hands so that they're not stuck in the carrying state and then switches to explodeState */
	
	// 0214E148
	bool switchState(StateFunction function);
	// 0214E10C
	void updateState();
	
	// 0214E010
	bool turnAroundState(); //when colliding with wall during walk cycle, bobomb changes direction 
	// 0214DED4
	bool walkAfterTurnAroundState(); //switched to by turnAroundState(), the bobomb continues its walk cycle once it has turned around 
	// 0214DD5C
	bool stunnedState();
	// 0214DB30
	bool stopSlidingState(); //called at the end of the slide when groundpounded
	// 0214D73C
	bool kickedState();
	// 0214D418
	bool explodeState();
	// 0214D33C
	bool montyTankState(); //spawns the bobomb as stunned; used in Monty Tank battle
	// 0214D1D8
	bool mainState();
	
	// 0214C3C0
	static void explosionCallback(ActiveCollider& self, ActiveCollider& other);
	
	// 0214E294
	static bool loadResources();

	static constexpr u16 objectID = 35;

	static constexpr u16 updatePriority = objectID;
	static constexpr u16 renderPriority = 48;
	
	// 02152458
	static const ActorProfile profile;
	
	// 02151A28
	static const ActiveColliderInfo activeColliderInfo;
	
	// 02151A44
	static const fx32 explosionBounds[9]; //also used by explodeState
	// 021519D4
	static const fx16 directionBasedXVels1[2]; //used by turnAroundState
	// 021519CC
	static const fx16 directionBasedXVels2[2]; //used by setRotationAndVelocityBasedOnDirection
	// 021519E0
	static const fx16 directionBasedYRots[2]; //used by walkAfterTurnAroundState
	// 02151A08
	static const fx16 directionBasedXVels3[4]; //used by walkingState
	// 02151A18
	static const fx16 directionBasedYVels[4]; //also used by walkingState
	// 021519D4
	static const u8 tileRelatedIndicies[4]; //used by explodeState
	
	// 021519E4
	static PointSensor topSensor;
	// 021519F0
	static PointSensor bottomSensor;
	// 021519FC
	static PointSensor sideSensor;
	
	
	ModelAnm model;
	
	StateFunction updateFunction;
	
	StateFunction prevUpdateFunction;
	
	u32 unusedObjectID; //used as an ID by onRender
	
	u16 explodeTimer; //counts up to 16 when the BobOmb explodes, used to determine when to delete the BobOmb 
	
	u16 stunnedTimer; //starts at 0xF0, counts down - when it reaches 0x3C the red blinking occurs until it is 0
	
	u8 linkedPlayerID; 
	
	bool explodeFlag;
	
	s8 updateStep;
	
	bool unusedTypeFlag; 
	
	bool isStunnedFromStomp; //used in stunnedState
	
	bool isSelfDestruct; //set to 1 when spawned with the "Self-destruct" behaviors
	
	u8 collisionlessTimer; /*If the spawn behavior is set to "Self-destructs (240 frames)", it is set to 5, and in stunnedState, is subtracted until it equals 0 -
	code shows that if equal to 0 then certain collision behaviors are configured*/
};