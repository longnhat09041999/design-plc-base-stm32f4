#include"LinkList.h"

// Kiểm tra xem con trỏ list có trống hay không 
int isEmpty(LinkList list){
    return list == NULL;
}

// Kiểm tra xem vị trí N có phải vị trí cuối cùng của danh sách dữ liệu hay không
bool isLastList(LinkList list, int a){
    int i = 0;
    while(i <= a){                        // Duyệt vị trí 0 đến vị trí a của danh sách dữ liệu
        if(i == a){                       // khi đang ở vị trí cần kiểm tra thì kiểm tra xem địa chỉ trong node có bằng NULL hay không
            if(list -> next == NULL){
                return true;              // Trả về true nếu đây là node cuối cùng
            }
            else{
                return false;             // Trả về false nếu đâu không phải node cuối cùng
            }
        }
        list = list->next;
        i++;
    }
}

// Kiểm tra xem vị trí N có phải vị trí cuối cùng của danh sách dữ liệu hay không, hàm đã được update
bool isLastListUpd(LinkList list, LinkList addr, int a){
    list = addr;
    int i = 0;
    while(i <= a){                        // Duyệt vị trí 0 đến vị trí a của danh sách dữ liệu
        if(i == a){                       // khi đang ở vị trí cần kiểm tra thì kiểm tra xem địa chỉ trong node có bằng NULL hay không
            if(list -> next == NULL){
                return true;              // Trả về true nếu đây là node cuối cùng
            }
            else{
                return false;             // Trả về false nếu đâu không phải node cuối cùng
            }
        }
        list = list->next;
        i++;
    }
}

// Chèn thêm node mới vào đầu danh sách dữ liệu 1
void addNewFirstNode(LinkList *head, char *listData){
    P_LinkList = NULL;
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));   // Khai báo con trỏ Node trung gian để tiến hành gán đến con trỏ Node dùng cho toàn bộ chương trình
    
    int lenghList = strlen(listData);                 // Độ dài của chuỗi cần truyền vào đầu danh sách dữ liệu
    for(int i = 0; i <= lenghList; i++){              // truyền chuỗi dữ liệu vào con trỏ Node tạm
        p -> list[i] = listData[i];                   
    }
    p -> next = NULL;            // Giá trị của con trỏ next đầu Node không chứa địa chỉ của node tiếp theo nên giá trị bằng NULL

    if(isEmpty(*head)){          // Nếu trong đầu danh sách dữ liệu chưa được cấp phép 
        *head = p;               // Cấp phép địa chỉ của node đầu tiên cho con trỏ node, lúc này danh sách dữ liệu là đơn, không chứa con trỏ trỏ tới node tiếp theo
    }
    else{                        // Cho node đầu của danh sách dữ liệu thành node 2, và chuyển node mới vào danh sách dữ liệu
        p -> next = *head;       // gán giá trị con trỏ next của node mới (là địa chỉ của node đầu cũ) 
        *head = p;               // Cấp phép địa chỉ của node đầu tiên cho con trỏ node
    }
    P_LinkList = *head;          // Cập nhật địa chỉ của node đầu tiên  
}

// hàm chèn thêm node mới vào đầu danh sách dữ liệu có cập nhật thêm tham trị cho địa chỉ đầu tiên của danh sách dữ liệu
void addFirstNode(LinkList *head, LinkList* addrNode, char *listData){
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));   // Khai báo con trỏ Node trung gian để tiến hành gán đến con trỏ Node dùng cho toàn bộ chương trình
    
    int lenghList = strlen(listData);                 // Độ dài của chuỗi cần truyền vào đầu danh sách dữ liệu
    for(int i = 0; i <= lenghList; i++){              // truyền chuỗi dữ liệu vào con trỏ Node tạm
        p -> list[i] = listData[i];                   
    }
    p -> next = NULL;            // Giá trị của con trỏ next đầu Node không chứa địa chỉ của node tiếp theo nên giá trị bằng NULL

    if(isEmpty(*head)){          // Nếu trong đầu danh sách dữ liệu chưa được cấp phép 
        *head = p;               // Cấp phép địa chỉ của node đầu tiên cho con trỏ node, lúc này danh sách dữ liệu là đơn, không chứa con trỏ trỏ tới node tiếp theo
    }
    else{                        // Cho node đầu của danh sách dữ liệu thành node 2, và chuyển node mới vào danh sách dữ liệu
        p -> next = *head;       // gán giá trị con trỏ next của node mới (là địa chỉ của node đầu cũ) 
        *head = p;               // Cấp phép địa chỉ của node đầu tiên cho con trỏ node
    }
    (*addrNode) = (*head);       // Cập nhật địa chỉ của node đầu tiên  
}

// chèn thêm node mới vào cuối danh sách dữ liệu
void addNewLastNode(LinkList *last, char *listData){
    struct Node* firstNode = (*last);                 // Lấy địa chỉ của node đầu tiên 
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));             // Khai báo con trỏ trung gian để gán cho địa chỉ cho node cuối của danh sách dữ liệu
    int lenghList = strlen(listData);                 // Độ dài của chuỗi cần truyền vào đầu danh sách dữ liệu
    for(int i = 0; i <= lenghList; i++){              // truyền chuỗi dữ liệu vào con trỏ Node tạm
        p -> list[i] = listData[i];                   
    }
    p -> next = NULL;                           // Giá trị của con trỏ next đầu Node không chứa địa chỉ của node tiếp theo nên giá trị bằng NULL
    while(1){                                   // Duyệt cho đến khi nào đến cuối danh sách dữ liệu thì thêm node mới   
        if((*last) -> next == NULL){            // Khi bằng Null, tức là giá trị của con trỏ chỉ node là địa chỉ của node cuối cùng 
            (*last) -> next = p;                // Gán địa của node mới được thêm vào
            break;                              // Thoát khỏi vòng lặp while
        }
        *last = (*last) -> next;                // Giá trị của con trỏ Node chứa địa chỉ của node tiếp theo
    }

    *last = firstNode;             // Gán lại địa chỉ cho node đầu tiên sau quá trình xử lý
}

// chèn thêm node mới vào cuối danh sách dữ liệu, hàm này được update từ hàm cũ
void addLastNode(LinkList *last, LinkList *addr, char *listData){
    if(isEmpty(*last)){
        addFirstNode(last, addr, listData); 
    }
    else{
        addNewLastNode(last, listData);
    }  
}

// Chèn vào vị trí bất kì trong danh sách dữ liệu
int addNodeAnyLoca(LinkList *NodeAny, char *listData, int a){
    struct Node* firstNode = (*NodeAny);                 // Lấy địa chỉ của node đầu tiên 
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));         // Khai báo con trỏ node trung gian để chèn thêm node được chỉ định vị trí cụ thể
    int lenghList = strlen(listData);                 // Độ dài của chuỗi cần truyền vào đầu danh sách dữ liệu
    for(int i = 0; i <= lenghList; i++){              // truyền chuỗi dữ liệu vào con trỏ Node tạm
        p -> list[i] = listData[i];                   
    }
    p -> next = NULL; 

    if(a == 0){                                                         // Nếu vị trí chèn là 0 thì tiến hành chèn node ở vị trí đầu tiên
        addNewFirstNode(NodeAny, listData);
        return 1;
    }
    else{
        int i = 0;
        while((*NodeAny) -> list != NULL){                   // Kiểm tra xem trong Node ở hiện tại thì dữ liệu của list có khác bằng NULL hay không  
            if(i == a - 1){                                  // Nếu đây là vị trí node sau vị trí node cần chèn
                if(isLastList(*NodeAny, i) == false){        // Nếu đây không phải là node cuối cùng
                    p -> next = (*NodeAny) -> next;          // con trỏ next của p được gán bằng địa chỉ của NodeAny tiếp theo
                    (*NodeAny) -> next = p;                  // hoàn thành quá trình gán
                    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
                    return 1;
                }
                else{
                    (*NodeAny) -> next = p;
                    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
                    return 1;
                }
            }
            else{
                *NodeAny = (*NodeAny) -> next;
                i++;
            }
        }
    }
    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
    return 0;
}

// chèn vào vị trí bất kì trong danh sách dữ liệu bản update, có thêm tham trị cho địa chỉ của node đầu tiên
int addNodeAnyLocaUpd(LinkList *NodeAny, LinkList *addr, char *listData, int a){
    struct Node* firstNode = (*NodeAny);                 // Lấy địa chỉ của node đầu tiên 
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));         // Khai báo con trỏ node trung gian để chèn thêm node được chỉ định vị trí cụ thể
    int lenghList = strlen(listData);                 // Độ dài của chuỗi cần truyền vào đầu danh sách dữ liệu
    for(int i = 0; i <= lenghList; i++){              // truyền chuỗi dữ liệu vào con trỏ Node tạm
        p -> list[i] = listData[i];                   
    }
    p -> next = NULL; 

    if(a == 0){                                                         // Nếu vị trí chèn là 0 thì tiến hành chèn node ở vị trí đầu tiên
        addFirstNode(NodeAny, addr, listData);
        return 1;
    }
    else{
        int i = 0;
        while((*NodeAny) -> list != NULL){                   // Kiểm tra xem trong Node ở hiện tại thì dữ liệu của list có khác bằng NULL hay không  
            if(i == a - 1){                                  // Nếu đây là vị trí node sau vị trí node cần chèn
                if(isLastListUpd(*NodeAny, *addr, i) == false){        // Nếu đây không phải là node cuối cùng
                    p -> next = (*NodeAny) -> next;          // con trỏ next của p được gán bằng địa chỉ của NodeAny tiếp theo
                    (*NodeAny) -> next = p;                  // hoàn thành quá trình gán
                    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
                    return 1;
                }
                else{
                    (*NodeAny) -> next = p;
                    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
                    return 1;
                }
            }
            else{
                *NodeAny = (*NodeAny) -> next;
                i++;
            }
        }
    }
    *NodeAny = firstNode;                    // trả lại giá trị địa chỉ của node đầu tiên cho con trỏ trỏ đến node
    return 0;
}

// Hàm trả về địa chỉ của Node khi lùi a node, i là vị trí node hiện tại
struct Node* addrListBack(LinkList list, int a, int posList){
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));         // Khai báo con trỏ node trung gian để 
    list = P_LinkList;                                                  // gán địa chỉ của node đầu tiên cho con trỏ list
    for(int i = 0; i < posList - a; i ++){
        list = (list->next);
        if(i == posList - a - 1){
            p = list;
            break;
        }
    }
    return p;
}

// hàm trả về địa chỉ của node khi tiến a node
struct Node* addrListGo(LinkList list, int a){
    for(int i = 0; i < a; i++){
        list = list->next;
    }
    return list;
}

// hàm trả về số list có trong danh sách dữ liệu 
int countList(LinkList list){
    int i = 0;                          // khởi tạo số list  = 0
    while(list != NULL){
        list = list->next;
        i++;
    }
    return i;                           // trả về tổng số list trong danh sách dữ liệu
}
// hàm đếm tổng số kí tự của các list có trong danh sách dữ liệu, dữ liệu của các list là 1 chuỗi kí tự
int countCharLists(LinkList list){
    int i = 0;                          // Khởi tạo số kí tự của tổng số các list là 0
    while(list != NULL){
        i += strlen(list->list);        // Cộng dồn số kí tự các list lại với nhau
        list = list->next;
    }
    return i;                           // Trả về tổng số kí tự có trong danh sách dữ liệu
}

// hàm kiểm tra xem với node hiện tại của danh sách dữ liệu thì trước đó đã tồn tại dữ liệu mà giống với dữ liệu của node hiện tại hay không
bool checkEqualNode(LinkList data, int posCurrent){
    char temp[6];

    for(int i = 0; i < posCurrent; i++){         // đưa con trỏ của danh sách dữ liệu dừng tại node thứ posCurrent  
        data->next;
    }

    strcpy(temp, data->list);                    // coppy dữ liệu trong node hiện tại vào biến temp[]

    data = P_LinkList;                           // đưa vị trí node hiện tại về node đầu tiên

    for(int i = 0; i < posCurrent; i++){        
         if(strcmp(temp, data->list) == 0){
             return true;                        // trả về <true> nếu dữ liệu trong các node trước đo đã tồn tại
         }
         data = data->next;         
    }
    return false;
}

// giải phóng tất cả node của List
int freeAllNode(LinkList *data){
    if((*data)->next != NULL){
        struct Node* x;
        x = (*data);
        (*data) = (*data)->next;
        free(x);
       
        return freeAllNode(data);
    }

    if((*data)->next == NULL){
        free(*data);
        (*data) = NULL;
        return 1;
    }
}

// hàm kiểm tra xem trong list x đã từng có hai node liền nhau lần lượt có các list[i] = ")" và List[i+1] = "(", hàm trả về vị trí List[i] = ")"
int check_2_dau_ngoac(LinkList data){     

    int x = countList(data);               // số lượng list có trong danh sách liên kết data

    for(int i = 0; i < x - 1; i++){
        if((strcmp(data->list, ")\0") == 0) && (strcmp(addrListGo(data, 1)->list, "(\0") == 0)){
                return i;
        }
        data = data->next;
    }

    return 0;         // không hai node liền nhau là 2 dấu ngoặc
}

// tìm vị trí node để chèn dấu "*" và "+" cho các câu lệnh <ALD>, <OLD>
// hay nếu trong danh sách liên kết có nhiều lần xuất hiện 2 node liền kề lần lượt là các dấu ")" và "(" 
// thì hàm sẽ trả về vị trí của dấu ")" của lần cuối
int posNode_xLD(LinkList data){
    int pos = 0;
    int x = countList(data);               // số lượng list có trong danh sách liên kết data

    for(int i = 0; i < x - 1; i++){
        if(data->next != NULL){
            if((strcmp(data->list, ")\0") == 0) && (strcmp(addrListGo(data, 1)->list, "(\0") == 0)){
                pos = i;
            }
            data = data->next;
        }
    }
    return pos;
}

// hàm tìm xem vị trí của dấu ")" nếu trong danh sách liên kết có các kí tự liền kề lần lượt là ) + ( hay ) * (  các kí tự này là vị trí cuối của danh sách liên kiết
int posNode_xLD2(LinkList data){
    int pos = 0;
    int x = countList(data);               // số lượng list có trong danh sách liên kết data

    for(int i = 0; i < x - 1; i++){
        if((data->next)->next != NULL){
            if((strcmp(data->list, ")\0") == 0) && (strcmp(addrListGo(data, 2)->list, "(\0") == 0)){
                pos = i;
            }
            data = data->next;
        }
    }
    return pos;
}

// đếm số lần xuất hiện các node liên tiếp )(
int number_dau(LinkList data){
    int number = 0;
    int x = countList(data);

    for(int i = 0; i < x - 1; i++){
        if((data->next)->next != NULL){
            if((strcmp(data->list, ")\0") == 0) && (strcmp(addrListGo(data, 1)->list, "(\0") == 0)){
                number++;
            }
            data = data->next;
        }
    }
    return number;
}



// hàm tìm xem trong list có bao nhiêu chuỗi char* x lặp lại
int numberChar_InList(LinkList data, char *x){
    int number_List = countList(data);                              // số list có trong danh sách dữ liệu
    int temp = 0;                                                      // số lần xuất hiện chuỗi x trong danh sách dữ liệu
    for(int i = 0; i < number_List; i++){
        if(strcmp(x, data->list) == 0){
            temp++;
        }
        data = data->next;
    }
    return temp;
}



// hàm coppy danh sách dữ liệu source bắt đầu từ vị trí được tham chiếu cho tới hết danh
// sách liên kết source vào danh sách liên kết target(target ban đầu là danh sách rỗng)
void coppy_List(LinkList *target, LinkList source, LinkList *addr_taget, int position){
    int number_List = countList(source);  
    for(int i = 0; i < position; i++){
        source = source->next;
    }

    while(source->next != NULL){
        addLastNode(target, addr_taget, source->list);
        source = source->next;
    }
    addLastNode(target, addr_taget, source->list);
}

// hàm xóa các node trong danh sách dữ liệu từ vị trí bất kí cho tới hết danh sách dữ liệu
void clear_Nodes(LinkList *data, LinkList *addr, int position){
    if(position == 0){
        freeAllNode(data);       // xóa hết tất cả các node
    }

    // xóa hết tất cả các node bắt đầu từ vị trí định sẵn
    else if(position != 0){
        for(int i = 0; i < position; i++){
            (*data) = (*data)->next;
        }
        freeAllNode(data);

        // thêm địa chỉ bằng NULL cho node cuối cùng của danh sách dữ liệu đã bị xóa
        (*data) = (*addr);
        for(int i = 0; i < position - 1; i++){
            (*data) = (*data)->next;
        }
        (*data)->next = NULL;
        (*data) = (*addr);
    }  
}

// hàm kiểm tra xem các node trong danh sách dữ liệu có phải các biến vào, ra, trung gian đảo hay không

