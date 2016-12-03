#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "j1Fonts.h"
#define CURSOR_WIDTH 2

enum ElementType
{
	LABEL,
	IMAGE,
	BUTTON,
	TEXT,
};

/*enum MouseState
{
	MOUSE_OUT,
	RIGHT_CLICK,
	LEFT_CLICK,
	MOUSE_OVER,
};*/
//-----------------UI_ELEMENT------------------

class UI_Element
{
public:
	UI_Element();
	UI_Element(SDL_Rect rect, bool isActive, bool isVisible);
	UI_Element(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible);
	UI_Element(iPoint pos, bool isActive, bool isVisible);

	~UI_Element();
	virtual bool Update();
	virtual void Draw();
	virtual void HandleInput();

	//Set Private Arguments
	void SetPos(int x, int y, int w, int h);
	void SetPos(int x, int y);
	void SetRect(SDL_Rect rect);
	void SetActiveTo(bool set);
	void SetVisibleTo(bool set);
	void SetCollisionBox(SDL_Rect box);
	void MoveElement(int x, int y);
	void SetMovingTo(bool moving);

	//Get Private Arguments
	iPoint GetPos();
	SDL_Rect GetRect();
	SDL_Rect GetCollisionBox();
	bool IsActive();
	bool IsVisible();
	bool IsMoving();
	//Test
	UI_Element* parent;
	p2List<UI_Element*> childs;
public:
	enum ElementType elementType;

private:
	iPoint pos;
	SDL_Rect rect;
	SDL_Rect collisionBox;
	
	
	bool is_moving = false;
	bool active;
	bool visible;
};	
//-----------------UI_ELEMENT_END------------------

//-----------------UI_SCENE------------------
class UI_Scene
{
public:
	UI_Scene();
	~UI_Scene();
	bool UpdateScene();
	UI_Element* CreateElement(ElementType type);
public:
	p2DynArray<UI_Element*> elements;
private:

};
//-----------------UI_SCENE_END------------------
// ------------------j1Gui-----------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//--------FACTORY METHODS--------------------
	// Gui creation functions
	UI_Scene* CreateScene();
	
	//Open Scene
	bool OpenScene(UI_Scene*);

	//Close Scene
	bool CloseScene(UI_Scene*);

	SDL_Texture* GetAtlas() const;
public:
	p2List<UI_Scene*> activeScene;
	p2DynArray<UI_Scene*> scenes;

private:

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};
//-------------------j1Gui_END-----------------
//---------------------Label--------------------

class Label:public UI_Element
{
public:
	Label(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible, p2SString textInput, iPoint textPos);
	~Label();
	bool Update();
	void Draw();
	void SetTextPos(int x, int y);
	void SetString(const char* word);
	bool Is_Over();

	//Get Private Arguments
	//bool Is_Selected();
	//void Set_Selected(bool is_selected);

	p2DynArray<Label*> all_labels;
	iPoint textPos;
private:
	SDL_Texture* texture;
	p2SString textInput;
	bool is_selected;
};
//----------------------Label_END----------------------
//------------------------Image------------------------

class Image :public UI_Element
{
public:
	Image(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible);
	~Image();
	bool Update();
	void Draw();

	SDL_Texture* texture;
};
//------------------------Image_END----------------------
//-------------------------Button------------------------

class Button :public UI_Element
{
public:
	Button(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible); // , enum MouseState buttonstate
	~Button();
	bool Update();
	void Draw();
	void HandleInput();
	bool Is_Over();

	//enum MouseState GetMouseState();
	//enum MouseState buttonstate;
};
//---------------------------Button_END-------------------
//---------------------------Text-------------------

class Text :public UI_Element
{
public:
	Text(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible, const char* word);
	~Text();

	bool Update();
	void Draw();
	void SetString(const char* word);
	const char* GetText();
	bool Is_Over();
	

private:
	//Font* font;
	p2SString textInput;
	bool is_selected;
	
};
//---------------------------Text_END-------------------

#endif // __j1GUI_H__