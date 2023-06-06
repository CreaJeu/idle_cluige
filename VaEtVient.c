//#include <stdlib.h> //already in cluige.h
//#include <stddef.h> //already in cluige.h
#include <cluige.h>
#include "VaEtVient.h"


static void deleteVaEtVient(Script* this_Script)
{
    struct VaEtVient* thisVaEtVient = (struct VaEtVient*)(this_Script->_sub_class);
    free(thisVaEtVient);
    //free(this_Script);
    this_Script->delete_Script(this_Script);
}

static void vaEtVient_process(Script* this_Script, float elapsed_seconds)
{
    struct VaEtVient* thisVaEtVient = (struct VaEtVient*)(this_Script->_sub_class);
    Node2D* this_Node2D = thisVaEtVient->ownerSprite->_this_Node2D;

    //bidon
    #ifdef PRE_CREAJAM2
    static StringBuilder sb;
    if(display == NULL)
        display = iCluige.iStringBuilder.string_alloc(&sb, 14);
    iCluige.iStringBuilder.replace(&sb, "time:\n%2.1f\n ^^", bidon);
    iCluige.iSpriteText.set_text(thisVaEtVient->ownerSprite, display);
    //printf("yo %s\n", display);
    //mvaddstr(4, 7, display);
    #endif // PRE_CREAJAM2

    static char* display = NULL;
    static float bidon = 0;
    static Vector2 speed = (Vector2) { 15., 0.};
    static Vector2 depl;

    if(thisVaEtVient->ownerSprite->text == NULL)
        iCluige.iSpriteText.set_text(thisVaEtVient->ownerSprite,
                                    "xxxx\nx  x\nx  x\nxxxx");

    //Vector2 screenSize = iCluige.get_screen_size();
    float minX = 3.5;//screenSize.x * .1;
    float maxX = 46;//screenSize.x * .4;
//    if(this_Node2D->position.x < 5 || (this_Node2D->position.x - 9) > screenSize.x)
    if(this_Node2D->position.x < minX)
    {
        iCluige.iVector2.k_mul(&speed, -1., &speed);
        this_Node2D->position.x = minX;
        //this_Node2D->position.y = 9;
    }
    else if(this_Node2D->position.x > maxX)
    {
        iCluige.iVector2.k_mul(&speed, -1., &speed);
        this_Node2D->position.x = maxX;
    }

    iCluige.iVector2.k_mul(&speed, elapsed_seconds, &depl);
    iCluige.iNode2D.move_local(this_Node2D, depl);

    bidon += elapsed_seconds;
    if(bidon > 99)
    {
        iCluige.quit_asked = true;
    }
}

struct VaEtVient* newVaEtVient(Node* node)
{
    Script* new_Script = iCluige.iScript.new_Script(node);
    struct VaEtVient* newVaEtVient = iCluige.checked_malloc(sizeof(struct VaEtVient));

    newVaEtVient->_this_Script = new_Script;

    new_Script->node = node;
    new_Script->delete_Script = deleteVaEtVient;
//    new_Script->ready = vaEtVient_ready;
    new_Script->process = vaEtVient_process;
    new_Script->_sub_class = newVaEtVient;

    node->script = new_Script;

    return newVaEtVient;
}

