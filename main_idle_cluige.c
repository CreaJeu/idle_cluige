#include <stdio.h>
#include <cluige.h>
#include <float.h>
#include <assert.h>
#include "VaEtVient.h"
#include "VaEtVientPlayer.h"

//if need (for debug) of SetConsoleOutputCP(CP_UTF8);
//#include <windows.h>

#include <math.h>


//#define IN_GAME_JAM_CREAJAM2
//#define IN_CREAJAM3
//#define IN_STUNJAM2023
//#define IN_GLOBALGAMEJAM2024
#define IN_OCTOJAM_2024

#ifdef IN_CREAJAM3
#include <../PDCurses/curses.h>
#include <math.h>

int RIGHT_ACTION;
int LEFT_ACTION;
int UP_ACTION;
int DOWN_ACTION;
int START_ACTION;
int QUIT_ACTION;

struct _Player
{
    SpriteText* ownerSprite;
	Script* this_Script;
	Vector2 speed;
	bool started;
};
typedef struct _Player Player;

void deletePlayer(Script* this_Script)
{
    //maybe one day
}

SpriteText* hudSpriteText;
static void process_Player(Script* this_Script, float elapsed_seconds)
{
    Player* thisPlayer =
        (Player*)(this_Script->_sub_class);
    Node2D* this_Node2D = thisPlayer->ownerSprite->_this_Node2D;
//    struct _Input* ii = iCluige.input;
    struct iiInput* iii = &iCluige.iInput;
    float weight = 2500;
    float impulseStrength = 4;

    if(iii->is_action_just_pressed(QUIT_ACTION))
    {
        iCluige.quit_asked = true;
        return;
    }
    if(!(thisPlayer->started))
    {
        if(iii->is_action_just_pressed(START_ACTION))//' '
        {
            iCluige.iNode2D.move_local(hudSpriteText->_this_Node2D, (Vector2){-5, -8});
            thisPlayer->started = true;
            thisPlayer->speed = (Vector2){impulseStrength * 2, 0};
        }
        return;
    }

    Vector2 gravity, impulse, dSpeed, dPos;
    Vector2* pos = &(this_Node2D->position);
    //planet center (hence gravity) is centered on local (0,0)
    gravity = *pos;
    float h = iCluige.iVector2.length(pos);
    // 1/h => normalize, 1/h² => gravity, 1/h^3.2 => fun
    if(h > 10 * FLT_EPSILON)
    {
        iCluige.iVector2.k_mul(&gravity, -weight/powf(h,3.2), &gravity);
    }

    iCluige.iVector2.set(&impulse, 0, 0);
    if(iii->is_action_just_pressed(UP_ACTION))
    {
        impulse.y -= impulseStrength;
    }
    if(iii->is_action_just_pressed(DOWN_ACTION))
    {
        impulse.y += impulseStrength;
    }
    if(iii->is_action_just_pressed(LEFT_ACTION))
    {
        impulse.x -= impulseStrength;
    }
    if(iii->is_action_just_pressed(RIGHT_ACTION))
    {
        impulse.x += impulseStrength;
    }

    iCluige.iVector2.k_mul(&gravity, elapsed_seconds, &dSpeed);
    iCluige.iVector2.add(&(thisPlayer->speed), &dSpeed, &(thisPlayer->speed));
    iCluige.iVector2.add(&(thisPlayer->speed), &impulse, &(thisPlayer->speed));

    iCluige.iVector2.set(&dPos, 0, 0);
    iCluige.iVector2.k_mul(&thisPlayer->speed, elapsed_seconds, &dPos);

    iCluige.iNode2D.move_local(this_Node2D, dPos);

}

Player* newPlayer(Node* this_Node)
{
    Script* new_Script = iCluige.iScript.new_Script(this_Node);
    Player* newPlayer = iCluige.checked_malloc(sizeof(Player));

    newPlayer->this_Script = new_Script;
    newPlayer->started = false;
    newPlayer->speed = (Vector2){0, 0};

    new_Script->node = this_Node;
    new_Script->delete_Script = deletePlayer;
    new_Script->process = process_Player;
    new_Script->_sub_class = newPlayer;

    this_Node->script = new_Script;

    Node2D* this_Node2D = (Node2D*)(this_Node->_sub_class);
    SpriteText* this_SpriteText = (SpriteText*)(this_Node2D->_sub_class);
	newPlayer->ownerSprite = this_SpriteText;
	iCluige.iNode2D.set_local_position(this_Node2D, (Vector2){-10,-20});

    return newPlayer;
}

#endif // NOT_IN_CREAJAM3

#ifdef IN_STUNJAM2023
#include <../PDCurses/curses.h>

int HIGHER_ACTION;
int LOWER_ACTION;
int WIDER_ACTION;
int NARROWER_ACTION;
int QUIT_ACTION;

struct _Player
{
	Script* this_Script;
    SpriteSVG* mouthUpSpriteSVG;
    SpriteSVG* mouthDownSpriteSVG;
    SpriteSVG* eyesSpriteSVG;
    float timeTillBlink;
    int blinkStep;
    float foe;
    int hitsTillFoe;
};
typedef struct _Player Player;
Player* newPlayer(Node* this_Node);

void deletePlayer(Script* this_Script)
{
    //maybe one day
}

static void process_Player(Script* this_Script, float elapsed_seconds)
{
    Player* thisPlayer =
        (Player*)(this_Script->_sub_class);
    struct iiInput* iii = &iCluige.iInput;
    float f = thisPlayer->foe;
    bool acted = false;

    if(iii->is_action_just_pressed(QUIT_ACTION))
    {
        iCluige.quit_asked = true;
        return;
    }
    if(iii->is_action_just_pressed(HIGHER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.y = 1 + (f * .5);
        thisPlayer->mouthDownSpriteSVG->scale.y = 1 + (f * .5);
        acted = true;
    }
    if(iii->is_action_just_pressed(LOWER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.y = 1 - (f * .5);
        thisPlayer->mouthDownSpriteSVG->scale.y = 1 - (f * .5);
        acted = true;
    }
    if(iii->is_action_just_pressed(WIDER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.x = 1 + (f * .5);
        thisPlayer->mouthDownSpriteSVG->scale.x = 1 + (f * .5);
        acted = true;
    }
    if(iii->is_action_just_pressed(NARROWER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.x = 1 - (f * .5);
        thisPlayer->mouthDownSpriteSVG->scale.x = 1 - (f * .5);
        acted = true;
    }

    if(f > 0)
    {
        if(acted)
        {
            if(thisPlayer->hitsTillFoe > 0)
            {
                thisPlayer->hitsTillFoe--;
            }
            else if(thisPlayer->hitsTillFoe == 0)
            {
                thisPlayer->hitsTillFoe = -1;
                Node2D* enemyNode2D = iCluige.iNode2D.new_Node2D();
                Node* enemyNode = enemyNode2D->_this_Node;
                iCluige.iNode.set_name(enemyNode, "Enemy");
                iCluige.iNode2D.move_local(enemyNode2D, (Vector2){99., 20.});
                iCluige.iNode.add_child(this_Script->node, enemyNode);
                newPlayer(enemyNode);
                Player* foe = (Player*)(enemyNode->script->_sub_class);
                foe->foe = -1;
            }
        }
    }
    else
    {
        Node2D* this_Node2D = (Node2D*)(this_Script->node->_sub_class);
        if(this_Node2D->position.x > 65)
        {
            iCluige.iNode2D.move_local(this_Node2D, (Vector2){-30 * elapsed_seconds, 0});
        }
    }


    float x_speed = 2;
    float y_speed = 2;
    float adv = (thisPlayer->mouthUpSpriteSVG->scale.x - 1);
    if(adv < 0)
    {
        thisPlayer->mouthUpSpriteSVG->scale.x += 1 * x_speed * elapsed_seconds;
        thisPlayer->mouthDownSpriteSVG->scale.x += x_speed * elapsed_seconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.x > 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.x = 1;
        }
    }
    else if(adv > 0)
    {
        thisPlayer->mouthUpSpriteSVG->scale.x -= 1 * x_speed * elapsed_seconds;
        thisPlayer->mouthDownSpriteSVG->scale.x -= x_speed * elapsed_seconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.x < 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.x = 1;
        }
    }

    adv = (thisPlayer->mouthUpSpriteSVG->scale.y - 1);
    if(adv < 0)
    {
        thisPlayer->mouthUpSpriteSVG->scale.y += y_speed * elapsed_seconds;
        thisPlayer->mouthDownSpriteSVG->scale.y += y_speed * elapsed_seconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.y > 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.y = 1;
        }
    }
    else if(adv > 0)
    {
        thisPlayer->mouthUpSpriteSVG->scale.y -= y_speed * elapsed_seconds;
        thisPlayer->mouthDownSpriteSVG->scale.y -= y_speed * elapsed_seconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.y < 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.y = 1;
        }
    }

    if(thisPlayer->timeTillBlink > 0)
    {
        thisPlayer->timeTillBlink -= elapsed_seconds;
        if(thisPlayer->blinkStep == 0)
        {
            thisPlayer->eyesSpriteSVG->scale.y = 1;
        }
        else if(thisPlayer->blinkStep == 1)
        {
            //close
            thisPlayer->eyesSpriteSVG->scale.y -= 5 * elapsed_seconds;
        }
        else
        {
            //open
            thisPlayer->eyesSpriteSVG->scale.y += 3 * elapsed_seconds;
        }
    }
    else
    {
        if(thisPlayer->blinkStep == 0)
        {
            thisPlayer->timeTillBlink = .2;
            thisPlayer->blinkStep = 1;
        }
        else if(thisPlayer->blinkStep == 1)
        {
            thisPlayer->timeTillBlink = .333;
            thisPlayer->blinkStep = 2;
        }
        else
        {
            thisPlayer->timeTillBlink = 4;
            thisPlayer->blinkStep = 0;
        }
    }
}

Player* newPlayer(Node* this_Node)
{
    Script* new_Script = iCluige.iScript.new_Script(this_Node);
    Player* newPlayer = iCluige.checked_malloc(sizeof(Player));

    newPlayer->this_Script = new_Script;
    newPlayer->blinkStep = 0;
    newPlayer->timeTillBlink = 3;
    newPlayer->foe = 1;
    newPlayer->hitsTillFoe = 5;

    new_Script->node = this_Node;
    new_Script->delete_Script = deletePlayer;
    new_Script->process = process_Player;
    new_Script->_sub_class = newPlayer;
    this_Node->script = new_Script;

	SpriteSVG* playerMouthUpSpriteSVG = iCluige.iSpriteSVG.new_SpriteSVG();
	Node* MouthUp = playerMouthUpSpriteSVG->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(MouthUp, "PlayerMouthUp");
	iCluige.iNode.add_child(this_Node, MouthUp);
//	playerMouthUpSpriteSVG->scale = (Vector2){3, 1};
	Vector2 essayPath[11] =
        {
            (Vector2){-18, -9},
            (Vector2){-17.9, -6},
            (Vector2){-12, 0},
            (Vector2){-6, 0},
            (Vector2){0, -6},
            (Vector2){0, -7},
            (Vector2){0, -5},
            (Vector2){6, 0},
            (Vector2){13, 0},
            (Vector2){19, -6},
            (Vector2){19, -9}
        };
    iCluige.iSpriteSVG.add_path_from_array(playerMouthUpSpriteSVG, essayPath, 11);

	SpriteSVG* playerMouthDownSpriteSVG = iCluige.iSpriteSVG.new_SpriteSVG();
	Node* mouthDown = playerMouthDownSpriteSVG->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(mouthDown, "PlayerMouthDown");
	iCluige.iNode.add_child(this_Node, mouthDown);
	Vector2 mouthDownPath[6] =
        {
            (Vector2){-8.9324678, 1.0217732},
            (Vector2){-8.9, 3.9805214},
            (Vector2){-4, 8.9},
            (Vector2){3, 8.9},
            (Vector2){8.8, 3.9805214},
            (Vector2){8.9324678, 1.0217732},
        };
    iCluige.iSpriteSVG.add_path_from_array(playerMouthDownSpriteSVG, mouthDownPath, 6);

    SpriteSVG* playerFaceSpriteSVG = iCluige.iSpriteSVG.new_SpriteSVG();
	Node* face = playerFaceSpriteSVG->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(face, "PlayerFace");
	iCluige.iNode.add_child(this_Node, face);
	Vector2 path4523[2] =
        {
            (Vector2){11.963193,-11.829885},
            (Vector2){19.777945,-5.47902}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(playerFaceSpriteSVG, path4523, 2);
	Vector2 path4525[2] =
        {
            (Vector2){14.635889,-9.825365},
            (Vector2){15.234362,-1.33634}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(playerFaceSpriteSVG, path4525, 2);
	Vector2 path4527[2] =
        {
            (Vector2){-11.289256,-11.161705},
            (Vector2){-17.238884,-5.07813}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(playerFaceSpriteSVG, path4527, 2);
	Vector2 path4529[2] =
        {
            (Vector2){-13.961951,-9.023555},
            (Vector2){-12.02713,-1.73725}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(playerFaceSpriteSVG, path4529, 2);

	Node2D* eyesNode2D = iCluige.iNode2D.new_Node2D();
	Node* eyesNode = eyesNode2D->_this_Node;
	iCluige.iNode.set_name(eyesNode, "eyes2d");
	iCluige.iNode2D.move_local(eyesNode2D, (Vector2){0, -24.});
	iCluige.iNode.add_child(this_Node, eyesNode);

	SpriteSVG* eyesSpriteSVG = iCluige.iSpriteSVG.new_SpriteSVG();
	Node* eyes = eyesSpriteSVG->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(eyes, "eyesSVG");
	iCluige.iNode.add_child(eyesNode, eyes);
	Vector2 pupil1[2] =
        {
            (Vector2){-14,5},
            (Vector2){0,-9}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(eyesSpriteSVG, pupil1, 2);
	Vector2 pupil2[2] =
        {
            (Vector2){14,5},
            (Vector2){0,-9}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(eyesSpriteSVG, pupil2, 2);

	Vector2 path4506[7] =
        {
            (Vector2){-22.842508,-.6},
            (Vector2){4.92332,-5.26367},
            (Vector2){7.32625,0.3433},
            (Vector2){4.1753534,6.56958},
            (Vector2){-4.1852534,4.47433},
            (Vector2){-7.725532,-0.14849},
            (Vector2){-5,-7}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(eyesSpriteSVG, path4506, 7);
	Vector2 path4506_9_2_7[7] =
        {
            (Vector2){22.842508,-.6},
            (Vector2){-4.92332,-5.26367},
            (Vector2){-7.32625,0.3433},
            (Vector2){-4.1753534,6.56958},
            (Vector2){4.1852534,4.47433},
            (Vector2){7.725532,-0.14849},
            (Vector2){5,-7}
        };
    iCluige.iSpriteSVG.add_path_from_array_relative(eyesSpriteSVG, path4506_9_2_7, 7);

	newPlayer->mouthUpSpriteSVG = playerMouthUpSpriteSVG;
	newPlayer->mouthDownSpriteSVG = playerMouthDownSpriteSVG;
	newPlayer->eyesSpriteSVG = eyesSpriteSVG;

    return newPlayer;
}

#endif // IN_STUNJAM2023


void debug_deque(const Deque* d)
{
	StringBuilder sb_dq;
	int N = iCluige.iDeque.size(d);
	int sizeDequeMsg = N * 8;//quick & dirty test
	char* dqMsg = iCluige.iStringBuilder.string_alloc(&sb_dq, sizeDequeMsg);
	iCluige.iStringBuilder.append(&sb_dq, "[ ");
	for(int iDq=0; iDq < N; iDq++)
    {
        uint64_t val = iCluige.iDeque.at(d, iDq).ui64;
        iCluige.iStringBuilder.append(&sb_dq, "%d ", val);//"%.1f "
    }
    iCluige.iStringBuilder.append(&sb_dq, "]");
	free(dqMsg);
}

void debug_dico(const SortedDictionary* d)
{
	StringBuilder sb_dq;
	int N = iCluige.iSortedDictionary.size(d);
	int sizeDequeMsg = (N + 1) * 20;//quick & dirty test
	char* dqMsg = iCluige.iStringBuilder.string_alloc(&sb_dq, sizeDequeMsg);
	iCluige.iStringBuilder.append(&sb_dq, "[ ");
	for(int iDq=0; iDq < N; iDq++)
    {
//        Pair* p = (Pair*)(iCluige.iDeque.at(&(d->_pairs), iDq).ptr);
        Variant var_pair = iCluige.iSortedDictionary.at(d, iDq);
        Pair* p = (Pair*)(var_pair.ptr);
        uint64_t k = p->first.ui64;
        char* v = (char*)(p->second.ptr);
        iCluige.iStringBuilder.append(&sb_dq, "%d:%s ", k, v);//"%.1f "
    }
    iCluige.iStringBuilder.append(&sb_dq, "]");
	free(dqMsg);
}

//int jj(int i, va_list args)
//{
//    int j = va_arg(args, int);
//    return j;
//}
//
//void jandk(int i, ...)
//{
//    va_list args;
//    va_start(args, i);
//    int j = jj(i, args);
//    va_arg(args, int);//needed even if jj() calls it too
//    int k = jj(i, args);
//    va_end(args);
//    return;
//}


static void test_utils_bool_from_parsed()
{
	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 3);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);
    bool res = true;
    utils_bool_from_parsed(&res, &placeholder_dico, "test_bool");
	if(!res)
    {
        printf("FAILED --- should be true  | test_utils_bool_from_parsed 1\n ");
    }

    res = false;
    utils_bool_from_parsed(&res, &placeholder_dico, "test_bool");
	if(res)
    {
        printf("FAILED --- should be false  | test_utils_bool_from_parsed 2\n ");
    }

    res = false;
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test_bool", "true");
    utils_bool_from_parsed(&res, &placeholder_dico, "test_bool");
	if(!res)
    {
        printf("FAILED --- should be true  | test_utils_bool_from_parsed 3\n ");
    }

    res = true;
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test_bool", "false");
    utils_bool_from_parsed(&res, &placeholder_dico, "test_bool");
	if(res)
    {
        printf("FAILED --- should be false  | test_utils_bool_from_parsed 4\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
}

static void test_utils_char_from_parsed()
{
	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 3);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);
    char res = 'a';
    utils_char_from_parsed(&res, &placeholder_dico, "test");
	if(res != 'a')
    {
        printf("FAILED --- should be 'a'  | utils_char_from_parsed 1\n ");
    }

    res = 'z';
    utils_char_from_parsed(&res, &placeholder_dico, "test");
	if(res != 'z')
    {
        printf("FAILED --- should be 'z'  | utils_char_from_parsed 2\n ");
    }

    res = 'a';
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test", "z");
    utils_char_from_parsed(&res, &placeholder_dico, "test");
	if(res != 'z')
    {
        printf("FAILED --- should be 'z'  | utils_char_from_parsed 3\n ");
    }

    res = 'z';
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test", "a");
    utils_char_from_parsed(&res, &placeholder_dico, "test");
	if(res != 'a')
    {
        printf("FAILED --- should be 'a'  | utils_char_from_parsed 4\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
}


static void test_utils_str_from_parsed()
{
	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 3);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);
    char* res = "azertyuiop";
    utils_str_from_parsed(&res, &placeholder_dico, "test");
	if(0 != strcmp(res, "azertyuiop"))
    {
        printf("FAILED --- should be \"azertyuiop\"  | utils_str_from_parsed 1\n ");
    }

    res = "z";
    utils_str_from_parsed(&res, &placeholder_dico, "test");
	if(0 != strcmp(res, "z"))
    {
        printf("FAILED --- should be \"z\"  | utils_str_from_parsed 2\n ");
    }

    res = "a";
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test", "\"poiuytrez\"");
    utils_str_from_parsed(&res, &placeholder_dico, "test");
	if(0 != strcmp(res, "poiuytrez"))
    {
        printf("FAILED --- should be \"poiuytrez\"  | utils_str_from_parsed 3\n ");
    }

    res = "z";
	iCluige.iSortedDictionary.insert(&placeholder_dico, "test", "\"azertyui\"");
    utils_str_from_parsed(&res, &placeholder_dico, "test");
	if(0 != strcmp(res, "azertyui"))
    {
        printf("FAILED --- should be \"azertyui\"  | utils_str_from_parsed 4\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
}

static void test_Node_instanciate()
{
	SortedDictionary* fcties = &(iCluige.iNode.node_factories);
	Checked_Variant cv_fcty = iCluige.iSortedDictionary.get(fcties, "Node");
	if(!(cv_fcty.valid))
    {
        printf("FAILED --- should be valid  | test_Node_instanciate 0\n ");
    }
    NodeFactory* fcty = (NodeFactory*)(cv_fcty.v.ptr);
	if((fcty->instanciate) == NULL)
    {
        printf("FAILED --- should be a function  | test_Node_instanciate 1\n ");
    }
	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 3);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);

//    Node* res = iCluige.iNode.new_Node();
//    iCluige.iNode.deserialize_dico(res, &placeholder_dico);
//    Node* res = fcty->instanciate(&placeholder_dico); ::must assert(missing name)

    //Node* res2 = iCluige.iNode.new_Node();
	iCluige.iSortedDictionary.insert(&placeholder_dico, "name", "\"azertyuiop\"");
    //iCluige.iNode.deserialize_dico(res2, &placeholder_dico);
    Node* res2 = fcty->instanciate(&placeholder_dico);
	if(res2 == NULL || 0 != strcmp(res2->name, "azertyuiop"))
    {
        printf("FAILED --- should be \"azertyuiop\"  | test_Node_instanciate 2\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
//    res->delete_Node(res);//calls free(res);
    res2->delete_Node(res2);//calls free(res2);
}

static void test_Node2D_instanciate()
{
	SortedDictionary* fcties = &(iCluige.iNode.node_factories);
	Checked_Variant cv_fcty = iCluige.iSortedDictionary.get(fcties, "Node2D");
	if(!(cv_fcty.valid))
    {
        printf("FAILED --- should be valid  | test_Node2D_instanciate 0\n ");
    }
    NodeFactory* fcty = (NodeFactory*)(cv_fcty.v.ptr);
	if((fcty->instanciate) == NULL)
    {
        printf("FAILED --- should be a function  | test_Node2D_instanciate 1\n ");
    }

	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 3);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);
	iCluige.iSortedDictionary.insert(&placeholder_dico, "name", "\"test_Node2D_instanciate\"");

//    Node2D* res = iCluige.iNode2D.new_Node2D();
//    iCluige.iNode2D.deserialize_dico(res, &placeholder_dico);
    Node2D* res = (Node2D*)(fcty->instanciate(&placeholder_dico)->_sub_class);
    res->_this_Node->delete_Node(res->_this_Node);//calls free(res) and recursiv

	iCluige.iSortedDictionary.insert(&placeholder_dico, "visible", "false");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "position", "Vector2(2.265, -3.2)");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "name", "\"a 1 23 4567890bcdefghijklmnopqrst\"");
//    Node2D* res2 = iCluige.iNode2D.new_Node2D();
//    iCluige.iNode2D.deserialize_dico(res2, &placeholder_dico);
    Node2D* res2 = (Node2D*)(fcty->instanciate(&placeholder_dico)->_sub_class);
	if(0 != strcmp(res2->_this_Node->name, "a 1 23 4567890bcdefghijklmnopqrst"))
    {
        printf("FAILED --- should be \"a 1 23 4567890bcdefghijklmnopqrst\"  | test_Node2D_deserialize_dico 2\n ");
    }
	if(res2->visible)
    {
        printf("FAILED --- should be false  | test_Node2D_deserialize_dico 3\n ");
    }
    float gap = fabs(res2->position.x - 2.265);
	if(gap > .0001)
    {
        printf("FAILED --- should be 2.265  | test_Node2D_deserialize_dico 4\n ");
    }
    gap = fabs(res2->position.y - -3.2);
	if(gap > .0001)
    {
        printf("FAILED --- should be -3.2  | test_Node2D_deserialize_dico 5\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
    res2->_this_Node->delete_Node(res2->_this_Node);//calls free(res2) and recursiv
}

static void test_SpriteText_instanciate()
{
	SortedDictionary* fcties = &(iCluige.iNode.node_factories);
	Checked_Variant cv_fcty = iCluige.iSortedDictionary.get(fcties, "Label");
	if(!(cv_fcty.valid))
    {
        printf("FAILED --- SpriteText not in factories  | test_SpriteText_instanciate 0\n ");
    }
    NodeFactory* fcty = (NodeFactory*)(cv_fcty.v.ptr);
	if((fcty->instanciate) == NULL)
    {
        printf("FAILED --- uninitialized SpriteText factory function  | test_SpriteText_instanciate 1\n ");
    }

	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 4);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);

//    must assert / seg fault
//    SpriteText* res = iCluige.iSpriteText.new_SpriteText();
//    iCluige.iSpriteText.deserialize_dico(res, &placeholder_dico);
//    res->_this_Node2D->_this_Node->delete_Node(res->_this_Node2D->_this_Node);//calls free(res) and recursiv

	iCluige.iSortedDictionary.insert(&placeholder_dico, "aaaaaaaaaa", "fake to test");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "text", "\"un es\"sai\n de te\"xte\n\nmultiligne\"");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "offset", "Vector2(-49.3, 42)");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "visible", "false");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "position", "Vector2(2.265, -3.2)");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "name", "\"a sprite text\"");
//    SpriteText* res2 = iCluige.iSpriteText.new_SpriteText();
    //iCluige.iSpriteText.deserialize_dico(res2, &placeholder_dico);
    Node* res2_n = fcty->instanciate(&placeholder_dico);
    Node2D* res2_n2d = (Node2D*)(res2_n->_sub_class);
    SpriteText* res2 = (SpriteText*)(res2_n2d->_sub_class);
	if(0 != strcmp(res2->_this_Node2D->_this_Node->name, "a sprite text"))
    {
        printf("FAILED --- should be \"a sprite text\"  | test_SpriteText_instanciate 2\n ");
    }
	if(res2->_this_Node2D->visible)
    {
        printf("FAILED --- should be false  | test_SpriteText_instanciate 3\n ");
    }
    float gap = fabs(res2->_this_Node2D->position.x - 2.265);
	if(gap > .0001)
    {
        printf("FAILED --- should be 2.265  | test_SpriteText_instanciate 4\n ");
    }
    gap = fabs(res2->_this_Node2D->position.y - -3.2);
	if(gap > .0001)
    {
        printf("FAILED --- should be -3.2  | test_SpriteText_instanciate 5\n ");
    }
    gap = fabs(res2->offset.x - -49.3);
	if(gap > .0001)
    {
        printf("FAILED --- should be -49.3  | test_SpriteText_instanciate 6\n ");
    }
    gap = fabs(res2->offset.y - 42);
	if(gap > .0001)
    {
        printf("FAILED --- should be 42  | test_SpriteText_instanciate 7\n ");
    }
	if(0 != strcmp(res2->text, "un es\"sai\0 de te\"xte\0\0multiligne"))
    {
        printf("FAILED --- should be \"un es\"sai...\"  | test_SpriteText_instanciate 8\n ");
    }
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
    res2->_this_Node2D->_this_Node->delete_Node(res2->_this_Node2D->_this_Node);//calls free(res2) and recursiv
}

static void test_SpriteSVG_instanciate()
{
	SortedDictionary* fcties = &(iCluige.iNode.node_factories);
	Checked_Variant cv_fcty = iCluige.iSortedDictionary.get(fcties, "Sprite2D");
	if(!(cv_fcty.valid))
    {
        printf("FAILED --- SpriteSVG not in factories  | test_SpriteSVG_instanciate\n ");
    }
    NodeFactory* fcty = (NodeFactory*)(cv_fcty.v.ptr);
	if((fcty->instanciate) == NULL)
    {
        printf("FAILED --- uninitialized SpriteSVG factory function  | test_SpriteSVG_instanciate\n ");
    }

	SortedDictionary placeholder_dico;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&placeholder_dico, VT_POINTER, VT_POINTER, 7);
    iCluige.iSortedDictionary.set_compare_keys_func(&placeholder_dico, iCluige.iDeque.default_compare_string_func);

//    must assert / seg fault
//    SpriteSVG* res = iCluige.iSpriteText.new_SpriteSVG();
//    iCluige.iSpriteSVG.deserialize_dico(res, &placeholder_dico);
//    res->_this_Node2D->_this_Node->delete_Node(res->_this_Node2D->_this_Node);//calls free(res) and recursiv

	iCluige.iSortedDictionary.insert(&placeholder_dico, "scale", "Vector2(-1.45, .66)");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "zzzzzz", "fake to test");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "svg_file_path", "\"./stunjam2023eyes.svg\"");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "offset", "Vector2(-49.3, 42)");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "name", "\"a sprite svg\"");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "visible", "false");
	iCluige.iSortedDictionary.insert(&placeholder_dico, "position", "Vector2(2.265, -3.2)");
//    SpriteSVG* res2 = iCluige.iSpriteSVG.new_SpriteSVG();
//    iCluige.iSpriteSVG.deserialize_dico(res2, &placeholder_dico);
    Node* res2_n = fcty->instanciate(&placeholder_dico);
    Node2D* res2_n2d = (Node2D*)(res2_n->_sub_class);
    SpriteSVG* res2 = (SpriteSVG*)(res2_n2d->_sub_class);
    //SpriteSVG* res2 = (SpriteSVG*)(fcty->instanciate(&placeholder_dico)->_sub_class);
	if(0 != strcmp(res2->_this_Node2D->_this_Node->name, "a sprite svg"))
    {
        printf("FAILED --- should be \"a sprite svg\"  | test_SpriteSVG_instanciate 1\n ");
    }
	if(res2->_this_Node2D->visible)
    {
        printf("FAILED --- should be false  | test_SpriteSVG_instanciate 2\n ");
    }
    float gap = fabs(res2->_this_Node2D->position.x - 2.265);
	if(gap > .0001)
    {
        printf("FAILED --- should be 2.265  | test_SpriteSVG_instanciate 3\n ");
    }
    gap = fabs(res2->_this_Node2D->position.y - -3.2);
	if(gap > .0001)
    {
        printf("FAILED --- should be -3.2  | test_SpriteSVG_instanciate 4\n ");
    }
    gap = fabs(res2->offset.x - -49.3);
	if(gap > .0001)
    {
        printf("FAILED --- should be -49.3  | test_SpriteSVG_instanciate 5\n ");
    }
    gap = fabs(res2->offset.y - 42);
	if(gap > .0001)
    {
        printf("FAILED --- should be 42  | test_SpriteSVG_instanciate 6\n ");
    }
    gap = fabs(res2->scale.x - -1.45);
	if(gap > .0001)
    {
        printf("FAILED --- should be -1.45  | test_SpriteSVG_instanciate 7\n ");
    }
    gap = fabs(res2->scale.y - .66);
	if(gap > .0001)
    {
        printf("FAILED --- should be .66  | test_SpriteSVG_instanciate 8\n ");
    }
    Path2D* path = (Path2D*)(iCluige.iDeque.at(&(res2->paths), 0).ptr);
    Vector2* point0 = (Vector2*)(iCluige.iDeque.at(&(path->_points), 0).ptr);
    utils_breakpoint_trick(point0, false);

    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&placeholder_dico);
    res2->_this_Node2D->_this_Node->delete_Node(res2->_this_Node2D->_this_Node);//calls free(res2) and recursiv
}

static void test_FileLineReader()
{
	FileLineReader flr;
	iCluige.iFileLineReader.fileLineReader_alloc(&flr, 100);
	bool ok = iCluige.iFileLineReader.open_file_start_reader(&flr, "no");
	if(ok)
	{
		printf("FAILED --- should be flase  | test_FileLineReader 1\n ");
	}
	ok = iCluige.iFileLineReader.open_file_start_reader(&flr, "test_corbeau.txt");
	if(!ok)
	{
		printf("FAILED --- should be true  | test_FileLineReader 2\n ");
	}
	ok = iCluige.iFileLineReader.feof(&flr, 0);
	if(ok)
	{
		printf("FAILED --- should be flase  | test_FileLineReader 3\n ");
	}
	const char* line = iCluige.iFileLineReader.get_line(&flr, 0);
	if(strcmp("Maître corbeau, sur un arbre perché,\n", line))
	{
		printf("FAILED --- should be \"Maître corbeau, sur un arbre perché,\n\"  | test_FileLineReader 4\n ");
	}
	//printf(line);
	int i = 1;
	while(!iCluige.iFileLineReader.feof(&flr, i))
	{
		line = iCluige.iFileLineReader.get_line(&flr, i);
		//printf(line);
		i++;
	}
	iCluige.iFileLineReader.forget_lines_before(&flr, i-3);//line # i is preserved
	//line = iCluige.iFileLineReader.get_line(&flr, i-4);//must assert
	//printf("\n-------------\n");
	for(int c = i-3; c < i; c++)
	{
		line = iCluige.iFileLineReader.get_line(&flr, c);
		//printf(line);
	}
	line = iCluige.iFileLineReader.get_line(&flr, i+1);
	if(line != NULL)
	{
		printf("FAILED --- should be NULL  | test_FileLineReader 5\n ");
	}
	line = iCluige.iFileLineReader.get_line(&flr, i+2);
	if(line != NULL)
	{
		printf("FAILED --- should be NULL  | test_FileLineReader 5.2\n ");
	}
	iCluige.iFileLineReader.close_file(&flr);

	//other file
	//printf("\n++++++++++++++++\n");
	ok = iCluige.iFileLineReader.open_file_start_reader(&flr, "azerty.tscn");//"test_tscn_parser.tscn");//
	if(!ok)
	{
		printf("FAILED --- should be true  | test_FileLineReader 6\n ");
	}
	i = 0;
	while(!iCluige.iFileLineReader.feof(&flr, i))
	{
		line = iCluige.iFileLineReader.get_line(&flr, i);
		//printf(line);
		iCluige.iFileLineReader.forget_lines_before(&flr, i);
		i++;
	}
	iCluige.iFileLineReader.close_file(&flr);
	iCluige.iFileLineReader.pre_delete_FileLineReader(&flr);
	//printf("\n end FileLineReader test \n");
}

static void test_TscnParser()
{
	TscnParser parser;
	iCluige.iTscnParser.tscn_parser_alloc(&parser, "test_TscnParser.tscn");//"autre_instancieur.tscn");//"test_tscn_parser.tscn");//"main.tscn = avec instanced = todo later");//
//	bool ok = parser.read_line(&parser);
//	ok = parser.read_line(&parser);
//	ok = parser.ignore_non_node(&parser);
//	ok = parser.node(&parser);
//	ok = parser.node(&parser);
	bool ok = parser.parse_scene(&parser);
	char* dbg = iCluige.iPackedScene.debug_recrusive(parser.scene_root, NULL);
//	utils_cluige_printf(0, 0, dbg);
	if(!ok)
	{
		printf("FAILED --- should be true  | test_TscnParser %d\n ", 1);
	}
	if(dbg == NULL)
	{
		printf("FAILED --- should be not null  | test_TscnParser %d\n ", 2);
	}
	utils_breakpoint_trick(&ok, false);
	utils_breakpoint_trick(dbg, false);
//	printf("%s\n\n", dbg);
	if(false)
	{ //don't re-generate unless wanted evolution in those test results
		FILE* bkp = fopen("test_TscnParser__wanted_res.txt", "w");
		fprintf(bkp, dbg);
		fclose(bkp);
	}
	iCluige.iTscnParser.pre_delete_TscnParser(&parser);
	FileLineReader wanted_res_flr;
	iCluige.iFileLineReader.fileLineReader_alloc(&wanted_res_flr, 200);
	iCluige.iFileLineReader.open_file_start_reader(&wanted_res_flr, "test_TscnParser__wanted_res.txt");
	int i = 0;
	char sub_dbg[200];
	int dbg_cursor = 0;
	while(!iCluige.iFileLineReader.feof(&wanted_res_flr, i))
	{
		const char* wanted_line = iCluige.iFileLineReader.get_line(&wanted_res_flr, i);
		size_t line_len = strcspn(dbg + dbg_cursor, "\n") + 1;
		assert(line_len < 199);
		strncpy(sub_dbg, dbg + dbg_cursor, line_len);
		*(sub_dbg + line_len) = '\0';
		if(strcmp(wanted_line, sub_dbg) != 0)
		{
			utils_breakpoint_trick(sub_dbg, true);
			printf("FAILED --- parsing inconsistant with last known in test_TscnParser__wanted_res.txt line %d  | test_TscnParser \n ", i+1);
			printf("\t%s \t\tshould be \n\t%s\n", sub_dbg, wanted_line);
		}
		iCluige.iFileLineReader.forget_lines_before(&wanted_res_flr, i);
		dbg_cursor += line_len;
		i++;
	}
	iCluige.iFileLineReader.close_file(&wanted_res_flr);
	iCluige.iFileLineReader.pre_delete_FileLineReader(&wanted_res_flr);
	iCluige.iPackedScene.pre_delete_PackedScene(parser.scene_root);
	free(parser.scene_root);
	free(dbg);
}

static const Node* test_node_against_packed_scene(const Node* n, const PackedScene* ps)
{
	bool found;
	const char* ps_type = ps->type;
	if(strcmp(ps_type, "Node") == 0)
	{
		if(strcmp(ps->name, n->name) != 0)
		{
			return n;
		}
	}
	const Node2D* n_2D = (const Node2D*)(n->_sub_class);
	if(strncmp(n->_class_name, "NodeNode2D", 10) == 0)
	{
		bool ps_visible;
		found = utils_bool_from_parsed(&ps_visible, &(ps->dico_node), "visible");
		if(found)
		{
			if(ps_visible != n_2D->visible)
			{
				return n;
			}
		}
		Vector2 ps_pos;
		found = utils_vector2_from_parsed(&ps_pos, &(ps->dico_node), "position");
		if(found)
		{
			if(iCluige.iVector2.distance_squared_to(&ps_pos, &(n_2D->position)) > iCluige.EPSILON)
			{
				return n;
			}
		}
	}
	if(strncmp(n->_class_name, "NodeNode2DSpriteText", 10) == 0)
	{
		const SpriteText* n_SprtTxt = (const SpriteText*)(n_2D->_sub_class);
		Vector2 ps_offset;
		found = utils_vector2_from_parsed(&ps_offset, &(ps->dico_node), "offset");
		if(found)
		{
			if(iCluige.iVector2.distance_squared_to(&ps_offset, &(n_SprtTxt->offset)) > iCluige.EPSILON)
			{
				return n;
			}
		}
		char* ps_txt;
		found = utils_str_from_parsed(&ps_txt, &(ps->dico_node), "text");
		if(found)
		{
			int first_line_len = strlen(n_SprtTxt->text);
			if(strncmp(ps_txt, n_SprtTxt->text, first_line_len) != 0)
			{
				return n;
			}
		}
	}
	else if(strncmp(n->_class_name, "NodeNode2DSpriteSVG", 10) == 0)
	{
		const SpriteSVG* n_SprtSVG = (const SpriteSVG*)(n_2D->_sub_class);
		Vector2 ps_offset;
		found = utils_vector2_from_parsed(&ps_offset, &(ps->dico_node), "offset");
		if(found)
		{
			if(iCluige.iVector2.distance_squared_to(&ps_offset, &(n_SprtSVG->offset)) > iCluige.EPSILON)
			{
				return n;
			}
		}
		Vector2 ps_scale;
		found = utils_vector2_from_parsed(&ps_scale, &(ps->dico_node), "scale");
		if(found)
		{
			if(iCluige.iVector2.distance_squared_to(&ps_scale, &(n_SprtSVG->scale)) > iCluige.EPSILON)
			{
				return n;
			}
		}
	}
	utils_breakpoint_trick(ps, 00=="warning : found type is not (yet) tested in PackedScene/Node");
	return NULL;
}

static const Node* test_node_against_packed_scene_tree(const Node* n, const PackedScene* ps)
{
	const Node* ok = test_node_against_packed_scene(n, ps);
	if(ok != NULL)
	{
		return ok;
	}
	int nb_chd =iCluige.iDeque.size(&(ps->children));
	for(int i = 0; i < nb_chd; i++)
	{
		const PackedScene* child_ps = (const PackedScene*)(iCluige.iDeque.at(&(ps->children), i).ptr);
		const Node* child_nde = iCluige.iNode.get_child(n , i);
		const Node* ook = test_node_against_packed_scene_tree(child_nde, child_ps);
		if(ook != NULL)
		{
			return ook;
		}
	}
	return NULL;
}

static void test_pksc_instanciate()
{
	TscnParser parser;
	iCluige.iTscnParser.tscn_parser_alloc(&parser, "test_pksc_instanciate.tscn");
	bool ok = parser.parse_scene(&parser);
	utils_breakpoint_trick(&ok, false);
//	char* dbg = iCluige.iPackedScene.debug_recrusive(parser.scene_root, NULL);
	PackedScene* ps = parser.scene_root;
	Node* my_game_root_node = iCluige.iPackedScene.instanciate(ps);
	iCluige.iNode.add_child(iCluige.public_root_2D, my_game_root_node);
	iCluige.iNode.print_tree_pretty(iCluige.public_root_2D);

//	Node2D* zzzzzzz = (Node2D*)(my_game_root_node->_sub_class);
//	iCluige.iNode2D.move_local(zzzzzzz, (Vector2){ 1.f, 1.f });

//	PackedScene* pps = iCluige.iPackedScene.get_packed_node(ps, "UnLabel");
//	SortedDictionary* ppd = &(pps->dico_node);
//	Checked_Variant got = iCluige.iSortedDictionary.get(ppd, "text");
//	char* got_val = (char*)(got.v.ptr);
//	got_val[1] = 'W';

//	Node* nn = iCluige.iNode.get_node(my_game_root_node, "Un _ = Sprite2D");
//	Node2D* dd = (Node2D*)(nn->_sub_class);
//	SpriteText* tt = (SpriteText*)(dd->_sub_class);
////	iCluige.iNode2D.move_local(dd, (Vector2){ 1.f, 1.f });
//	tt->offset.x++;

//	Node* nn = iCluige.iNode.get_node(my_game_root_node, "Un _ = Sprite2D/UnSousSprite2D/UnNode2D");
//	Node2D* dd = (Node2D*)(nn->_sub_class);
////	iCluige.iNode2D.move_local(dd, (Vector2){ -1.f, -1.f });
//	dd->visible = !(dd->visible);

//	Node* nn = iCluige.iNode.get_node(my_game_root_node, "unLabelFourbe");//UnAutreLabel");
//	Node2D* dd = (Node2D*)(nn->_sub_class);
//	SpriteText* tt = (SpriteText*)(dd->_sub_class);
//	char* got_val = tt->text;
//	got_val[1] = 'W';

	const Node* diff = test_node_against_packed_scene_tree(my_game_root_node, ps);

	if(diff != NULL)
	{
		char* pp = iCluige.iNode.get_path_mallocing(diff);
		printf("FAILED --- instanced Node %s different from PackedScene  | test_pksc_instanciate\n ", pp);
		free(pp);
	}

	iCluige.iPackedScene.pre_delete_PackedScene(ps);
	iCluige.iTscnParser.pre_delete_TscnParser(&parser);
}

#ifdef IN_OCTOJAM_2024

int RIGHT_ACTION;
int LEFT_ACTION;
int UP_ACTION;
int DOWN_ACTION;
int QUIT_ACTION;

Script* player_script;
Node2D* player_node2D;
Script* mystere_script;
Vector2 player_speed;
float player_speed_max;
float player_brake;//acceleration
float min_dist;
float max_dist;

void delete_mystere(Script* this_Script)
{//maybe one day
}

static void process_mystere(Script* this_Script, float elapsed_seconds)
{
	Node2D* this_node2D = (Node2D*)(this_Script->node->_sub_class);
	float d = iCluige.iVector2.distance_to(
			&(this_node2D->position),
			&(player_node2D->position));
	d = clamp_float(d, min_dist, max_dist);
	float s = 10 / d;
	SpriteSVG* this_spriteSVG = (SpriteSVG*)(this_node2D->_sub_class);
	this_spriteSVG->scale.x = s;
	this_spriteSVG->scale.y = s;
}

void delete_player(Script* this_Script)
{//maybe one day
}

static void process_Player(Script* this_Script, float elapsed_seconds)
{
//    struct _Input* ii = iCluige.input;
	struct iiInput* iii = &iCluige.iInput;

	if(iii->is_action_just_pressed(QUIT_ACTION))
	{
		iCluige.quit_asked = true;
		return;
	}

	float dx = 0;
	float dy = 0;
	if(iii->is_action_just_pressed(UP_ACTION))
	{
		dy -= 1;
	}
	if(iii->is_action_just_pressed(DOWN_ACTION))
	{
		dy += 1;
	}
	if(iii->is_action_just_pressed(LEFT_ACTION))
	{
		dx -= 1;
	}
	if(iii->is_action_just_pressed(RIGHT_ACTION))
	{
		dx += 1;
	}
	float epsilon_speed = 9;
	//<<<<<<<<<  xxxxxxxxx
	if(fabs(dx) == 0)
	{
		if(fabs(player_speed.x) < epsilon_speed)
		{
			player_speed.x = 0;
		}
		else
		{
			float ds = - copysignf(elapsed_seconds * player_brake, player_speed.x);
			player_speed.x += ds;
		}
	}
	else
	{
		player_speed.x = copysignf(player_speed_max, dx);
	}
	//  xxxxxxxxxx>>>>>>>>

	// <<<<<<<<yyyyyyyyy
	if(fabs(dy) == 0)
	{
		if(fabs(player_speed.y) < epsilon_speed)
		{
			player_speed.y = 0;
		}
		else
		{
			float ds = - copysignf(elapsed_seconds * player_brake, player_speed.y);
			player_speed.y += ds;
		}
	}
	else
	{
		player_speed.y = copysignf(player_speed_max, dy);
	}
	//  yyyyyyyyyy>>>>>>>>>>><

	Vector2 d = (Vector2)
	{
		elapsed_seconds * player_speed.x,
		elapsed_seconds * player_speed.y
	};

	iCluige.iNode2D.move_local(player_node2D, d);
}


#endif // IN_OCTOJAM_2024

int main()
{
//    jandk(0, 49, 3);
	//SetConsoleOutputCP(CP_UTF8);

	//init
	cluige_init();//makes all roots, set all functions pointers, etc.

#ifdef TMP_TEST_TO_PUT_IN_TEST_PROJECT
	test_utils_bool_from_parsed();
	test_utils_char_from_parsed();
	test_utils_str_from_parsed();
	test_Node_instanciate();
	test_Node2D_instanciate();
	test_SpriteText_instanciate();
	test_SpriteSVG_instanciate();
	test_FileLineReader();
	test_TscnParser();
	test_pksc_instanciate();

	SortedDictionary parse_placeholder;
	iCluige.iSortedDictionary.sorted_dictionary_alloc(&parse_placeholder, VT_POINTER, VT_POINTER, 10);
	iCluige.iSortedDictionary.set_compare_keys_func(&parse_placeholder, iCluige.iDeque.default_compare_string_func);
	iCluige.iSortedDictionary.insert(&parse_placeholder, "name", "O_Racine");
//	Node* my_root = iCluige.iNode.new_Node();
//	iCluige.iNode.deserialize_dico(my_root, &parse_placeholder);
	SortedDictionary* fcties = &(iCluige.iNode.node_factories);
	Checked_Variant cv_fcty = iCluige.iSortedDictionary.get(fcties, "Node");
	NodeFactory* fcty = (NodeFactory*)(cv_fcty.v.ptr);
	Node* my_root = fcty->instanciate(&parse_placeholder);
	utils_breakpoint_trick(my_root, false);
#endif // TMP_TEST_TO_PUT_IN_TEST_PROJECT

#ifdef IN_GLOBALGAMEJAM2024
    SortedDictionary d;
    iCluige.iSortedDictionary.sorted_dictionary_alloc(&d, VT_UINT64, VT_POINTER, 10);

    Variant erased_var = iCluige.iSortedDictionary.erase(&d, 1337);//don't forget to free elems before that
    char* erased_string = erased_var.ptr;
    utils_breakpoint_trick(erased_string, false);
    debug_dico(&(d));

    iCluige.iSortedDictionary.clear(&d);//don't forgt to free elems before that
    debug_dico(&(d));

    iCluige.iSortedDictionary.insert(&d, 42, "quaranteDeux");
    debug_dico(&(d));

    erased_var = iCluige.iSortedDictionary.erase(&d, 42);//don't forgt to free elems before that
    erased_string = erased_var.ptr;
    debug_dico(&(d));
    iCluige.iSortedDictionary.insert(&d, 42, "quaranteDeux");
    debug_dico(&(d));

    iCluige.iSortedDictionary.insert(&d, 2, "deux");
    debug_dico(&(d));
    iCluige.iSortedDictionary.insert(&d, 4, "quatre");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, 3, "trois");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, 42, "qurteDeux");// don't forget to delete/free old value if needed (not here, because local string)
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, 73, "soixxx");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert_last(&d, 90, "nonant");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert_first(&d, 1, "hun");
    debug_dico(&d);

    erased_var = iCluige.iSortedDictionary.erase(&d, 1337);//don't forgt to free elems before that
    erased_string = erased_var.ptr;
    debug_dico(&(d));

    for(int ri=0; ri < 3; ri++)
    {
        int r = rand();
        iCluige.iSortedDictionary.insert(&d, r, "...");
    }
    iCluige.iSortedDictionary.insert(&d, RAND_MAX, "MAX");
    debug_dico(&d);
    erased_var = iCluige.iSortedDictionary.erase(&d, 3);//don't forgt to free elems before that
    erased_string = erased_var.ptr;
    debug_dico(&d);
    erased_var = iCluige.iSortedDictionary.erase(&d, 1);//don't forgt to free elems before that
    erased_string = erased_var.ptr;
    debug_dico(&d);
    erased_var = iCluige.iSortedDictionary.erase(&d, RAND_MAX);//don't forgt to free elems before that
    erased_string = erased_var.ptr;
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, 0, "zero");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, RAND_MAX, "PLEIN");
    debug_dico(&d);

    iCluige.iSortedDictionary.clear(&d);//don't forgt to free elems before that
    debug_dico(&(d));

    iCluige.iSortedDictionary.insert(&d, 0, "zero");
    debug_dico(&d);
    iCluige.iSortedDictionary.insert(&d, RAND_MAX, "PLEIN");
    debug_dico(&d);

    //don't forget to delete d pointed values (no need here because ending whole game)
    iCluige.iSortedDictionary.pre_delete_SortedDictionary(&d);

//    Deque d;
////    d = iCluige.checked_malloc(sizeof(Deque));
//    iCluige.iDeque.deque_alloc(&d, VT_UINT64, 10);
//    d._sorted = true;
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 42);
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 2);
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 4);
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 3);
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 42);
////    debug_deque(&d);
//    iCluige.iDeque.insert_sorted(&d, 73);
////    debug_deque(&d);
//    for(int ri=0; ri < 35; ri++)
//    {
//        int r = rand();
//        iCluige.iDeque.insert_sorted(&d, r);
//    }
//    debug_deque(&d);
//
//    iCluige.iDeque.pre_delete_Deque(&d);
#endif // IN_GLOBALGAMEJAM2024

#ifdef IN_OCTOJAM_2024
	UP_ACTION = iCluige.iInput.add_action("up");
    iCluige.iInput.bind_key(UP_ACTION, 'e');
    iCluige.iInput.bind_key(UP_ACTION, 'E');
//    iCluige.iInput.bind_key(UP_ACTION, KEY_UP);

	DOWN_ACTION = iCluige.iInput.add_action("down");
    iCluige.iInput.bind_key(DOWN_ACTION, 'd');
    iCluige.iInput.bind_key(DOWN_ACTION, 'D');
//    iCluige.iInput.bind_key(DOWN_ACTION, KEY_DOWN);

	LEFT_ACTION = iCluige.iInput.add_action("left");
    iCluige.iInput.bind_key(LEFT_ACTION, 's');
    iCluige.iInput.bind_key(LEFT_ACTION, 'S');
//    iCluige.iInput.bind_key(LEFT_ACTION, KEY_LEFT);

	RIGHT_ACTION = iCluige.iInput.add_action("right");
    iCluige.iInput.bind_key(RIGHT_ACTION, 'f');
    iCluige.iInput.bind_key(RIGHT_ACTION, 'F');
//    iCluige.iInput.bind_key(RIGHT_ACTION, KEY_RIGHT);

	QUIT_ACTION = iCluige.iInput.add_action("quit");
    iCluige.iInput.bind_key(QUIT_ACTION, 'x');
    iCluige.iInput.bind_key(QUIT_ACTION, 'X');

	TscnParser parser;
	iCluige.iTscnParser.tscn_parser_alloc(&parser, "octojam_24.tscn");
	bool ok = parser.parse_scene(&parser);
	utils_breakpoint_trick(&ok, !ok);
//	char* dbg = iCluige.iPackedScene.debug_recrusive(parser.scene_root, NULL);
	PackedScene* ps = parser.scene_root;
	Node* my_game_root_node = iCluige.iPackedScene.instanciate(ps);
	iCluige.iNode.add_child(iCluige.public_root_2D, my_game_root_node);


	Node* player_node = iCluige.iNode.get_node(my_game_root_node, "player_Sprite2D");
	player_node2D = (Node2D*)(player_node->_sub_class);
	Script* new_Script = iCluige.iScript.new_Script(player_node);
	new_Script->node = player_node;
	new_Script->delete_Script = delete_player;
	new_Script->process = process_Player;
	new_Script->_sub_class = NULL;
	player_node->script = new_Script;
	player_speed_max = 300;
	player_brake = 400;

	min_dist = 15;
	max_dist = 1000;
	Node* mystere_node = player_node->next_sibling;
	while(mystere_node != NULL)
	{
//		Node2D* mystere_node2D = (Node2D*)(mystere_node->_sub_class);
//		SpriteSVG* mystere_SpriteSVG = (SpriteSVG*)(mystere_node2D->_sub_class);
		Script* mystere_Script = iCluige.iScript.new_Script(mystere_node);
		mystere_Script->node = mystere_node;
		mystere_Script->delete_Script = delete_mystere;
		mystere_Script->process = process_mystere;
		mystere_Script->_sub_class = NULL;
		mystere_node->script = mystere_Script;

		mystere_node = mystere_node->next_sibling;
	}

//	SpriteSVG* essai_sprtSVG = iCluige.iSpriteSVG.new_SpriteSVG();
//	Node2D* essai_sprtSVG_node2D = (Node2D*)(essai_sprtSVG->_this_Node2D);
//	Node* essai_sprtSVG_node = (Node*)(essai_sprtSVG_node2D->_this_Node);
//	iCluige.iNode.add_child(iCluige.public_root_2D, essai_sprtSVG_node);
//	iCluige.iNode2D.move_local(essai_sprtSVG_node2D, (Vector2){ 2.f, 2.f });
//	iCluige.iSpriteSVG.parse_file(essai_sprtSVG, "question.svg");
//	essai_sprtSVG->scale = (Vector2){ .1f, .1f };

	iCluige.iCamera2D.set_zoom(iCluige.iCamera2D.current_camera,
				(Vector2){.23f, .23f});
	Node2D* camera_n2D = iCluige.iCamera2D.current_camera->_this_Node2D;
	iCluige.iNode2D.move_local(camera_n2D, (Vector2){ 460.f, 220.f });
//	iCluige.iNode.print_tree_pretty(iCluige.public_root_2D);

	iCluige.iPackedScene.pre_delete_PackedScene(ps);
	iCluige.iTscnParser.pre_delete_TscnParser(&parser);

	cluige_run();
#endif // IN_GAME_JAM_CREAJAM2

#ifdef IN_GAME_JAM_CREAJAM2

	//game jam 2023_02_04
	Node2D* testUberRootNode2D = iCluige.iNode2D.new_Node2D();
	Node* myTestUberRootNode = testUberRootNode2D->_this_Node;
	iCluige.iNode.set_name(myTestUberRootNode, "test uber root");
	iCluige.iNode2D.move_local(testUberRootNode2D, (Vector2){36., 40.});
	iCluige.iNode.add_child(iCluige.public_root_2D, myTestUberRootNode);

	Node2D* myRootNode2D = iCluige.iNode2D.new_Node2D();
	Node* myRootNode = myRootNode2D->_this_Node;
	iCluige.iNode.set_name(myRootNode, "Game");
	iCluige.iNode2D.move_local(myRootNode2D, (Vector2){10., 25.});
	iCluige.iNode.add_child(/*iCluige.public_root_2D*/myTestUberRootNode, myRootNode);

	SpriteText* groundSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* groundNode = groundSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(groundNode, "Ground");
	iCluige.iNode2D.move_local(groundSpriteText->_this_Node2D, (Vector2){0, 1});//11.5});//-8.5});//
	iCluige.iNode.add_child(myRootNode, groundNode);
	iCluige.iSpriteText.set_text(groundSpriteText, "=================================================");


	SpriteText* blockSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* blockNode = blockSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(blockNode, "Block");
	blockSpriteText->offset = (Vector2){-2, -1.5};//origin at center
	iCluige.iNode2D.move_local(blockSpriteText->_this_Node2D, (Vector2){0., -2.5});//
	iCluige.iNode.add_child(myRootNode, blockNode);

	struct VaEtVient* blockScript = newVaEtVient(blockNode);
	blockScript->ownerSprite = blockSpriteText;


	SpriteText* playerSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* playerNode = playerSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(playerNode, "Player");
	iCluige.iNode.add_child(myRootNode, playerNode);
	iCluige.iSpriteText.set_text(playerSpriteText, " o\n'U`\n \"   ");
	playerSpriteText->offset = (Vector2){-1, -2};//origin at feet of player
	iCluige.iNode2D.move_local(playerSpriteText->_this_Node2D, (Vector2){25., 0.});
	struct VaEtVientPlayer* playerScript = newVaEtVientPlayer(playerNode);
	playerScript->ownerSprite = playerSpriteText;
/*

 o        #####
'U`       #   #
 "        #   #
          #####

 */

	//iCluige.wanted_frame_seconds = .1;
	//iCluige.clock->scale = .6;
	//iCluige.iNode.print_tree_pretty(iCluige.private_root_2D);

#endif // IN_GAME_JAM_CREAJAM2

#ifdef IN_STUNJAM2023
	HIGHER_ACTION = iCluige.iInput.add_action("HIGHER_ACTION");
    iCluige.iInput.bind_key(HIGHER_ACTION, 'e');
    iCluige.iInput.bind_key(HIGHER_ACTION, 'E');
    iCluige.iInput.bind_key(HIGHER_ACTION, KEY_UP);

	LOWER_ACTION = iCluige.iInput.add_action("LOWER_ACTION");
    iCluige.iInput.bind_key(LOWER_ACTION, 'd');
    iCluige.iInput.bind_key(LOWER_ACTION, 'D');
    iCluige.iInput.bind_key(LOWER_ACTION, KEY_DOWN);

	WIDER_ACTION = iCluige.iInput.add_action("WIDER_ACTION");
    iCluige.iInput.bind_key(WIDER_ACTION, 'f');
    iCluige.iInput.bind_key(WIDER_ACTION, 'F');
    iCluige.iInput.bind_key(WIDER_ACTION, KEY_RIGHT);

	NARROWER_ACTION = iCluige.iInput.add_action("NARROWER_ACTION");
    iCluige.iInput.bind_key(NARROWER_ACTION, 's');
    iCluige.iInput.bind_key(NARROWER_ACTION, 'S');
    iCluige.iInput.bind_key(NARROWER_ACTION, KEY_LEFT);

	QUIT_ACTION = iCluige.iInput.add_action("quit");
    iCluige.iInput.bind_key(QUIT_ACTION, 'x');
    iCluige.iInput.bind_key(QUIT_ACTION, 'X');

	Node2D* gameRootNode2D = iCluige.iNode2D.new_Node2D();
	Node* gameRootRootNode = gameRootNode2D->_this_Node;
	iCluige.iNode.set_name(gameRootRootNode, "Game");
	iCluige.iNode2D.move_local(gameRootNode2D, (Vector2){15., 15.});
	iCluige.iNode.add_child(iCluige.public_root_2D, gameRootRootNode);

	Node2D* playerNode2D = iCluige.iNode2D.new_Node2D();
	Node* playerNode = playerNode2D->_this_Node;
	iCluige.iNode.set_name(playerNode, "Player");
	iCluige.iNode2D.move_local(gameRootNode2D, (Vector2){20., 30.});
	iCluige.iNode.add_child(gameRootRootNode, playerNode);
	newPlayer(playerNode);

	SpriteText* hudSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* hudNode = hudSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(hudNode, "Hud");
	iCluige.iSpriteText.set_text(hudSpriteText,
            "\n             E\n  speak :  S   F\n             D\n\n\n  exit :  X\n\n\n\nOnly images here, sound \nis not yet possible !");
	iCluige.iNode2D.move_local(hudSpriteText->_this_Node2D, (Vector2){-12, 22});
	iCluige.iNode.add_child(gameRootRootNode, hudNode);


    //mvaddstr(20, 35, ". · · ·l ");
//	draw_line(16.8, 45, 3.5, 45);
//	draw_line(17.5, 50.2, 3.3, 56);

#endif // IN_STUNJAM2023

#ifdef NOT_IN_GAME_JAM

	//here hardcode or parse my scene from file
	//...
	Node* n = iCluige.iNode.new_Node();
	iCluige.iNode.set_name(n, "Idle Root");
	iCluige.iNode.add_child(iCluige.public_root_2D, n);

	//don't allocate nodes on stack, see delete_Node()
	//Node n;
	//iCluige.iNode.initZero(&n);

	//warning about node names:
	//hardcoded strings in the form char* s = "hardcoded"
	//are instanciated on the stack
	//don't do n->name = s, use instead set_name(n, s)
//	wchar_t* plouf = iCluige.checked_malloc(40 * sizeof(wchar_t));
//	plouf[0]=L'b';plouf[1]=L'o';plouf[2]=L'b';plouf[3]=L'\0';//OK
//	//plouf = L"bob";// <-- error on free() in delete_Node()
//	wprintf(L"%ls\n", plouf);
//	free(plouf);//test either plouf = "..." or plouf[0] = ...

	Node* pn;
	for(int i=0; i<3; i++)
    {
        pn = iCluige.iNode.new_Node();
        iCluige.iNode.add_child(n, pn);
        Node* pn01 = iCluige.iNode.new_Node();
        iCluige.iNode.add_child(pn, pn01);
    }//test on a gamer pc = 2minutes for 123456 iterations

	Node* pn01 = iCluige.iNode.new_Node();
	iCluige.iNode.add_child(pn, pn01);
	printf("my node name : %s\n", pn01->name);

	printf("\ntree : \n");
	iCluige.iNode.print_tree_pretty(iCluige.private_root_2D);


	iCluige.iNode.set_name(n, "Test Rename");
	printf("\ntree after root renamed: \n");
	iCluige.iNode.print_tree_pretty(iCluige.private_root_2D);

	//test Deque
	Deque dqTest;
	iCluige.iDeque.deque_alloc(&dqTest, VT_DOUBLE, 9);
	iCluige.iDeque.push_back(&dqTest, 3.3);
	iCluige.iDeque.push_back(&dqTest, 4.4);
	iCluige.iDeque.push_back(&dqTest, 5.5);
	iCluige.iDeque.push_front(&dqTest, 1.1);
	iCluige.iDeque.push_front(&dqTest, 0.0);
	iCluige.iDeque.insert(&dqTest, 2, 2.2);
	iCluige.iDeque.append(&dqTest, 7.7);
	iCluige.iDeque.insert(&dqTest, 6, 6.6);

//	Variant vLast = iCluige.iDeque.pop_back(&dqTest);
//	Variant vFirst = iCluige.iDeque.pop_front(&dqTest);
//    iCluige.iDeque.remove(&dqTest, 7);
//    iCluige.iDeque.remove(&dqTest, 4);
//    iCluige.iDeque.remove(&dqTest, 0);
//    iCluige.iDeque.clear(&dqTest);

	StringBuilder sb_dq;
	int sizeDequeMsg = iCluige.iDeque.size(&dqTest) * 9;//quick & dirty test
	char* dqMsg = iCluige.iStringBuilder.string_alloc(&sb_dq, sizeDequeMsg);
	//dqMsg[0] = 'u';dqMsg[1] = 0;
	iCluige.iStringBuilder.append(&sb_dq, "[ ");
	for(int iDq=0; iDq < iCluige.iDeque.size(&dqTest); iDq++)
    {
        double val = iCluige.iDeque.at(&dqTest, iDq).d;
        iCluige.iStringBuilder.append(&sb_dq, "%.1f ", val);
    }
    iCluige.iStringBuilder.append(&sb_dq, "]");
	iCluige.iDeque.pre_delete_Deque(&dqTest);
	free(dqMsg);

#endif // NOT_IN_GAME_JAM

#ifdef IN_CREAJAM3
	//game jam 2023_03_24
	UP_ACTION = iCluige.iInput.add_action("up");
    iCluige.iInput.bind_key(UP_ACTION, 'e');
    iCluige.iInput.bind_key(UP_ACTION, 'E');
    iCluige.iInput.bind_key(UP_ACTION, KEY_UP);

	DOWN_ACTION = iCluige.iInput.add_action("down");
    iCluige.iInput.bind_key(DOWN_ACTION, 'd');
    iCluige.iInput.bind_key(DOWN_ACTION, 'D');
    iCluige.iInput.bind_key(DOWN_ACTION, KEY_DOWN);

	LEFT_ACTION = iCluige.iInput.add_action("left");
    iCluige.iInput.bind_key(LEFT_ACTION, 's');
    iCluige.iInput.bind_key(LEFT_ACTION, 'S');
    iCluige.iInput.bind_key(LEFT_ACTION, KEY_LEFT);

	RIGHT_ACTION = iCluige.iInput.add_action("right");
    iCluige.iInput.bind_key(RIGHT_ACTION, 'f');
    iCluige.iInput.bind_key(RIGHT_ACTION, 'F');
    iCluige.iInput.bind_key(RIGHT_ACTION, KEY_RIGHT);

	START_ACTION = iCluige.iInput.add_action("start");
    iCluige.iInput.bind_key(START_ACTION, ' ');

	QUIT_ACTION = iCluige.iInput.add_action("quit");
    iCluige.iInput.bind_key(QUIT_ACTION, 'x');
    iCluige.iInput.bind_key(QUIT_ACTION, 'X');

	Node2D* gameRootNode2D = iCluige.iNode2D.new_Node2D();
	Node* gameRootRootNode = gameRootNode2D->_this_Node;
	iCluige.iNode.set_name(gameRootRootNode, "Game");
	iCluige.iNode2D.move_local(gameRootNode2D, (Vector2){50., 35.});
	iCluige.iNode.add_child(iCluige.public_root_2D, gameRootRootNode);


	hudSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* hudNode = hudSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(hudNode, "Hud");
	iCluige.iSpriteText.set_text(hudSpriteText,
            "start :  SPACE\n\n               E\nimpulse :    S   F\n               D\n\nexit :  X");
	iCluige.iNode2D.move_local(hudSpriteText->_this_Node2D, (Vector2){-40, -32});
	iCluige.iNode.add_child(gameRootRootNode, hudNode);

	SpriteText* planetSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* planetNode = planetSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(planetNode, "Planet");
	iCluige.iSpriteText.set_text(planetSpriteText,
            "  .-'-.\n /     \\\n        .\n|       |\n|       |\n`       '\n \\     /\n  `...'");
//	iCluige.iNode2D.move_local(planetSpriteText->_this_Node2D, (Vector2){1, 1.});
	planetSpriteText->offset = (Vector2){-4, -3};//origin at center
	iCluige.iNode.add_child(gameRootRootNode, planetNode);

	SpriteText* playerSpriteText = iCluige.iSpriteText.new_SpriteText();
	Node* playerNode = playerSpriteText->_this_Node2D->_this_Node;
	iCluige.iNode.set_name(playerNode, "Player");
	iCluige.iNode.add_child(gameRootRootNode, playerNode);
	iCluige.iSpriteText.set_text(playerSpriteText, "#");
//	iCluige.iNode2D.move_local(playerSpriteText->_this_Node2D, (Vector2){-1., -1.});
	/*Player* playerScript =*/ newPlayer(playerNode);
#endif // NOT_IN_GAME_JAM / NOT_IN_CREAJAM3

    //game loop
	//cluige_run();

//	wchar_t* t = L"└  ├";//;L"azerty";//// u"\u251c";//iCluige.checked_malloc(88 * sizeof(wchar_t));
//	wchar_t* l = L"";//      ░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓███";//;L"poiuytreza";////u"\u2514";//iCluige.checked_malloc(88 * sizeof(wchar_t));
////  ├  └
////	wchar_t t[88];
////	wchar_t l[88];
////	wscanf("%ls", t);
////	wscanf("%ls", l);
////	t[1] = 0;
////	t[0] = 0x251c;
////	l[1] = 0;
////	l[0] = 0x2514;
////	wprintf(t);
//	wprintf(L"%ls\n", t);
////	wprintf(l);
//	wprintf(L"%ls\n", l);

	int finish = cluige_finish();
#ifndef IN_OCTOJAM_2024
	if(NULL != "DEBUG")
	{
		printf("\nThe End ! Press ENTER key to quit...\n");
		getchar();
	}
#endif // IN_OCTOJAM_2024
//	utils_breakpoint_trick(NULL, true);
	return finish;
}


/*  old tests

	float* pf = iCluige.checked_malloc(sizeof(float));
	(*pf) = 3.141592;
	wprintf(L"test iCluige.checked_malloc : %f\n", (*pf));



//	Variant varb = iCluige.iVariant.from_val(VT_BOOL, true);
//	Variant varc = iCluige.iVariant.from_val(VT_CHAR, 'k');
//	Variant vari32 = iCluige.iVariant.from_val(VT_INT32, -49);
//	Variant varui32 = iCluige.iVariant.from_val(VT_UINT32, 3);
//	Variant vari64 = iCluige.iVariant.from_val(VT_INT64, -5000000000);
//	Variant varui64 = iCluige.iVariant.from_val(VT_UINT64, 5000000000);
//	Variant varf = iCluige.iVariant.from_val(VT_FLOAT, -49.3);
//	Variant vard = iCluige.iVariant.from_val(VT_DOUBLE, 3.14);
//	Variant varptr = iCluige.iVariant.from_val(VT_POINTER, &vard);
//	Variant* vv = (Variant*)(varptr.ptr);
//
//	Variant vaarb = iCluige.iVariant.from_val(VT_BOOL, false);
//	Variant vaarc = iCluige.iVariant.from_val(VT_CHAR, 'm');
//	Variant vaari32 = iCluige.iVariant.from_val(VT_INT32, -4);
//	Variant vaarui32 = iCluige.iVariant.from_val(VT_UINT32, 1);
//	Variant vaari64 = iCluige.iVariant.from_val(VT_INT64, -5000000001);
//	Variant vaarui64 = iCluige.iVariant.from_val(VT_UINT64, 5000000001);
//	Variant vaarf = iCluige.iVariant.from_val(VT_FLOAT, -19.1);
//	Variant vaard = iCluige.iVariant.from_val(VT_DOUBLE, 3.04);
//	Variant vaarptr = iCluige.iVariant.from_val(VT_POINTER, &vaard);
//	int comp = iCluige.iVariant.compare(VT_BOOL, varb, vaarb);
//	comp = iCluige.iVariant.compare(VT_CHAR, varc, vaarc);
//	comp = iCluige.iVariant.compare(VT_INT32, vari32, vaari32);
//	comp = iCluige.iVariant.compare(VT_UINT32, varui32, vaarui32);
//	comp = iCluige.iVariant.compare(VT_INT64, vari64, vaari64);
//	comp = iCluige.iVariant.compare(VT_UINT64, varui64, vaarui64);
//	comp = iCluige.iVariant.compare(VT_FLOAT, varf, vaarf);
//	comp = iCluige.iVariant.compare(VT_DOUBLE, vard, vaard);
//	comp = iCluige.iVariant.compare(VT_POINTER, varptr, vaarptr);

	char* stackheapTest = iCluige.iStringBuilder.stack_to_heap("essai stack to heap");
	char* formatheapTest = iCluige.iStringBuilder.formatted_to_heap(
            60, "essai %f form to heap %d %s", 49.3, 12, "blbl");
	free(stackheapTest);
	free(formatheapTest);
*/


/*

	Vector2 essayPath[9] =
        {
            (Vector2){16.8, 45},
            (Vector2){3.5, 45},
            (Vector2){17.5, 50.2},
            (Vector2){3.3, 56},
            (Vector2){20.1, 54},
            (Vector2){15, 97},
            (Vector2){23, 54},
            (Vector2){23, 97},
            (Vector2){29, 54}
        };
    iCluige.iSpriteSVG.add_path_from_array(playerSpriteSVG, essayPath, 9);
	Vector2 essayPath2[9]  =
        {
            (Vector2){32, 97},
            (Vector2){32, 49},
            (Vector2){45, 54},
            (Vector2){33, 45},
            (Vector2){45, 45},
            (Vector2){32, 42},
            (Vector2){45, 37},
            (Vector2){29, 38},
            (Vector2){31, 3}
        };
    iCluige.iSpriteSVG.add_path_from_array(playerSpriteSVG, essayPath2, 9);
	Vector2 essayPath3[6]  =
        {
            (Vector2){23, 37},
            (Vector2){23, 2},
            (Vector2){20, 38},
            (Vector2){17, 3},
            (Vector2){17, 42},
            (Vector2){3, 37}
        };
    iCluige.iSpriteSVG.add_path_from_array(playerSpriteSVG, essayPath3, 6);


        {
            (Vector2){0, 0},
            (Vector2){0, 5},
            (Vector2){5, 15},
            (Vector2){15, 20},
            (Vector2){25, 20},
            (Vector2){35, 15},
            (Vector2){40, 5},
            (Vector2){40, 0}
        };
*/
