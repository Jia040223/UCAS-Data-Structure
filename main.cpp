#include "head_file.h"


int main(int argc, char* argv[]) {
	if (argc == 1) { //ֱ������exe��û�в���
		GUI();
		return 0;
	}
	else if (argc == 2) { 
		if (!strcmp(argv[1], "cmd") || !strcmp(argv[1], "CMD")) { 
			cmd();
			return 0;
		}
		else if (!strcmp(argv[1], "gui") || !strcmp(argv[1], "GUI")) { 
			GUI();
			return 0;
		}
		else { //��������
			printf("unknown command\n");
			return 1;
		}
	}
	else { //��������
		printf("unknown command: too many argument\n");
		return 1;
	}
}


int cmd()
{
	Terminal_Input();
	getchar();
	return 0;
}

int GUI()
{
	Welcome();
	return 0;
}

