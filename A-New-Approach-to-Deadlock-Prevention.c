#include <stdio.h>

int max(int array[6], int size) {
    int index = 0;
    for (int i = 0; i < size; ++i) {
        if (array[index] < array[i])
            index = i;
    }
    return index;
}

int max_resource(int *array[10], int size,int restricted_index) {
    int index = 0;
    if (restricted_index == 0)
        index = 1;
    for (int i = 0; i < size; ++i) {
        if ((*(array[index]) < *(array[i]) ) && i!=restricted_index)
            index = i;
    }
    return index;
}

int compare(int work[6], int need[6], int no_of_resources) {
    int flag = 0;
    for (int i = 0; i < no_of_resources; ++i) {
        if (work[i] < need[i]) {
            ++flag;
        }
    }
    return flag;
}

int adding_resource(int work[6],int allocation[6],int need[6],int no_of_resources)
{
    for (int i = 0; i < no_of_resources; ++i) {
        work[i] += allocation[i];
        need[i] += allocation[i];
        allocation[i] = 0;
    }
}

void extra_resources(int work[6], int need[6], int no_of_resources, int flag[6]) {
    for (int j = 0; j < no_of_resources; ++j) {
        flag[j] = 0;
    }
    for (int i = 0; i < no_of_resources; ++i) {
        if (work[i] < need[i]) {
            flag[i] = 1;
        }
    }
}


int main() {
    int allocation[10][6], maximum[10][6], need[10][6],work[6],allocated_processes[10],safe_sequence[10];
    for (int m = 0; m < 10; ++m) {
        allocated_processes[m] = 0;
        safe_sequence[m] = 0;
    }
    int no_of_processes, no_of_resources;
    int *resources[6][10];
    printf("Enter The Number Of Processes : ");
    scanf("%d", &no_of_processes);
    printf("Enter The Number Of Resources : ");
    scanf("%d", &no_of_resources);
    printf("Enter The Allocation Matrix : \n");
    for (int i = 0; i < no_of_processes; i++) {
        for (int j = 0; j < no_of_resources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter The Maximum Matrix : \n");
    for (int i = 0; i < no_of_processes; i++) {
        for (int j = 0; j < no_of_resources; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }
    printf("Enter Work Matrix(The Resources Left) : ");
    for (int l = 0; l < no_of_resources; ++l) {
        scanf("%d",&work[l]);
    }
    printf("The Need Matrix is : \n");
    for (int i = 0; i < no_of_processes; i++) {
        for (int j = 0; j < no_of_resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }
    for (int k = 0; k < no_of_resources; ++k) {
        for (int i = 0; i < no_of_processes; ++i) {
            resources[k][i] = &allocation[i][k];
//            printf("%d ",*(resources[k][i]));
        }
//        printf("\n");
    }
//    printf("%d",max_resource(resources[0],no_of_processes));
    int i = 0;
    int process_executed = 0;
    while(1)
    {
        int current_executions = 0;
        if(i==0)
        {
            for (int j = 0; j < no_of_processes; ++j) {
                if((compare(work,need[j],no_of_resources)==0)&&(allocated_processes[j]==0))
                {
                    adding_resource(work,allocation[j],need[j],no_of_resources);
                    allocated_processes[j] = 1;
                    ++current_executions;
                    safe_sequence[process_executed++] = j;
                }
            }
        }
        else
        {
            for (int j = 0; j < no_of_processes; ++j) {
                if((compare(work,need[j],no_of_resources)==i)&&(allocated_processes[j]==0))
                {
                    int extra_resource[6];
                    extra_resources(work,need[j],no_of_resources,extra_resource);
                    for (int k = 0; k < no_of_resources; ++k) {
                        if(extra_resource[k])
                        {
                            int process_no = max_resource(resources[k],no_of_processes,j);
                            printf("ProcessNo[%d] Preempted\n",process_no);
                            adding_resource(work,allocation[process_no],need[process_no],no_of_resources);
//                            printf("%d",process_no);
                            ++current_executions;
                            i = 0;
                        }
                    }
                }
                if(i==0)
                    break;
            }
            if(i!=0)
                ++i;
        }
        if(current_executions==0 && i==0)
            i=1;
        if(process_executed==no_of_processes)
            break;
    }
    for (int n = 0; n < no_of_processes; ++n) {
        printf("ProcessNo[%d]->",safe_sequence[n]);
    }
}