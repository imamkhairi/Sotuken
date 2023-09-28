// Ｃ言語 × zenity の基本プログラム gui.c
// $ cc gui.c -o gui

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	NAMELEN	64
#define	CMDLEN	256

int main()
{
	char	name[NAMELEN];
	char	cmd[CMDLEN];
	FILE	*pp;
	int	s;

	// 文字列入力ダイアログ
	pp = popen("zenity --entry --text='お名前は？'", "r");
		// initiates a process by forking, constructing a pipe, and executing the shell
		// used to read and write to a unix pipe
		// r -> read O/P from command
		// w -> write data as I/P to command
	if (pp == NULL) return (1);
	if (fgets(name, NAMELEN, pp) == NULL) return (1);
			// char *str, int n, FILE *stream
			// read input from stream
			// stops when either (n-1) characters are read, the newline character is read, or the end-of-file is reached
			// if error occured return NULL value
			// 名前の文字列を入力，[キャンセル] で終了，[OK] で次へ
	pclose(pp);
	strtok(name, "\n");	// 末尾の改行文字を除去

	/* 豆：popen() ってのは fopen() の親戚ね．パイプのオープン．
		ファイルではなくコマンドとの間でデータ入出力する時に使うんだ．
		入出力が終わったら，pclose() も忘れずに． */

	// 質問ダイアログ
	sprintf(cmd, "zenity --question --text='%s さん，こんにちは．\nお元気ですか？'", name);
				// コマンドライン文字列を作成
	s = system(cmd);	// コマンドを実行
		// pass the ommand name or program name by command to the environment
		// to be executed by the command processor

	/* 豆：system() はコマンドを実行するだけね．
		終了状態だけは調べられるけど，データの入出力はできません． */

	// 通知ダイアログ
	if (s == 0) {		// [はい] だった場合
		system("zenity --info --text='なによりです'");
	} else {		// [いいえ] だった場合
		system("zenity --info --text='お大事に...'");
	}
	return (0);
}

