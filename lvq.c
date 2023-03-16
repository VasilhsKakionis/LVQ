//Dafni Skiadopoulou AM: 3075, Vasileios Kakionis AM: 2981, Nafsika Tzatzaki AM: 3087

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 7
#define N 100
#define err 0.00001

float X[5*N][2]; //data
float W[M][2]; //centers
float Wnew[M][2];
int O[M][5*N];   //clusters (indexes to X)
int indexO[M]; //for every cluster holds the num of data in cluster
float h = 0.3; //learning rate

void plotRes(){

	FILE *dataFile;
	dataFile = fopen("plotFile","w");
	int i,j;
	i = 0;
	while(i<N){
		fprintf(dataFile,"%f %f %f %f %f %f %f %f %f %f\n",X[i][0],X[i][1],X[i+100][0],X[i+100][1],X[i+200][0],X[i+200][1],X[i+300][0],X[i+300][1],X[i+400][0],X[i+400][1]);
		i++;
	}
	fclose(dataFile);


}

float distance(float x1,float x2,float w1,float w2){

	float m = pow((x1-w1),2) + pow((x2-w2),2);

	float d = pow(m,0.5);

	return d;

}

int checkIfEnd(){

	float e = 0.0;
	int i;
	for(i=0;i<M;i++){
		float x = pow((Wnew[i][0] - W[i][0]),2) + pow((Wnew[i][1] - W[i][1]),2);
		e = pow(x,0.5);
		if( e > err ){
			return 0;
		}
	}
	return 1;

}

float diaspora(){

	float d = 0.0;

	int i,j;
	for(i=0;i<M;i++){
		for(j=0;j<indexO[i];j++){
			d = d + pow(distance(X[O[i][j]][0],X[O[i][j]][1],Wnew[i][0],Wnew[i][1]),2);
		}
	}

	return d;

}


void main(){

   FILE *fp;
   FILE *fout;

   int t = 0;//epoxh
   int T = 0;//epanalhpseis
   int e = 0;

   fp = fopen("dataSet2.txt","r");

   srand(time(NULL));

   int i;

   for(i=0;i<M;i++){
	indexO[i] = 0;
   }

   float x1;
   float x2;

   for(i=0;i<5*N;i++){
   	float x1;
  	float x2;
	fscanf(fp,"%f",&x1);
	fscanf(fp,"%f",&x2);
	X[i][0] = x1;
	X[i][1] = x2;
   }

   int randomC[M];
   for(i=0;i<M;i++){
	randomC[i] == -1.0;
   }
   for(i=0;i<M;i++){
	int flag = 1;
	int r;
	while(flag){
		r = rand();
		int k;
		int s = 0;
		for(k=0;k<M;k++){
			if(r != randomC[k]){
				s++;
			}
		}
		if (s == M && r < 499){
			flag = 0;
		}
	}
	randomC[i] = r;	
	Wnew[i][0] = X[r][0];
	Wnew[i][1] = X[r][1];
   }

   do{
	for(i=0;i<M;i++){
		W[i][0] = Wnew[i][0];
		W[i][1] = Wnew[i][1];
	}
	int n;
	for(n=0;n<5*N;n++){

		int m = 0;
		int min = distance(X[n][0],X[n][1],Wnew[0][0],Wnew[0][1]);
		for(i=1;i<M;i++){
			if ( min > distance(X[n][0],X[n][1],Wnew[i][0],Wnew[i][1]) ){
				min = distance(X[n][0],X[n][1],Wnew[i][0],Wnew[i][1]);
				m = i;
			}
		}
		Wnew[m][0] = Wnew[m][0] + h*(X[n][0]-Wnew[m][0]);
		Wnew[m][1] = Wnew[m][1] + h*(X[n][1]-Wnew[m][1]);

		T++;
	}
	h = 0.9 * h;
	e = checkIfEnd();
	t++;
   }while(e == 0 && t < 1000);

   for(i=0;i<5*N;i++){
	float min;
	int c = 0;
	min = distance(X[i][0],X[i][1],Wnew[0][0],Wnew[0][1]);
	int j;
	for(j=1;j<M;j++){
		float d = distance(X[i][0],X[i][1],Wnew[j][0],Wnew[j][1]);
		if (d < min){
			min = d;
			c = j;
		}
	}

	O[c][indexO[c]] = i;
	indexO[c]++;
   }

   printf("t = %d\n",t);
   printO();



   fout = fopen("prototypesLVQ.txt","w");
   for(i=0;i<M;i++){
	fprintf(fout,"%f %f\n",Wnew[i][0],Wnew[i][1]);
   }

   float syn_d = diaspora();
   printf("Synolikh diaspora : %f\n",syn_d);
	
   fclose(fp);
   fclose(fout);

   plotRes();

   return;

}

void printO(){

	int i,j,start,steps;
	for(i=0;i<M;i++){
		printf("~~~~~~~~~~~\n");
		steps = 0;
		start = O[i][0];
		for(j=1;j<indexO[i];j++){
			if (O[i][j] == start+steps+1){
				steps++;
			}
			else{
				if(steps==0){
					printf(" [%d] \n",start);
				}
				else{
					printf(" [%d-%d] \n",start,start+steps);
				}
				start = O[i][j];
				steps = 0;
			}
		}
		if(steps==0){
				printf(" [%d] \n",start);
			}
		else{
			printf(" [%d-%d] \n",start,start+steps);
		}
		printf("\n");
	}
	return;
}

void printW(){
	int i;
	printf("~~~~~~~~~~W~~~~~~~~~~\n");
	for(i=0;i<M;i++){
		printf("%f  %f\n",W[i][0],W[i][1]);
	}
	printf("~~~~~~~~~Wnew~~~~~~~~\n");
	for(i=0;i<M;i++){
		printf("%f  %f\n",Wnew[i][0],Wnew[i][1]);
	}
	return;
}


//plot "plotFile" u 1:2, "plotFile" u 3:4, "plotFile" u 5:6,"plotFile" u  7:8, "plotFile" u 9:10, "prototypesLVQ.txt" u 1:2

