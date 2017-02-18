#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "PlayerDataManager.h"


using std::ofstream;
using std::ifstream;
using std::ios;
using std::cout;
using std::string;


PlayerDataManager *PlayerDataManager::_instance = 0; // Singleton Handle


PlayerDataManager* PlayerDataManager::getInstance() {
	if (!_instance) {
		_instance = new PlayerDataManager();
		_instance->LoadPlayerData(); // load data on first init
	}
	return _instance;
}

PlayerDataManager::PlayerDataManager() {
	// Zero-fill the struct
	pSaveData = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0
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

		pSaveData.currency = v[0];
		pSaveData.level01_unlocked = !!v[1];
		pSaveData.level02_unlocked = !!v[2];
		pSaveData.level03_unlocked = !!v[3];
		pSaveData.SF1 = !!v[4];
		pSaveData.DF6 = !!v[5];
		pSaveData.A10 = !!v[6];

		pSaveData.FG6_Bullet_damage = v[7];
		pSaveData.FG6_Bullet_speed = v[8];
		pSaveData.FG6_Bullet_ROF = v[9];
		pSaveData.FG6_Shield_hp = v[10];
		pSaveData.FG6_shield_recharge_rate = v[11];

		pSaveData.SF1_Bullet_damage = v[12];
		pSaveData.SF1_Bullet_speed = v[13];
		pSaveData.SF1_Bullet_ROF = v[14];
		pSaveData.SF1_Shield_hp = v[15];
		pSaveData.SF1_shield_recharge_rate = v[16];

		pSaveData.DF6_Bullet_damage = v[17];
		pSaveData.DF6_Bullet_speed = v[18];
		pSaveData.DF6_Bullet_ROF = v[19];
		pSaveData.DF6_Shield_hp = v[20];
		pSaveData.DF6_shield_recharge_rate = v[21];

		pSaveData.A10_Bullet_damage = v[22];
		pSaveData.A10_Bullet_speed = v[23];
		pSaveData.A10_Bullet_ROF = v[24];
		pSaveData.A10_Shield_hp = v[25];
		pSaveData.A10_shield_recharge_rate = v[26];

		pSaveData.currentFighter = v[27];

		// Convert the loaded data into appropriate values

	}
	else {
		std::cout << "Load playerData failed, creating a new file" << std::endl;
		SaveData();
	}

	ResetPlayerStats();

}

void PlayerDataManager::SaveData() {

	// Convert the loaded data into appropriate values

	std::ofstream myfile("PlayerData", ios::trunc);// open file and replace old data with new
	if (myfile.is_open()) {

		//myfile << "" << '|' << pSaveData. << "\n"; format template
		myfile << "currency" << '|' << pSaveData.currency << '\n';
		myfile << "level01" << '|' << pSaveData.level01_unlocked << '\n';
		myfile << "level02" << '|' << pSaveData.level02_unlocked << '\n';
		myfile << "level03" << '|' << pSaveData.level03_unlocked << '\n';
		myfile << "SF-1" << '|' << pSaveData.SF1 << '\n';
		myfile << "DF-6" << '|' << pSaveData.DF6 << '\n';
		myfile << "A-10" << '|' << pSaveData.A10 << '\n';

		myfile << "FG6_bullet_damage" << '|' << pSaveData.FG6_Bullet_damage << "\n";
		myfile << "FG6_bullet_speed" << '|' << pSaveData.FG6_Bullet_speed << "\n";
		myfile << "FG6_Bullet_rof" << '|' << pSaveData.FG6_Bullet_ROF << "\n";
		myfile << "FG6_shield_level" << '|' << pSaveData.FG6_Shield_hp << "\n";
		myfile << "FG6_shield_recharge " << '|' << pSaveData.FG6_shield_recharge_rate << "\n";

		myfile << "SF1_bullet_damage" << '|' << pSaveData.SF1_Bullet_damage << "\n";
		myfile << "SF1_bullet_speed" << '|' << pSaveData.SF1_Bullet_speed << "\n";
		myfile << "SF1_Bullet_rof" << '|' << pSaveData.SF1_Bullet_ROF << "\n";
		myfile << "SF1_shield_level" << '|' << pSaveData.SF1_Shield_hp << "\n";
		myfile << "SF1_shield_recharge " << '|' << pSaveData.SF1_shield_recharge_rate << "\n";

		myfile << "DF6_bullet_damage" << '|' << pSaveData.DF6_Bullet_damage << "\n";
		myfile << "DF6_bullet_speed" << '|' << pSaveData.DF6_Bullet_speed << "\n";
		myfile << "DF6_Bullet_rof" << '|' << pSaveData.DF6_Bullet_ROF << "\n";
		myfile << "DF6_shield_level" << '|' << pSaveData.DF6_Shield_hp << "\n";
		myfile << "DF6_shield_recharge " << '|' << pSaveData.DF6_shield_recharge_rate << "\n";

		myfile << "A10_bullet_damage" << '|' << pSaveData.A10_Bullet_damage << "\n";
		myfile << "A10_bullet_speed" << '|' << pSaveData.A10_Bullet_speed << "\n";
		myfile << "A10_Bullet_rof" << '|' << pSaveData.A10_Bullet_ROF << "\n";
		myfile << "A10_shield_level" << '|' << pSaveData.A10_Shield_hp << "\n";
		myfile << "A10_shield_recharge" << '|' << pSaveData.A10_shield_recharge_rate << "\n";

		myfile << "currentFighter" << '|' << pSaveData.currentFighter << "\n";
		myfile.close();
	}
	else {
		std::cout << "Save playerData failed, unable to open file to save" << std::endl;
	}

	// Update player Stats according to changes
	ResetPlayerStats();
}


bool PlayerDataManager::isFighterUnlocked(int value) {
	switch (value) {
	case 0:
		return true;
		break;

	case 1:
		return (pSaveData.SF1);
		break;

	case 2:
		return (pSaveData.DF6);
		break;

	case 3:
		return (pSaveData.A10);
		break;
	}
	return false;
}


void PlayerDataManager::damagePlayer(int amount) {

	if (pStat.getStatData().current_shield > 0) {
		pStat.getStatData().current_shield -= amount;
	}
	else {
		pStat.getStatData().current_health -= amount;
	}

	if (pStat.getStatData().current_shield < 0)
		pStat.getStatData().current_shield = 0;

}

void PlayerDataManager::ResetPlayerStats() {
	pStat.ResetPlayerStats(pSaveData.currentFighter);
}