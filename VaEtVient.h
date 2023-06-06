#ifndef VaEtVient_H_INCLUDED
#define VaEtVient_H_INCLUDED

#include "cluige.h"

struct VaEtVient
{
    //shortcut
    SpriteText* ownerSprite;

	//private
	Script* _this_Script;

	//virtual methods
//	void (*delete_Script)(Script*);
//	void (*process)(Node*, float delta);
};
struct VaEtVient* newVaEtVient(Node* node);

#endif // VaEtVient_H_INCLUDED
