#ifndef VaEtVientPlayer_H_INCLUDED
#define VaEtVientPlayer_H_INCLUDED

#include "cluige.h"

struct VaEtVientPlayer
{
    //shortcut
    SpriteText* ownerSprite;

	//private
	Script* _thisScript;

	//virtual methods
//	void (*deleteScript)(Script*);
//	void (*process)(Node*, float delta);
};
struct VaEtVientPlayer* newVaEtVientPlayer(Node* node);

#endif // VaEtVientPlayer_H_INCLUDED
