/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"
#include "system.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];
OS_STK    task4_stk[TASK_STACKSIZE];
/* Definition of Task Priorities */
#define R1_Priority         1
#define R2_Priority         2

#define TASK1_PRIORITY      11
#define TASK2_PRIORITY      12
#define TASK3_PRIORITY      13
#define TASK4_PRIORITY      14

void wait(int tick)
{
    #if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
        OS_CPU_SR  cpu_sr = 0;
    #endif
    int now,exit;
    OS_ENTER_CRITICAL();
    now=OSTimeGet();
    exit=now+tick;
    OS_EXIT_CRITICAL();
    while(1){        
        if(exit<=OSTimeGet())     
            break; 
   } 
}


void task1(void* pdata)
{
  INT8U prio=TASK1_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  INT8U err;
  OSTimeDly(2);
  while (1)
  { 
    ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    printf("Time %d\t\t Task 1\n",OSTime);
    while( 0 < ptcb->REMAINING_TIME){ 

    }
    printf("Time %d\t\t Task 1 get R2\n",OSTime);
    OSMutexPend(R2,0,&err);
    wait(3);
    printf("Time %d\t\t Task 1 get R1\n",OSTime);
    OSMutexPend(R1,0,&err);
    wait(2);
    printf("Time %d\t\t Task 1 release R1\n",OSTime);
    OSMutexPost(R1);
    printf("Time %d\t\t Task 1 release R2\n",OSTime);
    OSMutexPost(R2);
    
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->DEADLINE = ptcb->period + ptcb->DEADLINE;
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    if(todelay==0){
       OS_Sched();                 
    }
    ptcb->RESPONSE_TIME   = 0;
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{ 
  INT8U prio=TASK2_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  INT8U err;
  OSTimeDly(0);
  while (1)
  {
    
   ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    printf("Time %d\t\t Task 2\n",OSTime);
    while( 0 < ptcb->REMAINING_TIME){ 

    }
//    printf("Time %d\t\t Task 2 get R2\n",OSTime);
//    OSMutexPend(R2,0,&err);
//    wait(2);
//    printf("Time %d\t\t Task 2 get R1\n",OSTime);
//    OSMutexPend(R1,0,&err);
//    wait(4);
//    printf("Time %d\t\t Task 2 release R1\n",OSTime);
//    OSMutexPost(R1);
//    printf("Time %d\t\t Task 2 release R2\n",OSTime);
//    OSMutexPost(R2);
    
     printf("Time %d\t\t Task 2 get R1\n",OSTime);
    OSMutexPend(R1,0,&err);
    wait(7);
    printf("Time %d\t\t Task 2 get R2\n",OSTime);
    OSMutexPend(R2,0,&err);
    wait(2);
    printf("Time %d\t\t Task 2 release R2\n",OSTime);
    OSMutexPost(R2);
    printf("Time %d\t\t Task 2 release R1\n",OSTime);
    OSMutexPost(R1);
    
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->DEADLINE = ptcb->period + ptcb->DEADLINE;
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    if(todelay==0){
       OS_Sched();                
    }
    ptcb->RESPONSE_TIME   = 0;
  }
}

void task3(void* pdata)
{
  INT8U prio=TASK3_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  ptcb->REMAINING_TIME    = ptcb->compute;
  INT8U err;
  while (1)
  {
    
   ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    printf("Time %d\t\t Task 3\n",OSTime);
    while( 0 < ptcb->REMAINING_TIME){ 

    }
    printf("Time %d\t\t Task 3 get R1\n",OSTime);
    OSMutexPend(R1,0,&err);
    wait(7);
    printf("Time %d\t\t Task 3 get R2\n",OSTime);
    OSMutexPend(R2,0,&err);
    wait(2);
    printf("Time %d\t\t Task 3 release R2\n",OSTime);
    OSMutexPost(R2);
    printf("Time %d\t\t Task 3 release R1\n",OSTime);
    OSMutexPost(R1);
    
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->DEADLINE = ptcb->period + ptcb->DEADLINE;
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    if(todelay==0){
       OS_Sched();        
    }
    ptcb->RESPONSE_TIME   = 0;
  }
}

void task4(void* pdata)
{
  INT8U prio=TASK4_PRIORITY;
  OS_TCB *ptcb= OSTCBPrioTbl[prio];
  INT8U err;
  ptcb->REMAINING_TIME    = ptcb->compute;
  while (1)
  {
    ptcb->TASK_ACTUAL_START_TIME = OSTimeGet();  
    printf("Time%d\t\t Task 1\n",OSTime);
    wait(3);
    printf("Time%d\t\t Task 1 get R1",OSTime);
    OSMutexPend(R1,0,&err);
    wait(7);
    printf("Time%d\t\t Task 1 get R2",OSTime);
    OSMutexPend(R2,0,&err);
    wait(2);
    printf("Time%d\t\t Task 1 release R2",OSTime);
    OSMutexPost(R2);
    printf("Time%d\t\t Task 1 release R1",OSTime);
    OSMutexPost(R1);
    
    ptcb->RESPONSE_TIME = OSTimeGet() - ptcb->TASK_SHOULD_START_TIME;
    int todelay = ptcb->period - ptcb->RESPONSE_TIME;
    ptcb->TASK_SHOULD_START_TIME = ptcb->period + ptcb->TASK_SHOULD_START_TIME; 
    ptcb->DEADLINE = ptcb->period + ptcb->DEADLINE;
    ptcb->REMAINING_TIME    = ptcb->compute;
    OSTimeDly(todelay);
    if(todelay==0){
       OS_Sched();        
    }
    ptcb->RESPONSE_TIME   = 0;
  }
}




/* The main function creates two task and starts multi-tasking */
int main(void)
{
  INT8U err;
  R1=OSMutexCreate(R1_Priority,&err);
  R2=OSMutexCreate(R2_Priority,&err);
  OSTaskCreate(task1,NULL,(void *)&task1_stk[TASK_STACKSIZE-1],TASK1_PRIORITY, 1, 2 , 2, 28);
  OSTaskCreate(task2,NULL,(void *)&task2_stk[TASK_STACKSIZE-1],TASK2_PRIORITY, 2,0 , 3, 40);
  //OSTaskCreate(task3,NULL,(void *)&task3_stk[TASK_STACKSIZE-1],TASK3_PRIORITY, 3,0 , 3, 70);
  //OSTaskCreate(task4,NULL,(void *)&task4_stk[TASK_STACKSIZE-1],TASK4_PRIORITY, 4, 5, 20);
  printf("Time\t\t Event\n");
  OSTimeSet(0);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
