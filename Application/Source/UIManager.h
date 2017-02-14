#ifndef UIMANAGER_H
#define UIMANAGER_H


#include <string>
#include <queue>
#include <vector>

#include "MatrixStack.h"
#include "Mesh.h"
#include "Vertex.h"


// forward declaration
class Assignment03;


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
	void queueRenderText(Text text);

	// Render the given Text to the screen
	void renderTextOnScreen(Text text);

	// Load the font widths of a '.csv' into a vector for proper character spacing of text
	bool LoadFontWidth(std::string fontPath);

	// Reset all text positions to default to prevent them from flying away, must be called at end of Render()
	void reset();

	// Prints all the pending Text onto the screen
	void dequeueText();

private:

	Scene* _scene;

	std::vector<unsigned> currentFontWidth;

	unsigned anchor_offset[ANCHOR_TOTAL];

	// Pending Texts waiting to be printed
	std::queue<Text> currentTextQueue;

};
#endif