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
			break;
		case CMD_INPUT:
			if (!motion.SetAlphabet())
			{
				std::cout << "¾ËÆÄºª ÁÂÇ¥ º¹»ç ½ÇÆÐ" << std::endl;
			}
			break;
		default:
			break;
		}
	}
}