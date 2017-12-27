#include "huffman.h"
#include <qdebug.h>

void HuffmanTree(HuffTree hfm)//������������
{
	int s1, s2;//������Сֵ���ڶ���Сֵ
	nodenum = 2 * leaves - 1;//�����2 * Bmp_Type - 1���ڵ�
	for (int i = leaves; i < nodenum; i++)
	{
		unsigned long m = 999999999;//�������ֵ

        /***********�ҳ�������С����*******************/
		for (int j = 0; j < i; j++)
		{//ѭ��Ѱ�Ҹ�ĸ�������СȨֵҶ��
			if (!hfm[j].parent&&hfm[j].weight < m)
			{
				m = hfm[j].weight;
				s1 = j;//��Сֵ
			}
		}
		hfm[s1].parent = 1;//��ʾ�и�ĸ

		m = 999999999;
		for (int j = 0; j < i; j++)
		{
			if (!hfm[j].parent&&hfm[j].weight < m)
			{
				m = hfm[j].weight;
				s2 = j;//�ڶ���Сֵ
			}
		}
        /********************************************/

		hfm[s2].parent = 1;
		hfm[s1].parent = hfm[s2].parent = i;//���踸ĸ
		hfm[i].leftChild = s1;//���½ڵ㸳������
		hfm[i].rightChild = s2;//���½ڵ㸳���Һ���
		hfm[i].weight = hfm[s1].weight + hfm[s2].weight; //���½ڵ㸳��Ȩֵ
	}
}

void HuffmanCode(HuffTree hfm) //�������������
{
    /*for (int i = 0; i < leaves; i++)
	{
		int j = 512, q = i;//��Ҷ�ӽڵ���ǰ�ƹ��������룬���Դ��������ʼ
		while (hfm[q].parent)
		{//�ڵ��и�ĸ
			if (hfm[hfm[q].parent].leftChild == q)
				hfm[i].code[--j] = '0';//����ڵ��ֵ���ڸ�ĸ�����ӵ�ֵ������0
			else
				hfm[i].code[--j] = '1';//����ڵ��ֵ���ڸ�ĸ���Һ��ӵ�ֵ������1
			q = hfm[q].parent;//Ѱ����һ����ĸ
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
    }//������β�������ö�*/

    int i;
    int cur,next,index;
    char code_tmp[512];
    code_tmp[511]='\0';

    //��������ɨ
    for(i=0;i<leaves;i++)
    {
        index=512-1;

        for(cur=i,next=hfm[i].parent;next!=0;next=hfm[next].parent)
        {
            if(hfm[next].leftChild==cur)
                code_tmp[--index]='0';//��ڵ�
            else
                code_tmp[--index]='1';//�ҽڵ�
            cur=next;
        }
        strcpy(hfm[i].code,&code_tmp[index]);//�����뱣�浽hfm.code��
       // qDebug()<<hfm[i].code<<" "<<code_tmp[index]<<endl;
    }
}
