#include <windows.h>								// rand() 等を使用するために。
#include "../../../header/n600_tactics_/n600_200_hit_____/n600_200_050_hitRandom.h"




HitRandom::HitRandom()
{
};




int HitRandom::Evaluate(
	)
{
	return rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
}