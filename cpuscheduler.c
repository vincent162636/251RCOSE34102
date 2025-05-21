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
        ready[i] = new[i]; // Copy from global 'new' to local 'ready'
    }

    // Bubble sort 'ready' array by arrival time
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
        ready[i] = new[i]; // Copy from global 'new' to local 'ready'
    }

    // Bubble sort 'ready' array by arrival time
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
    ready[0].finish = time + ready[0].burst;
    ready[0].turnaround = ready[0].finish - ready[0].arrival;
    ready[0].waiting = ready[0].turnaround - ready[0].burst;
    time = ready[0].finish;
    ready[0].completed = 1;

    for (int j = 0; j < proc_count - 1; j++) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;
        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].completed == 0) {
                arrived[arrived_count] = ready[i];
                arrived_count++;
            }
        }

        int arrived_min = 0;

        if (arrived_count == 0) {
            break; // All processes might be completed or no new ones arrived
        }

        if (arrived_count > 1) {
            for (int i = 1; i < arrived_count; i++) {
                if (arrived[i].burst < arrived[arrived_min].burst){
                    arrived_min = i;
                }
            }
        }
        
        for (int i = 0; i < proc_count; i++) {
            if (ready[i].PID == arrived[arrived_min].PID) {
                ready[i].finish = time + ready[i].burst;
                ready[i].turnaround = ready[i].finish - ready[i].arrival;
                ready[i].waiting = ready[i].turnaround - ready[i].burst;
                time = ready[i].finish;
                ready[i].completed = 1;
                break;
            }
        }
    }

    for (int i = 0; i < proc_count; i++) {
            printf("%d %d %d\n", ready[i].finish, ready[i].turnaround, ready[i].waiting);
        }
    
    //Problem: if no one is in arrival at first try, it just gives up. Actually, we need to time++ and retry until all processes are done.
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
    ready[0].finish = time + ready[0].burst;
    ready[0].turnaround = ready[0].finish - ready[0].arrival;
    ready[0].waiting = ready[0].turnaround - ready[0].burst;
    time = ready[0].finish;
    ready[0].completed = 1;
    
    for (int j = 0; j < proc_count - 1; j++) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;
        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].completed == 0) {
                arrived[arrived_count] = ready[i];
                arrived_count++;
            }
        }

        int arrived_min = 0;

        if (arrived_count == 0) {
            break;
        }

        if (arrived_count > 1) {
            for (int i = 1; i < arrived_count; i++) {
                if (arrived[i].priority < arrived[arrived_min].priority){
                    arrived_min = i;
                }
            }
        }
        
        for (int i = 0; i < proc_count; i++) {
            if (ready[i].PID == arrived[arrived_min].PID) {
                ready[i].finish = time + ready[i].burst;
                ready[i].turnaround = ready[i].finish - ready[i].arrival;
                ready[i].waiting = ready[i].turnaround - ready[i].burst;
                time = ready[i].finish;
                ready[i].completed = 1;
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

}


void PPriority(void) {

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
    /*
    else if (answer == 5) PSJF();
    else PPriority();
    */
}

int main() {
    Create_Process();
    Create_Process();
    Create_Process();
    Schedule();
    return 0;
}