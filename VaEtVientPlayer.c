//#include <stdlib.h> //already in cluige.h
//#include <stddef.h> //already in cluige.h
#include <cluige.h>
#include "VaEtVientPlayer.h"


static void deleteVaEtVientPlayer(Script* this_Script)
{
    struct VaEtVientPlayer* thisVaEtVientPlayer =
        (struct VaEtVientPlayer*)(this_Script->_sub_class);
    free(thisVaEtVientPlayer);
    //free(this_Script);
    this_Script->delete_Script(this_Script);
}

static void vaEtVientPlayer_process(Script* this_Script, float elapsed_seconds)
{
    struct VaEtVientPlayer* thisVaEtVientPlayer =
        (struct VaEtVientPlayer*)(this_Script->_sub_class);
    Node2D* this_Node2D = thisVaEtVientPlayer->ownerSprite->_this_Node2D;

    static float accelY = 36.;
    static float speedY = 0.;

    speedY += accelY * elapsed_seconds;
    float deplY = speedY * elapsed_seconds;
    if(this_Node2D->position.y + deplY > -1)
    {
        speedY = -25.;//jump impulse
        iCluige.iNode2D.set_local_position(this_Node2D,
            (Vector2) { this_Node2D->position.x, -1. });
    }
    else
    {
        iCluige.iNode2D.move_local(this_Node2D, (Vector2) { 0., deplY });
    }
//    if(getch() == ' ')
//    {
//    }
}

struct VaEtVientPlayer* newVaEtVientPlayer(Node* node)
{
    Script* new_Script = iCluige.iScript.new_Script(node);
    struct VaEtVientPlayer* newVaEtVientPlayer = iCluige.checked_malloc(sizeof(struct VaEtVientPlayer));

    newVaEtVientPlayer->_this_Script = new_Script;

    new_Script->node = node;
    new_Script->delete_Script = deleteVaEtVientPlayer;
//    new_Script->ready = vaEtVient_ready;
    new_Script->process = vaEtVientPlayer_process;
    new_Script->_sub_class = newVaEtVientPlayer;

    node->script = new_Script;

    return newVaEtVientPlayer;
}

