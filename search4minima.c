/**
==============================
file   : search4minima.c
brief  : Search for minima between start to end.
author : closekn
date   : 2020/05/02
==============================
*/

//---- ヘッダファイル
#include <stdio.h>
#include <math.h>

//---- マクロ定数
#define BUF 65536		// 読込可能データ業数 

//---- 関数定義
void input(double *lim, double *start, double *end);	// 入力
void output(int n);										// 出力
int file_load();										// ファイルデータ読込
int calc_minima(double lim, double start, double end);	// 計測処理(極小値判定)
double calc_noise(int k, double end);					// ノイズサイズ測定

//---- 大域変数
char fname[BUF];		// ファイル名
double measur[BUF];		// 計測時間
double value[BUF];		// 計測値
int extreme[BUF]; 		// 極値の引数

//---- メイン
int main()
{
	double lim;		// しきい値
	double start;	// 調査開始計測時間
	double end;		// 調査終了計測時間
	int num_d;		// データ数
	int num_ex;		// 極値数

	//-- 入力
	input(&lim, &start, &end);
	
	//-- ファイル読込
	num_d = file_load();
	if ( num_d == 0 ) { return 0; }
	
	//-- 極値判断
	num_ex = calc_minima(lim, start, end);

	//-- 出力
	output(num_ex);
	
	return 0;
}

//---- 入力
void input(double *lim, double *start, double *end) {
	printf("テキストファイル名入力: "); scanf("%s", fname);
	printf("しきい値入力: "); scanf("%lf", lim);
	printf("開始時間: "); scanf("%lf", start);
	printf("終了時間: "); scanf("%lf", end);
}

//---- 出力
void output(int n) {
	double ave;		// 平均
	double ave2;  // 2乗の平均
	int k;			// 反復変数

	puts("");
	puts(" time  |  minima  | interval");
	puts("-----------------------------");

	printf("%.4f | %.6f |\n", measur[extreme[0]], value[extreme[0]]);
	ave = 0;
	ave2 = 0;
	for ( k = 1; k < n; k++ ) {
		double interval = measur[extreme[k]] - measur[extreme[k-1]];
		printf("%.4f | %.6f | %.4f\n", measur[extreme[k]], value[extreme[k]], interval);
		ave += interval;
		ave2 += interval * interval;
	}
	ave /= (n-1);
	ave2 /= (n-1);

	puts("-----------------------------");
	printf("極小値数 : %d\n", n);
	printf("インターバル平均 : %.6f\n", ave);
	printf("インターバル標準偏差 : %.12f\n", sqrt(ave2-(ave*ave)));
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

//---- 計算処理(極小値判定)
int calc_minima(double lim, double start, double end) {
	int k = 0;		// 反復変数
	double min;		// 極小値
	int pos;		// 極値引数
	int ct = 0;		// 極値数
	double noise;	// ノイズ

	while ( start > measur[k] ) { k++; }

	noise = calc_noise(k, end);

	while ( measur[k] <= end ) {
		while ( value[k] > lim ) { k++; if( end <= measur[k] ) { return ct; } }
		min = value[k];
		while ( value[k] <= lim + noise ) {
			if ( value[k] <= min ) { min = value[k]; pos = k; }
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
