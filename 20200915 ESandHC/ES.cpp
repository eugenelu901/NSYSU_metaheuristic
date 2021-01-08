#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <bitset>

#define BITSNUM 100              // bit���ƶq

//�f�u�A 9/15 �@�~

using namespace std;

bitset<BITSNUM> Initialization() {        //���@���H������l��
	bitset<BITSNUM> iniset ;	
	return iniset;
}

bitset<BITSNUM> Transition( bitset<BITSNUM> solution , int count ) {
	
	solution = Initialization() ;   //���k�s
	
	int i = 0 , n ; 
	n = count ;                 //�Q��count�Ӥ@�����W�[�A���ন�G�i��F��ɤO�k���ĪG 
	
    while ( n > 0 )                 // �o��while���ӤQ�i����G�i�� 
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
			fitness++ ;         // �Ƭݬ�1���X�ӡA��fitness�h�� 
	}
	
	return fitness ;     //�^�� fitness
}

void Determination( int &nowfitness, int &bestfitness, bitset<BITSNUM> &bestsolution, bitset<BITSNUM> &bits ) {
	
	if( nowfitness > bestfitness ) {          //�h����̷s���ͪ��Ѫ�fitness���S����ثe�̨θѦn 
		bestfitness = nowfitness ;
		bestsolution = bits ;
	} 
	
}

bool ifDone( bitset<BITSNUM> solution ) {              //���ҬO�_���e�F��̨θ� 

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
	
	while( /*count < 1000*/  true && !ifDone(bestsolution) ){                       //�] iteration 1000 (��true�i�H�@���]) 

	    bits = Transition( bestsolution , count ) ;                                 //  T
	    nowfitness = Evaluation( bits ) ;                                           //  E
		Determination( nowfitness, bestfitness, bestsolution, bits ) ;                //  D

		count++ ;
		cout << "iteration:" << count << " " << bestsolution << " " << "fitness:" << bestfitness << endl ;     // O
	}
	
	system("pause") ;
} 
