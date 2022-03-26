#ifndef HashTable_h_
#define HashTable_h_

#include "denGiaoThong.h"


#define numberBuckets     256     // số lượng các buckets cho bảng dữ liệu của các bộ timer, counter

// Tạo các Node cho danh sách dữ liệu chứa dữ liệu là tên counter hay timer, giá trị preset cho mỗi bộ, và độ phân giải cho các bộ timer tương thích với phần mềm microWin
struct Node1{
    char name[5];
    char type[5];
    int preValue;
    int resol;                              // độ phân giải chỉ dành cho các bộ timer
    struct Node1*next;                 
};
typedef struct Node1** Hash;      // con trỏ mẹ cỏ thể trỏ tới 1 mảng chứa giá trị là các địa chỉ của 
Hash HashMap;                     // node đầu tiên của 1 danh sách dữ liệu, 1 danh sách dữ liệu gồm các node chứa dữ liệu của các bộ couter hay timer

typedef struct Node1** addrN;     // tạo 1 mảng luôn chứa đại đầu tiên của các Node của bảng dữ liệu
addrN addrNode1s; 

void addFirstNode1(Hash* firstNode, struct Node1 data, addrN* addrFirst, int posIndex);
void addLastNode1(Hash* firstNode, struct Node1 data, addrN addrFirst, int posIndex);
int hashFunction(char *name);

#endif