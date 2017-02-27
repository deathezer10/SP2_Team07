#ifndef UIMANAGER_H
#define UIMANAGER_H


#include <tuple>
#include <string>
#include <map>
#include <queue>
#include <vector>

#include "MatrixStack.h"
#include "Vertex.h"
#include "Radar.h"


// forward declaration
class Scene;
class Mesh;


// Provides functions to display of HUDs
class UIManager {

public:
	enum UI_ANCHOR {

		ANCHOR_BOT_LEFT = 0, // Auto-Pads Upwards
		ANCHOR_BOT_RIGHT, // Auto-Pads Upwards
		ANCHOR_TOP_LEFT, // Auto-Pads Downwards
		ANCHOR_TOP_RIGHT, // Auto-Pads Downwards
		ANCHOR_BOT_CENTER, // Auto-Pads Upwards
		ANCHOR_TOP_CENTER, // Auto-Pads Downwards
		ANCHOR_CENTER_CENTER, // Auto-Pads Downwards

		ANCHOR_TOTAL

	};

	struct Text {
		Text(std::string text, Color col, UI_ANCHOR textAnchor) {
			value = text;
			color = col;
			anchor = textAnchor;
		}

		std::string value;
		Color color;
		UI_ANCHOR anchor;
	};

	struct MeshQueue {
		MeshQueue(Mesh* mesh, Vector3 position, Vector3 rotation, Vector3 scaling, bool lighting = false , bool wireframe = true) {
			this->mesh = mesh;
			this->position = position;
			this->rotation = rotation;
			this->scaling = scaling;
			this->lighting = lighting;
			this->wireframe = wireframe;
		}

		// GG man, to allow map to compare whether both struct are the same
		bool operator< (const MeshQueue& other) const {
			return (std::tie(mesh, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, scaling.x, scaling.y, scaling.z, lighting) <
				std::tie(other.mesh, other.position.x, other.position.y, other.position.z, other.rotation.x, other.rotation.y, other.rotation.z, other.scaling.x, other.scaling.y, other.scaling.z, other.lighting));
		}

		Mesh* mesh;
		Vector3 position;
		Vector3 rotation;
		Vector3 scaling;
		bool lighting;
		bool wireframe;
	};

	UIManager(Scene* scene);
	~UIManager() {};

	// Mesh of the Scene's Text mesh
	Mesh** textMesh;

	// Queue the given Text so that it gets called during Render()
	// Call this instead of renderTextOnScreen() if you want to render text inside Update()
	void queueRenderText(Text text);

	// Render the given Text to the screen
	// Call queueRenderText() if you want to render a text when inside Update()
	void renderTextOnScreen(Text text);

	// Load the font widths of a '.csv' into a vector to allow proper character spacing of all text characters
	bool LoadFontWidth(std::string fontPath);

	void renderPlayerHUD();

	// Reset all text positions to default to prevent them from flying away, must be called at end of Render()
	void reset();

	// Prints all the pending Text onto the screen
	void dequeueText();

	// Renders all the pending Meshes onto the screen
	void dequeueMesh();

	//Render the mesh onto the world
	void queueRenderMesh(MeshQueue meshQueue);

	//Render the mesh onto the screen with the given Screen Coordinates
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, Vector3 rotate, Vector3 scale);

	//Render the mesh onto the screen with the given Screen Coordinates for a given duration
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, Vector3 rotate, Vector3 scale, float duration);

	// Renders the Mesh on screen for a set duration, duration is refresh upon calling it using the same Meshqueue again
	void addTimedMeshToScreen(MeshQueue mesh, float duration);

	Radar radar;

	static bool showDebugInfo;

private:
	Scene* _scene;

	std::vector<unsigned> currentFontWidth;

	unsigned anchor_offset[ANCHOR_TOTAL];

	// Pending Texts waiting to be printed
	std::queue<Text> currentTextQueue;
	std::queue<MeshQueue> currentMeshQueue;
	std::map<MeshQueue, float> currentTimeMeshes;

	// Renders the Mesh and reduces the duration of the key inside the map
	void RenderTimedMeshOnScreen();

};
#endif