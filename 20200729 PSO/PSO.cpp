#include <bitset>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define PI acos(-1)

#define range 81
#define POINTNUM 10
#define ITERAION 250
#define RUN 30

#define C1 1
#define C2 1
#define w 0.5

using namespace std;

typedef struct point{
	double x1 = 0;
	double x2 = 0;
}aPoint;

typedef struct velocities{
	double Vx1 = 0;
	double Vx2 = 0;
}aVelocity;

vector<aPoint> pointlist ;
vector<aPoint> localbestlist ;
vector<double> localbestfitnesslist ;
vector<double> fitness_Record_List ;

vector<aVelocity> velocitylist ;

double globalbestfitness = -1;
aPoint globalbestpoint;

double randomR(){                        // 隨機產生0~1小數
	double r = rand() % 100 / (double)101 ;
	return r ;
}

int ifOutofRange( double num ){

	if(num > 40)
		return 40;
	else if(num < -40)
		return -40;

	return num;
}

double evaluation( double x1, double x2 ){
	double a = 20, b = 0.2, c = 2 * PI, d = 2;
	double sigma1 = pow(x1, 2) + pow(x2, 2);
	double sigma2 = cos(c * x1) + cos(c * x2);

	double fitness = -a * exp(-b * sqrt((1 / d) * sigma1)) - exp((1 / d) * sigma2) + a + exp(1);

	return fitness;
}

void initial(){

	pointlist.clear() ;
	localbestlist.clear();
	localbestfitnesslist.clear();

	aPoint temppoint;
	aVelocity tempvelocity;

	for (int i = 0; i < POINTNUM; i++){

		double x1 = ifOutofRange( (rand() % range) - 40 );      //隨機起始點
		double x2 = ifOutofRange( (rand() % range) - 40 ) ;

		temppoint.x1 = x1;
		temppoint.x2 = x2;

		pointlist.push_back(temppoint);            // 將隨機點push進去
		localbestlist.push_back(temppoint);        // 一開始的隨機點就是最好的點
		localbestfitnesslist.push_back(evaluation(temppoint.x1, temppoint.x2));

		// cout << pointlist[i].x1 << "," << pointlist[i].x2 << "," << localbestfitnesslist[i] << endl;
		// cout << localbestlist[i].x1 << "," << localbestlist[i].x2 << endl;


		double Vx1 = ifOutofRange( (rand() % range) - 40 ) ;    //隨機慣性的向量
		double Vx2 = ifOutofRange( (rand() % range) - 40 ) ;

		tempvelocity.Vx1 = Vx1;
		tempvelocity.Vx2 = Vx2;

		velocitylist.push_back(tempvelocity);


		// cout << velocitylist[i].Vx1 << "," << velocitylist[i].Vx2 << endl;


	}

}

aPoint findTheBest(){
	aPoint bestpoint;
	double bestfitness = -1 ;
	double fitness = -1 ;

	for (int i = 0; i < POINTNUM; i++){
		fitness = evaluation(pointlist[i].x1, pointlist[i].x2);

		if( fitness < localbestfitnesslist[i] ){
			localbestlist[i] = pointlist[i];
			localbestfitnesslist[i] = fitness;
		}

		if( bestfitness == -1 || fitness < bestfitness ){
			bestfitness = fitness;
			bestpoint = pointlist[i];
		}
	}

	if(globalbestfitness == -1 || bestfitness < globalbestfitness){
		globalbestfitness = bestfitness;
		return bestpoint;
	}

	return globalbestpoint;
}

double caldistance( double x1 , double x2 ){
	double result = 0;
	result = x1 - x2;

	return result;
}

double calNewVx1( int i ) {
	double x1 = 0;
	x1 = (w * caldistance(velocitylist[i].Vx1, pointlist[i].x1)) + (randomR() * C1 * caldistance(localbestlist[i].x1, pointlist[i].x1)) + (randomR() * C2 * caldistance(globalbestpoint.x1, pointlist[i].x1));

	return x1;
}

double calNewVx2( int i ) {
	double x2 = 0;
	x2 = (w * caldistance(velocitylist[i].Vx2, pointlist[i].x2)) + (randomR() * C1 * caldistance(localbestlist[i].x2, pointlist[i].x2)) + (randomR() * C2 * caldistance(globalbestpoint.x2, pointlist[i].x2));

	return x2;
}

void updateVelocities() {
	aVelocity newvelocity ;

	for (int i = 0; i < POINTNUM; i++){
		newvelocity.Vx1 = calNewVx1(i);
		newvelocity.Vx2 = calNewVx2(i);
		velocitylist[i] = newvelocity;
		// cout << velocitylist[i].Vx1 << "  " << velocitylist[i].Vx2 << endl;

		pointlist[i].x1 = pointlist[i].x1 + velocitylist[i].Vx1 ;
		pointlist[i].x2 = pointlist[i].x2 + velocitylist[i].Vx2 ;

		if(pointlist[i].x1 > 40)
			pointlist[i].x1 = 40;
		else if(pointlist[i].x1 < -40)
			pointlist[i].x1 = -40;

		if(pointlist[i].x2 > 40)
			pointlist[i].x2 = 40;
		else if(pointlist[i].x2 < -40)
			pointlist[i].x2 = -40;

		// cout << pointlist[i].x1 << "," << pointlist[i].x2 << "," << localbestfitnesslist[i] << endl;

	}
}

void recordFitness( double bestfitness, int iteration ) {  //紀錄每個iteration的fitness

  if (fitness_Record_List.size() < iteration + 1)  //如果還沒有該iteration的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
    fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}

int main() {


	FILE *fitnessOutput;
  	fitnessOutput=fopen("PSOfitness.txt","w");
	srand(time(NULL));

	int runcount = 0 ;

	while ( runcount < RUN ) {

		initial();
		int count = 0;

		// cout << globalbestpoint.x1 << "," << globalbestpoint.x2 << endl;
		// cout << globalbestfitness << endl ;  //讓小數輸出8位小數

		while( count < ITERAION ){

			updateVelocities();
			globalbestpoint = findTheBest() ;

			// cout << "iteration : " << count << "  ========================================" << endl;

			// FILE *fp;
			// string filename = "iteration"+ std::to_string(count) + ".txt" ;
			// fp=fopen(filename.c_str() ,"w");

			// for (int i = 0;  i < pointlist.size(); i++){

			// 	fprintf(fp, "%f\t%f\n", pointlist[i].x1, pointlist[i].x2 );       // O

			// 	// cout << "point " << i << ":" << endl;
			// 	// cout << setprecision(10) << pointlist[i].x1 << "  " << pointlist[i].x2 << "   " << evaluation( pointlist[i].x1, pointlist[i].x2 ) << endl;
			// }

			// fclose(fp);

      recordFitness( evaluation( globalbestpoint.x1, globalbestpoint.x2 ), count);

			// cout << setprecision(10) << globalbestpoint.x1 << "  " << globalbestpoint.x2 << "  " << evaluation( globalbestpoint.x1, globalbestpoint.x2 ) << endl ;
			count++;
		}


		cout << "RUN : " << runcount+1 << " done " << endl ;

		runcount++;

	}

	for ( int i = 0; i < ITERAION ; i++ ) {
		fprintf(fitnessOutput, "%d\t%f\n", i , fitness_Record_List[i]/RUN );       // O
	}

	fclose(fitnessOutput);

	cout << "success!";
	system("pause") ;

}
