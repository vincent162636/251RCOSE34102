#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES  32
#define MAX_TIME       500

typedef struct {
    int PID;
    int arrival;
    int burst;
    int burst_copy; // for RR and preemptive
    int ioburst;
    //io request and count = random
    int priority;
    int finish;
    int waiting;
    int turnaround;
    int execution;
} Process;

typedef struct {
    int PID;
    int finish;
    int waiting;
    int turnaround;
} Summary;

Process new[MAX_PROCESSES];
int proc_count = 0;
Summary result[6][MAX_PROCESSES];

int gantt_pid[6][MAX_TIME];
int gantt_len[6] = {0, 0, 0, 0, 0, 0};


int FCFS(int gantt_pid[], Summary summary[]) {
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
    int i = 0;
    int how_much = 0;

    while (1) {
        if (ready[i].arrival <= time && ready[i].execution == 0) {
            ready[i].burst_copy--;
            gantt_pid[time] = ready[i].PID;
            if (ready[i].burst_copy == 0) {
                ready[i].execution = 1;
                ready[i].finish = time + 1;
                ready[i].turnaround = ready[i].finish - ready[i].arrival;
                ready[i].waiting = ready[i].turnaround - ready[i].burst;
                how_much++;
                i++;
            }
        }
        else gantt_pid[time] = 0;
        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

int SJF(int gantt_pid[], Summary summary[]) {
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
    int i = 0;
    int how_much = 0;
    int process_to_run = proc_count + 1;
    int min_burst = MAX_TIME;

    while (1) {
        if (process_to_run == proc_count + 1) {
            min_burst = MAX_TIME;
            for (int j = 0; j < proc_count; j++) {
                if (ready[j].arrival <= time && ready[j].execution == 0) {
                    if (ready[j].burst < min_burst) {
                        min_burst = ready[j].burst;
                        process_to_run = j;
                    }   
                }
            }
        }

        if (process_to_run != proc_count + 1) {
            ready[process_to_run].burst_copy--;
            gantt_pid[time] = ready[process_to_run].PID;
            if (ready[process_to_run].burst_copy == 0) {
                ready[process_to_run].execution = 1;
                ready[process_to_run].finish = time + 1;
                ready[process_to_run].turnaround = ready[process_to_run].finish - ready[process_to_run].arrival;
                ready[process_to_run].waiting = ready[process_to_run].turnaround - ready[process_to_run].burst;
                how_much++;
                process_to_run = proc_count + 1;
            }

        }
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

int Priority(int gantt_pid[], Summary summary[]) {
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
    int i = 0;
    int how_much = 0;
    int process_to_run = proc_count + 1;
    int min_priority = MAX_TIME;

    while (1) {
        if (process_to_run == proc_count + 1) {
            min_priority = MAX_TIME;
            for (int j = 0; j < proc_count; j++) {
                if (ready[j].arrival <= time && ready[j].execution == 0) {
                    if (ready[j].priority < min_priority) {
                        min_priority = ready[j].priority;
                        process_to_run = j;
                    }   
                }
            }
        }

        if (process_to_run != proc_count + 1) {
            ready[process_to_run].burst_copy--;
            gantt_pid[time] = ready[process_to_run].PID;
            if (ready[process_to_run].burst_copy == 0) {
                ready[process_to_run].execution = 1;
                ready[process_to_run].finish = time + 1;
                ready[process_to_run].turnaround = ready[process_to_run].finish - ready[process_to_run].arrival;
                ready[process_to_run].waiting = ready[process_to_run].turnaround - ready[process_to_run].burst;
                how_much++;
                process_to_run = proc_count + 1;
            }

        }
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

int RR(int gantt_pid[], Summary summary[]) {
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
    int how_much = 0;
    
    Process rr_queue[MAX_PROCESSES];
    int front = 0;
    int rear = 0;
    int size = 0;
    
    int current_process = -1;
    int time_q_track = 0;

    int requeue = 0;
    Process requeue_process;

    while (1) {
        for (int j = 0; j < proc_count; j++) {
            if (ready[j].arrival == time && ready[j].execution == 0) {
                rr_queue[rear] = ready[j];
                rear = (rear + 1) % MAX_PROCESSES;
                size++;
                ready[j].execution = 1;
            }
        }

        if (requeue == 1) {
            rr_queue[rear] = requeue_process;
            rear = (rear + 1) % MAX_PROCESSES;
            size++;
            requeue = 0;
        }

        if (current_process == -1 && size > 0) {
            current_process = front;
            front = (front + 1) % MAX_PROCESSES;
            size--;
            time_q_track = 0;
        }

        if (current_process != -1) {
            gantt_pid[time] = rr_queue[current_process].PID;
            rr_queue[current_process].burst_copy--;
            time_q_track++;
            
            if (rr_queue[current_process].burst_copy == 0) {
                for (int k = 0; k < proc_count; k++)
                    if (ready[k].PID == rr_queue[current_process].PID) {
                        ready[k].finish     = time + 1;
                        ready[k].turnaround = ready[k].finish - ready[k].arrival;
                        ready[k].waiting    = ready[k].turnaround - ready[k].burst;
                        break;
                    }
                how_much++;
                current_process = -1;
            }
            else if (time_q_track == time_q) {
                requeue = 1;
                requeue_process = rr_queue[current_process];
                current_process = -1;
            }
        } 
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

int PSJF(int gantt_pid[], Summary summary[]) {
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
    int how_much = 0;

    while (1) {
        int process_to_run = proc_count + 1;
        int min_burst = MAX_TIME;
        
        for (int j = 0; j < proc_count; j++) {
            if (ready[j].arrival <= time && ready[j].execution == 0) {
                if (ready[j].burst_copy < min_burst) {
                    min_burst = ready[j].burst_copy;
                    process_to_run = j;
                }
            }
        }

        if (process_to_run != proc_count + 1) {
            gantt_pid[time] = ready[process_to_run].PID;
            ready[process_to_run].burst_copy--;
            if (ready[process_to_run].burst_copy == 0) {
                ready[process_to_run].execution = 1;
                ready[process_to_run].finish = time + 1;
                ready[process_to_run].turnaround = ready[process_to_run].finish - ready[process_to_run].arrival;
                ready[process_to_run].waiting = ready[process_to_run].turnaround - ready[process_to_run].burst;
                how_much++;
            }
        }
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

int PPriority(int gantt_pid[], Summary summary[]) {
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
    int how_much = 0;

    while (1) {
        int process_to_run = proc_count + 1;
        int min_priority = MAX_TIME;
        
        for (int j = 0; j < proc_count; j++) {
            if (ready[j].arrival <= time && ready[j].execution == 0) {
                if (ready[j].priority < min_priority) {
                    min_priority = ready[j].priority;
                    process_to_run = j;
                }
            }
        }

        if (process_to_run != proc_count + 1) {
            gantt_pid[time] = ready[process_to_run].PID;
            ready[process_to_run].burst_copy--;
            if (ready[process_to_run].burst_copy == 0) {
                ready[process_to_run].execution = 1;
                ready[process_to_run].finish = time + 1;
                ready[process_to_run].turnaround = ready[process_to_run].finish - ready[process_to_run].arrival;
                ready[process_to_run].waiting = ready[process_to_run].turnaround - ready[process_to_run].burst;
                how_much++;
            }
        }
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    for (int j = 0; j < proc_count; j++) {
        summary[j].PID        = ready[j].PID;
        summary[j].turnaround = ready[j].turnaround;
        summary[j].waiting    = ready[j].waiting;
    }

    return time + 1;
}

void reset_state(void)
{
    for (int p = 0; p < proc_count; ++p) {
        new[p].burst_copy = new[p].burst;
        new[p].execution  = 0;
    }
}

void print_gantt_chart(int gantt_pid[], int gantt_len) {
    int track = gantt_pid[0];
    int time = 0;
    for (int i = 1; i < gantt_len; i++) {
        if (gantt_pid[i] != track) {
            if (track == 0) printf("%d -Idle- ", time);
            else printf("%d -P%d- ", time, track);
            time = i;
            track = gantt_pid[i];
        }
    }
    printf("%d -P%d- %d", time, track, gantt_len);
    printf("\n");
}

void print_summary(Summary summary[6][MAX_PROCESSES]) {
    printf("Algorithm\tTurnaround\tWaiting\n");
    for (int i = 0; i < 6; i++) {
        if (i == 0) printf("FCFS\t");
        else if (i == 1) printf("SJF\t");
        else if (i == 2) printf("Priority\t");
        else if (i == 3) printf("RR\t");
        else if (i == 4) printf("Preemptive SJF\t");
        else if (i == 5) printf("Preemptive Priority\t");
        float avg_turnaround = 0;
        float avg_waiting = 0;
        for (int j = 0; j < proc_count; j++) {
            avg_turnaround += summary[i][j].turnaround;
            avg_waiting += summary[i][j].waiting;
        }
        avg_turnaround /= proc_count;
        avg_waiting /= proc_count;
        printf("%.2f\t%.2f\t\n", avg_turnaround, avg_waiting);
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
    printf("Priority: ");
    scanf("%d", &new[proc_count].priority);
    new[proc_count].execution = 0;
    proc_count++;
}

void Schedule(void) {
    gantt_len[0] = FCFS(gantt_pid[0], result[0]);
    gantt_len[1] = SJF(gantt_pid[1], result[1]);
    gantt_len[2] = Priority(gantt_pid[2], result[2]);
    gantt_len[3] = RR(gantt_pid[3], result[3]);
    gantt_len[4] = PSJF(gantt_pid[4], result[4]);
    gantt_len[5] = PPriority(gantt_pid[5], result[5]); 

    for (int i = 0; i < 6; i++) {
        if (i == 0) printf("FCFS\t");
        else if (i == 1) printf("SJF\t");
        else if (i == 2) printf("Priority\t");
        else if (i == 3) printf("RR\t");
        else if (i == 4) printf("Preemptive SJF\t");
        else if (i == 5) printf("Preemptive Priority\t");
        print_gantt_chart(gantt_pid[i], gantt_len[i]);
    }

    print_summary(result);
}

int main() {
    printf("Create Process\n");
    int num = 1;
    while (num == 1) {
        printf("PID 0 reserved for idle time\n");
        Create_Process();
        printf("1) Keep adding process\n");
        printf("2) Simulate algorithms\n");
        scanf("%d", &num);
    }
    Schedule();
    return 0;
}

/*
What's left?

1. Gantt Chart Printing
-> make a gantt tracker queue for every algorithm, and integrate it into the algorithm - done
-> make a gantt chart printer function, and get the queue from the algorithms to print the gantt chart. -> done
2. Random IO bursts
-> add a random io burst in gantt queue by integrating it into gantt chart printer function. 
-> so gantt chart printer function should get IO burst values and gantt tracker queue values to print the gantt chart.
3. Printing summary.
-> done
*/