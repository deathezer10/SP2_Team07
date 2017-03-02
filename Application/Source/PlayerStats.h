#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H

struct PlayerStat {

	float base_speed;
	int base_damage;
	int base_shield;
	
	int initial_bullet_damage;
	int initial_bullet_speed;
	float initial_bullet_cooldown;
	float initial_shield_capacity;
	float initial_shield_recoveryRate;

	int current_bullet_damage;
	int current_bullet_speed;
	float current_bullet_cooldown;
	float current_shield_capacity;
	float current_shield_recoveryRate;

	int current_fighter;
	int current_health;
	float current_shield;

	int currency_earned;

};

// Base Ship Stats before conversion
// Data Format: Speed,Damage,Shield
const int Ship_Stats[12] {

	    // Default: 
	    3, 2, 3,

		// SF-1
		5, 2, 1,

		// DF-6
		2, 4, 1,

		// A-10
		1, 2, 5

};


// Provides helper functions to convert Integers to the appropriate PlayerStats value
// Example: Converting BulletDamage from int '1' to float '7.5f'
class PlayerStats {

public:
	PlayerStats();
	~PlayerStats() {};

	PlayerStat* getStatData() { return &pStat; };

	void ConvertToPlayerStats();

	void ResetPlayerStats(int currentFighter);

	// Default life values before upgrades are added
	const int default_health = 100;
	const int default_shield = 50;
	const int multiplier_base_speed = 3; // Different Space Fighter Acceleration per level

	// Minimum Stats
	const int minimum_bullet_damage = 10;
	const int minimum_bullet_speed = 150;
	const int minimum_shield_capacity = 25;
	const int minimum_base_speed = 2;

	// Multiplier for each Upgrades level
	const int multiplier_bullet_damage = 5;
	const int multiplier_bullet_speed = 20;
	const int multiplier_shield_capacity = 20;

	// Cooldown time, dont forget there's a minimum value for these 
	// Formula: (baseCD - (subtracter * upgradeLevel)) + minimum
	const float base_cd_bullet_rof = 0.55f;
	const float substracter_cd_bullet_rof = 0.05f;
	const float minimum_bullet_cooldown = 0.10f;

	const float base_cd_shield_recovery = 5;
	const float substracter_cd_shield_recovery = 1.0f;
	const float minimum_shield_recoveryRate = 5.0f;

private:
	PlayerStat pStat;


};
#endif