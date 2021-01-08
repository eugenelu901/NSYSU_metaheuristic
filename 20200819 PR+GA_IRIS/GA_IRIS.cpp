#include <bitset>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM 150               // bit的數量
#define SELECTION 2               // 用哪種selection選擇父代  1 = Roulette, 2 = Tournament
#define POPULATION 26            // 每個迭代的父代數量
#define ITERATION 2500            // 迭代數
#define RUN 15                    // RUN數
#define CROSSOVER_RATE 0.5        // 交配率
#define MUTATION_RATE 0.01        // 變異率
#define CATEGORYNUM 3             // 分幾類

bool UseKmeans = true  ;

//呂季澤 7/15 GA 作業

typedef struct point{
	float y1 = 0;
	float y2 = 0;
	float y3 = 0;
	float y4 = 0;
}aPoint;//asd

using namespace std;
typedef vector<aPoint> points_List ;
typedef vector<int> aSolution ;

vector<float> fitness_Record_List;  //紀錄最佳解 畫圖用
vector<int> AnswerList;

vector<aSolution> FatherList ;                   //父代陣列

points_List PointList ;

int whichSelection = SELECTION ,pSize = POPULATION, iteration = ITERATION, runtime = RUN ;
float crossOverRate = CROSSOVER_RATE, mutationRate = MUTATION_RATE ;

float totalfitness = 0 ;

void outputUI(){                                   //參數顯示
	cout << "*****************************************\n" ;
	cout << "*   Data Clustering Problem             *\n" ;
	cout << "*              with Genetic Algorithm   *\n" ;
	cout << "*****************************************\n\n" ;
	cout << "Parameters:\n" ;
	cout << "Selection      : " << whichSelection << endl ;
	cout << "PopulationSize : " << pSize << endl;
	cout << "iteration      : " << iteration << endl;
	cout << "runtime        : " << runtime << endl;
	cout << "CrossoverRate  : " << crossOverRate << endl ;
	cout << "MutationRate   : " << mutationRate << endl << endl ;
}

void inputdata() {              //讀取地圖座標資料

	FILE *fp ;
	fp = fopen( "IRIS.txt", "r") ;

	aPoint temppoint ;
	string tempAns ;
	char tempchar;
	int count = 0;

	while( !feof( fp ) ){

		fscanf(fp, "%f", &temppoint.y1) ;
		fscanf(fp, "%f", &temppoint.y2) ;
		fscanf(fp, "%f", &temppoint.y3) ;
		fscanf(fp, "%f", &temppoint.y4) ;

		fscanf(fp, "%c", &tempchar) ;
		while ( tempchar != '\n' && !feof( fp ) ) {

			if( tempchar != ' ' && tempchar != '\n' ) {
				tempAns = tempAns + tempchar;
			}

			fscanf(fp, "%c", &tempchar) ;
		}

		PointList.push_back(temppoint);

		if( tempAns == "Iris-setosa" )
			AnswerList.push_back(0);
		else if ( tempAns == "Iris-versicolor" )
			AnswerList.push_back(1);
		else if ( tempAns == "Iris-virginica" )
			AnswerList.push_back(2);

		// cout << PointList[PointList.size()-1].y1 << " " << PointList[PointList.size()-1].y2 << " " << PointList[PointList.size()-1].y3 << " " << PointList[PointList.size()-1].y4 ;
		// cout << " " << AnswerList[AnswerList.size() - 1] << endl;

		tempAns = "";

		count++;

	}

	fclose(fp);
}

float CalAcc( aSolution solution ) {

	float correctcount = 0;

	for( int i = 0 ; i < solution.size() ; i++ ) {

		if( solution[i] == AnswerList[i] ) {
			correctcount++ ;
		}

	}

	float ans = correctcount / AnswerList.size() ;

	return ans ;

}

aPoint CalTheNewHeart( aSolution solution, int target ) {

	aPoint Heart ;
	int count = 0 ;

	for( int i = 0 ; i < solution.size() ; i++ ) {
		if( solution[i] == target ) {
			count++;
			Heart.y1 = Heart.y1 + PointList[i].y1 ;
			Heart.y2 = Heart.y2 + PointList[i].y2 ;
			Heart.y3 = Heart.y3 + PointList[i].y3 ;
			Heart.y4 = Heart.y4 + PointList[i].y4 ;
		}
	}

	Heart.y1 = Heart.y1 / count ;
	Heart.y2 = Heart.y2 / count ;
	Heart.y3 = Heart.y3 / count ;
	Heart.y4 = Heart.y4 / count ;

	// cout << count << endl ;

	return Heart;

}

float CalDistanceFromTwoPoint( aPoint p1, aPoint p2 ) {

	float disY1 = p1.y1 - p2.y1 ;
	float disY2 = p1.y2 - p2.y2 ;
	float disY3 = p1.y3 - p2.y3 ;
	float disY4 = p1.y4 - p2.y4 ;
	float ans = 0.0;

	if( disY1 < 0 ) disY1 = disY1 * -1 ;               // 自己手動取個絕對值
	if( disY2 < 0 ) disY2 = disY2 * -1 ;
	if( disY3 < 0 ) disY3 = disY3 * -1 ;
	if( disY4 < 0 ) disY4 = disY4 * -1 ;


	ans = sqrt( pow( disY1, 2 ) + pow( disY2, 2 ) + pow( disY3, 2 ) + pow( disY4, 2 ) ) ;  //兩點距離公式

	return ans ;

}

aSolution FindTheClosestHeart( aPoint Heart0, aPoint Heart1, aPoint Heart2) {

	aSolution NewSolution ;

	for( int i = 0 ; i < PointList.size() ; i++ ) {

		float dis0 = CalDistanceFromTwoPoint( PointList[i], Heart0 ) ;
		// cout << dis0 << endl ;
		float dis1 = CalDistanceFromTwoPoint( PointList[i], Heart1 ) ;
		// cout << dis1 << endl ;
		float dis2 = CalDistanceFromTwoPoint( PointList[i], Heart2 ) ;
		// cout << dis2 << endl << endl << endl ;

		if( dis0 < dis1 && dis0 < dis2 ){
			NewSolution.push_back(0) ;
		}
		else if ( dis1 < dis0 && dis1 < dis2 ) {
			NewSolution.push_back(1) ;
		}
		else if ( dis2 < dis0 && dis2 < dis1 ) {
			NewSolution.push_back(2) ;
		}

	}

	return NewSolution ;

}

aSolution Kmeans( aSolution solution, aPoint &Heart0, aPoint &Heart1, aPoint &Heart2 ) {

	aPoint tempHeart ;

	Heart0 = CalTheNewHeart( solution , 0);
	// cout << Heart0.y1 << " " << Heart0.y2 << " " << Heart0.y3 << " " << Heart0.y4 << endl ;
	Heart1 = CalTheNewHeart( solution , 1);
	// cout << Heart1.y1 << " " << Heart1.y2 << " " << Heart1.y3 << " " << Heart1.y4 << endl ;
	Heart2 = CalTheNewHeart( solution , 2);
	// cout << Heart2.y1 << " " << Heart2.y2 << " " << Heart2.y3 << " " << Heart2.y4 << endl ;

	if( Heart0.y1 < Heart1.y1 && Heart0.y1 < Heart2.y1 ){
		if( Heart1.y1 > Heart2.y1 ) {
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}
	}
	else if ( Heart1.y1 < Heart0.y1 && Heart1.y1 < Heart2.y1 ) {

		tempHeart = Heart0 ;
		Heart0 = Heart1;
		Heart1 = tempHeart ;

		if( Heart1.y1 > Heart2.y1 ) {
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}

	}
	else if ( Heart2.y1 < Heart0.y1 && Heart2.y1 < Heart1.y1 ) {

		tempHeart = Heart0 ;
		Heart0 = Heart2;
		Heart2 = tempHeart ;

		if( Heart1.y1 > Heart2.y1 ) {
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}
	}

	aSolution tempSolution = FindTheClosestHeart( Heart0, Heart1, Heart2 ) ;

	aSolution betterSolution = tempSolution ;

	for( int i = 0 ; i < betterSolution.size() ; i++ ) {
		if( betterSolution[i] == 1 )
			betterSolution[i] = 2 ;
		else if ( betterSolution[i] == 2 )
			betterSolution[i] = 1 ;
	}

	if( CalAcc( tempSolution ) < CalAcc( betterSolution ) ) {
		tempSolution = betterSolution ;
	}


	return tempSolution ;

}

void Initialization() {                       //隨機賦予值

	FatherList.clear();

	aSolution tempSolution ;

	aPoint Heart0 ;
	aPoint Heart1 ;
	aPoint Heart2 ;

	for( int i = 0 ; i < pSize ; i++ ) {

		while( true ) {

			int count0 = 0 ;
			int count1 = 0 ;
			int count2 = 0 ;


			for(int j = 0 ; j < AnswerList.size() ; j++) {           // 用判斷隨機數為奇數偶數來隨機賦予值
				tempSolution.push_back( rand() % 3 ) ;

				// cout << tempSolution[tempSolution.size()-1] << " " ;

				if(tempSolution[tempSolution.size()-1] == 0)
					count0++ ;
				else if (tempSolution[tempSolution.size()-1] == 1)
					count1++ ;
				else if (tempSolution[tempSolution.size()-1] == 2)
					count2++ ;

			}

			if( count0 != 0 && count1 != 0 && count2 != 0 ){
				break ;
			}

			tempSolution.clear();

		}


		FatherList.push_back(tempSolution) ;
		tempSolution.clear();

		// cout << endl << endl ;
	}

	if ( UseKmeans ) {

		for( int j = 0 ; j < 2 ; j++ ) {

			for( int i = 0 ; i < pSize ; i++ ) {

				FatherList[i] = Kmeans(FatherList[i], Heart0, Heart1, Heart2) ;

				// for( int x = 0 ; x < FatherList[i].size() ; x++ ){
				// 	cout << FatherList[i][x] << " " ;
				// }

				// cout << "acc : " << CalAcc( FatherList[i] ) << endl << endl ;
			}

		}

	}
	// else {

	// 	for( int i = 0 ; i < pSize ; i++ ) {

	// 		Heart0 = CalTheNewHeart( FatherList[i] , 0);
	// 		Heart1 = CalTheNewHeart( FatherList[i] , 1);
	// 		Heart2 = CalTheNewHeart( FatherList[i] , 2);

	// 	}
	// }

}

float Fitness( aSolution solution ){                 //傳入一個bitset 回傳該bitset的fitness

	float c10 = 0.0, c20 = 0.0, c30 = 0.0, c40 = 0.0;
	float c11 = 0.0, c21 = 0.0, c31 = 0.0, c41 = 0.0;
	float c12 = 0.0, c22 = 0.0, c32 = 0.0, c42 = 0.0;
	int count0 = 0 ;
	int count1 = 0 ;
	int count2 = 0 ;

	for(int i = 0 ; i < solution.size() ; i++ ){

		if( solution[i] == 0 ) {
			c10 = c10 + PointList[i].y1 ;
			c20 = c20 + PointList[i].y2 ;
			c30 = c30 + PointList[i].y3 ;
			c40 = c40 + PointList[i].y4 ;
			count0++;
		}
		else if( solution[i] == 1 ) {
			c11 = c11 + PointList[i].y1 ;
			c21 = c21 + PointList[i].y2 ;
			c31 = c31 + PointList[i].y3 ;
			c41 = c41 + PointList[i].y4 ;
			count1++;
		}
		else if( solution[i] == 2 ) {

			c12 = c12 + PointList[i].y1 ;
			c22 = c22 + PointList[i].y2 ;
			c32 = c32 + PointList[i].y3 ;
			c42 = c42 + PointList[i].y4 ;
			count2++;
		}

	}

	// cout << count0 << endl ;
	// cout << count1 << endl ;
	// cout << count2 << endl ;

	c10 = c10 / count0 ;
	c20 = c20 / count0 ;
	c30 = c30 / count0 ;
	c40 = c40 / count0 ;
	c11 = c11 / count1 ;
	c21 = c21 / count1 ;
	c31 = c31 / count1 ;
	c41 = c41 / count1 ;
	c12 = c12 / count2 ;
	c22 = c22 / count2 ;
	c32 = c32 / count2 ;
	c42 = c42 / count2 ;

	float fitness = 0.0;

	for(int i = 0 ; i < solution.size() ; i++ ){

		if( solution[i] == 0 ) {
			fitness =  fitness + pow( ( c10 - PointList[i].y1 ), 2 ) ;
			fitness =  fitness + pow( ( c20 - PointList[i].y2 ), 2 ) ;
			fitness =  fitness + pow( ( c30 - PointList[i].y3 ), 2 ) ;
			fitness =  fitness + pow( ( c40 - PointList[i].y4 ), 2 ) ;
		}
		else if( solution[i] == 1 ) {
			fitness =  fitness + pow( ( c11 - PointList[i].y1 ), 2 ) ;
			fitness =  fitness + pow( ( c21 - PointList[i].y2 ), 2 ) ;
			fitness =  fitness + pow( ( c31 - PointList[i].y3 ), 2 ) ;
			fitness =  fitness + pow( ( c41 - PointList[i].y4 ), 2 ) ;
		}
		else if( solution[i] == 2 ) {
			fitness =  fitness + pow( ( c12 - PointList[i].y1 ), 2 ) ;
			fitness =  fitness + pow( ( c22 - PointList[i].y2 ), 2 ) ;
			fitness =  fitness + pow( ( c32 - PointList[i].y3 ), 2 ) ;
			fitness =  fitness + pow( ( c42 - PointList[i].y4 ), 2 ) ;
		}
	}

	if( fitness == 0 ) {
		fitness = 100000 ;
	}


	return fitness ;
}

float randomR(){                                //取得0~1隨機小數
	float r = rand() % 100 / (float)101 ;
	return r ;
}

aSolution RouletteSelection(){                   //輪盤法則

	vector<aSolution> temFatherList = FatherList ;                //先依照fitness由大到小去排序
	vector<float> probList ;                //先依照fitness由大到小去排序

	float tempfitness = 0 ;

	for( int i = 0 ; i < temFatherList.size() ; i++ ) {
		probList.push_back( (1/Fitness(FatherList[i])) / totalfitness) ;
		// cout << Fitness(temFatherList[i])/totalfitness << endl ;
	}

	float dart = randomR() ;            //隨機射飛鏢

	for( int i = 0 ; i < probList.size() ; i++ ){           //這個for迴圈是在看靶，看自己射到哪並且回傳

		tempfitness = tempfitness + probList[i]  ;

		if( dart < tempfitness){
        	// cout << "    " << Fitness(temFatherList[i]) << "  " << dart << "<" << tempfitness << endl ;
			return temFatherList[i] ;
		}
	}

}

aSolution TournamentSelection(){                     //錦標賽法則

	int choose1 = rand() % pSize ;

	while( true ) {

		if( Fitness( FatherList[choose1] ) != 0 )
			break;

		choose1 = rand() % pSize ;                  //隨機選兩個來比賽
	}

	int choose2 = rand() % pSize ;
	while( true ) {

		if( Fitness( FatherList[choose2] ) != 0 )
			break;

		choose2 = rand() % pSize ;
	}

	if( (Fitness( FatherList[choose1] ) <= Fitness( FatherList[choose2] ))  )     //看誰的fitness大就回傳誰
		return FatherList[choose1] ;
	else if( Fitness( FatherList[choose1] ) > Fitness( FatherList[choose2] ) )
		return FatherList[choose2] ;

}

void Selection( aSolution &Pa, aSolution &Pb ) {                      //呼叫各種法則的function 會先進到這裡在依 whichSelection 去做選擇

	if( whichSelection == 1 ){
		Pa = RouletteSelection() ;     // 輪盤法則，各射一次飛鏢
		Pb = RouletteSelection() ;
	}
	if( whichSelection == 2 ) {
		Pa = TournamentSelection() ;   // 錦標賽法則, 競賽兩次選出兩個子代
		Pb = TournamentSelection() ;
	}

}

void Crossover( aSolution &Ca, aSolution &Cb, aSolution Pa, aSolution Pb ){      //交配function <3


	if( randomR() <= crossOverRate ){         //要交配

		for( int i = 0 ; i < 150 ; i++ ){

			if( randomR() <= 0.5 ){               //在切割點之前
				Ca.push_back(Pa[i]) ;
				Cb.push_back(Pb[i]) ;
			}
			else{                             //在切割點之後
				Ca.push_back(Pb[i]) ;
				Cb.push_back(Pa[i]) ;
			}

		}

	}
	else{                       // 不交配
		Ca = Pa ;
		Cb = Pb ;
	}

}

aSolution Mutation( aSolution solution ){          //變異function

	int r ;

	if( Fitness(solution) != 0 ) {
		for( int i = 0 ; i < solution.size() ; i++ ){
			if( randomR() <= mutationRate ){         //要變異
				while( true ) {
					r = rand() % 3 ;
					if(r != solution[i])
						break;
				}

				solution[i] = r ;

			}
		}
	}



	// for( int x = 0 ; x < solution.size() ; x++ ){
	// 	cout << solution[x] << " " ;
	// }

	// cout << endl ;

	return solution ;          //不變異就回傳原本的回去

}

void recordFitness( float bestfitness, int iteration ) {  //紀錄每個iteration的fitness

  if (fitness_Record_List.size() < iteration + 1)  //如果還沒有該iteration的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
    fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}

void ChangeSolution( aSolution &solution, int target1, int target2 ) {

	for( int i = 0 ; i < solution.size() ; i++ ) {
		if( solution[i] == target1 )
			solution[i] = target2 ;
		else if( solution[i] == target2 )
			solution[i] = target1 ;
	}

}

void SortFlower( aSolution &solution ) {

	aPoint tempHeart ;

	aPoint Heart0 = CalTheNewHeart( solution , 0);
	aPoint Heart1 = CalTheNewHeart( solution , 1);
	aPoint Heart2 = CalTheNewHeart( solution , 2);

	if( Heart0.y1 < Heart1.y1 && Heart0.y1 < Heart2.y1 ){
		if( Heart1.y1 > Heart2.y1 ) {
			ChangeSolution( solution, 1, 2 ) ;
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}
	}
	else if ( Heart1.y1 < Heart0.y1 && Heart1.y1 < Heart2.y1 ) {
		ChangeSolution( solution, 1, 0 ) ;
		tempHeart = Heart0 ;
		Heart0 = Heart1;
		Heart1 = tempHeart ;
		if( Heart1.y1 > Heart2.y1 ) {
			ChangeSolution( solution, 2, 0 ) ;
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}
	}
	else if ( Heart2.y1 < Heart0.y1 && Heart2.y1 < Heart1.y1 ) {
		ChangeSolution( solution, 2, 0 ) ;
		tempHeart = Heart0 ;
		Heart0 = Heart2;
		Heart2 = tempHeart ;
		if( Heart1.y1 > Heart2.y1 ) {
			ChangeSolution( solution, 1, 2 ) ;
			tempHeart = Heart1 ;
			Heart1 = Heart2 ;
			Heart1 = tempHeart;
		}
	}
}

int main() {

	srand(time(NULL));
	inputdata() ;

	FILE *f1;

	if( whichSelection == 1 ) {
		f1 = fopen("GAoutput_Roulette.txt", "w");
	}
	else if ( whichSelection == 2 ) {
		f1 = fopen("GAoutput_Tournament.txt", "w");
	}

	outputUI() ;                                            //使用者參數顯示
	system("pause") ;

	int countRun = 0 ;

	while ( countRun < runtime ) {

		Initialization();

		aSolution bestsolution ;

		int count = 0 ;

		while( count < iteration ){

			for(int i = 0 ; i < pSize ; i++ ){                            // E

				if( Fitness(bestsolution) == 0 || Fitness(FatherList[i]) < Fitness(bestsolution) ) {
					if( Fitness(FatherList[i]) != 0 ) {
						bestsolution = FatherList[i] ;
					}

				}

			}

			totalfitness = 0 ;
			for( int i = 0 ; i < FatherList.size() ; i++ ) {
				totalfitness = totalfitness + (1/Fitness(FatherList[i])) ;
			}

			vector<aSolution> Qlist ;

			aSolution Pa, Pb ;
			aSolution Ca, Cb ;

			for(int i = 0 ; i < ( pSize / 2 ) ; i++ ){

				Selection( Pa, Pb ) ;                  // 選出父代
				Crossover( Ca, Cb, Pa, Pb ) ;          // 交配
				Qlist.push_back( Mutation( Ca ) );      // 把變異過後的子代推進陣列
				Qlist.push_back( Mutation( Cb ) );


				Pa.clear();
				Pb.clear();
				Ca.clear();
				Cb.clear();

			}

			FatherList = Qlist ;                            //把該輪子代變成父代
			Qlist.clear() ;                            //清空子代以方便下輪使用

			SortFlower( bestsolution ) ;
			recordFitness( CalAcc(bestsolution), count);  //紀錄每iteration的fitness

			count++ ;

			cout << "iteration:" << count <<  "     fitness:" << Fitness(bestsolution) << " acc : " << CalAcc(bestsolution) << endl  ;       // debug 用


			for( int x = 0 ; x < bestsolution.size() ; x++ ){
				cout << bestsolution[x] << " " ;
			}

			cout << endl ;


		}

		FatherList.clear();  // 將父代陣列清空 以進行下一次run
		countRun++;

		cout << "run : " << countRun << " done " << endl ;

	}

	for (int i = 0; i < 1000; i++)
		fprintf(f1, "%d\t%f\n", (i*pSize + 1), (fitness_Record_List[i] / runtime));       // O

	fclose(f1);

	cout << "done! output exported!" << endl;

	system("pause") ;

}
