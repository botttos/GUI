#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class Button;
class Text;
class TextBox;
class Image;
class Label;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	SDL_Rect button_rect;
private:
	SDL_Texture* debug_tex;
	GuiImage* banner;
	GuiText* text;
	Button* button;
	Text* text1;
	TextBox* textbox;
	Image* image;
	Image* slider;
	Button* sliderbutton;
	Label* label;
};

#endif // __j1SCENE_H__