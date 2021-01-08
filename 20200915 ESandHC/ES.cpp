#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <bitset>

#define BITSNUM 100              // bit的數量

//呂季澤 9/15 作業

using namespace std;

bitset<BITSNUM> Initialization() {        //做一個隨機的初始解
	bitset<BITSNUM> iniset ;	
	return iniset;
}

bitset<BITSNUM> Transition( bitset<BITSNUM> solution , int count ) {
	
	solution = Initialization() ;   //先歸零
	
	int i = 0 , n ; 
	n = count ;                 //利用count來一直往上加，並轉成二進位達到暴力法的效果 
	
    while ( n > 0 )                 // 這個while拿來十進位轉二進位 
    {
    	int tempint = n % 2 ;

		if(tempint == 0) solution[i] = 0 ;
		else solution[i] = 1 ;
		
        i = i + 1;
        n = n / 2;
    }
		
	return solution ;
}

int Evaluation( bitset<BITSNUM> solution ) {
	
	int fitness = 0 ;
	
	for(int i = 0 ; i < BITSNUM ; i++){
		if( solution[i] == 1 ) 
			fitness++ ;         // 數看看1有幾個，看fitness多少 
	}
	
	return fitness ;     //回傳 fitness
}

void Determination( int &nowfitness, int &bestfitness, bitset<BITSNUM> &bestsolution, bitset<BITSNUM> &bits ) {
	
	if( nowfitness > bestfitness ) {          //去比較最新產生的解的fitness有沒有比目前最佳解好 
		bestfitness = nowfitness ;
		bestsolution = bits ;
	} 
	
}

bool ifDone( bitset<BITSNUM> solution ) {              //驗證是否提前達到最佳解 

	int count = 0 ;
	
	for(int i = 0 ; i < BITSNUM ; i++ ){
		if( solution[i] == 1 )
			count++ ;
			
		if( count == BITSNUM )
			return true ;
	}
	
	return false ;
}

int main() {
	
	bitset<BITSNUM> bits, bestsolution = Initialization() ;                         //  I
	
	int nowfitness = 0, count = 0,  bestfitness = Evaluation( bestsolution ) ;      //  E
	
	while( /*count < 1000*/  true && !ifDone(bestsolution) ){                       //跑 iteration 1000 (改true可以一直跑) 

	    bits = Transition( bestsolution , count ) ;                                 //  T
	    nowfitness = Evaluation( bits ) ;                                           //  E
		Determination( nowfitness, bestfitness, bestsolution, bits ) ;                //  D

		count++ ;
		cout << "iteration:" << count << " " << bestsolution << " " << "fitness:" << bestfitness << endl ;     // O
	}
	
	system("pause") ;
} 
