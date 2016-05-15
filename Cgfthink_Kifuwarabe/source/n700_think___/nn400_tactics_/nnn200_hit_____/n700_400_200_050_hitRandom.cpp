#include <windows.h>								// rand() 等を使用するために。
#include "../../../../header/n700_think___/nn400_tactics_/nnn200_hit_____/n700_400_200_050_hitRandom.h"




HitRandom::HitRandom()
{
};




int HitRandom::Evaluate(
	)
{
	return rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
}