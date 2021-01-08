#include <bitset>
#include <ctime>
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
#define POINTNUM 30
#define ITERAION 40
#define RUN 30

#define F 0.4
#define F1 0.5
#define F2 0.8

#define Cr 0.95

using namespace std;

typedef struct point{
	double x1 = 0;
	double x2 = 0;
}aPoint;

aPoint bestpoint ;

vector<aPoint> Pointlist ;
vector<aPoint> MutationList ;
vector<aPoint> CombinationList ;

vector<double> fitness_Record_List ;

double globalbestfitness = -1;
aPoint globalbestpoint;

double randomR(){                        // 隨機產生0~1小數
	double r = rand() % 100 / (double)101 ;
	return r ;
}

double ifOutofRange( double num ){

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

	Pointlist.clear() ;
	aPoint temppoint;

	bestpoint.x1 = 999 ;
	bestpoint.x2 = 999 ;

	for (int i = 0; i < POINTNUM; i++){

		double x1 = ifOutofRange( (rand() % range) - 40 );      //隨機起始點
		double x2 = ifOutofRange( (rand() % range) - 40 );

		temppoint.x1 = x1;
		temppoint.x2 = x2;

		Pointlist.push_back(temppoint);            // 將隨機點push進去

		// cout << Pointlist[i].x1 << "," << Pointlist[i].x2 << endl;

	}

}

vector<aPoint> Mutation() {

	vector<aPoint> TempList ;

	for( int i = 0 ; i < POINTNUM ; i++ ) {

		double r = rand() % POINTNUM ;

		double R1x = Pointlist[ r ].x1 ;
		double R1y = Pointlist[ r ].x2 ;

		r = rand() % POINTNUM ;

		double R2x = Pointlist[ r ].x1 ;
		double R2y = Pointlist[ r ].x2 ;

		r = rand() % POINTNUM ;

		double R3x = Pointlist[ r ].x1 ;
		double R3y = Pointlist[ r ].x2 ;

		r = rand() % POINTNUM ;

		double R4x = Pointlist[ r ].x1 ;
		double R4y = Pointlist[ r ].x2 ;

		aPoint newpoint ;

		// newpoint.x1 = ifOutofRange( R1x + ( F * ( R2x - R3x ) ) ) ;    // random +random random
		// newpoint.x2 = ifOutofRange( R1y + ( F * ( R2y - R3y ) ) ) ;

		newpoint.x1 = ifOutofRange( bestpoint.x1 + ( F * ( R2x - R3x ) ) ) ;  // best +random random
		newpoint.x2 = ifOutofRange( bestpoint.x2 + ( F * ( R2y - R3y ) ) ) ;

		// newpoint.x1 = ifOutofRange( bestpoint.x1 + ( F1 * ( R2x - R3x ) + ( F2 * ( bestpoint.x1 - R4x ) ) ) ) ;   // best +random random +best random
		// newpoint.x2 = ifOutofRange( bestpoint.x2 + ( F1 * ( R2y - R3y ) + ( F2 * ( bestpoint.x2 - R4y ) ) ) ) ;



		// cout << newpoint.x1 << "," << newpoint.x2 << endl ;

		TempList.push_back(newpoint) ;

	}

	// cout << "=======" ;

	return TempList;

}

vector<aPoint> Recombination(){

	vector<aPoint> TempList ;

	for( int i = 0 ; i < POINTNUM ; i++ ) {

		double r1 = randomR() ;
		double r2 = randomR() ;

		aPoint newpoint ;

		if ( r1 < Cr || i == 0 ) {
			newpoint.x1 = MutationList[i].x1 ;
		}
		else {
			newpoint.x1 = Pointlist[i].x1 ;
		}

		if ( r2 < Cr || i == 1 ) {
			newpoint.x2 = MutationList[i].x2 ;
		}
		else {
			newpoint.x2 = Pointlist[i].x2 ;
		}

		// cout << newpoint.x1 << "," << newpoint.x2 << endl ;

		TempList.push_back(newpoint) ;

	}

	// cout << "=======" ;

	return TempList;

} // 123

vector<aPoint> Selection(){

	vector<aPoint> TempList ;

	for( int i = 0 ; i < POINTNUM ; i++ ) {

		aPoint newpoint ;

		if ( bestpoint.x1 == 999 || evaluation( CombinationList[i].x1, CombinationList[i].x2 ) < evaluation( bestpoint.x1, bestpoint.x2 ) ) {
			bestpoint = CombinationList[i] ;
		}

		if( evaluation( CombinationList[i].x1, CombinationList[i].x2 ) < evaluation( Pointlist[i].x1, Pointlist[i].x2 ) ) {
			newpoint.x1 = CombinationList[i].x1 ;
			newpoint.x2 = CombinationList[i].x2 ;
		}
		else {
			newpoint.x1 = Pointlist[i].x1 ;
			newpoint.x2 = Pointlist[i].x2 ;
		}

		TempList.push_back(newpoint) ;

		// cout << newpoint.x1 << " " << newpoint.x2 << endl ;

	}

	// cout << "=======" ;

	return TempList;
}

double caldistance( double x1 , double x2 ){
	double result = 0;
	result = x1 - x2;

	return result;
}

void recordFitness( double bestfitness, int iteration ) {  //紀錄每個iteration的fitness

  if (fitness_Record_List.size() < iteration + 1)  //如果還沒有該iteration的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
    fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}

int main() {

	clock_t start, stop;

	FILE *fitnessOutput;
  fitnessOutput=fopen("DEfitness.txt","w");

	srand(time(NULL));

	int runcount = 0 ;

	start = clock(); //開始時間

	while ( runcount < RUN ) {

		initial();
		int count = 0;

		// cout << globalbestpoint.x1 << "," << globalbestpoint.x2 << endl;
		// cout << globalbestfitness << endl ;  //讓小數輸出8位小數

		while( count < ITERAION ){

			MutationList = Mutation() ;
			CombinationList = Recombination() ;
			Pointlist = Selection() ;

			// cout << "iteration : " << count << "  ========================================" << endl;

			// for ( int i = 0 ; i < POINTNUM ; i++ ) {
			// 	cout << evaluation( Pointlist[i].x1, Pointlist[i].x2 ) << endl ;
			// 	// cout << Pointlist[i].x1 << " , " << Pointlist[i].x2 << endl ;
			// }

			// FILE *fp;
			// string filename = "iteration"+ std::to_string(count) + ".txt" ;
			// fp=fopen(filename.c_str() ,"w");

			// for (int i = 0;  i < Pointlist.size(); i++){

			// 	fprintf(fp, "%f\t%f\n", Pointlist[i].x1, Pointlist[i].x2 );       // O

			// 	// cout << "point " << i << ":" << endl;
			// 	// cout << setprecision(10) << Pointlist[i].x1 << "  " << Pointlist[i].x2 << "   " << evaluation( Pointlist[i].x1, Pointlist[i].x2 ) << endl;
			// }

			// fclose(fp);

      recordFitness( evaluation( bestpoint.x1, bestpoint.x2 ), count);

			// cout << setprecision(10) << globalbestpoint.x1 << "  " << globalbestpoint.x2 << endl ;

			count++;
		}


		// cout << "RUN : " << runcount+1 << " done " << endl ;

		runcount++;

	}

	for ( int i = 0; i < ITERAION ; i++ ) {
		fprintf(fitnessOutput, "%d\t%f\n", i+1 , fitness_Record_List[i]/RUN );     // O
	}

	fclose(fitnessOutput);

	stop = clock(); //結束時間

	cout << "output has been exported ! " << endl ;
	cout << "executed time : " << double(stop - start) / CLOCKS_PER_SEC << " s " << endl;

	system("pause") ;

}
