#ifndef RADAR_H
#define RADAR_H

#include <vector>


using std::vector;



class NPC;
class Scene;

class Radar{

public:
	Radar(Scene* manager);
	~Radar(){};

	void addUnit(NPC* unit);
	void removeUnit(NPC* unit);

	void RenderRadar(float x, float y);


private:
	Scene* _scene;

	vector<NPC*> unitContainer;

	const float _RadarSize = 7.0f; // Background size
	const float _RadarEnemySize = 0.25f;
	const float _RadarPlayerSize = 0.25f;


};
#endif