//#include <stdlib.h> //already in cluige.h
//#include <stddef.h> //already in cluige.h
#include <cluige.h>
#include "VaEtVient.h"


static void deleteVaEtVient(Script* thisScript)
{
    struct VaEtVient* thisVaEtVient = (struct VaEtVient*)(thisScript->_subClass);
    free(thisVaEtVient);
    //free(thisScript);
    thisScript->deleteScript(thisScript);
}

static void vaEtVient_process(Script* thisScript, float elapsedSeconds)
{
    struct VaEtVient* thisVaEtVient = (struct VaEtVient*)(thisScript->_subClass);
    Node2D* thisNode2D = thisVaEtVient->ownerSprite->_thisNode2D;

    //bidon
    #ifdef PRE_CREAJAM2
    static StringBuilder sb;
    if(display == NULL)
        display = iCluige.iStringBuilder.stringAlloc(&sb, 14);
    iCluige.iStringBuilder.replace(&sb, "time:\n%2.1f\n ^^", bidon);
    iCluige.iSpriteText.setText(thisVaEtVient->ownerSprite, display);
    //printf("yo %s\n", display);
    //mvaddstr(4, 7, display);
    #endif // PRE_CREAJAM2

    static char* display = NULL;
    static float bidon = 0;
    static Vector2 speed = (Vector2) { 15., 0.};
    static Vector2 depl;

    if(thisVaEtVient->ownerSprite->text == NULL)
        iCluige.iSpriteText.setText(thisVaEtVient->ownerSprite,
                                    "xxxx\nx  x\nx  x\nxxxx");

    //Vector2 screenSize = iCluige.getScreenSize();
    float minX = 3.5;//screenSize.x * .1;
    float maxX = 46;//screenSize.x * .4;
//    if(thisNode2D->position.x < 5 || (thisNode2D->position.x - 9) > screenSize.x)
    if(thisNode2D->position.x < minX)
    {
        iCluige.iVector2.kMul(&speed, -1., &speed);
        thisNode2D->position.x = minX;
        //thisNode2D->position.y = 9;
    }
    else if(thisNode2D->position.x > maxX)
    {
        iCluige.iVector2.kMul(&speed, -1., &speed);
        thisNode2D->position.x = maxX;
    }

    iCluige.iVector2.kMul(&speed, elapsedSeconds, &depl);
    iCluige.iNode2D.moveLocal(thisNode2D, depl);

    bidon += elapsedSeconds;
    if(bidon > 99)
    {
        iCluige.quitAsked = true;
    }
}

struct VaEtVient* newVaEtVient(Node* node)
{
    Script* newScript = iCluige.iScript.newScript(node);
    struct VaEtVient* newVaEtVient = iCluige.checkedMalloc(sizeof(struct VaEtVient));

    newVaEtVient->_thisScript = newScript;

    newScript->node = node;
    newScript->deleteScript = deleteVaEtVient;
//    newScript->ready = vaEtVient_ready;
    newScript->process = vaEtVient_process;
    newScript->_subClass = newVaEtVient;

    node->script = newScript;

    return newVaEtVient;
}

