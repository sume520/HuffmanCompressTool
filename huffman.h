#include <iostream>
#include <stack>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string.h>
#include <string>

extern unsigned int leaves, nodenum;//��Ч��Ҷ�ӳ���,node_max=2*Bmp_length-1
extern unsigned int Length;
extern unsigned int len;//��׺����

typedef struct {
	unsigned char unchar;
	unsigned int parent;//��ĸ
	unsigned int leftChild;//����
	unsigned int rightChild;//�Һ���
	unsigned int weight;//Ȩֵ
	char code[512];//����������
}HaffNode, *HuffTree;

void HuffmanTree(HuffTree hfm);//������������
void HuffmanCode(HuffTree hfm); //�������������
