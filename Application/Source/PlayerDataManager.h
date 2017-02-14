#ifndef PlayerDataManager_H
#define PlayerDataManager_H

struct PlayerData {
	int currency;
	bool level01_unlocked;
	bool level02_unlocked;
	bool level03_unlocked;
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

};

class PlayerDataManager {
	static PlayerDataManager* _instance;

public:
	static PlayerDataManager* getInstance();

	void SaveData();
	void LoadPlayerData();
	PlayerData* getPlayerData() { return &pData; };


protected:
	PlayerDataManager();
	~PlayerDataManager() { delete _instance; };

private:
	PlayerData pData;




};
#endif