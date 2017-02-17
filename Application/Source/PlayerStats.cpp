#include "PlayerStats.h"
#include "PlayerDataManager.h"


PlayerStats::PlayerStats() {
	// Zero-fill struct
	pStat = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0,
	};
}

void PlayerStats::ResetPlayerStats(int currentFighter) {

	PlayerSaveData* save = PlayerDataManager::getInstance()->getPlayerData();

	// Fill the Struct with Default values of the currently selected Space Fighter

	switch (currentFighter) {
	case 0:
		pStat.initial_bullet_speed = save->FG6_Bullet_speed;
		pStat.initial_bullet_cooldown = (float)save->FG6_Bullet_ROF;
		pStat.initial_shield_recoveryRate = (float)save->FG6_shield_recharge_rate;
		break;

	case 1:
		pStat.initial_bullet_speed = save->SF1_Bullet_speed;
		pStat.initial_bullet_cooldown = (float)save->SF1_Bullet_ROF;
		pStat.initial_shield_recoveryRate = (float)save->SF1_shield_recharge_rate;
		break;

	case 2:
		pStat.initial_bullet_speed = save->DF6_Bullet_speed;
		pStat.initial_bullet_cooldown = (float)save->DF6_Bullet_ROF;
		pStat.initial_shield_recoveryRate = (float)save->DF6_shield_recharge_rate;
		break;

	case 3:
		pStat.initial_bullet_speed = save->A10_Bullet_speed;
		pStat.initial_bullet_cooldown = (float)save->A10_Bullet_ROF;
		pStat.initial_shield_recoveryRate = (float)save->A10_shield_recharge_rate;
		break;

	}

	pStat.base_speed = (float)Ship_Stats[(3 * currentFighter)];
	pStat.base_damage = Ship_Stats[(3 * currentFighter) + 1];
	pStat.base_shield = Ship_Stats[(3 * currentFighter) + 2];

	pStat.initial_bullet_damage = Ship_Stats[(3 * currentFighter) + 1];
	pStat.initial_shield_capacity = Ship_Stats[(3 * currentFighter) + 2];

	// Dump all initial stats to current stats
	pStat.current_bullet_damage = pStat.initial_bullet_damage;
	pStat.current_bullet_speed = pStat.initial_bullet_speed;
	pStat.current_bullet_cooldown = pStat.initial_bullet_cooldown;
	pStat.current_shield_capacity = pStat.initial_shield_capacity;
	pStat.current_shield_recoveryRate = pStat.initial_shield_recoveryRate;

	pStat.current_fighter = currentFighter;
	pStat.current_health = default_health;
	pStat.current_shield = default_shield;
	// End of Struct Resetting

	ConvertToPlayerStats(); // Convert the Save Data Stats to Real Gameplay Stats
}

void PlayerStats::ConvertToPlayerStats() {

	// Stats Formula
	pStat.initial_bullet_damage = ((pStat.base_damage + pStat.initial_bullet_damage) * multiplier_bullet_damage) + minimum_bullet_damage;
	pStat.initial_bullet_speed = (pStat.initial_bullet_speed * multiplier_bullet_speed) + minimum_bullet_speed;
	pStat.initial_bullet_cooldown = base_cd_bullet_rof - (pStat.initial_bullet_cooldown * substracter_cd_bullet_rof) + minimum_bullet_cooldown;
	pStat.initial_shield_capacity = ((pStat.base_shield + pStat.initial_shield_capacity) * multiplier_shield_capacity) + minimum_shield_capacity;
	pStat.initial_shield_recoveryRate = base_cd_shield_recovery - (pStat.initial_shield_recoveryRate * substracter_cd_shield_recovery) + minimum_shield_recoveryRate;

	// Dump all initial stats to current stats
	pStat.current_bullet_damage = pStat.initial_bullet_damage;
	pStat.current_bullet_speed = pStat.initial_bullet_speed;
	pStat.current_bullet_cooldown = pStat.initial_bullet_cooldown;
	pStat.current_shield_capacity = pStat.initial_shield_capacity;
	pStat.current_shield_recoveryRate = pStat.initial_shield_recoveryRate;

	// Life Values
	pStat.current_health = default_health;
	pStat.current_shield = default_shield + pStat.initial_shield_capacity;
	pStat.base_speed *= multiplier_base_speed;

}