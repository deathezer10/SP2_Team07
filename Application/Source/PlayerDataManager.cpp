#include "PlayerDataManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using std::ofstream;
using std::ifstream;
using std::ios;
using std::cout;
using std::string;

PlayerDataManager *PlayerDataManager::_instance = 0;


PlayerDataManager* PlayerDataManager::getInstance() {
	if (!_instance) {
		_instance = new PlayerDataManager();
		_instance->LoadPlayerData(); // load data on first init
	}
	return _instance;
}

PlayerDataManager::PlayerDataManager() {
	pData = {

		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0

	};

	pStatData = {

		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0,

	};

}


void PlayerDataManager::LoadPlayerData() {
	std::string line;
	std::string delimiter = "|";

	std::vector<int> v;

	std::ifstream myfile("PlayerData");
	if (myfile.is_open()) {
		while (std::getline(myfile, line)) {

			std::string token = line.substr(0, line.find(delimiter));
			line.erase(0, token.length() + 1);
			int x = 0;
			std::stringstream convert(line);
			convert >> x;
			v.push_back(x);
		}
		myfile.close();

		pData.currency = v[0];
		pData.level01_unlocked = !!v[1];
		pData.level02_unlocked = !!v[2];
		pData.level03_unlocked = !!v[3];
		pData.SF1 = !!v[4];
		pData.DF6 = !!v[5];
		pData.A10 = !!v[6];

		pData.FG6_Bullet_damage = v[7];
		pData.FG6_Bullet_speed = v[8];
		pData.FG6_Bullet_ROF = v[9];
		pData.FG6_Shield_hp = v[10];
		pData.FG6_shield_recharge_rate = v[11];

		pData.SF1_Bullet_damage = v[12];
		pData.SF1_Bullet_speed = v[13];
		pData.SF1_Bullet_ROF = v[14];
		pData.SF1_Shield_hp = v[15];
		pData.SF1_shield_recharge_rate = v[16];

		pData.DF6_Bullet_damage = v[17];
		pData.DF6_Bullet_speed = v[18];
		pData.DF6_Bullet_ROF = v[19];
		pData.DF6_Shield_hp = v[20];
		pData.DF6_shield_recharge_rate = v[21];

		pData.A10_Bullet_damage = v[22];
		pData.A10_Bullet_speed = v[23];
		pData.A10_Bullet_ROF = v[24];
		pData.A10_Shield_hp = v[25];
		pData.A10_shield_recharge_rate = v[26];

		pData.currentFighter = v[27];

		switch (pData.currentFighter){

		case 0:
			current_bullet_damage = v[7];
			current_bullet_speed = v[8];
			current_bullet_cooldown = v[9];
			current_shield_capacity = v[10];
			current_shield_recoveryRate = v[11];
			break;

		case 1:
			current_bullet_damage = v[12];
			current_bullet_speed = v[13];
			current_bullet_cooldown = v[14];
			current_shield_capacity = v[15];
			current_shield_recoveryRate = v[16];
			break;

		case 2:
			current_bullet_damage = v[17];
			current_bullet_speed = v[18];
			current_bullet_cooldown = v[19];
			current_shield_capacity = v[20];
			current_shield_recoveryRate = v[21];
			break;

		case 3:
			current_bullet_damage = v[22];
			current_bullet_speed = v[23];
			current_bullet_cooldown = v[24];
			current_shield_capacity = v[25];
			current_shield_recoveryRate = v[26];
			break;

		}

	}
	else {
		std::cout << "Load playerData failed, unable to open file to load" << std::endl;
	}

	ResetPlayerStat();

}

void PlayerDataManager::SaveData(int currentFighter) {

	switch (currentFighter){

	case 0:
		current_bullet_damage = pData.FG6_Bullet_damage;
		current_bullet_speed = pData.FG6_Bullet_speed;
		current_bullet_cooldown = pData.FG6_Bullet_ROF;
		current_shield_capacity = pData.FG6_Shield_hp;
		current_shield_recoveryRate = pData.FG6_shield_recharge_rate;
		break;

	case 1:
		current_bullet_damage = pData.SF1_Bullet_damage;
		current_bullet_speed = pData.SF1_Bullet_speed;
		current_bullet_cooldown = pData.SF1_Bullet_ROF;
		current_shield_capacity = pData.SF1_Shield_hp;
		current_shield_recoveryRate = pData.SF1_shield_recharge_rate;
		break;

	case 2:
		current_bullet_damage = pData.DF6_Bullet_damage;
		current_bullet_speed = pData.DF6_Bullet_speed;
		current_bullet_cooldown = pData.DF6_Bullet_ROF;
		current_shield_capacity = pData.DF6_Shield_hp;
		current_shield_recoveryRate = pData.DF6_shield_recharge_rate;
		break;

	case 3:
		current_bullet_damage = pData.A10_Bullet_damage;
		current_bullet_speed = pData.A10_Bullet_speed;
		current_bullet_cooldown = pData.A10_Bullet_ROF;
		current_shield_capacity = pData.A10_Shield_hp;
		current_shield_recoveryRate = pData.A10_shield_recharge_rate;
		break;

	}

	std::ofstream myfile("PlayerData", ios::trunc);// open file and replace old data with new
	if (myfile.is_open()) {

		//myfile << "" << '|' << pData. << "\n"; format template
		myfile << "currency" << '|' << pData.currency << '\n';
		myfile << "level01" << '|' << pData.level01_unlocked << '\n';
		myfile << "level02" << '|' << pData.level02_unlocked << '\n';
		myfile << "level03" << '|' << pData.level03_unlocked << '\n';
		myfile << "SF-1" << '|' << pData.SF1 << '\n';
		myfile << "DF-6" << '|' << pData.DF6 << '\n';
		myfile << "A-10" << '|' << pData.A10 << '\n';

		myfile << "FG6_bullet_damage" << '|' << pData.FG6_Bullet_damage << "\n";
		myfile << "FG6_bullet_speed" << '|' << pData.FG6_Bullet_speed << "\n";
		myfile << "FG6_Bullet_rof" << '|' << pData.FG6_Bullet_ROF << "\n";
		myfile << "FG6_shield_level" << '|' << pData.FG6_Shield_hp << "\n";
		myfile << "FG6_shield_recharge " << '|' << pData.FG6_shield_recharge_rate << "\n";

		myfile << "SF1_bullet_damage" << '|' << pData.SF1_Bullet_damage << "\n";
		myfile << "SF1_bullet_speed" << '|' << pData.SF1_Bullet_speed << "\n";
		myfile << "SF1_Bullet_rof" << '|' << pData.SF1_Bullet_ROF << "\n";
		myfile << "SF1_shield_level" << '|' << pData.SF1_Shield_hp << "\n";
		myfile << "SF1_shield_recharge " << '|' << pData.SF1_shield_recharge_rate << "\n";

		myfile << "DF6_bullet_damage" << '|' << pData.DF6_Bullet_damage << "\n";
		myfile << "DF6_bullet_speed" << '|' << pData.DF6_Bullet_speed << "\n";
		myfile << "DF6_Bullet_rof" << '|' << pData.DF6_Bullet_ROF << "\n";
		myfile << "DF6_shield_level" << '|' << pData.DF6_Shield_hp << "\n";
		myfile << "DF6_shield_recharge " << '|' << pData.DF6_shield_recharge_rate << "\n";

		myfile << "A10_bullet_damage" << '|' << pData.A10_Bullet_damage << "\n";
		myfile << "A10_bullet_speed" << '|' << pData.A10_Bullet_speed << "\n";
		myfile << "A10_Bullet_rof" << '|' << pData.A10_Bullet_ROF << "\n";
		myfile << "A10_shield_level" << '|' << pData.A10_Shield_hp << "\n";
		myfile << "A10_shield_recharge" << '|' << pData.A10_shield_recharge_rate << "\n";

		myfile << "currentFighter" << '|' << pData.currentFighter << "\n";
		myfile.close();
	}
	else {
		std::cout << "Save playerData failed, unable to open file to save" << std::endl;
	}

	// Update player Stats according to changes
	ResetPlayerStat();
}


void PlayerDataManager::ResetPlayerStat() {
	pStatData = {

		Ship_Stats[(3 * pData.currentFighter)], // Base Speed
		Ship_Stats[(3 * pData.currentFighter) + 1], // Base Damage
		Ship_Stats[(3 * pData.currentFighter) + 2], // Base Shield

		// Initial Stats
		Ship_Stats[(3 * pData.currentFighter) + 1] + (current_bullet_damage * multiplier_bullet_damage), // Bullet Damage
		base_bullet_speed + (current_bullet_speed * multiplier_bullet_speed), // Bullet Speed 
		base_bullet_cooldown + (current_bullet_cooldown * multiplier_bullet_cooldown), // Bullet Cooldown
		Ship_Stats[(3 * pData.currentFighter) + 2] + (current_shield_capacity * multiplier_shield_capacity), // Shield Capacity
		base_shield_recoveryrate + (current_shield_recoveryRate * multiplier_shield_recoveryRate), // Shield Recovery

		// Current Stats
		Ship_Stats[(3 * pData.currentFighter) + 1] + (current_bullet_damage * multiplier_bullet_damage), // Bullet Damage
		base_bullet_speed + (current_bullet_speed * multiplier_bullet_speed), // Bullet Speed 
		base_bullet_cooldown + (current_bullet_cooldown * multiplier_bullet_cooldown), // Bullet Cooldown
		Ship_Stats[(3 * pData.currentFighter) + 2] + (current_shield_capacity * multiplier_shield_capacity), // Shield Capacity
		base_shield_recoveryrate + (current_shield_recoveryRate * multiplier_shield_recoveryRate), // Shield Recovery

		pData.currentFighter,
		default_health,
		default_shield


	};
}