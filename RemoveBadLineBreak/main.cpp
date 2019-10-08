#include<stdio.h>
#include<string.h>
#include<malloc.h>

//�־��� ������ .txt���� Ȯ��
bool isFileFormatTXT(char* filepath) {

	int len = strlen(filepath);

	if (filepath[len - 4] == '.' &&
		(filepath[len - 3] == 't' || filepath[len - 3] == 'T') &&
		(filepath[len - 2] == 'x' || filepath[len - 2] == 'X') &&
		(filepath[len - 1] == 't' || filepath[len - 1] == 'T')) {

		return true;
	}

	return false;
}

//���� ��θ� �޾� ���� ������ �������� �ٹٲ��� ����
void removeBadLineBreak(char* filepath) {

	/*
	������ ������ ����
	��ħǥ(.) �ڿ� ��Ÿ�� ����
	���� ���� ���ڰ� �빮���� ���

	*/

#define BUFSIZE 2048

	char *newpath;
	int len = strlen(filepath);
	newpath = (char*)malloc(sizeof(char)*(len + 10));
	strcpy_s(newpath, BUFSIZE, filepath);
	strcpy_s(&newpath[len - 4], BUFSIZE, "_new.txt");

	int removedCnt = 0;
	int lineCnt = 0;

	FILE* fp;
	fopen_s(&fp, filepath, "r");

	char *buf[2];
	int id = 0;
	char* res;

	buf[0] = (char*)malloc(sizeof(char)*(BUFSIZE));
	buf[1] = (char*)malloc(sizeof(char)*(BUFSIZE));

	FILE* fpNew;
	fopen_s(&fpNew, newpath, "w");
	char first;

#define BADLINEBREAK 0
#define GOODLINEBREAK 1

	int flag = BADLINEBREAK;

	res = fgets(buf[1], BUFSIZE, fp);//�� ���� �д´�

	while (1) {
		flag = BADLINEBREAK;
		id = 1 - id;//���� ���� ���� �ٷ� ����
		lineCnt++;
		res = fgets(buf[1 - id], BUFSIZE, fp);

		if (res == NULL) {
			fputs(buf[id], fpNew);
			break;
		}

		if (buf[id][strlen(buf[id]) - 2] == '.') {//������ ���ڰ� .
			flag = GOODLINEBREAK;
		}
		else {
			first = buf[1 - id][0];
			int c = 0;
			while (first == ' ' && c < strlen(buf[1 - id])) {
				first = buf[1 - id][++c];
			}

			if ('A' <= first && first <= 'Z') {//���� ���� ù ���ڰ� �빮��
				flag = GOODLINEBREAK;
			}
		}

		//���� �ٹٲ��� ����
		if (flag == BADLINEBREAK) {
			len = strlen(buf[id]);
			buf[id][len - 1] = 0;
		}

		//������Ͽ� ���
		fputs(buf[id], fpNew);
	}

	printf("change %d line, remove %d LineBreak.\n", lineCnt, removedCnt);

	//����
	free(newpath);
	free(buf[0]);
	free(buf[1]);
	fclose(fp);
	fclose(fpNew);
}

int main(int argc, char* argv[]) {

	//char a[] = { "C:\\Users\\grklimt3\\source\\repos\\RemoveLineBreak\\Debug\\temp.txt" };

	for (int i = 0; i < argc; i++) {
		if (isFileFormatTXT(argv[i])) {
			removeBadLineBreak(argv[i]);
			printf("OK       %s\n", argv[i]);
		}
		else
			printf("NO       %s\n", argv[i]);

	}
	//removeBadLineBreak(a);

	getchar();

	return 0;
}