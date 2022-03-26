#ifndef LinkList_h
#define LinkList_h

#include"denGiaoThong.h"


// Định nghĩa các giá trị.
#define maxCharList           8             // Số kí tự lớn nhất có thể có trong 1 list bao gồm cả kí tự NULL.

// Tạo node cho danh sách dữ liệu chứa các list[i] và địa chỉ của các list.
struct Node{
    char list[maxCharList];                 // Chuỗi các kí có trong 1 list.
    struct Node* next;                      // next là con trỏ trong list chứa địa chỉ của list tiếp theo.                        
};
typedef struct Node* LinkList;              // LinkList là con trỏ chứa địa chỉ của list đầu tiên
LinkList List;                              // con trỏ List dùng để truy xuất dữ liệu trên danh sách dữ liệu (dữ liệu là các list có trong file list2)
struct Node* P_LinkList;                    // con trỏ luôn luôn chứa địa chỉ của node đầu tiên

LinkList List_Temp;                         // list này để xử lý từng network trong chương trình LADDER
LinkList P_LinkList_Temp;                   // con trỏ luôn luôn chứa địa chỉ của node đầu đầu tiên của list_temp

LinkList List_Out;                          // List_Out sẽ lưu các chuỗi kí tự để in vào chương trình main.c
LinkList P_Link_Out;           

LinkList List_Temp2;                        // list này để xử lý với các lệnh ED, EU, TON, TOF, CTU, CTUD,  
LinkList P_LinkList_Temp2; 

LinkList List_Temp3;                        // list này để xử lý với các lệnh ED, EU, TON, TOF, CTU, CTUD,  
LinkList P_LinkList_Temp3; 



int isEmpty(LinkList list);                                            // Kiểm tra xem con trỏ list có trống hay không 
bool isLastList(LinkList list, int a);                                 // Kiểm tra xem vị trí N có phải vị trí cuối cùng của danh sách dữ liệu hay không
bool isLastListUpd(LinkList list, LinkList addr, int a);               // Kiểm tra xem vị trí N có phải vị trí cuối cùng của danh sách dữ liệu hay không, hàm đã được update
void addNewFirstNode(LinkList *first, char *list);                     // Chèn thêm node mới vào đầu danh sách dữ liệu
void addFirstNode(LinkList *head, LinkList* addrNode, char *listData); // Chèn thêm node mới vào đầu danh sách dữ liệu, cập nhập thêm tham trị cho địa chỉ của node đầu tiên
void addNewLastNode(LinkList *last, char *listData);                   // chèn thêm node mới vào cuối danh sách dữ liệu
void addLastNode(LinkList *last, LinkList *addr, char *listData);      // chèn thêm node mới vào cuối danh sách dữ liệu bản update
int addNodeAnyLoca(LinkList *NodeAny, char *listData, int a);          // Chèn thêm node vào vị trí bất kì trong danh sách dữ liệu
int addNodeAnyLocaUpd(LinkList *NodeAny, LinkList *addr, char *listData, int a);   // Chèn thêm node vào vị trí bất kì trong danh sách dữ liệu, hàm đã được update 
struct Node* addrListBack(LinkList list, int a, int posList);          // Hàm trả về địa chỉ của Node khi lùi a node
struct Node* addrListGo(LinkList list, int a);                         // hàm trả về địa chỉ của Node khi tiến a Node
void insertBeforNode(LinkList *nodeNew,char *listData, int n);         // insert a new node any position in the data list, n is new node position
int countList(LinkList List);                                          // hàm trả về số list có trong danh sách dữ liệu   
int countCharLists(LinkList list);                                     // hàm đếm tổng số kí tự của các list có trong danh sách dữ liệu
bool checkEqualNode(LinkList data, int posCurrent);// hàm kiểm tra xem với node hiện tại của danh sách dữ liệu thì trước đó đã tồn tại dữ liệu mà giống với dữ liệu của node hiện tại hay không
int freeAllNode(LinkList *data);
int check_2_dau_ngoac(LinkList data);              // hàm kiểm tra xem trong list x đã từng có hai node liền nhau lần lượt có các list[i] = "(" và List[i+1] = ")", hàm trả về vị trí dấu "("
int posNode_xLD(LinkList data);                    // tìm vị trí node để chèn dấu "*" và "+" cho các câu lệnh <ALD>, <OLD>
int posNode_xLD2(LinkList data);
int number_dau(LinkList data);
int numberChar_InList(LinkList data, char *x);                         // hàm tìm xem trong list có bao nhiêu chuỗi char* x lặp lại
void coppy_List(LinkList *target, LinkList source, LinkList *addr_taget, int position); // hàm coppy danh sách dữ liệu source bắt đầu từ vị trí được tham chiếu cho tới hết danh
                                                                                        // sách liên kết source vào danh sách liên kết target(target ban đầu là danh sách rỗng)
void clear_Nodes(LinkList *data, LinkList *addr, int position);                         // hàm xóa các node trong danh sách dữ liệu từ vị trí bất kí cho tới hết danh sách dữ liệu



#endif 


