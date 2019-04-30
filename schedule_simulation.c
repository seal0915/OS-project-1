#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define maxn 32

int main(){
    int my_clock = 0;
    char policy[maxn]; scanf("%s",policy);
    int n; scanf("%d",&n);

    char name[n][maxn];
    int ready[n];
    int trans[n];
    int remain[n];
    int finish[n];
    for(int i=0; i<n; i++){
        scanf("%s%d%d",name[i],&ready[i],&trans[i]);
        finish[i] = 0;
        remain[i] = trans[i];
    }

    int count = n;
    if(strcmp(policy,"FIFO") == 0){
        while(count){
            int exe_ID;
            for(int i=0; i<n; i++){
                if(finish[i] == 0){
                    if(my_clock >= ready[i]){
                        exe_ID = i;
                        break;
                    }
                }
            }
            finish[exe_ID] = 1;
            my_clock += trans[exe_ID];
            printf("Name: %32s | ready: %10d | trans %10d | finish time %10d |\n",name[exe_ID], ready[exe_ID], trans[exe_ID], my_clock);
            count--;
        }
    }
    else if(strcmp(policy,"RR") == 0){
         while(count){
            int flag = 0;
            for(int i=0; i<n; i++){
                if(finish[i] == 0){
                    if(my_clock >= ready[i]){
                        flag = 1;
                        remain[i]-= 500;
                        if(remain[i]<= 0){
                            my_clock = my_clock + remain[i] + 500;
                            finish[i] = 1;
                            remain[i] = 0;
                            printf("Name: %32s | ready: %10d | trans %10d | finish time %10d |\n",name[i], ready[i], trans[i], my_clock);
                            count--;
                        }
                        else{
                            my_clock += 500; 
                        }
                    }
                }
            }
            if(flag == 0){
                my_clock++;
            }
        }
    }
    else if(strcmp(policy,"SJF") == 0){
        while(count){
            int min = 2147483647;
            int exe_ID;
            for(int i=0; i<n; i++){
                if(finish[i] == 0){
                    if(my_clock >= ready[i]){
                        if(trans[i] < min){
                            min = trans[i];
                            exe_ID = i;
                        }
                    }
                }
            }
            finish[exe_ID] = 1;
            my_clock += trans[exe_ID];
            printf("Name: %32s | ready: %10d | trans %10d | finish time %10d |\n",name[exe_ID], ready[exe_ID], trans[exe_ID], my_clock);
            count--;
        }
    }
    else if(strcmp(policy,"PSJF") == 0){
        while(count){
            int min = 2147483647;
            int exe_ID;
            for(int i=0; i<n; i++){
                if(finish[i] == 0){
                    if(my_clock >= ready[i]){
                        if(remain[i] < min){
                            min = remain[i];
                            exe_ID = i;
                        }
                    }
                }
            }
            remain[exe_ID]--;
            my_clock += 1; 
            if(remain[exe_ID] == 0){
                finish[exe_ID] = 1;
                printf("Name: %32s | ready: %10d | trans %10d | finish time %10d |\n",name[exe_ID], ready[exe_ID], trans[exe_ID], my_clock);
                count--;
            }  
        }
    }
    return 0;
}