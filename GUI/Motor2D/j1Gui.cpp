#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	//Change image
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	
	
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;

	for (p2List_item<UI_Scene*>* item = activeScene.start; item != NULL; item = item->next) 
	{
		item->data->UpdateScene();
	}
	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	p2List_item<UI_Scene*>* item=activeScene.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	activeScene.clear();

	scenes.Clear();
	return true;
}

UI_Scene * j1Gui::CreateScene()
{
	UI_Scene* ret;
	ret = new UI_Scene();
	scenes.PushBack(ret);
	return ret;
}

bool j1Gui::OpenScene(UI_Scene * scene)
{
	bool ret = false;
	for (uint i = 0; i < scenes.Count(); ++i)
	{
		if (scenes[i] == scene) {
			activeScene.add(scene);
			ret = true;
		}
	}
	return ret;
}

bool j1Gui::CloseScene(UI_Scene *scene)
{
	bool ret = false;
	for (p2List_item<UI_Scene*>* item = activeScene.start; item != NULL; item = item->next) {
		if (item->data == scene) {
			activeScene.del(item);
			ret = true;
		}
	}
	return false;
}


// const getter for atlas
 SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// START UI_Scene---------------------------------------------------
UI_Scene::UI_Scene()
{
}

UI_Scene::~UI_Scene()
{
}

bool UI_Scene::UpdateScene()
{
	bool ret = true;
	if (elements.Count() == 0) {
		ret = false;
	}
	if (ret == true) {
		for (int i = 0; i < elements.Count(); i++) {
			ret = elements[i]->Update();
		}
	}
	return ret;
}

UI_Element * UI_Scene::CreateElement(ElementType type)
{
	UI_Element* ret = nullptr;

	switch (type)
	{
	case LABEL:
		ret = new Label(SDL_Rect{ 0, 0, 0, 0 }, iPoint(0, 0), true, true,nullptr,iPoint(0,0));
		elements.PushBack(ret);
		break;
	case IMAGE:
		ret = new Image(SDL_Rect{ 0,0,0,0 }, iPoint(0, 0), true, true);
		elements.PushBack(ret);
		break;
	case BUTTON:
		ret = new Button(SDL_Rect{ 0,0,0,0 }, iPoint(0, 0), true, true);
		elements.PushBack(ret);
		break;
	case TEXT:
		ret = new Text(SDL_Rect{ 0,0,0,0 }, iPoint(0, 0), true, true, " ");
		elements.PushBack(ret);
	case TEXTBOX:
		ret = new TextBox(SDL_Rect{ 0,0,0,0 }, iPoint(0, 0), true, true, "default");
		elements.PushBack(ret);
		break;
	default:
		ret = nullptr;
		break;
	}
	return ret;
}

// START UI_Element---------------------------------------------------------

UI_Element::UI_Element()
{
}

UI_Element::UI_Element(SDL_Rect rect, bool isActive, bool isVisible):rect(rect), pos(pos), active(isActive), visible(isVisible)
{
}

UI_Element::UI_Element(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible):rect(rect), pos(pos),active(isActive),visible(isVisible)
{
}

UI_Element::UI_Element(iPoint pos, bool isActive, bool isVisible) :  pos(pos), active(isActive), visible(isVisible)
{
}

void UI_Element::SetPos(int x, int y, int w, int h)
{
	pos.x = x;
	pos.y = y;

	//We can change collision box if we want with SetCollisionBox(SDL_Rect set)
	collisionBox.h = h;
	collisionBox.w = w;
	collisionBox.x = x;
	collisionBox.y = y;
}

void UI_Element::SetPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
	collisionBox.x = x;
	collisionBox.y = y;
}

void UI_Element::SetRect(SDL_Rect set)
{
	rect.h = set.h;
	rect.w = set.w;
	rect.x = set.x;
	rect.y = set.y;
}

UI_Element::~UI_Element()
{
}

void UI_Element::Draw()
{
	return;
}

bool UI_Element::Update()
{
	return true;
}

void UI_Element::HandleInput()
{
}
iPoint UI_Element::GetPos()
{
	return pos;
}
SDL_Rect UI_Element::GetRect()
{
	return rect;
}
SDL_Rect UI_Element::GetCollisionBox()
{
	return collisionBox;
}
bool UI_Element::IsActive()
{
	return active ==true;
}
void UI_Element::SetActiveTo(bool set)
{
	active = set;
}
bool UI_Element::IsVisible()
{
	return visible==true;
}
bool UI_Element::IsMoving()
{
	return is_moving;
}
void UI_Element::SetVisibleTo(bool set)
{
	visible = set;
}
void UI_Element::SetCollisionBox(SDL_Rect set)
{
	collisionBox.h = set.h;
	collisionBox.w = set.w;
	collisionBox.x = set.x;
	collisionBox.y = set.y;
}

void UI_Element::MoveElement(int x, int y)
{
	pos.x = pos.x + x;
	pos.y = pos.y + y;
	collisionBox.x = collisionBox.x + x;
	collisionBox.y = collisionBox.y + y;

	if (childs.start != nullptr)
	{
		p2List_item<UI_Element*>* it = childs.start;
		for (; it->next != nullptr; it = it->next)
		{
			it->data->MoveElement(x, y);
		}
		it->data->MoveElement(x, y);
	}
	
}

void UI_Element::SetMovingTo(bool moving)
{
	is_moving = moving;
}

// START Label---------------------------------------------------------
Label::Label(SDL_Rect rect,iPoint pos, bool isActive, bool isVisible, p2SString textInput, iPoint textPos) :UI_Element(rect, pos, isActive, isVisible),textInput(textInput), textPos(textPos)
{
	elementType = LABEL;
	all_labels.PushBack(this);
}

Label::~Label()
{
}

void Label::Draw()
{
	if (IsVisible()) 
	{
		//TODO
		if (Is_Over() == false)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == true)
			{
				int x, y;
				App->input->GetMouseMotion(x, y);
				App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x - x, GetPos().y - App->render->camera.y -y, &GetRect());
				App->render->Blit(App->font->Print(textInput.GetString()), textPos.x - App->render->camera.x, textPos.y - App->render->camera.y);
			}
			else
			{
				App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y, &GetRect());
				App->render->Blit(App->font->Print(textInput.GetString()), textPos.x - App->render->camera.x, textPos.y - App->render->camera.y);
			}
		}
	}
}

bool Label::Update()
{
	bool ret = true;
	Draw();
	return ret;
}

void Label::SetTextPos(int x, int y)
{
	textPos.x = x;
	textPos.y = y;
}
void Label::SetString(const char * word)
{
	textInput = word;
}
bool Label::Is_Over()
{
	return false;
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > GetCollisionBox().x && x < (GetCollisionBox().w + GetCollisionBox().x) &&
		y > GetCollisionBox().y && y < (GetCollisionBox().h + GetCollisionBox().y))
	{
		return true;
	}
	else
	{
		return false;
	}
}


// START Image---------------------------------------------------------
Image::Image(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible) :UI_Element(rect, pos, isActive, isVisible)
{
	elementType = IMAGE;
}

Image::~Image()
{
}

void Image::Draw()
{
	if (IsVisible() == true) 
	{
		/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x -x, GetPos().y - App->render->camera.y -y, &GetRect());
		}
		else*/
		App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y, &GetRect());
	}
}

bool Image::Is_Over()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > GetCollisionBox().x && x < (GetCollisionBox().w + GetCollisionBox().x) &&
		y > GetCollisionBox().y && y < (GetCollisionBox().h + GetCollisionBox().y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Image::Update()
{
	bool ret = true;
	Draw();
	return ret;
}

// START Button---------------------------------------------------------

Button::Button(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible) :UI_Element(rect, pos, isActive, isVisible)
{
}

Button::~Button()
{
}

void Button::Draw()
{
	if (IsVisible() == true) 
	{
		App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y, &GetRect());
	}
}

bool Button::Update()
{
	
	Draw();
	return true;
}

void Button::HandleInput()
{

}

bool Button::Is_Over()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > GetCollisionBox().x && x < (GetCollisionBox().w + GetCollisionBox().x) &&
		y > GetCollisionBox().y && y < (GetCollisionBox().h + GetCollisionBox().y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// START Text---------------------------------------------------------

Text::Text(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible, const char* word) : UI_Element(rect, pos, isActive, isVisible)
{
	textInput = word;
}

Text::~Text()
{
	
}

bool Text::Update()
{
	Draw();
	return true;
}

void Text::Draw()
{
	if (IsVisible() == true)
	{
		App->render->Blit(App->font->Print(textInput.GetString()), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y);
	}
}

void Text::SetString(const char * word)
{
	textInput = word;
}

const char * Text::GetText()
{
	return textInput.GetString();
}

bool Text::Is_Over()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > GetCollisionBox().x && x < (GetCollisionBox().w + GetCollisionBox().x) &&
		y > GetCollisionBox().y && y < (GetCollisionBox().h + GetCollisionBox().y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

TextBox::TextBox(SDL_Rect rect, iPoint pos, bool isActive, bool isVisible, const char * word) : UI_Element(rect, pos, isActive, isVisible)
{
	textInput = word;
}

TextBox::~TextBox()
{
}

bool TextBox::Update()
{
	Draw();
	return true;
}

void TextBox::Draw()
{
	if (IsVisible() == true)
	{
		App->render->Blit(App->gui->GetAtlas(), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y, &GetRect());
		App->render->Blit(App->font->Print(textInput.GetString()), GetPos().x - App->render->camera.x, GetPos().y - App->render->camera.y);
	}
}

void TextBox::SetString(const char * word)
{
	textInput = word;
}

const char * TextBox::GetText()
{
	return textInput.GetString();
}

bool TextBox::Is_Over()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (x > GetCollisionBox().x && x < (GetCollisionBox().w + GetCollisionBox().x) &&
		y > GetCollisionBox().y && y < (GetCollisionBox().h + GetCollisionBox().y))
	{
		return true;
	}
	else
	{
		return false;
	}
}
