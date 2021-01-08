#include <bitset>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM 10000              // bit的數量
#define SELECTION 2             // 用哪種selection選擇父代  1 = Roulette, 2 = Tournament
#define POPULATION 30           // 每個迭代的父代數量
#define ITERATION 1000000          // 迭代數
#define RUN 30                  // RUN數
#define CROSSOVER_RATE 0.8      // 交配率
#define MUTATION_RATE 0.15       // 變異率

//呂季澤 7/15 GA 作業

using namespace std;
typedef bitset<BITSNUM> bits_set ;
typedef vector<bits_set> bits_List ;

vector<float> fitness_Record_List;  //紀錄最佳解 畫圖用
bits_List pList ;                   //父代陣列

int whichSelection = SELECTION ,pSize = POPULATION, iteration = ITERATION, runtime = RUN ;
float crossOverRate = CROSSOVER_RATE, mutationRate = MUTATION_RATE ;

void outputUI(){                                   //參數顯示
	cout << "*****************************************\n" ;
	cout << "*   One-Max Problem                     *\n" ;
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

bits_set Initialization() {                       //隨機賦予值

	bits_set iniset ;

	for(int i = 0 ; i < BITSNUM ; i++)            // 用判斷隨機數為奇數偶數來隨機賦予值
		iniset[i] = rand()%2 ;

	return iniset;
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

bits_set RouletteSelection(){                   //輪盤法則

	bits_List tempList = pList ;                //先依照fitness由大到小去排序

	int totalfitness = 0 ;
	int tempfitness = 0 ;

	for( int i = 0 ; i < tempList.size() ; i++ )            //這邊我有依照他們fitness的排名去做機率的倍增，目的是希望可以更快收斂
		totalfitness = totalfitness + Fitness(tempList[i]) ;

	int dart = rand() % ( totalfitness + 1 ) ;              //隨機射飛鏢

	for( int i = 0 ; i < tempList.size() ; i++ ){           //這個for迴圈是在看靶，看自己射到哪並且回傳

		tempfitness = tempfitness + Fitness(tempList[i])  ;

		if( dart < tempfitness){
        // cout << pList[i] << "    " << Fitness(pList[i]) << "  " << dart << "<" << tempfitness << endl ;
			return tempList[i] ;
		}
	}

}

bits_set TournamentSelection(){                     //錦標賽法則

	int choose1 = rand() % pSize ;                  //隨機選兩個來比賽
	int choose2 = rand() % pSize ;

	if( Fitness( pList[choose1] ) >= Fitness( pList[choose2] ) )     //看誰的fitness大就回傳誰
		return pList[choose1] ;
	else if( Fitness( pList[choose1] ) < Fitness( pList[choose2] ) )
		return pList[choose2] ;

}

void Selection( bits_set &Pa, bits_set &Pb ) {                      //呼叫各種法則的function 會先進到這裡在依 whichSelection 去做選擇

	if( whichSelection == 1 ){
		Pa = RouletteSelection() ;     // 輪盤法則，各射一次飛鏢
		Pb = RouletteSelection() ;
	}
	if( whichSelection == 2 ) {
		Pa = TournamentSelection() ;   // 錦標賽法則, 競賽兩次選出兩個子代
		Pb = TournamentSelection() ;
	}

}

void Crossover( bits_set &Ca, bits_set &Cb, bits_set Pa, bits_set Pb ){      //交配function <3

	int cutpoint = rand() % BITSNUM ;         //隨機宣告切割點

	if( randomR() <= crossOverRate ){         //要交配

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

	if( randomR() <= mutationRate ){         //要變異

		if( solution[ mutationPoint] == 0 )
			solution[ mutationPoint] = 1 ;
		else if( solution[ mutationPoint] == 1 )
			solution[ mutationPoint] = 0 ;

	}

	return solution ;          //不變異就回傳原本的回去

}

void recordFitness( int bestfitness, int iteration ) {  //紀錄每個iteration的fitness

  if (fitness_Record_List.size() < iteration + 1)  //如果還沒有該iteration的紀錄 新增到vector裡
    fitness_Record_List.push_back(bestfitness);
  else  //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
    fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}

int main() {

	FILE *fp;

	if( whichSelection == 1 ) {
		fp = fopen("GAoutput_Roulette.txt", "w");
	}
	else if ( whichSelection == 2 ) {
		fp = fopen("GAoutput_Tournament.txt", "w");
	}

	srand(time(0));
	outputUI() ;                                            //使用者參數顯示
	system("pause") ;

	int countRun = 0 ;

	while ( countRun < runtime ) {

		for(int i = 0 ; i < pSize ; i++ )                       //  I
			pList.push_back( Initialization() ) ;               //將起點的隨機解加入父代陣列裡

		bits_set bestsolution ;

		int count = 0 ;

		while( count < iteration ){

			for(int i = 0 ; i < pSize ; i++ ){                            // E
				if( Fitness(bestsolution) == 0 || Fitness(pList[i]) > Fitness(bestsolution) )
					bestsolution = pList[i] ;
			}

			bits_List Qlist ;

			for(int i = 0 ; i < ( pSize / 2 ) ; i++ ){

				bits_set Pa, Pb ;
				Selection( Pa, Pb ) ;                  // 選出父代

				bits_set Ca, Cb ;
				Crossover( Ca, Cb, Pa, Pb ) ;          // 交配    T

				Qlist.push_back( Mutation( Ca ) );      // 把變異過後的子代推進陣列
				Qlist.push_back( Mutation( Cb ) );

			}

			pList = Qlist ;                            //把該輪子代變成父代
			Qlist.clear() ;                            //清空子代以方便下輪使用

      		recordFitness(Fitness(bestsolution), count);  //紀錄每iteration的fitness

			count++ ;
			cout << "iteration:" << count <<  "     fitness:" << Fitness(bestsolution)  << endl  ;       // debug 用

		}

		pList.clear();  // 將父代陣列清空 以進行下一次run
		countRun++;

		cout << "run : " << countRun << " done " << endl ;

	}

	for (int i = 0; i < 1000; i++)
		fprintf(fp, "%d\t%f\n", (i*pSize + 1), (fitness_Record_List[i] / runtime));       // O

	fclose(fp);

	cout << "done! output exported!" << endl;

	system("pause") ;

}
