#ifndef UIMANAGER_H
#define UIMANAGER_H


#include <string>
#include <queue>
#include <vector>

#include "MatrixStack.h"
#include "Mesh.h"
#include "Vertex.h"


// forward declaration
class Scene;


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
	
	//Render the mesh onto the screen with the given Screen Coordinates
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, Vector3 rotate, Vector3 scale);

private:
	Scene* _scene;

	std::vector<unsigned> currentFontWidth;

	unsigned anchor_offset[ANCHOR_TOTAL];

	// Pending Texts waiting to be printed
	std::queue<Text> currentTextQueue;

	bool showDebugInfo = false;

};
#endif