#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#define False 0
#define True 1
 
//Data structure
char NAME[100]={0};//Resource Name 
int Max[100][100]={0};//Maximum claim matrix
int Allocation[100][100]={0};//Allocation matrix
int Need[100][100]={0};//Need matrix
int Available[100]={0};//Available matrix
int Request[100]={0};//Request matrix 
int Work[100]={0};//Work matrix: the resource can get 
int Finish[100]={0}; //Finish matrix: system have enough resource to allocate to each process
int Security[100]={0};//Safety sequence
 
int M=100;//max process 5
int N=100;//max resource 3
 

void init()
{
    int i,j,m,n;
	int number,flag;
	int temp[100]={0};
	printf("\nEnter number of resources: ");
	scanf("%d",&n);
	N=n;
	for(i=0;i<n;i++)
	{
		NAME[i]=i;
		printf("\nEnter The initial number of resource %d :",i);	
		scanf("%d",&number);
		Available[i]=number;
	}
	
    printf("\nEnter number of processes: \t");
	scanf("%d",&m);
	M=m;
    printf("\nEnter The Maximum Claim Table:\n");
	do{
		flag = False;
		for(i=0;i<M;i++)
			for(j=0;j<N;j++){
				scanf("%d",&Max[i][j]);
				if(Max[i][j]>Available[j])
					flag = True;				
			}
		if(flag)
			printf("It's out of size, Please retype Maximum Claim Table!\n");								
	} while(flag);
	
	do{
		flag=False;
      	printf("\nEnter Allocated Resource Table:\n");
		for(i=0;i<M;i++)
		{
			for(j=0;j<N;j++)
		  	{
				scanf("%d",&Allocation[i][j]);
				if(Allocation[i][j]>Max[i][j])  
					flag=True;				
				Need[i][j]=Max[i][j]-Allocation[i][j];
				temp[j]+=Allocation[i][j];
		  	}
		}
		if(flag)
			printf("It's out of size, Please retype Allocated Resource Table!\n");								
	}while(flag);
	
	for(j=0;j<N;j++)
		Available[j]=Available[j]-temp[j];
}
 
/********顯示資源配置矩陣********/
void showdata()
{
	int i,j;
	printf("-------------------------------------------------------------\n");
    printf("\nAvailable Resources:");
	for(i=0;i<N;i++)
        printf("%c  ",NAME[i]);
	printf("\n");
	for(j=0;j<N;j++)
        printf("%d  ",Available[j]);
	printf("\n");
	printf("The current resource configuration of the system is as follows:\n");
	printf("            Max   	 Allocation    Need\n");
	printf("NAME     ");

	for(j=0;j<3;j++){
		for(i=0;i<N;i++)
			printf("%c  ",NAME[i]);
		printf("     ");
	}
	printf("\n");

	for(i=0;i<M;i++){
		printf(" P%d        ",i);
	    for(j=0;j<N;j++)
			printf("%d  ",Max[i][j]);
		printf("     "); 
		for(j=0;j<N;j++)
			printf("%d  ",Allocation[i][j]);
		printf("     "); 
		for(j=0;j<N;j++)
			printf("%d  ",Need[i][j]);
		printf("\n");
	}
}
 
int test(int i) 
{ 
	for(int j=0;j<N;j++)
	{
		Available[j]=Available[j]-Request[j];
		Allocation[i][j]=Allocation[i][j]+Request[j];
		Need[i][j]=Need[i][j]-Request[j];
	}
	return True;
}
 
int Retest(int i) //與test操作相反 
{ 
	for(int j=0; j<N; j++)
	{
		Available[j] = Available[j] + Request[j];
		Allocation[i][j] = Allocation[i][j] - Request[j];
		Need[i][j] = Need[i][j] + Request[j];
	}
	return True;
}
 
int safe()
{
    // initial
	int i,j,k=0,m,apply;
	for(j=0;j<N;j++)
        Work[j] = Available[j];
    for(i=0;i<M;i++) 
    	Finish[i] = False;
    //Safety sequence
	for(i=0;i<M;i++){ 
        // sleep(1); //=================================================

		apply=0;
		for(j=0;j<N;j++){
			if(Finish[i]==False && Need[i][j]<=Work[j])
			{   
				apply++;
				if(apply==N)
				{  
					for(m=0;m<N;m++)
				        Work[m]=Work[m]+Allocation[i][m];
					Finish[i]=True;
					Security[k++]=i;
					i=-1; 		
				}
			}
		}
	}
	
	for(i=0;i<M;i++){
		if(Finish[i]==False){
			printf("System not safe!\n");
			return False;
		}
	}
    printf("System is safe!\n");
    printf("there is a safety sequence:");
	for(i=0;i<M;i++){
		printf("P%d",Security[i]);
		if(i<M-1) 
			printf("->");
	}
	printf("\n");
	return True;
}
 
/********利用銀行家演算法對申請資源進行試分********/

void bank()
{
	int flag = True;
	int i,j;
 
	printf("Input which process need to be allocate(0-%d):",M-1); 
    scanf("%d",&i);
    
	printf("Input number of resource that process %d need:\n",i);
	for(j=0;j<N;j++)
	{
		printf("%c:",NAME[j]);
		scanf("%d",&Request[j]);
	}
	
    for (j=0;j<N;j++)
	{
		if(Request[j]>Need[i][j])//Check allocate is lower than need 
		{ 
			printf("Allocate not allow, it's oversize than need！\n");
			flag = False;
			break;
		}else{

            if(Request[j]>Available[j])//Check allocate is lower than system can use 
			{                         
				printf("Allocate not allow, it's oversize than system can use!\n");
				flag = False;
				break;
			}
		}
    }
    if(flag) {
		test(i); 
		showdata(); 
		if(!safe()) //find safety sequence
		{
			Retest(i);
			showdata();
		}
    }
}
 
 
int main()
{	
    clock_t t1, t2;
	char choice;

	init(); // initial all the parameter, and calculate the Need matrix
    t1 = clock();
    showdata(); // print out the input

	if(!safe()) exit(0); //Safaty algorithm
    t2 = clock();
    printf("Times: %lf\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
	do{
		printf("\t-------------------Banker's algorithm------------------\n");
		printf("R(r):request to allocate resource\n");	
		printf("E(e):Exit       \n");
		printf("plz chooses:");
		rewind(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
			case 'r':
			case 'R':
				bank();break;			
			case 'e':
			case 'E':
				exit(0);
			default: printf("Error!\n");break;
		}
	} while(choice);
}
 

