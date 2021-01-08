#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM        100    // bit的數量
#define SELECTION      2      // 用哪種selection選擇父代  1 = Roulette, 2 = Tournament
#define POPULATION     20     // 每個迭代的父代數量
#define ITERATION      1000   // 迭代數
#define RUN            30     // RUN數
#define CROSSOVER_RATE 0.8    // 交配率
#define MUTATION_RATE  0.1    // 變異率
#define H              4      // 區域數
#define N              4      // searcher數
#define W              4      // 每個區域的貨物數

using namespace std;
typedef bitset<BITSNUM> bits_set ;
typedef vector<bits_set> bits_List ;

vector<bits_set> searcherList ;
vector<bits_List> goodsList ;
vector<bits_List> sampleList ;

vector<int> TaList;
vector<int> TbList;

vector<float> fitness_Record_List;  //紀錄最佳解 畫圖用

void outputUI(){                                  //參數顯示
	cout << "*****************************************\n" ;
	cout << "*   One-Max Problem                     *\n" ;
	cout << "*              with Search Economic     *\n" ;
	cout << "*****************************************\n\n" ;
	cout << "Parameters:\n" ;
	cout << "Selection      : " << SELECTION << endl ;
	cout << "PopulationSize : " << POPULATION << endl;
	cout << "ITERATION      : " << ITERATION << endl;
	cout << "RUN        : " << RUN << endl;
	cout << "CROSSOVER_RATE  : " << CROSSOVER_RATE << endl ;
	cout << "MUTATION_RATE   : " << MUTATION_RATE << endl << endl ;
} // 123

void Initialization() {                       //隨機賦予值

	TaList.clear();
	TbList.clear();

	for(int i = 0 ; i < H ; i++) {
		for( int j = 0 ; j < W ; j++ ) {
			TaList.push_back(1) ;
			TbList.push_back(1) ;
		}
	}

}

bits_set GetARamdomSolution() {                       //隨機賦予值

	bits_set iniset ;

	for(int i = 0 ; i < BITSNUM ; i++)            // 用判斷隨機數為奇數偶數來隨機賦予值
		iniset[i] = rand()%2 ;

	return iniset;
}

void ResourceArrangment() {                       //隨機賦予值

	bits_List tempList ;

	for( int i = 0 ; i < H ; i++ ) {

		for( int j = 1 ; j < W ; j++ ) {
			tempList.push_back( GetARamdomSolution() ) ;
		}

		goodsList.push_back(tempList);
		tempList.clear();

	}

	for( int i = 0 ; i < N ; i++ )
		searcherList.push_back( goodsList[i][ rand() % W ] ) ;

}

int Fitness( bits_set solution ){                 //傳入一個bitset 回傳該bitset的fitness
	int fitness = 0 ;

	for(int i = 0 ; i < BITSNUM ; i++ ){
		if( solution[i] == 1 )
			fitness++ ;
	}

	return fitness ;
}

float randomR(){                                //取得0~1隨機小數
	float r = rand() % 100 / (float)101 ;
	return r ;
}

bits_set TournamentSelection(){                     //錦標賽法則

	int choose1 = rand() % POPULATION ;                  //隨機選兩個來比賽
	int choose2 = rand() % POPULATION ;

	if( Fitness( pList[choose1] ) >= Fitness( pList[choose2] ) )     //看誰的fitness大就回傳誰
		return pList[choose1] ;
	else if( Fitness( pList[choose1] ) < Fitness( pList[choose2] ) )
		return pList[choose2] ;

}

void Selection( bits_set &Pa, bits_set &Pb ) {                      //呼叫各種法則的function 會先進到這裡在依 SELECTION 去做選擇

	Pa = TournamentSelection() ;   // 錦標賽法則, 競賽兩次選出兩個子代
	Pb = TournamentSelection() ;

}

void Crossover( bits_set &Ca, bits_set &Cb, bits_set Pa, bits_set Pb ){      //交配function <3

	int cutpoint = rand() % BITSNUM ;         //隨機宣告切割點

	if( randomR() <= CROSSOVER_RATE ){         //要交配

		for( int i = 0 ; i < BITSNUM ; i++ ){

			if( i <= cutpoint){               //在切割點之前
				Ca[i] = Pa[i] ;
				Cb[i] = Pb[i] ;
			}
			else{                             //在切割點之後
				Ca[i] = Pb[i] ;
				Cb[i] = Pa[i] ;
			}
		}
	}
	else{                       // 不交配
		Ca = Pa ;
		Cb = Pb ;
	}

}

bits_set Mutation( bits_set solution ){          //變異function

	int mutationPoint = rand() % BITSNUM ;

	if( randomR() <= MUTATION_RATE ){         //要變異

		if( solution[ mutationPoint] == 0 )
			solution[ mutationPoint] = 1 ;
		else if( solution[ mutationPoint] == 1 )
			solution[ mutationPoint] = 0 ;

	}

	return solution ;          //不變異就回傳原本的回去

}

void Transition() {



}

void recordFitness( int bestfitness, int iteration ) {  //紀錄每個ITERATION的fitness

  if (fitness_Record_List.size() < iteration + 1)  //如果還沒有該ITERATION的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的ITERATION裡 最後輸出時會平均
    fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}

int main() {

	FILE *fp;

	fp = fopen("SEoutput_Tournament.txt", "w");

	srand(time(0));
	int countRun = 0 ;

	while ( countRun < RUN ) {

		Initialization() ;
		ResourceArrangment() ;

		bits_set bestsolution ;
		int count = 0 ;

		while( count < ITERATION ){

			for(int i = 0 ; i < POPULATION ; i++ ){                            // E
				if( Fitness(bestsolution) == 0 || Fitness(pList[i]) > Fitness(bestsolution) )
					bestsolution = pList[i] ;
			}

			bits_List Qlist ;

			for(int i = 0 ; i < ( POPULATION / 2 ) ; i++ ){

				bits_set Pa, Pb ;
				Selection( Pa, Pb ) ;                  // 選出父代

				bits_set Ca, Cb ;
				Crossover( Ca, Cb, Pa, Pb ) ;          // 交配    T

				Qlist.push_back( Mutation( Ca ) );      // 把變異過後的子代推進陣列
				Qlist.push_back( Mutation( Cb ) );

			}

			pList = Qlist ;                            //把該輪子代變成父代
			Qlist.clear() ;                            //清空子代以方便下輪使用

      		recordFitness(Fitness(bestsolution), count);  //紀錄每ITERATION的fitness

			count++ ;
			// cout << "ITERATION:" << count <<  "     fitness:" << Fitness(bestsolution) << endl <<  bestsolution << endl  ;       // debug 用

		}

		pList.clear();  // 將父代陣列清空 以進行下一次run
		countRun++;

		cout << "run : " << countRun << " done " << endl ;

	}

	for (int i = 0; i < 1000; i++)
		fprintf(fp, "%d\t%f\n", (i*POPULATION + 1), (fitness_Record_List[i] / RUN));       // O

	fclose(fp);

	cout << "done! output exported!" << endl;

	system("pause") ;

}
