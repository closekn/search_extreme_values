/**
==============================
file   : search4maxima.c
brief  : Search for maxima between start to end.
author : closekn
update : 2020/12/25
==============================
*/

//---- ヘッダファイル
#include <stdio.h>
#include <math.h>

//---- マクロ定数
#define BUF 65535 	// 読込可能データ業数

//---- 関数定義
void input(double *lim, double *start, double *end);		// 入力
int input_base_index(int *base_index, int num_ex);			// 基準極値インデックスの入力
void output(int n, int base_index);											// 出力
int file_load();																				// ファイルデータ読込
int calc_maxima(double lim, double start, double end);	// 計測処理(極大値判定)
double calc_noise(int k, double end);										// ノイズサイズ測定

//---- 大域変数
char fname[BUF];		// ファイル名
double measur[BUF];		// 計測時間
double value[BUF];		// 計測値
int extreme[BUF]; 		// 極値の引数

//---- メイン
int main()
{
	double lim;			// しきい値
	double start;		// 調査開始計測時間
	double end;			// 調査終了計測時間
	int num_d;			// データ数
	int num_ex;			// 極値数
	int base_index; // 基準極値インデックス

	//-- 入力
	input(&lim, &start, &end);
	
	//-- ファイル読込
	num_d = file_load();
	if ( num_d == 0 ) { return 0; }
	
	//-- 極値判断
	num_ex = calc_maxima(lim, start, end);

	//-- 出力
	base_index = -1;
	while ( 1 ) {
		output(num_ex, base_index);
		if ( input_base_index(&base_index, num_ex) ) { break; }
	}
	
	return 0;
}

//---- 入力
void input(double *lim, double *start, double *end) {
	printf("テキストファイル名入力: "); scanf("%s", fname);
	printf("しきい値入力: "); scanf("%lf", lim);
	printf("開始時間: "); scanf("%lf", start);
	printf("終了時間: "); scanf("%lf", end);
}

//---- 基準極値インデックスの入力
int input_base_index(int *base_index, int num_ex) {
	int check_break = 0;

	printf("\n基準とするピークのNo. (-1で終了) : "); scanf("%d", base_index);
	(*base_index)--;
	if ( ! (0 <= *base_index && *base_index < num_ex) ) { check_break = 1; }

	return check_break;
}

//---- 出力
void output(int n, int base_index) {
	double ave;		// 平均
	int k;			// 反復変数

	puts("");
	printf(" No. |  時間  |    値    "); if ( base_index != -1 ) { printf("| ピーク比較 base No.%d", base_index+1); } puts("");
	printf("-------------------------"); if ( base_index != -1 ) { printf("-----------------------"); } puts("");

	ave = 0;
	for ( k = 0; k < n; k++ ) {
		printf(" %3d | %.4f | %.6f ", k+1, measur[extreme[k]], value[extreme[k]]);
		if ( base_index != -1 ) { printf("|  %10.3f %%", value[extreme[k]]/value[extreme[base_index]]*100); }
		puts("");
		ave += value[extreme[k]];
	}
	ave /= n;

	printf("-------------------------"); if ( base_index != -1 ) { printf("-----------------------"); } puts("");
	printf("極大値数 : %d\n", n);
	printf("極大値平均 : %.6f\n", ave);
}

//---- ファイル読込
int file_load() {
	FILE *fp;		// ファイルポインタ
	char buf[BUF];	// 一時配列
	int n = 0;		// データ数
	
	fp = fopen(fname, "r");
	if ( fp == NULL ) {
		printf("can't open.\n");
		return 0;
	}
	
	while ( fgets(buf, BUF, fp) != NULL ) {
		sscanf(buf, "%lf %lf", &measur[n], &value[n]);
		n++;
	}
	
	fclose(fp);

	return n;
}

//---- 計算処理(極大値判定)
int calc_maxima(double lim, double start, double end) {
	int k = 0;		// 反復変数
	double max;		// 極大値
	int pos;		// 極値引数
	int ct = 0;		// 極値数
	double noise;	// ノイズ

	while ( start > measur[k] ) { k++; }

	noise = calc_noise(k, end);

	while ( measur[k] <= end ) {
		while ( value[k] < lim ) { k++; if( end <= measur[k] ) { return ct; } }
		max = value[k];
		while ( value[k] >= lim - noise ) {
			if ( value[k] >= max ) { max = value[k]; pos = k; }
			k++;
		}
		extreme[ct] = pos;
		ct++;
	}
	
	return ct;
}

//---- ノイズサイズ測定
double calc_noise(int k, double end) {
	double max = 0;		// 最大ノイズ
	double dif;			// 前後差

	while ( measur[k] < end ) {
		dif = fabs(value[k+1]-value[k]);
		if ( max < dif ) { max = dif; }
		k++;
	}

	return max;
}
