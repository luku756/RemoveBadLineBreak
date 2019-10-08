#include<stdio.h>
#include<string.h>
#include<malloc.h>

//주어진 파일이 .txt인지 확인
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

//파일 경로를 받아 파일 내에서 부적절한 줄바꿈을 제거
void removeBadLineBreak(char* filepath) {

	/*
	적절한 엔터의 조건
	마침표(.) 뒤에 나타난 엔터
	엔터 다음 문자가 대문자인 경우

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

	res = fgets(buf[1], BUFSIZE, fp);//한 줄을 읽는다

	while (1) {
		flag = BADLINEBREAK;
		id = 1 - id;//다음 줄을 현재 줄로 변경
		lineCnt++;
		res = fgets(buf[1 - id], BUFSIZE, fp);

		if (res == NULL) {
			fputs(buf[id], fpNew);
			break;
		}

		if (buf[id][strlen(buf[id]) - 2] == '.') {//마지막 문자가 .
			flag = GOODLINEBREAK;
		}
		else {
			first = buf[1 - id][0];
			int c = 0;
			while (first == ' ' && c < strlen(buf[1 - id])) {
				first = buf[1 - id][++c];
			}

			if ('A' <= first && first <= 'Z') {//다음 줄의 첫 문자가 대문자
				flag = GOODLINEBREAK;
			}
		}

		//나쁜 줄바꿈을 제거
		if (flag == BADLINEBREAK) {
			len = strlen(buf[id]);
			buf[id][len - 1] = 0;
		}

		//출력파일에 출력
		fputs(buf[id], fpNew);
	}

	printf("change %d line, remove %d LineBreak.\n", lineCnt, removedCnt);

	//해제
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