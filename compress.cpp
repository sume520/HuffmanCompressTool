#include "compress.h"
#include <QFileDialog>
#include <qstring.h>
#include <QMessageBox>


unsigned int leaves = 0, nodenum;//有效的叶子长度,node_max=2*Bmp_length-1
unsigned int Length = 0;
unsigned int len = 0;//后缀长度

void Init()//初始化数据
{
    leaves=0;
    nodenum=0;
    Length=0;
    len=0;
}

bool greater(HaffNode &a, HaffNode &b)//sort()函数从大到小排序
{
    return a.weight > b.weight;
}

bool OpenFile(char *filename, HuffTree &huffmanlist) //统计weight
{
    unsigned char Bmp_char;//字节字符
    FILE * File = NULL;
    //以二进制形式打开文件
    if ((File = fopen(filename, "rb")) == NULL)
    {
        return false;
    }
    huffmanlist = new HaffNode[512];

    //初始化数据
    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 512; j++)
            huffmanlist[i].code[j] = 0;
    for (int i = 0; i < 512; i++)
    {
        huffmanlist[i].weight = 0;
        huffmanlist[i].leftChild = 0;
        huffmanlist[i].rightChild = 0;
        huffmanlist[i].parent = 0;
        huffmanlist[i].unchar = i;
    }

    while (fread(&Bmp_char, 1, 1, File))//一次一个字节读文件
    {
        huffmanlist[Bmp_char].weight++;//统计权值
        Length++;
    }

    sort(huffmanlist, huffmanlist + 512, greater);//以大到小排序

    for (int i = 0; i < 512; i++)//计算非零权值的数量
        if (huffmanlist[i].weight)
            leaves++;
    fclose(File);//关闭文件
    //cout << "打开完毕" << endl;
    return true;
}

void getTypeName(char *str, char *&typeName)
{
    len = strlen(str);
    int i = len;
    while (str[i] != '.'&&i >= 0) { i--; }//从后往前找到后缀
    //cout << i << endl;
    //cout << i << " ";
    len = len - i;
    //cout << len << endl;
    typeName = new char(len - i + 1);
    if (i >= 0)
    {
        strncpy(typeName, str + i, len);
    }
    typeName[len] = '\0';
}

bool Compress(HuffTree hfm, char*filename, char *htfname)//压缩
{
    HuffmanTree(hfm);//创建哈夫曼树
    HuffmanCode(hfm);//创建哈夫曼编码

    FILE * File = NULL;
    if ((File = fopen(filename, "rb")) == NULL)//打开文件
    {
        //cout << "打开.hfm文件失败！" << endl;
        return false;
    }
    //FILE * outFile = fopen(strcat(htfname, ".hfm"), "wb");//打开输出文件
    FILE * outFile = fopen(htfname, "wb");
    unsigned char temp;
    char buffer[256] = "\0";//缓存流
    //float len1 = 0;
    char *typeName;
    getTypeName(filename, typeName);

    //string filetype = typeName;

    /******************写入头文件******************/
    fwrite((char*)&len, sizeof(unsigned int), 1, outFile);//存后缀长度
    fwrite(typeName, sizeof(char), len, outFile);//存后缀
    fwrite((char *)&leaves, sizeof(unsigned int), 1, outFile);//存叶子长度

    for (int i = 0; i < leaves; i++) {//存每个编号对应的字符，权重
        fwrite((char*)&hfm[i].unchar, sizeof(unsigned char), 1, outFile);
        fwrite((char*)&hfm[i].weight, sizeof(unsigned long), 1, outFile);
        //printf("%d:%c %d\t", i, hfm[i].unchar, hfm[i].weight);
    }
    /********************************************/

    while (fread((char *)&temp, sizeof(unsigned char), 1, File))
    {//文件不为空
        for (int i = 0; i < leaves; i++)//找对应字符
            if (temp == hfm[i].unchar)
                strcat(buffer, hfm[i].code);//给缓存流赋值（哈夫曼编码）
        while (strlen(buffer) >= 8) {//缓存流大于等于8个bits进入循环，每读到8位压缩一次
            temp = 0;
            for (int i = 0; i < 8; ++i)//每8个bits循环一次
                temp = (temp << 1) | (buffer[i] - 48);//将bits存入temp
            fwrite((char *)&temp, sizeof(unsigned char), 1, outFile);//写入文件
            strcpy(buffer, buffer + 8);//将写入文件的bits删除
           // len1++;
        }
    }
    unsigned int l = strlen(buffer);//将剩余不足为8的bits的个数给l
    if (l) {//如果最后长度不为0，既存在不足8bits的字符
        temp = 0;
        for (int i = 0; i < l; i++)
            temp = (temp << 1) | (buffer[i] - 48);//前移一位
        temp <<= 8 - l;//将存入的bits置顶
        fwrite((char *)&temp, sizeof(unsigned char), 1, outFile);//补足后写入
    }

    fflush(outFile);
    fclose(File);
    fclose(outFile);
    return true;
}

bool Extract(char*in,string extfile) //解压缩
{
    char *typeName = NULL;//后缀名
    int length;//后缀长度

    char exf[sizeof(extfile)];
    strcpy(exf, extfile.data());//string转char*
    FILE * File = NULL;
    if ((File = fopen(in, "rb"))==NULL)//二进制方式读取压缩文件
    {
        return false;
    }

    /*********************读入文件头******************************/
    fread((char*)&length, sizeof(unsigned int), 1, File);//读取后缀长度
    typeName = new char(length);
    fread(typeName, sizeof(char), length, File);//读取后缀
    typeName[length] = '\0';
    QFile::rename(exf,strcat(exf, typeName));//rename extrace file
    FILE * outFile = fopen(exf, "wb");//二进制方式写入解压文件
    fread((char *)&leaves, sizeof(unsigned int), 1, File);//读长度
    /***********************************************************/

    int node_max = 2 * leaves - 1;//计算出节点个数
    HuffTree hfm = new HaffNode[node_max];
    for (int i = 0; i < leaves; i++)//初始化为0
        for (int j = 0; j < leaves; j++)
            hfm[i].code[j] = 0;

    //读出数据到hfm数组中
    for (int i = 0; i < node_max; i++)
    {
        if (i < leaves) {
            fread((char *)&hfm[i].unchar, sizeof(unsigned char), 1, File);
            fread((char *)&hfm[i].weight, sizeof(unsigned long), 1, File);
        }
        hfm[i].leftChild = 0;
        hfm[i].rightChild = 0;
        hfm[i].parent = 0;
    }
    HuffmanTree(hfm);//利用读出的字符和权重构建哈夫曼树
    unsigned char buffer;
    int root = node_max - 1;//根节点下标
    while (fread((char *)&buffer, sizeof(unsigned char), 1, File)) {//从上往下扫描哈夫曼树
        for (int i = 0; i < 8; i++)
        {
            if (buffer & 128)//判断最前面一位是0或1
                root = hfm[root].rightChild;
            else
                root = hfm[root].leftChild;
            if (root < leaves) {//当root<leave时说明已经数据可以扫描出来则将最后一个写入到outFile中
                fwrite(&hfm[root].unchar, sizeof(unsigned char), 1, outFile);
             root = node_max - 1;//复位
            }
            buffer <<= 1;//将后面一位向前移到第一位
        }
    }

    delete[] hfm;
    fclose(File);
    fclose(outFile);
    QFile::remove(extfile.data());//删除不带后缀的同名文件
    return true;
}
