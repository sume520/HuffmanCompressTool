#pragma once
#include "huffman.h"
#include <iostream>
using namespace std;

bool greater(HaffNode &a, HaffNode &b);
bool OpenFile(char *filename, HuffTree &huffmanlist); //Õ≥º∆weight
void getTypeName(char *str, char *&typeName);
bool Compress(HuffTree hfm, char*filename, char *htfname);//—πÀı
bool Extract(char*in,string extfile); //Ω‚—πÀı
void Init();
