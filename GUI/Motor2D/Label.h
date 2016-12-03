#pragma once
#ifndef __Label_H__
#define __Label_H__

#include "j1Module.h"
#include "j1Gui.h"
class Label : public UI_Element
{
public:
	p2SString textInput;
	SDL_Rect box;
public:
	bool Draw();
	bool Update();
};

#endif // __Label_H__