/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*					WIN32 PORT & LINUX PORT
*                          (c) Copyright 2004, Werner.Zimmermann@fht-esslingen.de
*                 (Similar to Example 1 of the 80x86 Real Mode port by Jean J. Labrosse)
*                                           All Rights Reserved
** *****************************************************************************************************
*		Further modified by mikael.jakas@puv.fi, jukka.matila@vamk.fi
* *****************************************************************************************************
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"
#define _CRT_SECURE_NO_WARNINGS

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        4       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
OS_EVENT *Mbox; 
#define PLAYER_1 0x01
#define PLAYER_2 0x02
#define DRAW 0x04
OS_FLAG_GRP* GameLogic;
INT8U board[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
INT8U err;


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data);        /* Function prototype of Startup task           */
void  Player (void *data);
void Draw(void* data);
void Analyze(void* data);


/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main (void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

	Mbox = OSMboxCreate((void *) 0);

    GameLogic = OSFlagCreate(0, &err);
	
    OSFlagPost(GameLogic, DRAW, OS_FLAG_SET, &err);

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

    OSStart();                                             /* Start multitasking                       */

    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart (void *pdata)
{
    INT16U key;
    INT8U one, two;
    one = 'X';
    two = 'O';

    pdata = pdata;                                         /* Prevent compiler warning                 */

    OSTaskCreate(Analyze, (void*)0, (void*)&TaskStk[3][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Draw, (void *) 0, (void*)&TaskStk[2][TASK_STK_SIZE - 1], 2);
	OSTaskCreate(Player, (void *) &one, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 3);
    OSTaskCreate(Player, (void *) &two, (void *)&TaskStk[1][TASK_STK_SIZE - 1], 4);
	
    for (;;) {	
        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                exit(0);  	                           /* End program                              */
            }
        }		
        OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void Player (void *data)
{
    srand(GetCurrentThreadId());
	INT8U temp;

    for (;;) {
        if (*((INT8U*)data) == 'X')
            OSFlagPend(GameLogic, DRAW, OS_FLAG_CONSUME, 0, &err);
        else if(*((INT8U*)data) == '0')
            OSFlagPend(GameLogic, PLAYER_1, OS_FLAG_CONSUME, 0, &err);
        do {
            temp = rand() % 9;
        } while (board[temp / 3][temp % 3] != ' ');
		OSMboxPost(Mbox, (void *) &temp); 
        if (*((INT8U*)data) == 1)
            OSFlagPost(GameLogic, PLAYER_1, OS_FLAG_SET, &err);
        else if (*((INT8U*)data) == 2)
            OSFlagPost(GameLogic, PLAYER_2, OS_FLAG_SET, &err);
		OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }
}

void Draw(void* data)
{
    srand(GetCurrentThreadId());
    INT8U temp, i, j;

    for (;;)
    {
        temp = *((INT8U*)OSMboxPend(Mbox, 0, &err));
        board[temp / 3][temp % 3] = 'X';
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
                PC_DispChar(10 + i, 10 + j, board[i][j], DISP_FGND_WHITE + DISP_BGND_BLACK);
        }
        temp = *((INT8U*)OSMboxPend(Mbox, 0, &err));
        board[temp / 3][temp % 3] = 'O';
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
                PC_DispChar(10 + i, 10 + j, board[i][j], DISP_FGND_WHITE + DISP_BGND_BLACK);
        }
        OSFlagPost(GameLogic, DRAW, OS_FLAG_SET, &err);
        OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
    }
}

void Analyze(void* data)
{
    srand(GetCurrentThreadId());
    INT8U winner = 0, i, cont = 0;
    INT8U one, two;
    one = 'X';
    two = 'O';

    for (;;)
    {
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')
            winner = board[0][0];

        if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[1][1] != ' ')
            winner = board[0][0];

        for (i = 0; i < 3 && winner == 0; i++)
        {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            {
                winner = board[i][0];
                break;
            }
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            {
                winner = board[0][i];
                break;
            }
        }
        for (i = 0; i < 9; i++)
        {
            if (board[i/3][i%3] == ' ')
            {
                cont = 1;
                break;
            }
        }
        if (winner == 'X')
            PC_DispStr(10, 15, "Player 1 wins.", DISP_FGND_WHITE + DISP_BGND_BLACK);
        else if (winner == 'O')
            PC_DispStr(10, 15, "Player 2 wins.", DISP_FGND_WHITE + DISP_BGND_BLACK);
        else if (cont == 0)
            PC_DispStr(10, 15, "The game results in a tie.", DISP_FGND_WHITE + DISP_BGND_BLACK);

        if (cont == 0 || winner != 0)
        {
            OSTaskDel(2);
            OSTaskDel(3);
            OSTaskDel(4);
            OSTimeDlyHMSM(0, 0, 5, 0);                         /* Wait 5 seconds                          */
            PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */
            for (i = 0; i < 9; i++)
                board[i / 3][i % 3] = ' ';
            OSTaskCreate(Draw, (void*)0, (void*)&TaskStk[2][TASK_STK_SIZE - 1], 2);
            OSTaskCreate(Player, (void*)&one, (void*)&TaskStk[0][TASK_STK_SIZE - 1], 3);
            OSTaskCreate(Player, (void*)&two, (void*)&TaskStk[1][TASK_STK_SIZE - 1], 4);
            cont = 0;
            winner = 0;
        }
        else cont = 0;
        OSTimeDlyHMSM(0, 0, 0, 500);                         /* Wait half a second                          */
    }
}
/*
*********************************************************************************************************
*                                      NON-TASK FUNCTIONS 
*********************************************************************************************************
*/

void Print_to_Screen(INT8U *text_ptr)
{
	INT8U err;
	INT8U delay, i;
	INT8U symbols[80]= {' '};
	delay = -1;

	OSSemPend(RandomSem, 0, &err); /*  Acquire semaphore for random number generator numbers      */
	while(delay < 1 || delay >3)
		delay = rand();
	OSSemPost(RandomSem);

	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); 
	OSTimeDlyHMSM(0, 0, delay, 0);
	for(i=0; i<strlen(text_ptr); i++)
		PC_DispChar(33, 15, text_ptr[i], DISP_FGND_WHITE);
	OSTimeDlyHMSM(0, 0, delay, 0);
	for(i=0; i<strlen(symbols); i++)
		PC_DispChar(33, 0, symbols[i], DISP_FGND_GREEN);
}