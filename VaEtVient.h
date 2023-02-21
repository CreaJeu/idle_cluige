#ifndef VaEtVient_H_INCLUDED
#define VaEtVient_H_INCLUDED

#include "cluige.h"

struct VaEtVient
{
    //shortcut
    SpriteText* ownerSprite;

	//private
	Script* _thisScript;

	//virtual methods
//	void (*deleteScript)(Script*);
//	void (*process)(Node*, float delta);
};
struct VaEtVient* newVaEtVient(Node* node);

#endif // VaEtVient_H_INCLUDED
