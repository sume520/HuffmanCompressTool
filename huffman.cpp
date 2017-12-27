#include "huffman.h"
#include <qdebug.h>

void HuffmanTree(HuffTree hfm)//构建哈夫曼树
{
	int s1, s2;//定义最小值，第二最小值
	nodenum = 2 * leaves - 1;//最多有2 * Bmp_Type - 1个节点
	for (int i = leaves; i < nodenum; i++)
	{
		unsigned long m = 999999999;//定义最大值

        /***********找出两个最小的数*******************/
		for (int j = 0; j < i; j++)
		{//循环寻找父母非零的最小权值叶子
			if (!hfm[j].parent&&hfm[j].weight < m)
			{
				m = hfm[j].weight;
				s1 = j;//最小值
			}
		}
		hfm[s1].parent = 1;//表示有父母

		m = 999999999;
		for (int j = 0; j < i; j++)
		{
			if (!hfm[j].parent&&hfm[j].weight < m)
			{
				m = hfm[j].weight;
				s2 = j;//第二最小值
			}
		}
        /********************************************/

		hfm[s2].parent = 1;
		hfm[s1].parent = hfm[s2].parent = i;//赋予父母
		hfm[i].leftChild = s1;//给新节点赋予左孩子
		hfm[i].rightChild = s2;//给新节点赋予右孩子
		hfm[i].weight = hfm[s1].weight + hfm[s2].weight; //给新节点赋予权值
	}
}

void HuffmanCode(HuffTree hfm) //计算哈夫曼编码
{
    /*for (int i = 0; i < leaves; i++)
	{
		int j = 512, q = i;//从叶子节点往前推哈夫曼编码，所以从数组最后开始
		while (hfm[q].parent)
		{//节点有父母
			if (hfm[hfm[q].parent].leftChild == q)
				hfm[i].code[--j] = '0';//如果节点的值等于父母的左孩子的值，存入0
			else
				hfm[i].code[--j] = '1';//如果节点的值等于父母的右孩子的值，存入1
			q = hfm[q].parent;//寻找下一个父母
		}
	}
	for (int i = 0; i < leaves; i++)
	{
		int j = 0, q = 0;
		while (!hfm[i].code[j])
		{
			j++;
		}
		while (j != 512)
		{
			hfm[i].code[q] = hfm[i].code[j];
			hfm[i].code[j] = NULL;
			q++;
			j++;
		}
    }//将数组尾的数据置顶*/

    int i;
    int cur,next,index;
    char code_tmp[512];
    code_tmp[511]='\0';

    //从上往下扫
    for(i=0;i<leaves;i++)
    {
        index=512-1;

        for(cur=i,next=hfm[i].parent;next!=0;next=hfm[next].parent)
        {
            if(hfm[next].leftChild==cur)
                code_tmp[--index]='0';//左节点
            else
                code_tmp[--index]='1';//右节点
            cur=next;
        }
        strcpy(hfm[i].code,&code_tmp[index]);//讲编码保存到hfm.code中
       // qDebug()<<hfm[i].code<<" "<<code_tmp[index]<<endl;
    }
}
