//
// /source/s675_hit_____/s675_050_hitRandom.cpp
//

extern "C" {

	#include <windows.h>								// rand() 等を使用するために。
	#include "../../header/h675_hit_____/h675_050_hitRandom.h"

}




HitRandom::HitRandom()
{
};




int HitRandom::Evaluate_AdjNode(
	)
{
	int score = 0;

	score = rand() % 100; // 0 ～ 99 のランダムな評価値を与える。

	return score;
}