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
    int execution_order;
} Process;

Process new[MAX_PROCESSES];
int gantt_time[MAX_TIME];
int gantt_pid[MAX_TIME];

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
    int current_execution_order = 1;
    for (int i = 0; i < proc_count; i++) {
        ready[i].finish = time + ready[i].burst;
        ready[i].turnaround = ready[i].finish - ready[i].arrival;
        ready[i].waiting = ready[i].turnaround - ready[i].burst;
        ready[i].execution_order = current_execution_order;
        current_execution_order++;
        time = ready[i].finish;
    }

    // Gantt Chart Generation for FCFS
    if (proc_count > 0) {
        int gantt_idx = 0;
        int last_event_end_time = gantt_time[0]; // Should be 0, initialized in main

        // For FCFS, the 'ready' array is already in execution order after sorting by arrival.
        for (int i = 0; i < proc_count; i++) {
            Process current_process = ready[i];
            int process_start_time_for_gantt = current_process.finish - current_process.burst;

            // Check for idle time before this process starts
            if (process_start_time_for_gantt > last_event_end_time) {
                if (gantt_idx < MAX_TIME) {
                    gantt_pid[gantt_idx] = 0; // PID 0 for Idle
                    gantt_time[gantt_idx] = process_start_time_for_gantt; // Idle ends, process starts
                    gantt_idx++;
                } 
            }

            // Add current process to Gantt chart
            if (gantt_idx < MAX_TIME) {
                gantt_pid[gantt_idx] = current_process.PID;
                gantt_time[gantt_idx] = current_process.finish;
                gantt_idx++;
            } 
            last_event_end_time = current_process.finish;
        }

        // Print Gantt Chart
        printf("\nGantt Chart (FCFS):\n");
        int current_chart_time = 0; // Assuming Gantt chart always starts at 0 for this visualization
        // If you want to use the global gantt_time[0] as a potential start offset, use: 
        // int current_chart_time = gantt_time[0]; // where gantt_time[0] is set in main
        printf("%d", current_chart_time);

        for (int k = 0; k < gantt_idx; k++) {
            if (gantt_pid[k] == 0) { // Idle segment
                // Idle segment from current_chart_time to gantt_time[k]
                printf(" --[IDLE]-- %d", gantt_time[k]);
            } else { // Process segment
                // Process segment from current_chart_time to gantt_time[k]
                printf(" --[P%d]-- %d", gantt_pid[k], gantt_time[k]);
            }
            current_chart_time = gantt_time[k]; // The end of this segment is the start of the next potential one
        }
        printf("\n");
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
    int current_execution_order = 1;

    while (completed < proc_count) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;

        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].execution_order == 0) {
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
                ready[i].execution_order = current_execution_order;
                current_execution_order++;
                completed++;
                break;
            }
        }
    }

    // Gantt Chart Generation for SJF
    if (proc_count > 0) {
        Process executed_processes[MAX_PROCESSES];
        for (int i = 0; i < proc_count; i++) {
            executed_processes[i] = ready[i];
        }

        // Sort executed_processes by execution_order
        for (int i = 0; i < proc_count - 1; i++) {
            for (int j = 0; j < proc_count - i - 1; j++) {
                if (executed_processes[j].execution_order > executed_processes[j+1].execution_order) {
                    Process temp = executed_processes[j];
                    executed_processes[j] = executed_processes[j+1];
                    executed_processes[j+1] = temp;
                }
            }
        }

        int gantt_idx = 0;
        int last_event_end_time = 0; // Assuming Gantt chart always starts at 0

        for (int i = 0; i < proc_count; i++) {
            Process current_process = executed_processes[i];
            // Ensure process has been executed (execution_order > 0)
            if (current_process.execution_order == 0) continue; 

            int process_start_time_for_gantt = current_process.finish - current_process.burst;

            if (process_start_time_for_gantt > last_event_end_time) {
                if (gantt_idx < MAX_TIME) {
                    gantt_pid[gantt_idx] = 0; 
                    gantt_time[gantt_idx] = process_start_time_for_gantt;
                    gantt_idx++;
                } 
            }

            if (gantt_idx < MAX_TIME) {
                gantt_pid[gantt_idx] = current_process.PID;
                gantt_time[gantt_idx] = current_process.finish;
                gantt_idx++;
            } 
            last_event_end_time = current_process.finish;
            if (gantt_idx >= MAX_TIME) break; 
        }

        // Print Gantt Chart
        printf("\nGantt Chart (SJF):\n");
        int current_chart_time = 0; 
        printf("%d", current_chart_time);

        for (int k = 0; k < gantt_idx; k++) {
            if (gantt_pid[k] == 0) {
                printf(" --[IDLE]-- %d", gantt_time[k]);
            } else {
                printf(" --[P%d]-- %d", gantt_pid[k], gantt_time[k]);
            }
            current_chart_time = gantt_time[k]; 
        }
        printf("\n");
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
    int current_execution_order = 1;

    while (completed < proc_count) {
        Process arrived[MAX_PROCESSES];
        int arrived_count = 0;

        for (int i = 0; i < proc_count; i++) {
            if (time >= ready[i].arrival && ready[i].execution_order == 0) {
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
                ready[i].execution_order = current_execution_order;
                current_execution_order++;
                completed++;
                break;
            }
        }
    }

    // Gantt Chart Generation for Priority
    if (proc_count > 0) {
        Process executed_processes[MAX_PROCESSES];
        for (int i = 0; i < proc_count; i++) {
            executed_processes[i] = ready[i];
        }

        // Sort executed_processes by execution_order
        for (int i = 0; i < proc_count - 1; i++) {
            for (int j = 0; j < proc_count - i - 1; j++) {
                if (executed_processes[j].execution_order > executed_processes[j+1].execution_order) {
                    Process temp = executed_processes[j];
                    executed_processes[j] = executed_processes[j+1];
                    executed_processes[j+1] = temp;
                }
            }
        }

        int gantt_idx = 0;
        int last_event_end_time = 0; // Assuming Gantt chart always starts at 0

        for (int i = 0; i < proc_count; i++) {
            Process current_process = executed_processes[i];
            // Ensure process has been executed (execution_order > 0)
            if (current_process.execution_order == 0) continue;
            
            int process_start_time_for_gantt = current_process.finish - current_process.burst;

            if (process_start_time_for_gantt > last_event_end_time) {
                if (gantt_idx < MAX_TIME) {
                    gantt_pid[gantt_idx] = 0; 
                    gantt_time[gantt_idx] = process_start_time_for_gantt;
                    gantt_idx++;
                } 
            }

            if (gantt_idx < MAX_TIME) {
                gantt_pid[gantt_idx] = current_process.PID;
                gantt_time[gantt_idx] = current_process.finish;
                gantt_idx++;
            } 
            last_event_end_time = current_process.finish;
            if (gantt_idx >= MAX_TIME) break; 
        }

        // Print Gantt Chart
        printf("\nGantt Chart (Priority):\n");
        int current_chart_time = 0; 
        printf("%d", current_chart_time);

        for (int k = 0; k < gantt_idx; k++) {
            if (gantt_pid[k] == 0) {
                printf(" --[IDLE]-- %d", gantt_time[k]);
            } else {
                printf(" --[P%d]-- %d", gantt_pid[k], gantt_time[k]);
            }
            current_chart_time = gantt_time[k]; 
        }
        printf("\n");
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
    int current_execution_order = 1;

    while(1) {
        if (ready[num].arrival <= time && ready[num].execution_order == 0) {
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
                ready[num].execution_order = current_execution_order;
                current_execution_order++;
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
    int current_execution_order = 1;

    while (completed < proc_count) {

        Process arrived[MAX_PROCESSES];
        int arrived_cnt = 0;

        for (int i = 0; i < proc_count; i++)
            if (time >= ready[i].arrival && ready[i].execution_order == 0)
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
                    ready[i].execution_order = current_execution_order;
                    current_execution_order++;
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
    int current_execution_order = 1;

    while (completed < proc_count) {

        Process arrived[MAX_PROCESSES];
        int arrived_cnt = 0;

        for (int i = 0; i < proc_count; i++)
            if (time >= ready[i].arrival && ready[i].execution_order == 0)
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
                    ready[i].execution_order = current_execution_order;
                    current_execution_order++;
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
    new[proc_count].execution_order = 0;
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
    gantt_time[0] = 0;
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
-> make a gantt tracker queue for every algorithm, and integrate it into the algorithm
-> make a gantt chart printer function, and get the queue from the algorithms to print the gantt chart.
2. Random IO bursts
-> add a random io burst in gantt queue by integrating it into gantt chart printer function. 
-> so gantt chart printer function should get IO burst values and gantt tracker queue values to print the gantt chart.
3. Printing summary.
-> make a summary printer function, and get the queue from the algorithms to print the summary.
*/
