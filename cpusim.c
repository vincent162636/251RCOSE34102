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

Process new[MAX_PROCESSES];
int proc_count = 0;

int gantt_pid[MAX_TIME];
int gantt_len = 0;

int FCFS(int gantt_pid[]) {
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
                ready[i].finish = time + ready[i].burst;
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

    return time + 1;
}

int SJF(int gantt_pid[]) {
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
                ready[process_to_run].finish = time + ready[process_to_run].burst;
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

    return time + 1;
}

int Priority(int gantt_pid[]) {
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
                ready[process_to_run].finish = time + ready[process_to_run].burst;
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

    return time + 1;
}

int RR(int gantt_pid[]) {
    int time_q = 0;
    printf("Time Quantum: ");
    scanf("%d", &time_q);

    Process waiting[MAX_PROCESSES];
    for (int i = 0; i < proc_count; i++) {
        waiting[i] = new[i];
    }

    for (int i = 0; i < proc_count - 1; i++) {
        for (int j = 0; j < proc_count - i - 1; j++) {
            if (waiting[j].arrival > waiting[j+1].arrival) {
                Process temp = waiting[j];
                waiting[j] = waiting[j+1];
                waiting[j+1] = temp;
            }
        }
    }

    int time = 0;
    int how_much = 0;
    
    Process ready[MAX_PROCESSES];
    int ready_front = 0;
    int ready_rear = 0;
    int ready_size = 0;
    
    int current_process = -1;
    int time_q_track = 0;

    int requeue = 0;
    Process requeue_process;

    while (1) {
        for (int j = 0; j < proc_count; j++) {
            if (waiting[j].arrival == time && waiting[j].execution == 0) {
                ready[ready_rear] = waiting[j];
                ready_rear = (ready_rear + 1) % MAX_PROCESSES;
                ready_size++;
                waiting[j].execution = 1;
            }
        }

        if (requeue == 1) {
            ready[ready_rear] = requeue_process;
            ready_rear = (ready_rear + 1) % MAX_PROCESSES;
            ready_size++;
            requeue = 0;
        }

        if (current_process == -1 && ready_size > 0) {
            current_process = ready_front;
            ready_front = (ready_front + 1) % MAX_PROCESSES;
            ready_size--;
            time_q_track = 0;
        }

        if (current_process != -1) {
            gantt_pid[time] = ready[current_process].PID;
            ready[current_process].burst_copy--;
            time_q_track++;
            
            if (ready[current_process].burst_copy == 0) {
                for (int k = 0; k < proc_count; k++)
                    if (waiting[k].PID == ready[current_process].PID) {
                        waiting[k].finish     = time + 1;
                        waiting[k].turnaround = waiting[k].finish - waiting[k].arrival;
                        waiting[k].waiting    = waiting[k].turnaround - waiting[k].burst;
                        break;
                    }
                how_much++;
                current_process = -1;
            }
            else if (time_q_track == time_q) {
                requeue = 1;
                requeue_process = ready[current_process];
                current_process = -1;
            }
        } 
        else {
            gantt_pid[time] = 0;
        }

        if (how_much == proc_count) break;
        time++;
    }

    return time + 1;
}

int PSJF(int gantt_pid[]) {
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
                ready[process_to_run].finish = time + ready[process_to_run].burst;
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

    return time + 1;
}

int PPriority(int gantt_pid[]) {
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
                ready[process_to_run].finish = time + ready[process_to_run].burst;
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

    return time + 1;
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
    new[proc_count].execution = 0;
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

    if (answer == 1) gantt_len = FCFS(gantt_pid);
    else if (answer == 2) gantt_len = SJF(gantt_pid);
    else if (answer == 3) gantt_len = Priority(gantt_pid);
    else if (answer == 4) gantt_len = RR(gantt_pid);
    else if (answer == 5) gantt_len = PSJF(gantt_pid);
    else if (answer == 6) gantt_len = PPriority(gantt_pid);

    print_gantt_chart(gantt_pid, gantt_len);
}

int main() {
    printf("Create Process\n");
    int num = 1;
    while (num == 1) {
        printf("PID 0 reserved for idle time");
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
-> make a gantt tracker queue for every algorithm, and integrate it into the algorithm - done
-> make a gantt chart printer function, and get the queue from the algorithms to print the gantt chart. -> done
2. Random IO bursts
-> add a random io burst in gantt queue by integrating it into gantt chart printer function. 
-> so gantt chart printer function should get IO burst values and gantt tracker queue values to print the gantt chart.
3. Printing summary.
-> make a summary printer function, and get the queue from the algorithms to print the summary. -> I think I should integrate it into cantt chart printing.
*/