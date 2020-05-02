/**
==============================
file   : search4minima.c
brief  : Search for minima between start to end.
author : closekn
date   : 2020/05/02
==============================
*/

//---- �w�b�_�t�@�C��
#include <stdio.h>
#include <math.h>

//---- �}�N���萔
#define BUF 65536		// �Ǎ��\�f�[�^�Ɛ� 

//---- �֐���`
void input(double *lim, double *start, double *end);	// ����
void output(int n);										// �o��
int file_load();										// �t�@�C���f�[�^�Ǎ�
int calc_minima(double lim, double start, double end);	// �v������(�ɏ��l����)
double calc_noise(int k, double end);					// �m�C�Y�T�C�Y����

//---- ���ϐ�
char fname[BUF];		// �t�@�C����
double measur[BUF];		// �v������
double value[BUF];		// �v���l
int extreme[BUF]; 		// �ɒl�̈���

//---- ���C��
int main()
{
	double lim;		// �������l
	double start;	// �����J�n�v������
	double end;		// �����I���v������
	int num_d;		// �f�[�^��
	int num_ex;		// �ɒl��

	//-- ����
	input(&lim, &start, &end);
	
	//-- �t�@�C���Ǎ�
	num_d = file_load();
	if ( num_d == 0 ) { return 0; }
	
	//-- �ɒl���f
	num_ex = calc_minima(lim, start, end);

	//-- �o��
	output(num_ex);
	
	return 0;
}

//---- ����
void input(double *lim, double *start, double *end) {
	printf("�e�L�X�g�t�@�C��������: "); scanf("%s", fname);
	printf("�������l����: "); scanf("%lf", lim);
	printf("�J�n����: "); scanf("%lf", start);
	printf("�I������: "); scanf("%lf", end);
}

//---- �o��
void output(int n) {
	double ave;		// ����
	double ave2;  // 2��̕���
	int k;			// �����ϐ�

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
	printf("�ɏ��l�� : %d\n", n);
	printf("�C���^�[�o������ : %.6f\n", ave);
	printf("�C���^�[�o���W���΍� : %.12f\n", sqrt(ave2-(ave*ave)));
}

//---- �t�@�C���Ǎ�
int file_load() {
	FILE *fp;		// �t�@�C���|�C���^
	char buf[BUF];	// �ꎞ�z��
	int n = 0;		// �f�[�^��
	
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

//---- �v�Z����(�ɏ��l����)
int calc_minima(double lim, double start, double end) {
	int k = 0;		// �����ϐ�
	double min;		// �ɏ��l
	int pos;		// �ɒl����
	int ct = 0;		// �ɒl��
	double noise;	// �m�C�Y

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

//---- �m�C�Y�T�C�Y����
double calc_noise(int k, double end) {
	double max = 0;		// �ő�m�C�Y
	double dif;			// �O�㍷

	while ( measur[k] < end ) {
		dif = fabs(value[k+1]-value[k]);
		if ( max < dif ) { max = dif; }
		k++;
	}

	return max;
}
