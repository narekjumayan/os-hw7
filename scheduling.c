#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

int compareByArrivalTime(const void *a, const void *b) {
    const Process *p1 = (const Process *) a;
    const Process *p2 = (const Process *) b;
    return p1->arrival_time - p2->arrival_time;
}

void fcfs(Process *processes, int size) {
    qsort(processes, size, sizeof(Process), compareByArrivalTime);
    int current_time = 0;
    for (int i = 0; i < size; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        current_time += processes[i].burst_time;

        processes[i].turnaround_time =
                current_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        processes[i].response_time = processes[i].waiting_time;
    }
}

int compareByBurstTime(const void *a, const void *b) {
    const Process *p1 = a;
    const Process *p2 = b;

    if (p1->burst_time < p2->burst_time) return -1;
    if (p1->burst_time > p2->burst_time) return 1;
    return compareByArrivalTime(a, b);
}

void sjf(Process *processes, int size) {
    int current_time = 0;

    for (int i = 0; i < size; i++) {
        int shortest = -1;

        for (int j = i; j < size; j++) {
            if (processes[j].arrival_time <= current_time) {
                if (shortest == -1 ||
                    compareByBurstTime(&processes[j], &processes[shortest]) < 0) {
                    shortest = j;
                    }
            }
        }

        if (shortest == -1) {
            for (int j = i; j < size; j++) {
                if (shortest == -1 ||
                    processes[j].arrival_time < processes[shortest].arrival_time) {
                    shortest = j;
                    }
            }

            current_time = processes[shortest].arrival_time;
        }

        Process temp = processes[i];
        processes[i] = processes[shortest];
        processes[shortest] = temp;

        int start_time = current_time;
        current_time += processes[i].burst_time;

        processes[i].turnaround_time =
                current_time - processes[i].arrival_time;
        processes[i].waiting_time =
                processes[i].turnaround_time - processes[i].burst_time;
        processes[i].response_time =
                start_time - processes[i].arrival_time;
    }
}
void displayResults(Process *processes, int size) {
    double total_waiting = 0;
    double total_turnaround = 0;
    double total_response = 0;

    int current_time = 0;

    printf("\nGantt Chart:\n");

    for (int i = 0; i < size; i++) {
        if (current_time < processes[i].arrival_time) {
            printf("| IDLE ");
            current_time = processes[i].arrival_time;
        }

        printf("| P%d ", processes[i].pid);
        current_time += processes[i].burst_time;
    }

    printf("|\n\n");

    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");

    for (int i = 0; i < size; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);

        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        total_response += processes[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / size);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / size);
    printf("Average Response Time: %.2f\n", total_response / size);
}

int main() {
    int n;

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 100) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    Process processes[100];
    Process fcfs_processes[100];
    Process sjf_processes[100];

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("Enter the arrival time and burst time for process %d: ", i + 1);

        if (scanf("%d %d", &processes[i].arrival_time,
                  &processes[i].burst_time) != 2 ||
            processes[i].arrival_time < 0 ||
            processes[i].burst_time <= 0) {
            printf("Invalid process input.\n");
            return 1;
            }

        fcfs_processes[i] = processes[i];
        sjf_processes[i] = processes[i];
    }

    printf("\nFirst Come First Served (FCFS)\n");
    fcfs(fcfs_processes, n);
    displayResults(fcfs_processes, n);

    printf("\nShortest Job First (SJF)\n");
    sjf(sjf_processes, n);
    displayResults(sjf_processes, n);
    return 0;
}


