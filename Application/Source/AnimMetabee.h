#ifndef ANIM_METABEE_H
#define ANIM_METABEE_H

#include "IAnimation.h"
#include <vector>

using std::vector;


// Responsible for playing Metabee's animation
class AnimMetabee : public IAnimation {


public:

	// Mtb Animation State Enums
	enum MTB_ANIM_STATE {

		ANIM_RESETTING = 0,
		ANIM_NONE,

		ANIM_RUN_1,
		ANIM_RUN_2,

		ANIM_SHOOT,

		ANIM_RAISE_SWORD,
		ANIM_SLASH,

		ANIM_SQUAT,
		ANIM_JUMP,
		ANIM_JUMPSLASH,

		ANIM_RAISE_CANNONS,
		ANIM_CHARGE_CANNONS,
		ANIM_FIRE_CANNONS,

		ANIM_SIZE


	};

	// Mtb Scaling Enums
	enum MTB_TRANSFORM_SCALE {

		SCALE_ENERGYBALL,
		SCALE_CHARGEDPARTICLE,
		SCALE_SIZE

	};

	// Mtb Translation Enums
	enum MTB_TRANSFORM_TRANSLATE {

		TRS_LeftArmBulletY = 0,

		TRS_EnergyBallX,
		TRS_EnergyBallY,
		TRS_EnergyBallZ,

		TRS_Charged_ParticleX,
		TRS_Charged_ParticleY,
		TRS_Charged_ParticleZ,

		TRS_FIRST_ChestX,
		TRS_FIRST_ChestY,
		TRS_FIRST_ChestZ,

		TRS_ChestX,
		TRS_ChestY,
		TRS_ChestZ,

		TRANSLATE_SIZE

	};

	// Mtb First Rotation Enums
	enum MTB_TRANSFORM_F_ROT {

		ROT_FIRST_ChestX,
		ROT_FIRST_ChestY,
		ROT_FIRST_ChestZ,
		ROT_FIRST_SIZE

	};

	// Mtb Rotation Enums
	enum MTB_TRANSFORM_ROT {

		ROT_SwordX = 0,
		ROT_SwordY,
		ROT_SwordZ,

		ROT_HeadX,
		ROT_HeadY,
		ROT_HeadZ,

		ROT_ChestX,
		ROT_ChestY,
		ROT_ChestZ,

		ROT_AbdomenX,
		ROT_AbdomenY,
		ROT_AbdomenZ,

		ROT_LeftShoulderX,
		ROT_LeftShoulderY,
		ROT_LeftShoulderZ,

		ROT_LeftArmX,
		ROT_LeftArmY,
		ROT_LeftArmZ,

		ROT_RightShoulderX,
		ROT_RightShoulderY,
		ROT_RightShoulderZ,

		ROT_RightArmX,
		ROT_RightArmY,
		ROT_RightArmZ,

		ROT_LeftThighX,
		ROT_LeftThighY,
		ROT_LeftThighZ,

		ROT_LeftFootX,
		ROT_LeftFootY,
		ROT_LeftFootZ,

		ROT_RightThighX,
		ROT_RightThighY,
		ROT_RightThighZ,

		ROT_RightFootX,
		ROT_RightFootY,
		ROT_RightFootZ,

		ROT_SIZE

	};

	AnimMetabee(MS* mStack);
	~AnimMetabee();

	void popHead();
	void popChest();
	void popAbdomen();

	void popLeftShoulder();
	void popLeftArm();

	void popRightShoulder();
	void popRightArm();

	void popLeftThigh();
	void popLeftFoot();

	void popRightThigh();
	void popRightFoot();

	void popSword();

	void popLeftBullet();
	void popEnergyBall();
	void popChargedParticle();

	virtual void ProcessAnimation(float dt);
	virtual void ProcessMovement(float dt);

	// Reset all rotations to 0 with the given Speed and returns true when done
	bool resetAnimation(float spd, float dt); 
	
	void setAnimState(const MTB_ANIM_STATE & state) {
		currentAnimState_ = state;
	}

	const MTB_ANIM_STATE & getAnimState() {
		return currentAnimState_;
	}

	const Vector3& getPosition() {
		return mtbPosition;
	}

	const bool & isSwordOut() {
		return isSwordOut_;
	}

	const bool & isChargedParticleOut() {
		return isChargedParticleOut_;
	}

	const bool & isAnimationPlaying() {
		return isAnimationPlaying_;
	}

	const bool & isMovable() {
		return isMovable_;
	}

	void pauseAnimation() {
		isAnimationPlaying_ = true;
	}

	void restartAnimation() {
		isAnimationPlaying_ = false;
	}

	vector<float> transform_rotate_list;

private:
	MS* mStack_;

	MTB_ANIM_STATE currentAnimState_ = ANIM_NONE;

	const float rotSpeedFastestestest_ = 800.0f;
	const float rotSpeedFastestest_ = 400.0f;
	const float rotSpeedFastest_ = 200.0f;
	const float rotSpeedFaster_ = 150.0f;
	const float rotSpeedFast_ = 100.0f;
	const float rotSpeedNormal_ = 75.0f;
	const float rotSpeedSlow_ = 50.0f;
	const float rotSpeedSlower_ = 25.0f;
	const float rotSpeedSlowest_ = 10.0f;

	const float trsSpeedInsanelyFaster_ = 500.0f;
	const float trsSpeedInsanelyFastbutNotFaster_ = 250.0f;
	const float trsSpeedInsanelyFast_ = 100.0f;
	const float trsSpeedFastestest_ = 20.0f;
	const float trsSpeedFastest_ = 10.0f;
	const float trsSpeedFaster_ = 7.5f;
	const float trsSpeedNormal_ = 6.0f;
	const float trsSpeedSlow_ = 5.0f;
	const float trsSpeedSlower_ = 2.5f;
	const float trsSpeedSlowest_ = 0.75f;
	const float trsSpeedSlowestest_ = 0.5f;

	bool isSwordOut_ = true;
	bool isAnimationPlaying_ = false;
	bool isMovable_ = true;
	bool isChargedParticleOut_ = false;

	MS msLeftArmBullet;
	MS msSword;
	MS msHead;
	MS msChest;
	MS msAbdomen;
	MS msLeftShoulder;
	MS msLeftArm;
	MS msRightShoulder;
	MS msRightArm;
	MS msLeftThigh;
	MS msLeftFoot;
	MS msRightThigh;
	MS msRightFood;

	// Animation helper variables
	float trsJumpSlashDistance = 0;
	float mtbSlashCount_ = 0;
	float mtbShootCount_ = 0;
	float mtbBallChargeCount_ = 0;

	const float mtbSlashDistance = 30;
	const float mtbMoveSpeed = 15.0f;
	const float mtbTurnSpeed = 150.0f;
	const float mtbSprintSpeed = 2.0f;

	float currentRotation = 0;
	float deltaTime = 0;

	vector<float> transform_first_rotate_list;
	vector<float> transform_translate_list;
	vector<float> transform_scale_list;

	Vector3 mtbPosition;

};
#endif