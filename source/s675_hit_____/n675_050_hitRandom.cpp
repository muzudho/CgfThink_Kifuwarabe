#include <windows.h>								// rand() �����g�p���邽�߂ɁB


extern "C" {

	#include "../../header/h675_hit_____/n675_050_hitRandom.h"

}




HitRandom::HitRandom()
{
};




int HitRandom::Evaluate_AdjNode(
	)
{
	int score = 0;

	score = rand() % 100; // 0 �` 99 �̃����_���ȕ]���l��^����B

	return score;
}