#ifndef PlayerDataManager_H
#define PlayerDataManager_H

#include <map>

struct PlayerData {
	int currency;

	// Unlocked levels
	bool level01_unlocked;
	bool level02_unlocked;
	bool level03_unlocked;

	// Unlocked fighters
	bool SF1;
	bool DF6;
	bool A10;

	//default fighter upgrades
	int FG6_Bullet_damage;// current level of bullet damage
	int FG6_Bullet_speed;// current level of bullet speed
	int FG6_Bullet_ROF;// current level of rate of fire
	int FG6_Shield_hp;// current level of total shield ship has
	int FG6_shield_recharge_rate;// current level of shield recharge rate

	//fast type fighter upgrades
	int SF1_Bullet_damage;
	int SF1_Bullet_speed;
	int SF1_Bullet_ROF;
	int SF1_Shield_hp;
	int SF1_shield_recharge_rate;

	//high damage type fighter upgrades
	int DF6_Bullet_damage;
	int DF6_Bullet_speed;
	int DF6_Bullet_ROF;
	int DF6_Shield_hp;
	int DF6_shield_recharge_rate;

	//high durability type fighter upgrade
	int A10_Bullet_damage;
	int A10_Bullet_speed;
	int A10_Bullet_ROF;
	int A10_Shield_hp;
	int A10_shield_recharge_rate;

	// Currently selected fighter
	int currentFighter;

};

struct PlayerStat {

	int base_speed;
	int base_damage;
	int base_shield;

	int initial_bullet_damage;
	int initial_bullet_speed;
	int initial_bullet_cooldown;
	int initial_shield_capacity;
	int initial_shield_recoveryRate;

	int current_bullet_damage;
	int current_bullet_speed;
	int current_bullet_cooldown;
	int current_shield_capacity;
	int current_shield_recoveryRate;

	int current_fighter;
	int current_health;
	int current_shield;


};

const int Ship_Stats[12] {

	// Format: Speed-Damage-Shield
	// Default: 
	1, 1, 1,

		// SF-1
		2, 1, 0,

		// DF-6
		1, 2, 0,

		// A-10
		0, 1, 2

};

class PlayerDataManager {
	static PlayerDataManager* _instance;

public:
	static PlayerDataManager* getInstance();

	// Flushes the PlayerData file and write pData into it
	// Args 0: Current Space Fighter that the player is using
	void SaveData(int currentFighter);

	// Load the PlayerData file and store it into a vector
	void LoadPlayerData();

	// Reset the PlayerStat to it's default value, useful for resetting variables like Health
	void ResetPlayerStat();

	// Contains information about player purchased Upgrades
	PlayerData* getPlayerData() { return &pData; };

	// Contains information about player upgrades purchased
	PlayerStat* getPlayerStats() { return &pStatData; };

private:
	PlayerDataManager();
	~PlayerDataManager() { delete _instance; };

	PlayerData pData;
	PlayerStat pStatData;
	
	int current_bullet_damage;
	int current_bullet_speed;
	int current_bullet_cooldown;
	int current_shield_capacity;
	int current_shield_recoveryRate;

	const int base_bullet_speed = 1;
	const int base_bullet_cooldown = 1;
	const int base_shield_recoveryrate = 1;

	const int multiplier_bullet_damage = 1;
	const int multiplier_bullet_speed = 1;
	const int multiplier_bullet_cooldown = 2;
	const int multiplier_shield_capacity = 1;
	const int multiplier_shield_recoveryRate = 2;

	const int default_health = 100;
	const int default_shield = 100;

};
#endif