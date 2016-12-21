#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Fonts.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso_walk.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	// SCENE ELEMENTS
	UI_Scene* scene;
	scene = App->gui->CreateScene();
	App->gui->OpenScene(scene);

	//Image
	image = (Image*)scene->CreateElement(IMAGE);
	image->SetRect(SDL_Rect{ 494, 642, 332, 50, });
	image->SetPos(450, 100, 332, 50);
	// Button
	button = (Button*)scene->CreateElement(BUTTON);
	button->SetRect(SDL_Rect{ 5, 118, 219, 58 });
	button->SetPos(350, 150, 219, 58);
	
	// Text1
	text1 = (Text*)scene->CreateElement(TEXT);
	text1->SetString("Hello World");
	int w, h;
	App->font->CalcSize(text1->GetText(), w, h);
	text1->SetPos(300, 100, w, h);
	button->childs.add(text1);

	// Textbox
	textbox = (TextBox*)scene->CreateElement(TEXTBOX);
	textbox->SetString("");
	App->font->CalcSize(textbox->GetText(), w, h);
	textbox->SetRect(SDL_Rect{ 0, 0, 0, 0, });
	textbox->SetPos(450, 100, w, h);
	
	//Slider Image
	slider = (Image*)scene->CreateElement(IMAGE);
	slider->SetRect(SDL_Rect{ 972, 786, 10, 156, });
	slider->SetPos(450, 400, 332, 50);

	//SliderButton
	sliderbutton = (Button*)scene->CreateElement(BUTTON);
	sliderbutton->SetRect(SDL_Rect{ 958, 936, 10, 9 });
	sliderbutton->SetPos(450, 400, 332, 50);
	sliderbutton->childs.add(text1);

	//Label
	label = (Label*)scene->CreateElement(LABEL);
	label->SetRect(SDL_Rect{ 5, 118, 219, 58 });
	label->SetPos(250, 50, 332, 50);
	label->SetString("Helloo");
	label->SetTextPos(20, 20);
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// MOVEMENT ----------------------------
	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && text1->Is_Over() == true) || text1->IsMoving() == true)
	{
		int x, y;
		App->input->GetMouseMotion(x, y);
		text1->SetPos(text1->GetPos().x + x, text1->GetPos().y + y);
		text1->SetMovingTo(true);
	}
	
	else if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && button->Is_Over() == true) || button->IsMoving() == true)
	{
		int x, y;
		App->input->GetMouseMotion(x, y);
		button->MoveElement(x, y);
		//Lateral Scroll
		SDL_Rect r = { button->GetRect().x - x, button->GetRect().y, button->GetRect().w , button->GetRect().h };
		button->SetRect(r);
		//End lateral 
		button->SetMovingTo(true);
	}

	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP))
	{
		text1->SetMovingTo(false);
	}

	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP))
	{
		button->SetMovingTo(false);
	}
	//-------Slider
	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && sliderbutton->Is_Over() == true) || sliderbutton->IsMoving() == true)
	{
		int x, y;
		App->input->GetMouseMotion(x, y);
		sliderbutton->MoveElement(0, y);
		sliderbutton->SetMovingTo(true);
	}
	if ((App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP))
	{
		sliderbutton->SetMovingTo(false);
	}
	// STATIC ------------------------------------------------
	// Button
	
	if (button->Is_Over() == true)
	{
		button->SetRect(SDL_Rect{ 417, 172, 219, 58 });

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT || App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			button->SetRect(SDL_Rect{ 648, 172, 219, 58 });
		}
	}
	else
	{
		button->SetRect(SDL_Rect{ 6, 116, 219, 58 });
	}
	
	// Text1
	if (text1->Is_Over() == true)
	{
		text1->SetString("Mouse over");

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			text1->SetString("Left button");
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
		{
			text1->SetString("Right button");
		}
	}
	else
	{
		text1->SetString("Hello World");
	}

	//TextBox
	if (image->Is_Over() == true)
	{
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			textbox->SetRect(SDL_Rect{ 958, 936, 10, 9});
		}
	}
	
	// debug pathfing ---------------------------------------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Gui ---

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		App->render->camera.y += floor(200.0f * dt);
	}


	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->render->camera.y -= floor(200.0f * dt);
	}


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->render->camera.x += floor(200.0f * dt);
	}


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
	App->render->camera.x -= floor(200.0f * dt);
	}
	/*
	Mirare si el raton se encuentra encima de algun elemento,
	*/
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
