#include <bitset>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#define BITSNUM 100

using namespace std;

vector<float> fitness_Record_List;                            //這個陣列用來記錄每個iteration的fitness 最後要畫圖用

bitset<BITSNUM> Initialization() {                            // 把當前解進行隨機初始化

	bitset<BITSNUM> iniset ;

	for(int i = 0 ; i < BITSNUM ; i++)
		iniset[i] = rand()%2 ;

	return iniset;
}

bitset<BITSNUM> Transition( bitset<BITSNUM> solution ) {         //隨機把一個bit 0->1 1->0 並回傳 產生新解

	int num = ( rand() % BITSNUM ) ;

	if( solution[num] == 0 )
		solution[num] = 1 ;
	else
		solution[num] = 0 ;

	return solution ;
}

int Evaluation( bitset<BITSNUM> solution ) {         // 讀入一個solution並回傳該solution的fitness多少

	int fitness = 0 ;

	for(int i = 0 ; i < BITSNUM ; i++){
		if( solution[i] == 1 )
			fitness++ ;
	}

	return fitness ;
}

float randomR(){                           //取得一個0~1隨機小數並回傳
	float r = rand() % 100 / (float)101 ;
	return r ;
}

float getPa( float Temperture , int nowfitness, int bestfitness ) {              //計算該iteration的Pa
	float Pa = exp( (nowfitness-bestfitness) / Temperture ) ;
	return Pa ;
}

void Determination( int &nowfitness, int &bestfitness, float Temperture, bitset<BITSNUM> &bestsolution, bitset<BITSNUM> &bits ) {

	if( randomR() < getPa( Temperture , nowfitness, bestfitness ) ) {             //如果Pa大於一個隨機數 就對當前的bestsolution進行交換
		bestfitness = nowfitness ;
		bestsolution = bits ;
	}

}

void recordFitness( int bestfitness, int iteration ) {         //紀錄每個iteration的fitness

	if ( fitness_Record_List.size() < iteration+1 )              //如果還沒有該iteration的紀錄 新增到vector裡
		fitness_Record_List.push_back(bestfitness);
	else                                                         //如果有的話 把它加進應該加進的iteration裡 最後輸出時會平均
		fitness_Record_List[iteration] = fitness_Record_List[iteration] + bestfitness;

}


int main() {

  FILE *fp;
  fp=fopen("SAoutput.txt","w");
	srand(time(0));

	int runtime = 0;

	while( runtime < 30 ) {

		bitset<BITSNUM> bits, bestsolution = Initialization() ;                         //  I

		float Temperture = 1.2 , a = 0.99 ;                                             //  I
		int nowfitness = 0, count = 0 ,  bestfitness = Evaluation( bestsolution ) ;     //  E

		while( count < 1000 ){

			bits = Transition( bestsolution ) ;                                          //  T
			nowfitness = Evaluation( bits ) ;                                            //  E
			Determination( nowfitness, bestfitness, Temperture, bestsolution, bits ) ;   //  D
			Temperture = Temperture * a ;                                                //降火， a = 0.9

			recordFitness(bestfitness, count);                                           //紀錄每iteration的fitness

			count++ ;
			// cout << "iteration:" << count << " " << bestsolution << " " << "fitness:" << bestfitness << endl ;   // O

		}

		runtime++;

	}

	for (int i = 0 ; i < 1000 ; i++ )
		fprintf( fp, "%d\t%f\n", (i+1), (fitness_Record_List[i]/30) );


	fclose(fp);

	cout << "done! output exported!" << endl;

}
