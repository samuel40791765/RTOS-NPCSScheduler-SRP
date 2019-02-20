Modified the kernel to have tasks support resource utilization on the Nios II 12sp2. Kernel was modified to support both the Non Preemptive Critical Section and the Stack Resource Policy. <br/>
Code only includes the basic OS Files of the RTOS System and the files I modified.<br/><br/>


PART 1 NPCS Implementation <br/>
1.	Implementation Description: <br/>
The main objective of this project is to implement non-preemptible critical section within our earliest deadline first (EDF) scheduler. After observing the structure of this code, I decided to implement my code within OSMutexPend and OSMutexPost. Both of these functions are responsible for deciding which resources to use, so I decided to rewrite the code within this function.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic1.png)

(1)	Non-Premptible Section <br/>
In a non-preemptible critical section implementation, the scheduler never allows a context switch whenever there are resources in use. To implement this, I decided to use the function OSSchedLock to control when to disable context switches.I implemented OSSchedLock() within OSMutexPend and OSSchedUnlock() within OSMutexPost. By doing this I disable context switches whenever we grab a resource and enable context switches again whenever we let go of the resource. <br/>

OSMutexPend: <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic2.png)
 

OSMutexPost: <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic3.png)

(2)	Task Simulation <br/>
The tasks we are trying to simulate can be divided into four main parts: <br/>
(a)	Task Awaiting time (Arrival Time) <br/>
(b)	Task Computation Time (CPU running time) <br/>
(c)	Task using Resource 1 time <br/>
(d)	Task using Resource 2 time <br/>

a: We simulate the task awaiting time by delaying the task briefly before it starts running the infinity while loop. After the task’s arrival time is up, we start running the task periodically. <br/>
b: We simulate the CPU running time the same way we simulated task computing in our previous projects. We put the remaining time in another while loop and subtract from the OSTCBCur->RemainingTime within OSTimeTick to simulate task computing. <br/>
c & d: Because our tasks cannot be preempted when we are using resources, we do not have to simulate the task the same way as our CPU running time.
Instead, we implemented a “wait” function within the application layer to simulate waiting for a certain amount of time. 
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic4.png)


The wait function used to simulate resource usage time <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic5.png)

2.	Simulation Results <br/>
(1)	Task Set 1 (Arrival Time, Period): Task1: (2,28) CPU: 2 R2: 3 R1:2 Task2: (0,40) CPU: 3 R1: 7 R2:2 <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic6.png)
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic7.png)

(2)	Task Set 2 (Arrival Time, Period):<br/>
Task1: (5,55) CPU: 7 <br/>
Task2: (4,59) CPU: 3 R2: 2 R1:4 <br/>
Task3: (0,70) CPU: 3 R1: 7 R1:2 <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic8.png)
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic9.png)

3.	Schedulability Analysis <br/>
(1)	Task Set 1: <br/>
Because the periods and computation times for both of these tasks are optimal, the only time they interfere with each other is every 120 ticks. Within our output, the only time that an interference happens is when Task 1 arrives before Task 2 finishes (At Time 2). Even though this is a NPCS protocol, because Task 2 has not locked up any resources, Task 1 can preempt Task 2 because it has an earlier deadline. In this task set, the earliest deadline task truly finishes first, while the later deadline task is finished later. <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic10.png)
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic11.png)


(2)	Task Set 2: <br/>
Task 1 and Task 2 both arrive when Task 3 is computing. Task 1 arrives at Time 5, and Task 2 arrives at Time 4. Both of these tasks have a nearer deadline than Task 3. Normally in an EDF scheduler Task 3 would be preempted by the earlier deadline tasks, Task 1 and Task 2. However, Task 3 is currently using resources so preemption is disabled. Task 1 runs after Task 3 is finished using its resources, and Task 2 runs after Task 1 (Task 1 has an earlier deadline). Even thought this is a EDF scheduler, the NPCS results in a less responsive result for the earlier deadline tasks. <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic12.png)

At Time 61-88, the tasks obey the rules of the EDF scheduler and run the task with the earlier deadline. There are no resource interferences within this time span.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic13.png)


 
PART 2 SRP Implementation <br/>
1.	Implementation Description: <br/>
The main objective of this project is to implement the Stack-Resource Policy. I decided to implement my code within OSTimeTick, OSMutexPend and OSMutexPost. <br/>
(1)	Variables <br/>
First, I implemented the variables I needed within the OS_EVENT and OS_TCB structures. <br/>
(a)	OS_EVENT: <br/>
I implemented a new “ceiling” variable to save the system ceiling of resources.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic14.png)

(b)	OS_TCB: <br/>
I implemented a new variable to save the original deadline of the task. The deadline will be changed/inherited often in a SRP setting so we have to save the original deadline.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic15.png) <br/>

(c)	Global Variables: <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic16.png) <br/>
I.	SYSTEM_CEILING: to save the current system ceiling <br/>
II.	PREV_SYSTEM_CEILING: to save the previous system ceiling <br/>
 
III.	HOLDING_RESOURCE_PRIORITY: to save the priority of the task currently holding the resource <br/>

(2)	New Implementations <br/>
(a)	OSTimeTick: <br/>
In OSTimeTick, I had to add an if statement to check whether or not the current task has to inherit a new task’s deadline.
We use HOLDING_RESOURCE_PRIORITY to check if there are resources being held. We also check if the arriving task’s priority is higher than the SYSTEM_CEILING’s priority.
If all of these are valid, we allow the current task to inherit the arriving task’s deadline and continue running the current task.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic17.png)

(b)	OSMutexPend: <br/>
In OSMutexPend, we simply change the previous system ceiling and system ceiling according the current values. If SYSTEM_CEILING is “0”, we can directly change the value. If SYSTEM_CEILING is larger than “0”, we have to adjust the previous system ceiling to the old system ceiling and change the current system ceiling into the new one.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic18.png)
 
(c)	OSMutexPost: <br/>
In OSMutexPost, we do two things.
First we change the current task’s deadline back to its original deadline. Next, we change the system ceiling into the previous system ceiling. If PREV_SYSTEM_CEILING is “0”, that means we can change the current system ceiling directly back to “0”.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic19.png)

(3)	Modifications to EDF <br/>
I had to modify the EDF scheduler I previously implemented in OSSchedNew. Besides checking which task has earlier deadline, we also have to check if the current system ceiling has a value. If it doesn’t we can just check for the earliest deadline. If there is a system ceiling, we have to check if the task has both an earlier deadline and a higher priority than the system ceiling before we can schedule it.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic20.png)

2.	Simulation Results <br/>
(1)	Task Set 1 (Arrival Time, Period): <br/>
Task1: (2,28) CPU: 2 R2: 3 R1:2 <br/>
Task2: (0,40) CPU: 3 R1: 7 R2:2 <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic21.png)
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic22.png)

 

(2)	Task Set 2 (Arrival Time, Period): <br/>
Task1: (5,55) CPU: 7 <br/>
Task2: (4,59) CPU: 3 R2: 2 R1:4 <br/>
Task3: (0,70) CPU: 3 R1: 7 R1:2 <br/>
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic23.png)
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic24.png)
 
 

3.	Schedulability Analysis <br/>
(1)	Task Set 1: <br/>
At Time 2, Task 2 has not used any resources yet so it is pre-emptible by Task 1. At Time 86, Task 1 arrives and has an earlier deadline than Task 2. However, Task 2 is currently using resources and Task 1 does not have a higher preemption level than the system ceiling. Due to these circumstances, Task 2 has inherit Task 1’s deadline of 114.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic25.png)



(2)	Task Set 2: <br/>
At Time 4, Task 3 is already using resources so the system ceiling is raised to 2. Since Task 2’s preemption level is not high enough and cannot be scheduled, Task 3 inherits its deadline.
However, at Time 5 Task 1’s preemption level is higher than the system ceiling. Task 1 preempts Task 3. We do not have to be afraid of deadlocks because Task 1 has no resource conflicts with Task 3. After Task 1 is finished, we return back to Task 3 and finish running it.
![alt text](https://raw.githubusercontent.com/samuel40791765/RTOS-NPCSScheduler-SRP/master/projectimages/pic26.png)


