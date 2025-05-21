#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES  32
#define MAX_TIME       500

typedef struct {
    int PID;
    int arrival;
    int burst;
    int burst_copy; // for RR
    int ioburst;
    //io request and count = random
    int priority;
    int finish;
    int waiting;
    int turnaround;
    int completed;
} Process;

Process new[MAX_PROCESSES];

int proc_count = 0;
void FCFS() {
    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }

    int time = ready[0].arrival;
    for (int i = 0; i < proc_count; i++) {
        ready[i].finish = time + ready[i].burst;
        ready[i].turnaround = ready[i].finish - ready[i].arrival;
        ready[i].waiting = ready[i].turnaround - ready[i].burst;
        time = ready[i].finish;
    }

    for (int i = 0; i < proc_count; i++) {
        printf("%d %d %d\n", ready[i].finish, ready[i].turnaround, ready[i].waiting);
    }

}

void SJF(void) {
    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }
    
    int time = ready[0].arrival;
    int completed = 0;

    while (completed < proc_count) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;

        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].completed == 0) {
                arrived[arrived_count] = ready[i];
                arrived_count++;
            }
        }

        if (arrived_count == 0) {
            time++;
            continue;
        }

        int arrived_min = 0;
        
        for (int i = 1; i < arrived_count; i++) {
            if (arrived[i].burst < arrived[arrived_min].burst){
                arrived_min = i;
            }
        }    
        
        
        for (int i = 0; i < proc_count; i++) {
            if (ready[i].PID == arrived[arrived_min].PID) {
                ready[i].finish = time + ready[i].burst;
                ready[i].turnaround = ready[i].finish - ready[i].arrival;
                ready[i].waiting = ready[i].turnaround - ready[i].burst;
                time = ready[i].finish;
                ready[i].completed = 1;
                completed++;
                break;
            }
        }
    }

    for (int i = 0; i < proc_count; i++) {
            printf("%d %d %d\n", ready[i].finish, ready[i].turnaround, ready[i].waiting);
    }
    
}

void Priority(void) {
    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }
    
    int time = ready[0].arrival;
    int completed = 0;

    while (completed < proc_count) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;

        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].completed == 0) {
                arrived[arrived_count] = ready[i];
                arrived_count++;
            }
        }

        if (arrived_count == 0) {
            time++;
            continue;
        }

        int arrived_min = 0;
        
        for (int i = 1; i < arrived_count; i++) {
            if (arrived[i].priority < arrived[arrived_min].priority){
                arrived_min = i;
            }
        }    
        
        
        for (int i = 0; i < proc_count; i++) {
            if (ready[i].PID == arrived[arrived_min].PID) {
                ready[i].finish = time + ready[i].burst;
                ready[i].turnaround = ready[i].finish - ready[i].arrival;
                ready[i].waiting = ready[i].turnaround - ready[i].burst;
                time = ready[i].finish;
                ready[i].completed = 1;
                completed++;
                break;
            }
        }
    }

    for (int i = 0; i < proc_count; i++) {
            printf("%d %d %d\n", ready[i].finish, ready[i].turnaround, ready[i].waiting);
    }
}

void RR(void) {
    int time_q = 0;
    printf("Time Quantum: ");
    scanf("%d", &time_q);

    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }

    int time = 0;
    int completed = 0;
    int num = 0;
    int flag = 0;

    while(1) {
        if (ready[num].arrival <= time && ready[num].completed == 0) {
            flag = 1;

            if (time_q < ready[num].burst_copy) {
                time += time_q;
                ready[num].burst_copy -= time_q;
            } 
            else {
                time += ready[num].burst_copy;
                ready[num].finish = time;
                ready[num].turnaround = ready[num].finish - ready[num].arrival;
                ready[num].waiting = ready[num].turnaround - ready[num].burst;
                ready[num].completed = 1;
                completed++;
            }
        }

        if (completed == proc_count) break;

        num++;
        if (num >= proc_count) {
            if (flag == 0 && completed < proc_count) {
                time++;
            }
            num = 0;
            flag = 0;
        }
    }

    for (int i = 0; i < proc_count; i++) {
            printf("%d %d %d\n", ready[i].finish, ready[i].turnaround, ready[i].waiting);
        }
        
}

void PSJF(void) {
    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }

    int time      = ready[0].arrival;
    int completed = 0;

    while (completed < proc_count) {

        Process arrived[MAX_PROCESSES];
        int arrived_cnt = 0;

        for (int i = 0; i < proc_count; i++)
            if (time >= ready[i].arrival && ready[i].completed == 0)
                arrived[arrived_cnt++] = ready[i];

        if (arrived_cnt == 0) {
            time++;
            continue;
        }

        int sel = 0;
        for (int i = 1; i < arrived_cnt; i++)
            if (arrived[i].burst_copy < arrived[sel].burst_copy)
                sel = i;

        for (int i = 0; i < proc_count; i++)
            if (ready[i].PID == arrived[sel].PID) {

                ready[i].burst_copy--;
                time++;

                if (ready[i].burst_copy == 0) {
                    ready[i].finish     = time;
                    ready[i].turnaround = ready[i].finish - ready[i].arrival;
                    ready[i].waiting    = ready[i].turnaround - ready[i].burst;
                    ready[i].completed  = 1;
                    completed++;
                }
                break;
            }
    }

    for (int i = 0; i < proc_count; i++) {
        printf("%d %d %d\n",
         ready[i].finish,
         ready[i].turnaround,
         ready[i].waiting);
    }
        
}


void PPriority(void) {
    Process ready[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        ready[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (ready[j].arrival > ready[j+1].arrival) {
                Process temp = ready[j];
                ready[j] = ready[j+1];
                ready[j+1] = temp;
            }
        }
    }

    int time      = ready[0].arrival;
    int completed = 0;

    while (completed < proc_count) {

        Process arrived[MAX_PROCESSES];
        int arrived_cnt = 0;

        for (int i = 0; i < proc_count; i++)
            if (time >= ready[i].arrival && ready[i].completed == 0)
                arrived[arrived_cnt++] = ready[i];

        if (arrived_cnt == 0) {
            time++;
            continue;
        }

        int sel = 0;
        for (int i = 1; i < arrived_cnt; i++)
            if (arrived[i].priority < arrived[sel].priority)
                sel = i;

        for (int i = 0; i < proc_count; i++)
            if (ready[i].PID == arrived[sel].PID) {

                ready[i].burst_copy--;
                time++;

                if (ready[i].burst_copy == 0) {
                    ready[i].finish     = time;
                    ready[i].turnaround = ready[i].finish - ready[i].arrival;
                    ready[i].waiting    = ready[i].turnaround - ready[i].burst;
                    ready[i].completed  = 1;
                    completed++;
                }
                break;
            }
    }

    for (int i = 0; i < proc_count; i++) {
        printf("%d %d %d\n",
         ready[i].finish,
         ready[i].turnaround,
         ready[i].waiting);
    }
}


void Create_Process(void) {
    if (proc_count == MAX_PROCESSES) {
        printf("Error. Max number of Processes reached.");
        return;
    }
    printf("PID: ");
    scanf("%d", &new[proc_count].PID);
    printf("Arrival Time: ");
    scanf("%d", &new[proc_count].arrival);
    printf("CPU Burst Time: ");
    scanf("%d", &new[proc_count].burst);
    new[proc_count].burst_copy = new[proc_count].burst;
    printf("I/O Burst Time: ");
    scanf("%d", &new[proc_count].ioburst);
    printf("Priority: ");
    scanf("%d", &new[proc_count].priority);
    new[proc_count].completed = 0;
    proc_count++;
}

void Schedule(void) {
    printf("Select Algorithm: \n");
    printf("1) FCFS\n");
    printf("2) SJF\n");
    printf("3) Priority\n");
    printf("4) RR\n");
    printf("5) Preemptive SJF\n");
    printf("6) Preemptive Priority\n");
    int answer = 0;
    scanf("%d", &answer);

    if (answer == 1) FCFS();
    else if (answer == 2) SJF();
    else if (answer == 3) Priority();
    else if (answer == 4) RR();
    else if (answer == 5) PSJF();
    else if (answer == 6) PPriority();
}

int main() {
    printf("Create Process\n");
    int num = 1;
    while (num == 1) {
        Create_Process();
        printf("1) Keep adding process\n");
        printf("2) Choose algorithm\n");
        scanf("%d", &num);
    }
    Schedule();
    return 0;
}

/*
What's left?

1. Gantt Chart Printing
2. Random IO bursts
3. Printing summary.
*/