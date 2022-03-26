#include"HashTable.h"


int isEmptyNode1(struct Node1* firstNode){
    return firstNode == NULL;
}


// thêm vào đầu danh sách dữ liệu là một index của bảng dữ liệu 
void addFirstNode1(Hash* firstNode, struct Node1 data, addrN*addrFirst, int posIndex){
    struct Node1* p = (struct Node1*)malloc(sizeof(struct Node1*));
    // gán các giá trị cho con trỏ p
    strcpy(p->name, data.name);
    strcpy(p->type, data.type);
    p->preValue = data.preValue;
    p->resol = data.resol;
    p->next = NULL;

    if(isEmptyNode1((*firstNode)[posIndex]) == 1){
        (*firstNode)[posIndex] = p;
    }
    else{
        p->next = (*firstNode)[posIndex];
        (*firstNode)[posIndex] = p;
    }

    (*addrFirst)[posIndex] = p;
}

// thêm vào đầu danh sách dữ liệu là một index của bảng dữ liệu 
void addLastNode1(Hash* firstNode, struct Node1 data, addrN addrFirst, int posIndex){
    struct Node1* p = (struct Node1*)malloc(sizeof(struct Node1*));
    // gán các giá trị cho con trỏ p
    strcpy(p->name, data.name);
    p->preValue = data.preValue;
    p->resol = data.resol;
    p->next = NULL;

    while(1){
        if((*firstNode)[posIndex]->next == NULL){
            (*firstNode)[posIndex]->next = p;
            break;
        }
        (*firstNode)[posIndex] = (*firstNode)[posIndex]->next;
    }
    (*firstNode)[posIndex] = addrFirst[posIndex];
}

// hàm biến chuỗi kí tự thành số
int hashFunction(char *name){
    char temp[5];
    for(int i = 0; i < strlen(name); i++){
        temp[i] = name[i+1];
    }

    return (atoi(temp));
}