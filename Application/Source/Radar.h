#ifndef RADAR_H
#define RADAR_H

#include <vector>


using std::vector;



class NPC;
class Scene;

class Radar {

public:
	Radar(Scene* manager);
	~Radar() {};

	void addUnit(NPC* unit);
	void removeUnit(NPC* unit);

	void RenderRadar(float x, float y);

private:
	Scene* _scene;

	vector<NPC*> unitContainer; // To access the position of the units

	const float _RadarRange = 4.0f; // Distance before the Radar can detect any enemy, change this to increase Radar's detection range
	const float _RadarSize = 7; // Background size of the radar
	const float _RadarSizeSquared; // Helper variable: Square length of the Radar
	const float _RadarEnemySize = 0.007f; // Enemy Icon Size: Scaling is relative to _RadarSize :: (This * _RadarSize)
	const float _RadarPlayerSize = 0.005f; // Player Icon Size: Scaling is relative to _RadarSize :: (This * _RadarSize)


};
#endif