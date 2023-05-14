#include <stdio.h>
#include <cluige.h>
#include <float.h>
#include "VaEtVient.h"
#include "VaEtVientPlayer.h"

//if need (for debug) of SetConsoleOutputCP(CP_UTF8);
//#include <windows.h>


#ifdef NOT_IN_CREAJAM3
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
	Script* thisScript;
	Vector2 speed;
	bool started;
};
typedef struct _Player Player;

void deletePlayer(Script* thisScript)
{
    //maybe one day
}

SpriteText* hudSpriteText;
static void process_Player(Script* thisScript, float elapsedSeconds)
{
    Player* thisPlayer =
        (Player*)(thisScript->_subClass);
    Node2D* thisNode2D = thisPlayer->ownerSprite->_thisNode2D;
//    struct _Input* ii = iCluige.input;
    struct iiInput* iii = &iCluige.iInput;
    float weight = 2500;
    float impulseStrength = 4;

    if(iii->is_action_just_pressed(QUIT_ACTION))
    {
        iCluige.quitAsked = true;
        return;
    }
    if(!(thisPlayer->started))
    {
        if(iii->is_action_just_pressed(START_ACTION))//' '
        {
            iCluige.iNode2D.moveLocal(hudSpriteText->_thisNode2D, (Vector2){-5, -8});
            thisPlayer->started = true;
            thisPlayer->speed = (Vector2){impulseStrength * 2, 0};
        }
        return;
    }

    Vector2 gravity, impulse, dSpeed, dPos;
    Vector2* pos = &(thisNode2D->position);
    //planet center (hence gravity) is centered on local (0,0)
    gravity = *pos;
    float h = iCluige.iVector2.length(pos);
    // 1/h => normalize, 1/h² => gravity, 1/h^3.2 => fun
    if(h > 10 * FLT_EPSILON)
    {
        iCluige.iVector2.kMul(&gravity, -weight/powf(h,3.2), &gravity);
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

    iCluige.iVector2.kMul(&gravity, elapsedSeconds, &dSpeed);
    iCluige.iVector2.add(&(thisPlayer->speed), &dSpeed, &(thisPlayer->speed));
    iCluige.iVector2.add(&(thisPlayer->speed), &impulse, &(thisPlayer->speed));

    iCluige.iVector2.set(&dPos, 0, 0);
    iCluige.iVector2.kMul(&thisPlayer->speed, elapsedSeconds, &dPos);

    iCluige.iNode2D.moveLocal(thisNode2D, dPos);

}

Player* newPlayer(Node* thisNode)
{
    Script* newScript = iCluige.iScript.newScript(thisNode);
    Player* newPlayer = iCluige.checkedMalloc(sizeof(Player));

    newPlayer->thisScript = newScript;
    newPlayer->started = false;
    newPlayer->speed = (Vector2){0, 0};

    newScript->node = thisNode;
    newScript->deleteScript = deletePlayer;
    newScript->process = process_Player;
    newScript->_subClass = newPlayer;

    thisNode->script = newScript;

    Node2D* thisNode2D = (Node2D*)(thisNode->_subClass);
    SpriteText* thisSpriteText = (SpriteText*)(thisNode2D->_subClass);
	newPlayer->ownerSprite = thisSpriteText;
	iCluige.iNode2D.setLocalPosition(thisNode2D, (Vector2){-10,-20});

    return newPlayer;
}

#endif // NOT_IN_CREAJAM3

#ifndef NOT_IN_STUNJAM2023
#include <../PDCurses/curses.h>

int HIGHER_ACTION;
int LOWER_ACTION;
int WIDER_ACTION;
int NARROWER_ACTION;
int QUIT_ACTION;

struct _Player
{
	Script* thisScript;
    SpriteSVG* mouthUpSpriteSVG;
    SpriteSVG* mouthDownSpriteSVG;
    SpriteSVG* eyesSpriteSVG;
    float timeTillBlink;
    int blinkStep;
};
typedef struct _Player Player;

void deletePlayer(Script* thisScript)
{
    //maybe one day
}

static void process_Player(Script* thisScript, float elapsedSeconds)
{
    Player* thisPlayer =
        (Player*)(thisScript->_subClass);
    struct iiInput* iii = &iCluige.iInput;

    if(iii->is_action_just_pressed(QUIT_ACTION))
    {
        iCluige.quitAsked = true;
        return;
    }
    if(iii->is_action_just_pressed(HIGHER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.y = 1 + (1 * .5);
        thisPlayer->mouthDownSpriteSVG->scale.y = 1.5;
    }
    if(iii->is_action_just_pressed(LOWER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.y = 1 - (1 * .5);
        thisPlayer->mouthDownSpriteSVG->scale.y = .5;
    }
    if(iii->is_action_just_pressed(WIDER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.x = 1.5;
        thisPlayer->mouthDownSpriteSVG->scale.x = 1.5;
    }
    if(iii->is_action_just_pressed(NARROWER_ACTION))
    {
        thisPlayer->mouthUpSpriteSVG->scale.x = .5;
        thisPlayer->mouthDownSpriteSVG->scale.x = .5;
    }

    float x_speed = 2;
    float y_speed = 2;
    if(thisPlayer->mouthUpSpriteSVG->scale.x < 1)
    {
        thisPlayer->mouthUpSpriteSVG->scale.x += 1 * x_speed * elapsedSeconds;
        thisPlayer->mouthDownSpriteSVG->scale.x += x_speed * elapsedSeconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.x > 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.x = 1;
        }
    }
    else if(thisPlayer->mouthUpSpriteSVG->scale.x > 1)
    {
        thisPlayer->mouthUpSpriteSVG->scale.x -= 1 * x_speed * elapsedSeconds;
        thisPlayer->mouthDownSpriteSVG->scale.x -= x_speed * elapsedSeconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.x < 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.x = 1;
        }
    }

    if(thisPlayer->mouthUpSpriteSVG->scale.y < 1)
    {
        thisPlayer->mouthUpSpriteSVG->scale.y += y_speed * elapsedSeconds;
        thisPlayer->mouthDownSpriteSVG->scale.y += y_speed * elapsedSeconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.y > 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.y = 1;
        }
    }
    else if(thisPlayer->mouthUpSpriteSVG->scale.y > 1)
    {
        thisPlayer->mouthUpSpriteSVG->scale.y -= y_speed * elapsedSeconds;
        thisPlayer->mouthDownSpriteSVG->scale.y -= y_speed * elapsedSeconds;
        if(thisPlayer->mouthUpSpriteSVG->scale.y < 1)
        {
            thisPlayer->mouthUpSpriteSVG->scale.y = 1;
        }
    }

    if(thisPlayer->timeTillBlink > 0)
    {
        thisPlayer->timeTillBlink -= elapsedSeconds;
        if(thisPlayer->blinkStep == 0)
        {
            thisPlayer->eyesSpriteSVG->scale.y = 1;
        }
        else if(thisPlayer->blinkStep == 1)
        {
            //close
            thisPlayer->eyesSpriteSVG->scale.y -= 5 * elapsedSeconds;
        }
        else
        {
            //open
            thisPlayer->eyesSpriteSVG->scale.y += 3 * elapsedSeconds;
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

Player* newPlayer(Node* thisNode)
{
    Script* newScript = iCluige.iScript.newScript(thisNode);
    Player* newPlayer = iCluige.checkedMalloc(sizeof(Player));

    newPlayer->thisScript = newScript;
    newPlayer->blinkStep = 0;
    newPlayer->timeTillBlink = 3;

    newScript->node = thisNode;
    newScript->deleteScript = deletePlayer;
    newScript->process = process_Player;
    newScript->_subClass = newPlayer;
    thisNode->script = newScript;

	SpriteSVG* playerMouthUpSpriteSVG = iCluige.iSpriteSVG.newSpriteSVG();
	Node* MouthUp = playerMouthUpSpriteSVG->_thisNode2D->_thisNode;
	iCluige.iNode.setName(MouthUp, "PlayerMouthUp");
	iCluige.iNode.addChild(thisNode, MouthUp);
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

	SpriteSVG* playerMouthDownSpriteSVG = iCluige.iSpriteSVG.newSpriteSVG();
	Node* mouthDown = playerMouthDownSpriteSVG->_thisNode2D->_thisNode;
	iCluige.iNode.setName(mouthDown, "PlayerMouthDown");
	iCluige.iNode.addChild(thisNode, mouthDown);
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

    SpriteSVG* playerFaceSpriteSVG = iCluige.iSpriteSVG.newSpriteSVG();
	Node* face = playerFaceSpriteSVG->_thisNode2D->_thisNode;
	iCluige.iNode.setName(face, "PlayerFace");
	iCluige.iNode.addChild(thisNode, face);
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

	Node2D* eyesNode2D = iCluige.iNode2D.newNode2D();
	Node* eyesNode = eyesNode2D->_thisNode;
	iCluige.iNode.setName(eyesNode, "eyes2d");
	iCluige.iNode2D.moveLocal(eyesNode2D, (Vector2){0, -24.});
	iCluige.iNode.addChild(thisNode, eyesNode);

	SpriteSVG* eyesSpriteSVG = iCluige.iSpriteSVG.newSpriteSVG();
	Node* eyes = eyesSpriteSVG->_thisNode2D->_thisNode;
	iCluige.iNode.setName(eyes, "eyesSVG");
	iCluige.iNode.addChild(eyesNode, eyes);
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

#endif // NOT_IN_STUNJAM2023


int main()
{
    //SetConsoleOutputCP(CP_UTF8);

    //init
	cluigeInit();//makes all roots, set all functions pointers, etc.

#ifndef NOT_IN_STUNJAM2023
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

	Node2D* gameRootNode2D = iCluige.iNode2D.newNode2D();
	Node* gameRootRootNode = gameRootNode2D->_thisNode;
	iCluige.iNode.setName(gameRootRootNode, "Game");
	iCluige.iNode2D.moveLocal(gameRootNode2D, (Vector2){15., 15.});
	iCluige.iNode.addChild(iCluige.publicRoot2D, gameRootRootNode);

	Node2D* playerNode2D = iCluige.iNode2D.newNode2D();
	Node* playerNode = playerNode2D->_thisNode;
	iCluige.iNode.setName(playerNode, "Player");
	iCluige.iNode2D.moveLocal(gameRootNode2D, (Vector2){20., 30.});
	iCluige.iNode.addChild(gameRootRootNode, playerNode);
	newPlayer(playerNode);



//	iCluige.iNode2D.moveLocal(playerSpriteSVG->_thisNode2D, (Vector2){44, -10.});
//	/*Player* playerScript =*/ newPlayer(playerNode);

    //mvaddstr(20, 35, ". · · ·l ");
//	drawLine(16.8, 45, 3.5, 45);
//	drawLine(17.5, 50.2, 3.3, 56);

#endif // NOT_IN_GAME_JAM

#ifdef NOT_IN_GAME_JAM

	//here hardcode or parse my scene from file
	//...
	Node* n = iCluige.iNode.newNode();
	iCluige.iNode.setName(n, "Idle Root");
	iCluige.iNode.addChild(iCluige.publicRoot2D, n);

	//don't allocate nodes on stack, see deleteNode()
	//Node n;
	//iCluige.iNode.initZero(&n);

	//warning about node names:
	//hardcoded strings in the form char* s = "hardcoded"
	//are instanciated on the stack
	//don't do n->name = s, use instead setName(n, s)
//	wchar_t* plouf = iCluige.checkedMalloc(40 * sizeof(wchar_t));
//	plouf[0]=L'b';plouf[1]=L'o';plouf[2]=L'b';plouf[3]=L'\0';//OK
//	//plouf = L"bob";// <-- error on free() in deleteNode()
//	wprintf(L"%ls\n", plouf);
//	free(plouf);//test either plouf = "..." or plouf[0] = ...

	Node* pn;
	for(int i=0; i<3; i++)
    {
        pn = iCluige.iNode.newNode();
        iCluige.iNode.addChild(n, pn);
        Node* pn01 = iCluige.iNode.newNode();
        iCluige.iNode.addChild(pn, pn01);
    }//test on a gamer pc = 2minutes for 123456 iterations

	Node* pn01 = iCluige.iNode.newNode();
	iCluige.iNode.addChild(pn, pn01);
	printf("my node name : %s\n", pn01->name);

	printf("\ntree : \n");
	iCluige.iNode.printTreePretty(iCluige.privateRoot2D);


	iCluige.iNode.setName(n, "Test Rename");
	printf("\ntree after root renamed: \n");
	iCluige.iNode.printTreePretty(iCluige.privateRoot2D);

	//test Deque
	Deque dqTest;
	iCluige.iDeque.dequeAlloc(&dqTest, VT_DOUBLE, 9);
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
	char* dqMsg = iCluige.iStringBuilder.stringAlloc(&sb_dq, sizeDequeMsg);
	//dqMsg[0] = 'u';dqMsg[1] = 0;
	iCluige.iStringBuilder.append(&sb_dq, "[ ");
	for(int iDq=0; iDq < iCluige.iDeque.size(&dqTest); iDq++)
    {
        double val = iCluige.iDeque.at(&dqTest, iDq).d;
        iCluige.iStringBuilder.append(&sb_dq, "%.1f ", val);
    }
    iCluige.iStringBuilder.append(&sb_dq, "]");
	iCluige.iDeque.deleteDeque(&dqTest);
	free(dqMsg);

#elifdef NOT_IN_CREAJAM3
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

	Node2D* gameRootNode2D = iCluige.iNode2D.newNode2D();
	Node* gameRootRootNode = gameRootNode2D->_thisNode;
	iCluige.iNode.setName(gameRootRootNode, "Game");
	iCluige.iNode2D.moveLocal(gameRootNode2D, (Vector2){50., 35.});
	iCluige.iNode.addChild(iCluige.publicRoot2D, gameRootRootNode);


	hudSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* hudNode = hudSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(hudNode, "Hud");
	iCluige.iSpriteText.setText(hudSpriteText,
            "start :  SPACE\n\n               E\nimpulse :    S   F\n               D\n\nexit :  X");
	iCluige.iNode2D.moveLocal(hudSpriteText->_thisNode2D, (Vector2){-40, -32});
	iCluige.iNode.addChild(gameRootRootNode, hudNode);

	SpriteText* planetSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* planetNode = planetSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(planetNode, "Planet");
	iCluige.iSpriteText.setText(planetSpriteText,
            "  .-'-.\n /     \\\n        .\n|       |\n|       |\n`       '\n \\     /\n  `...'");
//	iCluige.iNode2D.moveLocal(planetSpriteText->_thisNode2D, (Vector2){1, 1.});
	planetSpriteText->offset = (Vector2){4, 3};//origin at center
	iCluige.iNode.addChild(gameRootRootNode, planetNode);

	SpriteText* playerSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* playerNode = playerSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(playerNode, "Player");
	iCluige.iNode.addChild(gameRootRootNode, playerNode);
	iCluige.iSpriteText.setText(playerSpriteText, "#");
//	iCluige.iNode2D.moveLocal(playerSpriteText->_thisNode2D, (Vector2){-1., -1.});
	/*Player* playerScript =*/ newPlayer(playerNode);
#endif // NOT_IN_GAME_JAM / NOT_IN_CREAJAM3

    //game loop
	cluigeRun();

//	wchar_t* t = L"└  ├";//;L"azerty";//// u"\u251c";//iCluige.checkedMalloc(88 * sizeof(wchar_t));
//	wchar_t* l = L"";//      ░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓███";//;L"poiuytreza";////u"\u2514";//iCluige.checkedMalloc(88 * sizeof(wchar_t));
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

	printf("Finishing...\n");
    return cluigeFinish(); //TODO
}


/*  old tests

	float* pf = iCluige.checkedMalloc(sizeof(float));
	(*pf) = 3.141592;
	wprintf(L"test iCluige.checkedMalloc : %f\n", (*pf));



//	Variant varb = iCluige.iVariant.fromVal(VT_BOOL, true);
//	Variant varc = iCluige.iVariant.fromVal(VT_CHAR, 'k');
//	Variant vari32 = iCluige.iVariant.fromVal(VT_INT32, -49);
//	Variant varui32 = iCluige.iVariant.fromVal(VT_UINT32, 3);
//	Variant vari64 = iCluige.iVariant.fromVal(VT_INT64, -5000000000);
//	Variant varui64 = iCluige.iVariant.fromVal(VT_UINT64, 5000000000);
//	Variant varf = iCluige.iVariant.fromVal(VT_FLOAT, -49.3);
//	Variant vard = iCluige.iVariant.fromVal(VT_DOUBLE, 3.14);
//	Variant varptr = iCluige.iVariant.fromVal(VT_POINTER, &vard);
//	Variant* vv = (Variant*)(varptr.ptr);
//
//	Variant vaarb = iCluige.iVariant.fromVal(VT_BOOL, false);
//	Variant vaarc = iCluige.iVariant.fromVal(VT_CHAR, 'm');
//	Variant vaari32 = iCluige.iVariant.fromVal(VT_INT32, -4);
//	Variant vaarui32 = iCluige.iVariant.fromVal(VT_UINT32, 1);
//	Variant vaari64 = iCluige.iVariant.fromVal(VT_INT64, -5000000001);
//	Variant vaarui64 = iCluige.iVariant.fromVal(VT_UINT64, 5000000001);
//	Variant vaarf = iCluige.iVariant.fromVal(VT_FLOAT, -19.1);
//	Variant vaard = iCluige.iVariant.fromVal(VT_DOUBLE, 3.04);
//	Variant vaarptr = iCluige.iVariant.fromVal(VT_POINTER, &vaard);
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

#ifdef IN_GAME_JAM_CREAJAM1

	//game jam 2023_02_04
	Node2D* testUberRootNode2D = iCluige.iNode2D.newNode2D();
	Node* myTestUberRootNode = testUberRootNode2D->_thisNode;
	iCluige.iNode.setName(myTestUberRootNode, "test uber root");
	iCluige.iNode2D.moveLocal(testUberRootNode2D, (Vector2){36., 40.});
	iCluige.iNode.addChild(iCluige.publicRoot2D, myTestUberRootNode);

	Node2D* myRootNode2D = iCluige.iNode2D.newNode2D();
	Node* myRootNode = myRootNode2D->_thisNode;
	iCluige.iNode.setName(myRootNode, "Game");
	iCluige.iNode2D.moveLocal(myRootNode2D, (Vector2){10., 25.});
	iCluige.iNode.addChild(/*iCluige.publicRoot2D*/myTestUberRootNode, myRootNode);

	SpriteText* groundSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* groundNode = groundSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(groundNode, "Ground");
	iCluige.iNode2D.moveLocal(groundSpriteText->_thisNode2D, (Vector2){0, 1});//11.5});//-8.5});//
	iCluige.iNode.addChild(myRootNode, groundNode);
	iCluige.iSpriteText.setText(groundSpriteText, "=================================================");


	SpriteText* blockSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* blockNode = blockSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(blockNode, "Block");
	blockSpriteText->offset = (Vector2){2, 1.5};//origin at center
	iCluige.iNode2D.moveLocal(blockSpriteText->_thisNode2D, (Vector2){0., -2.5});//
	iCluige.iNode.addChild(myRootNode, blockNode);

	struct VaEtVient* blockScript = newVaEtVient(blockNode);
	blockScript->ownerSprite = blockSpriteText;


	SpriteText* playerSpriteText = iCluige.iSpriteText.newSpriteText();
	Node* playerNode = playerSpriteText->_thisNode2D->_thisNode;
	iCluige.iNode.setName(playerNode, "Player");
	iCluige.iNode.addChild(myRootNode, playerNode);
	iCluige.iSpriteText.setText(playerSpriteText, " o\n'U`\n \"   ");
	playerSpriteText->offset = (Vector2){1, 2};//origin at feet of player
	iCluige.iNode2D.moveLocal(playerSpriteText->_thisNode2D, (Vector2){25., 0.});
	struct VaEtVientPlayer* playerScript = newVaEtVientPlayer(playerNode);
	playerScript->ownerSprite = playerSpriteText;
/*

 o        #####
'U`       #   #
 "        #   #
          #####

 */

	//iCluige.wantedFrameSeconds = .1;
	//iCluige.clock->scale = .6;
	//iCluige.iNode.printTreePretty(iCluige.privateRoot2D);

#endif // IN_GAME_JAM_CREAJAM1

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
