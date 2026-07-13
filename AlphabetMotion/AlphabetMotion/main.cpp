#include "AlphabetMotion.h"


int main()
{
	AlphabetMotion motion;

	while (1)
	{
		if (!motion.WaitTrigger())
			continue;
		switch (motion.GetCommand())
		{
		case CMD_NONE:
			break;

		case CMD_MOVE:
			motion.StartMotion();
			break;
		case CMD_STOP:
			// 정지 기능 쓰레드에서 동작
			break;
		case CMD_INPUT:
			if (!motion.SetAlphabet())
			{
				std::cout << "알파벳 좌표 복사 실패" << std::endl;
			}
			break;
		default:
			break;
		}
	}
}