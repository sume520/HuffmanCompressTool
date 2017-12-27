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

extern unsigned int leaves, nodenum;//有效的叶子长度,node_max=2*Bmp_length-1
extern unsigned int Length;
extern unsigned int len;//后缀长度

typedef struct {
	unsigned char unchar;
	unsigned int parent;//父母
	unsigned int leftChild;//左孩子
	unsigned int rightChild;//右孩子
	unsigned int weight;//权值
	char code[512];//哈夫曼编码
}HaffNode, *HuffTree;

void HuffmanTree(HuffTree hfm);//构建哈夫曼树
void HuffmanCode(HuffTree hfm); //计算哈夫曼编码
