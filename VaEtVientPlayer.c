//#include <stdlib.h> //already in cluige.h
//#include <stddef.h> //already in cluige.h
#include <cluige.h>
#include "VaEtVientPlayer.h"


static void deleteVaEtVientPlayer(Script* thisScript)
{
    struct VaEtVientPlayer* thisVaEtVientPlayer =
        (struct VaEtVientPlayer*)(thisScript->_subClass);
    free(thisVaEtVientPlayer);
    //free(thisScript);
    thisScript->deleteScript(thisScript);
}

static void vaEtVientPlayer_process(Script* thisScript, float elapsedSeconds)
{
    struct VaEtVientPlayer* thisVaEtVientPlayer =
        (struct VaEtVientPlayer*)(thisScript->_subClass);
    Node2D* thisNode2D = thisVaEtVientPlayer->ownerSprite->_thisNode2D;

    static float accelY = 36.;
    static float speedY = 0.;

    speedY += accelY * elapsedSeconds;
    float deplY = speedY * elapsedSeconds;
    if(thisNode2D->position.y + deplY > -1)
    {
        speedY = -25.;//jump impulse
        iCluige.iNode2D.setLocalPosition(thisNode2D,
            (Vector2) { thisNode2D->position.x, -1. });
    }
    else
    {
        iCluige.iNode2D.moveLocal(thisNode2D, (Vector2) { 0., deplY });
    }
//    if(getch() == ' ')
//    {
//    }
}

struct VaEtVientPlayer* newVaEtVientPlayer(Node* node)
{
    Script* newScript = iCluige.iScript.newScript(node);
    struct VaEtVientPlayer* newVaEtVientPlayer = iCluige.checkedMalloc(sizeof(struct VaEtVientPlayer));

    newVaEtVientPlayer->_thisScript = newScript;

    newScript->node = node;
    newScript->deleteScript = deleteVaEtVientPlayer;
//    newScript->ready = vaEtVient_ready;
    newScript->process = vaEtVientPlayer_process;
    newScript->_subClass = newVaEtVientPlayer;

    node->script = newScript;

    return newVaEtVientPlayer;
}

