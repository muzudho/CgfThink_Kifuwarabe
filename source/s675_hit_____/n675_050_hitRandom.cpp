#include <windows.h>								// rand() 等を使用するために。
#include "../../header/h675_hit_____/n675_050_hitRandom.h"




HitRandom::HitRandom()
{
};




int HitRandom::Evaluate(
	)
{
	return rand() % 100; // 0 ～ 99 のランダムな評価値を与える。
}