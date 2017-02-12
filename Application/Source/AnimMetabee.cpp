#include "Application.h"
#include "AnimMetabee.h"



AnimMetabee::AnimMetabee(MS* mStack) : mStack_(mStack) {

	/*
	Populate all the vectors
	*/
	for (unsigned i = 0; i < TRANSLATE_SIZE; i++) {
		transform_translate_list.push_back(0);
	}
	for (unsigned i = 0; i < SCALE_SIZE; i++) {
		transform_scale_list.push_back(1); // Scale has to be 1
	}
	for (unsigned i = 0; i < ROT_SIZE; i++) {
		transform_rotate_list.push_back(0);
	}
	for (unsigned i = 0; i < ROT_FIRST_SIZE; i++) {
		transform_first_rotate_list.push_back(0);
	}
}

AnimMetabee::~AnimMetabee() {
}

bool AnimMetabee::resetAnimation(float spd, float dt) {

	isAnimationPlaying_ = true;

	// Slowly reset back to zero
	for (auto &rotation : transform_rotate_list) {

		if (rotation >= 1 || rotation <= -1) {
			rotation += (0 - rotation) * trsSpeedSlower_ * dt * spd;
		}
		else {
			rotation = 0;
		}
	}

	if (transform_first_rotate_list[ROT_FIRST_ChestX] >= 1 || transform_first_rotate_list[ROT_FIRST_ChestX] <= -1) {
		transform_first_rotate_list[ROT_FIRST_ChestX] += (0 - transform_first_rotate_list[ROT_FIRST_ChestX]) * trsSpeedSlower_ * dt * spd;
	}
	else {
		transform_first_rotate_list[ROT_FIRST_ChestX] = 0;
	}


	for (size_t i = 0; i < transform_rotate_list.size(); i++) {
		if (transform_rotate_list[i] != 0) {
			break;
		}
		else if (i == transform_rotate_list.size() - 1 && transform_rotate_list[i] == 0) { // All rotation is now zero
			transform_translate_list[TRS_ChestY] = 0;
			return true;
		}
	}


	return false;
}

void AnimMetabee::ProcessMovement(float dt) {

	currentRotation = Math::DegreeToRadian(transform_first_rotate_list[ROT_FIRST_ChestY]);
	deltaTime = dt;

	if (isMovable_) {

		if (Application::IsKeyPressed(0x57)) { // W key
			transform_translate_list[TRS_ChestZ] += mtbMoveSpeed * cos(currentRotation) * dt;
			transform_translate_list[TRS_ChestX] += mtbMoveSpeed * sin(currentRotation) * dt;

		}
		else if (Application::IsKeyPressed(0x53)) { // S key
			transform_translate_list[TRS_ChestZ] -= mtbMoveSpeed * cos(currentRotation) * dt;
			transform_translate_list[TRS_ChestX] -= mtbMoveSpeed * sin(currentRotation) * dt;

		}

		if (Application::IsKeyPressed(0x41)) { // A key
			transform_first_rotate_list[ROT_FIRST_ChestY] += mtbTurnSpeed * dt;
		}

		if (Application::IsKeyPressed(0x44)) { // D key
			transform_first_rotate_list[ROT_FIRST_ChestY] -= mtbTurnSpeed * dt;
		}

	}

	// Assign metabee's position
	mtbPosition.x = transform_translate_list[TRS_ChestX];
	mtbPosition.y = transform_translate_list[TRS_ChestY];
	mtbPosition.z = transform_translate_list[TRS_ChestZ];

}

void AnimMetabee::ProcessAnimation(float dt) {
	
	// Play animation if metabee is idle, commented for submission
	//if (!isAnimationPlaying_) {

	//	if (Application::IsKeyPressed('1')) {
	//		currentAnimState_ = ANIM_SHOOT;
	//	}
	//	if (Application::IsKeyPressed('2')) {
	//		currentAnimState_ = ANIM_RAISE_SWORD;
	//	}
	//	if (Application::IsKeyPressed('3')) {
	//		currentAnimState_ = ANIM_RAISE_CANNONS;
	//	}

	//}

	// Movement animation state
	if (isMovable_) {
		if (Application::IsKeyPressed(0x57) || Application::IsKeyPressed(0x53)) {
			if (currentAnimState_ != ANIM_RUN_1 && currentAnimState_ != ANIM_RUN_2) {
				currentAnimState_ = AnimMetabee::ANIM_RUN_1;

			}
		}
	}

	// If movement key is not pressed, reset animation
	if (!Application::IsKeyPressed(0x57) && !Application::IsKeyPressed(0x53) && (currentAnimState_ == ANIM_RUN_1 || currentAnimState_ == ANIM_RUN_2)) {
		currentAnimState_ = AnimMetabee::ANIM_RESETTING;
	}

	// Animation Logic
	switch (currentAnimState_) {

	case ANIM_RESETTING:

		if (resetAnimation(1, dt)) {
			currentAnimState_ = ANIM_NONE;
			isAnimationPlaying_ = false;
		}

		break;

	case ANIM_NONE:

		// No animation, reset everything to default
		isSwordOut_ = true;
		isMovable_ = true;
		isChargedParticleOut_ = false;
		isAnimationPlaying_ = false;

		break;

	case ANIM_RUN_1:

		// Sprinting increases animation speed
		if (Application::IsKeyPressed(VK_LSHIFT))
			dt *= mtbSprintSpeed;

		isAnimationPlaying_ = true;

		if (transform_rotate_list[ROT_LeftShoulderX] >= -45) {

			transform_rotate_list[ROT_LeftShoulderX] -= rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightShoulderX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightThighX] += rotSpeedFast_ * dt;

			transform_rotate_list[ROT_LeftThighX] -= rotSpeedFast_ * dt;

			if (transform_rotate_list[ROT_LeftArmX] > -45) {
				transform_rotate_list[ROT_LeftArmX] -= rotSpeedFastest_ * dt;
				transform_rotate_list[ROT_RightArmX] -= rotSpeedFastest_ * dt;
				transform_rotate_list[ROT_RightFootX] += rotSpeedFast_ * dt;
				transform_rotate_list[ROT_LeftFootX] += rotSpeedFast_ * dt;
			}
		}
		else {
			currentAnimState_ = ANIM_RUN_2;
		}

		break;

	case ANIM_RUN_2:

		// Sprinting increases animation speed
		if (Application::IsKeyPressed(VK_LSHIFT))
			dt *= mtbSprintSpeed;

		isAnimationPlaying_ = true;

		if (transform_rotate_list[ROT_LeftShoulderX] <= 45) {

			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightShoulderX] -= rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightThighX] -= rotSpeedFast_ * dt;

			transform_rotate_list[ROT_LeftThighX] += rotSpeedFast_ * dt;

		}
		else {
			currentAnimState_ = ANIM_RUN_1;
		}

		break;

	case ANIM_SHOOT:

		isAnimationPlaying_ = true;
		isSwordOut_ = false;
		isMovable_ = false;

		if (transform_rotate_list[ROT_LeftShoulderX] > -90) {
			transform_rotate_list[ROT_LeftShoulderX] -= rotSpeedFastest_ * dt;
			transform_rotate_list[ROT_RightShoulderX] -= rotSpeedFastest_ * dt;
		}
		else if (transform_rotate_list[ROT_RightShoulderY] < 40) {
			transform_rotate_list[ROT_RightShoulderY] += rotSpeedFastest_ * dt;
		}
		else if (transform_rotate_list[ROT_LeftArmY] < 90) {
			transform_rotate_list[ROT_LeftArmY] += rotSpeedFastest_ * dt;
		}
		else if (transform_rotate_list[ROT_HeadX] < 15) { // Tilt head and body
			transform_rotate_list[ROT_HeadX] += rotSpeedSlow_ * dt;

			transform_rotate_list[ROT_RightThighX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_LeftThighX] -= rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_LeftFootX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_AbdomenX] += rotSpeedSlow_ * dt;

			transform_translate_list[TRS_ChestY] -= trsSpeedSlowestest_ * dt;

		}
		else if (transform_translate_list[TRS_LeftArmBulletY] > -25) { // Shoot bullet
			transform_translate_list[TRS_LeftArmBulletY] -= trsSpeedInsanelyFastbutNotFaster_ * dt;
		}
		else if (mtbShootCount_ < 15) {
			mtbShootCount_++;
			transform_translate_list[TRS_LeftArmBulletY] = 0;

			// recoil backwards every 2 bullet shot
			if ((int)mtbShootCount_ % 2) {
				transform_translate_list[TRS_ChestZ] -= trsSpeedSlowestest_ * cos(currentRotation);
				transform_translate_list[TRS_ChestX] -= trsSpeedSlowestest_ * sin(currentRotation);
			}

		}
		else {
			// Done, reset animation
			transform_translate_list[TRS_LeftArmBulletY] = 0;
			mtbShootCount_ = 0;
			currentAnimState_ = ANIM_RESETTING;
		}

		break;

	case ANIM_RAISE_SWORD:

		isAnimationPlaying_ = true;
		isMovable_ = false;

		if (transform_rotate_list[ROT_LeftShoulderX] > -90) {

			transform_rotate_list[ROT_LeftShoulderX] -= rotSpeedFastest_ * dt;
			transform_rotate_list[ROT_LeftShoulderZ] -= rotSpeedFastest_ * dt;

			transform_rotate_list[ROT_RightShoulderX] -= rotSpeedFastest_ * dt;
			transform_rotate_list[ROT_RightShoulderY] += rotSpeedNormal_ * dt;

			transform_rotate_list[ROT_SwordX] += rotSpeedFastestest_ * dt;

		}
		else if (transform_rotate_list[ROT_LeftArmY] > -30) {
			transform_rotate_list[ROT_LeftArmX] -= rotSpeedSlow_ * dt;
			transform_rotate_list[ROT_LeftArmY] -= rotSpeedSlow_ * dt;
		}
		else if (transform_rotate_list[ROT_HeadX] < 15) {
			transform_rotate_list[ROT_HeadX] += rotSpeedSlow_ * dt;

			transform_rotate_list[ROT_RightThighX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_LeftThighX] -= rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_LeftFootX] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_AbdomenX] += rotSpeedSlower_ * dt;
			transform_rotate_list[ROT_FIRST_ChestX] += rotSpeedFast_ * dt;

			transform_translate_list[TRS_ChestY] -= trsSpeedSlowest_ * dt;
		}
		else {
			currentAnimState_ = ANIM_SLASH;
		}

		break;

	case ANIM_SLASH:
		if (transform_translate_list[TRS_FIRST_ChestZ] < mtbSlashDistance) {
			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedFastestestest_;
			transform_translate_list[TRS_FIRST_ChestZ] += trsSpeedInsanelyFaster_ * dt;
		}
		else if (mtbSlashCount_ < 7) {

			transform_rotate_list[ROT_ChestY] += 45.0f;

			transform_translate_list[TRS_FIRST_ChestZ] = -mtbSlashDistance;
			mtbSlashCount_++;

		}

		// Reset transforms after slashing 7 times
		if (mtbSlashCount_ == 7) {
			transform_translate_list[TRS_FIRST_ChestZ] = 0;
			transform_rotate_list[ROT_ChestY] = 0;
			transform_rotate_list[ROT_LeftShoulderX] = 0;

			if (resetAnimation(10.0f, dt)) {
				currentAnimState_ = ANIM_SQUAT;
				mtbSlashCount_ = 0;
			}

		}
		break;

	case ANIM_SQUAT:

		if (transform_rotate_list[ROT_LeftThighX] > -75) {

			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedSlow_ * dt;
			transform_rotate_list[ROT_RightShoulderX] += rotSpeedSlow_ * dt;

			transform_first_rotate_list[ROT_FIRST_ChestX] += rotSpeedSlow_ * dt;

			transform_rotate_list[ROT_LeftThighX] -= rotSpeedFast_ * dt;
			transform_rotate_list[ROT_LeftFootX] += rotSpeedFast_ * dt;

			transform_rotate_list[ROT_RightThighX] -= rotSpeedFast_ * dt;
			transform_rotate_list[ROT_RightFootX] += rotSpeedFast_ * dt;

			transform_translate_list[TRS_ChestY] -= dt * trsSpeedSlowestest_;

		}
		else {
			currentAnimState_ = ANIM_JUMP;
		}

		break;

	case ANIM_JUMP:

		if (transform_rotate_list[ROT_LeftShoulderX] > -140) {

			transform_rotate_list[ROT_LeftShoulderX] -= rotSpeedFastestest_ * dt;
			transform_rotate_list[ROT_RightShoulderX] -= rotSpeedFastestest_ * dt;

			transform_rotate_list[ROT_LeftShoulderY] += rotSpeedNormal_ * dt;
			transform_rotate_list[ROT_RightShoulderY] += rotSpeedNormal_ * dt;

			transform_rotate_list[ROT_LeftThighX] += rotSpeedFast_ * dt;
			transform_rotate_list[ROT_RightThighX] += rotSpeedFast_ * dt;

			transform_rotate_list[ROT_SwordZ] -= rotSpeedSlow_ * dt;

			transform_translate_list[TRS_ChestY] += dt * trsSpeedFastest_ * 4;
			trsJumpSlashDistance += dt * trsSpeedFastest_ * 4;

			transform_translate_list[TRS_ChestZ] += trsSpeedFastest_ * 3 * cos(currentRotation) * dt;
			transform_translate_list[TRS_ChestX] += trsSpeedFastest_ * 3 * sin(currentRotation) * dt;

		}
		else {
			trsJumpSlashDistance = transform_translate_list[TRS_ChestY] - (trsJumpSlashDistance);
			currentAnimState_ = ANIM_JUMPSLASH;
		}

		break;

	case ANIM_JUMPSLASH:

		if (transform_translate_list[TRS_ChestY] > trsJumpSlashDistance) {

			transform_translate_list[TRS_ChestY] -= trsSpeedFastest_ * 3 * dt;
			transform_translate_list[TRS_ChestZ] += trsSpeedFastest_ * 3 * cos(currentRotation) * dt;
			transform_translate_list[TRS_ChestX] += trsSpeedFastest_ * 3 * sin(currentRotation) * dt;

			transform_rotate_list[ROT_SwordX] += rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_SwordZ] += rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedFastestest_ * dt;
			transform_rotate_list[ROT_RightShoulderX] += rotSpeedFastestest_ * dt;

		}
		else if (resetAnimation(1, dt)) {
			trsJumpSlashDistance = 0;
			currentAnimState_ = ANIM_NONE;
		}

		break;

	case ANIM_RAISE_CANNONS:

		isAnimationPlaying_ = true;
		isSwordOut_ = false;
		isMovable_ = false;

		if (transform_rotate_list[ROT_LeftShoulderX] > -100) {
			transform_rotate_list[ROT_LeftShoulderX] -= rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_RightShoulderX] -= rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightShoulderZ] -= rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_LeftShoulderZ] -= rotSpeedFaster_ * dt;

			transform_rotate_list[ROT_RightArmY] -= rotSpeedFaster_ * dt;
			transform_rotate_list[ROT_LeftArmY] -= rotSpeedFaster_ * dt;

			// Tilt head and body
			if (transform_rotate_list[ROT_HeadX] < 15) {
				transform_rotate_list[ROT_HeadX] += rotSpeedSlow_ * dt;

				transform_rotate_list[ROT_RightThighX] += rotSpeedFaster_ * dt;

				transform_rotate_list[ROT_LeftThighX] -= rotSpeedFaster_ * dt;
				transform_rotate_list[ROT_LeftFootX] += rotSpeedFaster_ * dt;

				transform_rotate_list[ROT_AbdomenX] += rotSpeedSlower_ * dt;

				transform_translate_list[TRS_ChestY] -= trsSpeedSlowestest_ * dt;
			}

		}
		else {
			isChargedParticleOut_ = true;
			currentAnimState_ = ANIM_CHARGE_CANNONS;
		}

		break;

	case ANIM_CHARGE_CANNONS:
		if (transform_scale_list[SCALE_ENERGYBALL] < 10 && mtbBallChargeCount_ < 5) {

			if (transform_scale_list[SCALE_ENERGYBALL] > 9) {
				transform_scale_list[SCALE_ENERGYBALL] -= trsSpeedFastestest_ * dt;
			}
			else {
				transform_scale_list[SCALE_ENERGYBALL] += trsSpeedSlower_ * dt;
			}

			mtbBallChargeCount_ += dt; // To ensure breaking out of this statement

			if (transform_translate_list[TRS_Charged_ParticleY] < 3.5f) {

				transform_translate_list[TRS_Charged_ParticleY] += trsSpeedFaster_ * dt;

				transform_scale_list[SCALE_CHARGEDPARTICLE] -= (dt);

			}
			else {
				transform_scale_list[SCALE_CHARGEDPARTICLE] = 1;
				transform_translate_list[TRS_Charged_ParticleY] = 0;
			}

		}
		else {
			transform_translate_list[TRS_EnergyBallZ] += 3;
			transform_translate_list[TRS_ChestZ] -= 3 * cos(currentRotation);
			transform_translate_list[TRS_ChestX] -= 3 * sin(currentRotation);
			currentAnimState_ = ANIM_FIRE_CANNONS;
		}
		break;

	case ANIM_FIRE_CANNONS:

		transform_translate_list[TRS_EnergyBallZ] += trsSpeedFastestest_ * 2 * dt; // Move the ball
		transform_translate_list[TRS_Charged_ParticleY] = 4; // Hide the particle
		transform_scale_list[SCALE_CHARGEDPARTICLE] = 0.2f; // Scale down 

		if (transform_translate_list[TRS_EnergyBallZ] >= 20) {
			// Reset everything
			if (resetAnimation(1, dt)) {
				mtbBallChargeCount_ = 0;
				transform_translate_list[TRS_EnergyBallZ] = 0;
				transform_scale_list[SCALE_ENERGYBALL] = 1;
				isChargedParticleOut_ = false;
				currentAnimState_ = ANIM_NONE;
			}
		}
		else if (transform_rotate_list[ROT_LeftShoulderX] < -55) { // Fast Recoil effect
			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedFastestest_ * dt;
			transform_rotate_list[ROT_RightShoulderX] += rotSpeedFastestest_ * dt;
		}
		else if (transform_rotate_list[ROT_LeftShoulderX] < -35) { // Slow Recoil effect
			transform_rotate_list[ROT_LeftShoulderX] += rotSpeedNormal_ * dt;
			transform_rotate_list[ROT_RightShoulderX] += rotSpeedNormal_ * dt;
		}

		break;

	}
}

void AnimMetabee::popHead() {
	mStack_->Rotate(transform_rotate_list[ROT_HeadZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_HeadY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_HeadX], 1, 0, 0);
}

void AnimMetabee::popChest() {

	mStack_->Translate(transform_translate_list[TRS_ChestX], transform_translate_list[TRS_ChestY], transform_translate_list[TRS_ChestZ]);

	mStack_->Rotate(transform_rotate_list[ROT_ChestZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_ChestY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_ChestX], 1, 0, 0);

	mStack_->Translate(transform_translate_list[TRS_FIRST_ChestX], transform_translate_list[TRS_FIRST_ChestY], transform_translate_list[TRS_FIRST_ChestZ]);

	mStack_->Rotate(transform_first_rotate_list[ROT_FIRST_ChestZ], 0, 0, 1);
	mStack_->Rotate(transform_first_rotate_list[ROT_FIRST_ChestY], 0, 1, 0);
	mStack_->Rotate(transform_first_rotate_list[ROT_FIRST_ChestX], 1, 0, 0);
}

void AnimMetabee::popAbdomen() {
	mStack_->Rotate(transform_rotate_list[ROT_AbdomenZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_AbdomenY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_AbdomenX], 1, 0, 0);
}

void AnimMetabee::popLeftShoulder() {
	mStack_->Rotate(transform_rotate_list[ROT_LeftShoulderZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_LeftShoulderY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_LeftShoulderX], 1, 0, 0);
}

void AnimMetabee::popLeftArm() {
	mStack_->Rotate(transform_rotate_list[ROT_LeftArmZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_LeftArmY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_LeftArmX], 1, 0, 0);
}

void AnimMetabee::popRightShoulder() {
	mStack_->Rotate(transform_rotate_list[ROT_RightShoulderZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_RightShoulderY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_RightShoulderX], 1, 0, 0);
}
void AnimMetabee::popRightArm() {
	mStack_->Rotate(transform_rotate_list[ROT_RightArmZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_RightArmY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_RightArmX], 1, 0, 0);

}

void AnimMetabee::popLeftThigh() {
	mStack_->Rotate(transform_rotate_list[ROT_LeftThighZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_LeftThighY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_LeftThighX], 1, 0, 0);

}
void AnimMetabee::popLeftFoot() {
	mStack_->Rotate(transform_rotate_list[ROT_LeftFootZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_LeftFootY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_LeftFootX], 1, 0, 0);
}

void AnimMetabee::popRightThigh() {
	mStack_->Rotate(transform_rotate_list[ROT_RightThighZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_RightThighY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_RightThighX], 1, 0, 0);
}
void AnimMetabee::popRightFoot() {
	mStack_->Rotate(transform_rotate_list[ROT_RightFootZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_RightFootY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_RightFootX], 1, 0, 0);
}

void AnimMetabee::popSword() {
	mStack_->Rotate(transform_rotate_list[ROT_SwordZ], 0, 0, 1);
	mStack_->Rotate(transform_rotate_list[ROT_SwordY], 0, 1, 0);
	mStack_->Rotate(transform_rotate_list[ROT_SwordX], 1, 0, 0);
}

void AnimMetabee::popLeftBullet() {
	mStack_->Translate(0, transform_translate_list[TRS_LeftArmBulletY], 0);
}

void AnimMetabee::popEnergyBall() {
	mStack_->Translate(transform_translate_list[TRS_EnergyBallX], transform_translate_list[TRS_EnergyBallY], transform_translate_list[TRS_EnergyBallZ]);
	mStack_->Scale(transform_scale_list[SCALE_ENERGYBALL], transform_scale_list[SCALE_ENERGYBALL], transform_scale_list[SCALE_ENERGYBALL]);
}

void AnimMetabee::popChargedParticle() {
	mStack_->Translate(transform_translate_list[TRS_Charged_ParticleX], transform_translate_list[TRS_Charged_ParticleY], transform_translate_list[TRS_Charged_ParticleZ]);
	mStack_->Scale(transform_scale_list[SCALE_CHARGEDPARTICLE], transform_scale_list[SCALE_CHARGEDPARTICLE], transform_scale_list[SCALE_CHARGEDPARTICLE]);
}