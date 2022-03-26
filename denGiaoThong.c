#include"denGiaoThong.h"            
#include"LinkList.h"
#include"xuLyFile.h"
#include"HashTable.h"

/*==================================================================================================================================================


 ===============             ===              ===                    ==============              ===         ===
 ===============          ===    ===     ===     ===                 ================            ===       ===
       ===               ===       =======        ===                ===            ===          ===     ===
       ===               ===         ==           ===                ===           ===           ===   ===
       ===                ===                    ===                 ===         ===             === ===
       ===                  ===                 ===                  ===      ====               === ===
       ===                    ===             ===                    ===         ===             ===   ===
       ===                      ===         ===                      ===           ===           ===     ===
       ===                        ===     ===                        ===            ===          ===       ===
================                   ===   ===                         ================            ===         ===
================                      ===                            ==============              ===           ===


====================================================================================================================================================*/



int rowNumberTxt;                        // số dòng trong file txt
int rowNumberStl;                        // số dòng trong file stl

/*==================================================================================================================================================
Hàm main, hàm xử lý chính (bộ não của chương trình), code ra kết quả như không biết tại sao nó lại ra :)))))     
====================================================================================================================================================*/
int main()
{
    int numberList = 0;                      // số list[i] có trong file list
    int numberCharList = 13;                 // số kí tự có trong file list

    /*---------------------------------------------------------------convert file.txt thành file STL------------------------------------------------------------------------------------*/

    FILE *FReadTxt  = fopen(fileTxt,"r");                      // mở file cần chuyển đổi (chỉ đọc)
    FILE *FWriteStl = fopen(fileStl,"w");                      // mở file sau khi chuyển đổi (chỉ ghi)
    
    char n[3] = "N\n\0";                                       // kí tự N để thay thế cho kết thúc của 1 network
    
    rowNumberTxt = countRowNumber(fileTxt);                    // số dòng của file cần chuyển đổi
    int count = 0;
    char command[50];                                        
     
    while(count <= rowNumberTxt){
        readCommandInfile(FReadTxt,command, 40);                // đọc một câu lệnh từ file cần chuyển đổi.
        if(stlCommandType(command) == true){                    // nếu là 1 câu lệnh của STL thì
            writeCommandInFile(FWriteStl,command);              // viết lệnh này vào file cần lưu
        }
        else {
            if(endNetWorkCommand(command) == true){             // nếu là câu câu kết thúc 1 network thì
                writeCommandInFile(FWriteStl, n);               // viết kí tự N sau network để báo hiệu kết thúc 1 network
            }
        }
        count++;
    }         
    count = 0;
    fclose(FReadTxt);                                           // đóng file cần chuyển đổi  
    fclose(FWriteStl);                                          // đóng file sau khi chuyển đổi

    /*----------------------------------------------------------------convert file STL thành file lIST giai đoạn 1 -------------------------------------------------------------------*/ 
    

    FILE *FReadStl = fopen(fileStl,"r");                             // mở file stl (chỉ đọc)
    FILE *FWriteList1 = fopen(fileList1, "w");

    elementCommandStlLine_t *p_element;
    p_element = &element;

    rowNumberStl = countRowNumber(fileStl);                      // số dòng của file stl
    int commandType;

    /* bắt đầu in các câu lệnh vào file list 
       dạng dòng lệnh trong file list có dạng:

       listcodebd = ['LD','I0.0','TON','T97',',','1000','N','LDN','T97','=','Q0.0','N']
    */     
    writeCommandInFile(FWriteList1, "listcodebd = [\0");                  // ghi chuỗi kí tự <listcodebd = [> vào file list
    while(count < rowNumberStl){
        readCommandInfile(FReadStl, command, 40);                         // đọc một câu lệnh trong file stl
        commandType = dissectionCommandStl(command, p_element);           // bóc tách các thành của 1 dòng lệnh 
        if(commandType == 0){ 
            add_Number_CharList(element, &numberCharList, commandType);   // cộng dồn số kí tự có trong file list                                                        
            write1Type(FWriteList1, element, count, rowNumberStl - 1);    // nếu giá trị trả về là 0 thì dòng lệnh dạng <N> và tiến hành ghi vào file list
            numberList += 1;                                              // cộng dồn số list[] có trong file list
        }                                                              
        else if(commandType == 1){
            add_Number_CharList(element, &numberCharList, commandType);   // cộng dồn số kí tự có trong file list
            write2Type(FWriteList1, element);                             // nếu giá trị trả về là 1 thì dòng lệnh dạng <TON    T38, 100> và tiến hành ghi vào file list       
            numberList += 4;                                              // số list cộng thêm 4
 
        }                                                              
        else {
            add_Number_CharList(element, &numberCharList, commandType);   // cộng dồn số kí tự có trong file list
            write3Type(FWriteList1, element);                             // nếu giá trị trả về là 1 thì dòng lệnh dạng <AN     T38> và tiến hành ghi vào file list
            numberList += 2;                                              //số list cộng thêm 2
        } 
        count++;                                                           
    }
    count = 0;
    writeCommandInFile(FWriteList1,"]\0");                              

    fclose(FReadStl);                                            // đóng file stl
    fclose(FWriteList1);                                         // đóng file list

/*----------------------------------------------------------------------convert file STL thành file lIST giai đoạn 2 --------------------------------------------------------------*/ 
/*
   - chuyển các kí tự '.' thành các kí tự '_'
   - thêm kí tự 'H' vào tiếp điểm thường đóng để phân biệt tiếp điểm thường đóng và tiếp điểm thường mở 
   - file list sau khi chuyển đổi có dạng dòng lệnh như sau: 

         listcode = ['LD', 'I0_0', 'TON', 'T97', '1000', 'N', 'LDN', 'T97H', '=', 'Q0_0', 'N']
*/

    FILE *FReadList1 = fopen(fileList1, "r");
    FILE *FWriteList2 = fopen(fileList2, "w");

    char *listLine; 

    listLine = malloc(sizeof(char) * (numberCharList + 1));
    readCommandInfile(FReadList1, listLine, numberCharList + 1);    // đọc dòng lệnh trong file list đã được chuyển đổi ở giai đoạn 1 vào chuỗi list 
    int lenght_Line = strlen(listLine);

    // tạo con trỏ trong mảng 2 chiều (mảng hai chiều để lưu các list[i]) đồng thời do số lượng list[i] quá nhiều 
    // và do không biết chính xác số list là bao nhiêu nên chúng ta sẽ cấp phát động lưu địa chỉ vào một con trỏ của con trỏ

    char **p_ArrayTowDirec;                                         // tạo con trỏ trỏ tới con trỏ 
                                                                    // con trỏ mẹ này chứa dữ liệu là địa chỉ đầu tiên của chuỗi dữ liệu
                                                                    // có dữ liệu là địa chỉ của các hàng trong mảng 2 chiều

     p_ArrayTowDirec = malloc(sizeof(int*) * numberList);           // cấp phát bộ nhớ động cho một mảng mẹ gồm numberList phần tử
                                                                    // hàm malloc() thực hiện tự động cấp phép 1 mảng dữ liệu đã quy định
                                                                    // hàm malloc() trả về giá trị địa chỉ đầu của mảng dữ liệu và lưu vào con trỏ mẹ P_ArrayTowDirec

    for(int i = 0; i < numberList; i++){                            // cấp phát bộ nhớ động cho numberList mảng con, mỗi mảng gồm  maxCharList phần tử
        p_ArrayTowDirec[i] = malloc(sizeof(char) * maxCharList);
    }
    
    save_into_lists(p_ArrayTowDirec, numberList, listLine);         // lưu các câu lệnh, biến hay thời gian vào các list
    change_List(p_ArrayTowDirec, numberList);                       // thay đổi các kí tự cần thiết trong các list
    save_Into_File_List(FWriteList2, p_ArrayTowDirec, numberList);  // lưu các list vào file list

    fclose(FReadList1);                                             // đóng file stl
    fclose(FWriteList2);                                            // đóng file list

/*-------------------------Đưa data trong dữ liệu của mảng 2 chiều được truy cập bằng con trỏ mẹ (p_ArrayTowDirec) vào danh sách dữ liệu LinkList---------------------------------------*/

// Chúng ta tiến hành lưu các element theo thứ  tự vào danh sách dữ liệu (ngoại trừ dấu phẩy)
    List = NULL;
    count = 0;

    for(int j = 0; j < numberList; j++){
        if(strcmp(p_ArrayTowDirec[j], ",\0") != 0 ){
            if(List == NULL){                                   // if the list is empty
                addNewFirstNode(&List, p_ArrayTowDirec[j]);     // chèn note đầu tiên vào danh sách liên kết 
            }
            else{
                addNewLastNode(&List, p_ArrayTowDirec[j]);      // Chèn note vào cuối danh sách liên kết
            }          
        }
        else{
            continue;
        }
    }

/*--------------------------------------------------------------convert file STL thành file lIST giai đoạn 3 ------------------------------------------------------------------------*/ 

/*   Tách các nhánh bằng dấu <(>, <)>

*/

    FILE *FWriteList3 = fopen(fileList3, "w");           // tạo file để lưu list sau khi xử lý
    fprintf((FILE*)FWriteList3, "listxl = [");
    int number_nguac_phai = 0;                           // số lượng dấu ngoặc tròn phải
    int number_nguac_trai = 0;                           // số lượng dấu ngoặc tròn trái
    count = 0;
    while(List->next != NULL){
        if( (strcmp(List->list, "ALD\0") == 0)  || 
            (strcmp(List->list, "OLD\0") == 0)  ||
            (strcmp(List->list, "TON\0") == 0)  || 
            (strcmp(List->list, "TOF\0") == 0)  ||
            (strcmp(List->list, "CTUD\0") == 0) ||
            (strcmp(List->list, "CTU\0") == 0))
        {   
            if(number_nguac_phai > number_nguac_trai){
                fprintf((FILE*)FWriteList3, "')',");
                number_nguac_trai++;
            }

            fprintf((FILE*)FWriteList3, "'%s',", List);
            List = List->next;
            count++;
            continue;
        }
        else{
            if(strcmp(List->list, "=\0") == 0){
                if(number_nguac_phai > number_nguac_trai){
                    fprintf((FILE*)FWriteList3, "')',");
                    number_nguac_trai++;
                }
                else{
                    fprintf((FILE*)FWriteList3, "'%s',", List);
                    List = List->next;
                    count++;
                    continue;
                }
            }
            else{
                if( (strcmp(List->list, "LD\0") == 0) || (strcmp(List->list, "LDN\0") == 0)){
                    if((count == 0) || 
                        (strcmp(addrListBack(List, 2, count)->list, "=\0") == 0)   || 
                        (strcmp(addrListBack(List, 1, count)->list, "N\0") == 0)   ||
                        (strcmp(addrListBack(List, 1, count)->list, "ALD\0") == 0) ||    
                        (strcmp(addrListBack(List, 1, count)->list, "OLD\0") == 0) ){
                            fprintf((FILE*)FWriteList3, "'(',");
                            number_nguac_phai++;
                            fprintf((FILE*)FWriteList3, "'%s',", List);
                            List = List->next;
                            count++;
                            continue;
                    }
                    else{
                        if(number_nguac_phai == number_nguac_trai){
                            fprintf((FILE*)FWriteList3, "'(',");
                            number_nguac_phai++;
                            fprintf((FILE*)FWriteList3, "'%s',", List);
                            List = List->next;
                            count++;
                            continue;
                        }
                        else{
                            fprintf((FILE*)FWriteList3, "')',");
                            number_nguac_trai++;
                            fprintf((FILE*)FWriteList3, "'(',");
                            number_nguac_phai++;
                            fprintf((FILE*)FWriteList3, "'%s',", List);
                            List = List->next;
                            count++;
                            continue;
                        }
                    }
                }
                else{
                    fprintf((FILE*)FWriteList3, "'%s',", List);
                    List = List->next;
                    count++;
                }
            }
        }
    }
    fprintf((FILE*)FWriteList3, "'N']");
    List = P_LinkList;            // trả về địa chỉ node đầu tiên cho con trỏ List(là con trỏ chứa giá trị là địa chỉ của node đầu tiên)

    fclose(FWriteList3);

/*--------------------------------------Lưu các list ở file list3 vào mảng một chiều (bao gồm cả dấu ngoặc trái tròn và dấu ngoặc phải tròn )-------------------------------------------*/ 

    FILE *FReadList3 = fopen(fileList3, "r");     // mở fileList3 để tiến hành đọc dữ liệu, mục đính chính ở đây là lưu các list vào danh sách dữ liệu để chuẩn bị xử lý vào file .c

    // Đếm số kí tự có trong file list3
    int soKiTuList3 = (number_nguac_phai + number_nguac_trai)*4 + countCharLists(List) + countList(List)*3 + 13;

    char *p_FReadList3;                                            // con trỏ chứa địa chỉ của chuỗi kí tự (là chuỗi kí tự có trong file list3)

    p_FReadList3 = (char*)malloc(sizeof(char)*soKiTuList3);       // cấp phát mảng dữ liệu, địa chỉ đầu mảng là giá trị của con trỏ p_FReadList3

    readCommandInfile(FReadList3, p_FReadList3, soKiTuList3);      // lưu dòng lệnh trong file list3 vào chuỗi p_FReadList3

    char **p_List;                                                 // con trỏ mẹ chứa địa chỉ của mảng 2 chiều, là ma trận mảng chứa các list có trong file list3
    
    int soListTrongFileList3 = number_nguac_phai + number_nguac_trai + countList(List);       // Tổng số list có trong file list3 (bao gồm cả dấu ngoặc)
    p_List = (char**)malloc(sizeof(int*)*soListTrongFileList3);            // cấp phép cho con trỏ mẹ địa chỉ của mảng sẽ chứa địa chỉ chuỗi kí tự của ma trận mảng

    for(int i = 0; i < soListTrongFileList3; i++){                 // cấp phép dữ liệu cho ma trận mảng hai chiều
        p_List[i] = (char*)malloc(sizeof(char)*maxCharList);
    }
    
    save_into_lists(p_List, soListTrongFileList3, p_FReadList3);   // lưu dữ liệu vào mảng hai chiều 

    fclose(FReadList3);               // Close file list3 

/*---------------------------------------------------- lấy dữ liệu là các biến vào, ra, biến trung gian là các biến đảo (có kí tự H cuối cùng) -----------------------------------------------------------------------*/

























/*------------------------------------thay đổi dữ liệu trong các list trong danh sách dữ liệu có các biến đã thêm kí tự H ở trên thì ta sẽ xóa H đi ---------------------------------------------*/

while(List->next != NULL){
    if(checkCharH(List->list) == true){
        (List->list)[4] = '\0';                  // bỏ kí tự H có trong dữ liệu của danh sách liên kết 
    }
    List = List->next;
}
List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu


// tính số đầu vào ra hay các bộ timer, counter có trong chương trình LADDER
int so_dau_vao = numberInstruc(List, 'I');                  // số biến đầu vào
int so_dau_ra = numberInstruc(List, 'Q');                   // số biến đầu ra
int so_bien_trung_gian = numberInstruc(List, 'M');          // số biến trung gian
int so_Bo_timer = numberInstruc(List, 'T');                 // số bộ timer
int so_Bo_counter = numberInstruc(List, 'C');               // số bộ counter


/*------------------------------------------------cấp phép mảng hai chiều chứa dữ liệu là các kí tự của biến đầu vào ------------------------------------------------------------*/

char **p_adIn = (char**)malloc(sizeof(int*)*so_dau_vao);

for(int i = 0; i < so_dau_vao; i++){
    p_adIn[i] = (char*)malloc(sizeof(char)*5);
}

int posCurrentArray = 0;              // khởi tạo vị trí biến đầu vào đầu tiên
// đưa dữ liệu vào mảng hai chiều
while(List->next != NULL){
    if(checkArray2Dir(p_adIn, List->list, posCurrentArray) == false && checkVar(List->list, 'I') == true){
        strcpy(p_adIn[posCurrentArray], List->list);
        posCurrentArray++;
    }
    List = List->next;
}

// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
dispArray2dir(p_adIn, so_dau_vao);

List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu

/*------------------------------------------------cấp phép mảng hai chiều chứa dữ liệu là các kí tự của biến đầu ra ------------------------------------------------------------*/

char **p_adOut = (char**)malloc(sizeof(int*)*so_dau_ra);

for(int i = 0; i < so_dau_ra; i++){
    p_adOut[i] = (char*)malloc(sizeof(char)*5);
}

posCurrentArray = 0;                    // khởi tạo vị trí biến đầu ra đầu tiên
// đưa dữ liệu vào mảng hai chiều
while(List->next != NULL){
    if(checkArray2Dir(p_adOut, List->list, posCurrentArray) == false && checkVar(List->list, 'Q') == true){
        strcpy(p_adOut[posCurrentArray], List->list);
        posCurrentArray++;
    }
    List = List->next;
}

// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
dispArray2dir(p_adOut, so_dau_ra);
List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu

/*------------------------------------------------cấp phép mảng hai chiều chứa dữ liệu là các kí tự của biến trung gian------------------------------------------------------------*/

char **p_adTg = (char**)malloc(sizeof(int*)*so_bien_trung_gian);

for(int i = 0; i < so_bien_trung_gian; i++){
    p_adTg[i] = (char*)malloc(sizeof(char)*5);
}

posCurrentArray = 0;                    // khởi tạo vị trí biến đầu ra đầu tiên
// đưa dữ liệu vào mảng hai chiều
while(List->next != NULL){
    if(checkArray2Dir(p_adTg, List->list, posCurrentArray) == false && checkVar(List->list, 'M') == true){
        strcpy(p_adTg[posCurrentArray], List->list);
        posCurrentArray++;
    }
    List = List->next;
}

// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
dispArray2dir(p_adTg, so_bien_trung_gian);
List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu

/*----------------------------------------------------cấp phép mảng hai chiều chứa dữ liệu là các kí tự của các bộ timer-------------------------------------------------------------*/

char **p_adTm = (char**)malloc(sizeof(int*)*so_Bo_timer);

for(int i = 0; i < so_Bo_timer; i++){
    p_adTm[i] = (char*)malloc(sizeof(char)*5);
}

posCurrentArray = 0;                    // khởi tạo vị trí biến đầu ra đầu tiên

// đưa dữ liệu vào mảng hai chiều
while(List->next != NULL){

    if(checkArray2Dir(p_adTm, List->list, posCurrentArray) == false && checkVar(List->list, 'T') == true){
        strcpy(p_adTm[posCurrentArray], List->list);
        posCurrentArray++;
    }
    List = List->next;
}

// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
dispArray2dir(p_adTm, so_Bo_timer);
List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu

/*----------------------------------------------------cấp phép mảng hai chiều chứa dữ liệu là các kí tự của các bộ counter-------------------------------------------------------------*/

char **p_adCt = (char**)malloc(sizeof(int*)*so_Bo_counter);

for(int i = 0; i < so_Bo_counter; i++){
    p_adCt[i] = (char*)malloc(sizeof(char)*5);
}

posCurrentArray = 0;                    // khởi tạo vị trí biến đầu ra đầu tiên
// đưa dữ liệu vào mảng hai chiều
while(List->next != NULL){
    if(checkArray2Dir(p_adCt, List->list, posCurrentArray) == false && checkVar(List->list, 'C') == true){
        strcpy(p_adCt[posCurrentArray], List->list);
        posCurrentArray++;
    }
    List = List->next;
}
List = P_LinkList;  
// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
dispArray2dir(p_adCt, so_Bo_counter);
//List = P_LinkList;          // đưa con trỏ danh sách dữ liệu về vị trí ban đầu


/*-----------------------------------Viết vào file define.h trong IDESTM32 để khai báo chân, port, các biến vào ra, và các biến trung gian-----------------------------------------*/

    FILE *fileWriteDefineH = fopen(fileDefineH, "w");

    // ghi tên file vào để tránh trùng lặp ở file khác
    fprintf((FILE*)fileWriteDefineH, "#ifndef INC_CSDL_H_\n#define INC_CSDL_H_\n");

    // định nghĩa các pin đầu vào tương thích với vi điều khiển
    fprintf((FILE*)fileWriteDefineH, "\n// Định nghĩa các chân đầu vào\n");
    
    for(int i = 0; i < so_dau_vao; i++){
        fprintf((FILE*)fileWriteDefineH, "#define %s_PIN GPIO_PIN_\n", p_adIn[i]);
    }

    // định nghĩa các port đầu vào tương thích với vi điều khiển
    fprintf((FILE*)fileWriteDefineH, "\n// Định nghĩa các port đầu vào\n");
    
    for(int i = 0; i < so_dau_vao; i++){
        fprintf((FILE*)fileWriteDefineH, "#define %s_PORT GPIO\n", p_adIn[i]);
    }

    // định nghĩa các pin đầu ra tương thích với vi điều khiển
    fprintf((FILE*)fileWriteDefineH, "\n// Định nghĩa các chân đầu ra\n");
    
    for(int i = 0; i < so_dau_ra; i++){
        fprintf((FILE*)fileWriteDefineH, "#define %s_PIN GPIO_PIN_\n", p_adOut[i]);
    }

    // định nghĩa các port đầu ra tương thích với vi điều khiển
    fprintf((FILE*)fileWriteDefineH, "\n// Định nghĩa các port đầu ra\n");
    
    for(int i = 0; i < so_dau_ra; i++){
        fprintf((FILE*)fileWriteDefineH, "#define %s_PORT GPIO\n", p_adOut[i]);
    }
    
    // Khai báo biến đầu vào
    fprintf((FILE*)fileWriteDefineH, "\n// Khai báo biến đầu vào\n");
    
    for(int i = 0; i < so_dau_vao; i++){
        fprintf((FILE*)fileWriteDefineH, "uint16_t %s;\n", p_adIn[i]);
    }

    // Khai báo biến đầu ra
    fprintf((FILE*)fileWriteDefineH, "\n// Khai báo biến đầu ra\n");
    
    for(int i = 0; i < so_dau_ra; i++){
        fprintf((FILE*)fileWriteDefineH, "uint16_t %s;\n", p_adOut[i]);
    }

    // Khai báo các biến trung gian
    fprintf((FILE*)fileWriteDefineH, "\n// Khai báo biến trung gian\n");
    
    for(int i = 0; i < so_bien_trung_gian; i++){
        fprintf((FILE*)fileWriteDefineH, "uint16_t %s;\n", p_adTg[i]);
    }

    // khai câu lệnh đọc nhõ vào, xuất ngõ ra, và kết thúc hàm define.h
    fprintf((FILE*)fileWriteDefineH, "\nvoid read_Pin_Input();       // đọc các chân đầu vào\n");
    fprintf((FILE*)fileWriteDefineH, "void write_Pin_Output();     // viết các chân đầu ra\n");
    fprintf((FILE*)fileWriteDefineH, "\n#endif\n");

    fclose(fileWriteDefineH);

/*-----------------------------------------Viết vào file define.c trong IDESTM32 để đọc các pin đầu vào, và xuất các chân đầu ra------------------------------------------------------*/

    FILE *fileWriteDefineC = fopen(fileDefineC, "w");

    // viết vào hàm đọc các chân đầu vào file define.c
    fprintf((FILE*)fileWriteDefineC, "\n\n// đọc các chân đầu vào\n");
    fprintf((FILE*)fileWriteDefineC, "\nvoid read_Pin_Input(){\n");
    for(int i = 0; i < so_dau_vao; i++){
        fprintf((FILE*)fileWriteDefineC, "	%s = !HAL_GPIO_ReadPin(%s_PORT, %s_PIN);\n", p_adIn[i], p_adIn[i], p_adIn[i]);
    }
    fprintf((FILE*)fileWriteDefineC, "\n}\n");

    // viết hàm xuất đẩu ra vào file define.c
    fprintf((FILE*)fileWriteDefineC, "\n\n// đọc các chân đầu ra\n");
    fprintf((FILE*)fileWriteDefineC, "\nvoid write_Pin_Output(){\n");
    for(int i = 0; i < so_dau_ra; i++){
        fprintf((FILE*)fileWriteDefineC, "	if(%s == 1){\n", p_adOut[i]);
        fprintf((FILE*)fileWriteDefineC, "		HAL_GPIO_WritePin(%s_PORT, %s_PIN, ENABLE);\n", p_adOut[i], p_adOut[i]);
        fprintf((FILE*)fileWriteDefineC, "		}\n");
        fprintf((FILE*)fileWriteDefineC, "	else{\n");
        fprintf((FILE*)fileWriteDefineC, "		HAL_GPIO_WritePin(%s_PORT, %s_PIN, DISABLE);\n", p_adOut[i], p_adOut[i]);
        fprintf((FILE*)fileWriteDefineC, "	}\n");
    }
    fprintf((FILE*)fileWriteDefineC, "\n}\n");

    fclose(fileWriteDefineC);

/*----------------------------------------lưu dữ liệu vào các buckets chứa dữ liệu của các bộ timer và counter (dữ liệu theo manual của s7-200)-------------------------------------------*/

    HashMap = (Hash)malloc(sizeof(Hash)*numberBuckets);
    for(int i = 0; i < 256; i ++){
        HashMap[i] = NULL;
    }
    addrNode1s = (struct Node1**)malloc(sizeof(struct Node1**)*numberBuckets);
    for(int i = 0; i < 256; i ++){
        addrNode1s[i] = NULL;
    }
    struct Node1 data;
    char nameTimer[5];
    char nameCounter[5];
    int resolution = 0;            // độ phân giải của timer 
    char typeName[5] = "\0"; 

    for(int i = 0; i < 256; i++){
        if(i == 32 || i == 96 || i == 0 || i == 64){resolution = 1;}     // độ phân giải 1ms
        else if((i >= 33 && i <= 36) || (i >= 97 && i <= 100) || (i >= 1 && i <= 4) || (i >= 65 && i <= 68)){resolution = 10;} // độ phân giải 10ms
        else if((i >= 37 && i <= 63) || (i >= 101 && i <= 255) || (i >= 5 && i <= 31) || (i >= 69 && i <= 95)){resolution = 100;}// độ phân giải 100ms
        
            sprintf(nameTimer, "T%d\0", i);
            data.resol = resolution;
            data.preValue = 0;
            strcpy(data.type,typeName);
            strcpy(data.name, nameTimer);         

            // thêm vào đầu node ở đây là dữ liệu của các bộ timer
            addFirstNode1(&HashMap, data, &addrNode1s, i);

            sprintf(nameCounter, "C%d\0", i);
            data.resol = 0;
            data.preValue = 0;
            strcpy(data.type, typeName);
            strcpy(data.name, nameCounter);

            // thêm vào node sau cùng ở đây là dữ liệu của counter
            addLastNode1(&HashMap, data, addrNode1s, i);
    }

/*------------------------------------------------đưa các preset value trong các bộ timer và counter vào bảng dữ liệu HashTable-----------------------------------------------------*/

    char nameInstruc[5];
    int valueInstruc;

    while(List->next != NULL){
        if( strcmp(List->list, "TON\0") == 0 || 
            strcmp(List->list, "TOF\0") == 0 || 
            strcmp(List->list, "CTU\0") == 0 || 
            strcmp(List->list, "CTD\0") == 0 || 
            strcmp(List->list, "CTUD\0")== 0 ){

            name_value(List, nameInstruc, &valueInstruc);                  // lấy tên bộ counter, timer, và giá trị preset của chúng
            while(1){
                if(strcmp(HashMap[hashFunction(nameInstruc)]->name, nameInstruc) == 0){
                    HashMap[hashFunction(nameInstruc)]->preValue = valueInstruc;                   // gán giá trị preset cho tên bộ lệnh tương ứng
                    strcpy(HashMap[hashFunction(nameInstruc)]->type, List->list);                  // gán kiểu timer hay counter
                    break;
                }
                HashMap[hashFunction(nameInstruc)] = HashMap[hashFunction(nameInstruc)]->next;
            }
            HashMap[hashFunction(nameInstruc)] = addrNode1s[hashFunction(nameInstruc)];
        }
        List = List->next;
    }
    List = P_LinkList;                // trả lại vị trí node đầu tiên của danh sách liên kết

/*---------------------------------------------------------------đếm số lần xuất hiện lệnh sườn lên và sườn xuống <ED> <EU>------------------------------------------------------------*/
   
    int numberEdgUp = 0;              // số lệnh sườn lên có trong chương trình LADDER
    int numberEdgDo = 0;              // số lệnh sườn xuống có trong chương trình LADDER
    
    while(List->next != NULL){
        if(strcmp(List->list, "EU\0") == 0){
            numberEdgUp++;
        }
        if(strcmp(List->list, "ED\0") == 0){
            numberEdgDo++;
        }
        List = List->next;
    }
    List = P_LinkList; 


/*-----------------------------------ghi các khai báo biến timer, counter, sườn lên, sườn xuống, và một số biến khác trong hàm main.c của IDE STM32-------------------------------------*/

    FILE * FWirteDecla = fopen(fileTemp, "w");

    // viết vào file các biến cho các bộ timer
    for(int i = 0; i < so_Bo_timer; i ++){
        fprintf((FILE*)FWirteDecla, "\n// Khởi tạo các biến cho timer %s, %s\n\n", HashMap[hashFunction(p_adTm[i])]->name, HashMap[hashFunction(p_adTm[i])]->type);
        fprintf((FILE*)FWirteDecla, "uint16_t vao%s = 0;\n", HashMap[hashFunction(p_adTm[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint32_t tre%s = %d*%d;\n", HashMap[hashFunction(p_adTm[i])]->name, HashMap[hashFunction(p_adTm[i])]->preValue, HashMap[hashFunction(p_adTm[i])]->resol);
        fprintf((FILE*)FWirteDecla, "uint16_t en_%s = 0;\n", HashMap[hashFunction(p_adTm[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint32_t demHt%s = 0;\n", HashMap[hashFunction(p_adTm[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint16_t %s = 0;\n", HashMap[hashFunction(p_adTm[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint16_t %sH = 0;\n", HashMap[hashFunction(p_adTm[i])]->name);
    }

    // viết vào file các biến cho các bộ counter
    for(int i = 0; i < so_Bo_counter; i++){
        HashMap[hashFunction(p_adCt[i])] = HashMap[hashFunction(p_adCt[i])]->next;
        fprintf((FILE*)FWirteDecla, "\n// Khởi tạo các biến cho counter %s, %s\n\n", HashMap[hashFunction(p_adCt[i])]->name, HashMap[hashFunction(p_adCt[i])]->type);
        if(strcmp(HashMap[hashFunction(p_adCt[i])]->type, "CTU\0") == 0){
            fprintf((FILE*)FWirteDecla, "uint16_t tang%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
            fprintf((FILE*)FWirteDecla, "uint16_t nhoTang%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        }

        if(strcmp(HashMap[hashFunction(p_adCt[i])]->type, "CTD\0") == 0){
            fprintf((FILE*)FWirteDecla, "uint16_t giam%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
            fprintf((FILE*)FWirteDecla, "uint16_t nhoGiam%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        }

        if(strcmp(HashMap[hashFunction(p_adCt[i])]->type, "CTUD\0") == 0){
            fprintf((FILE*)FWirteDecla, "uint16_t tang%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
            fprintf((FILE*)FWirteDecla, "uint16_t giam%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
            fprintf((FILE*)FWirteDecla, "uint16_t nhoTang%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
            fprintf((FILE*)FWirteDecla, "uint16_t nhoGiam%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        }
        fprintf((FILE*)FWirteDecla, "uint16_t valueHt%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint16_t value%s = %d;\n", HashMap[hashFunction(p_adCt[i])]->name, HashMap[hashFunction(p_adCt[i])]->preValue);
        fprintf((FILE*)FWirteDecla, "uint16_t %s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint16_t %sH = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);
        fprintf((FILE*)FWirteDecla, "uint16_t reset%s = 0;\n", HashMap[hashFunction(p_adCt[i])]->name);

        HashMap[hashFunction(p_adCt[i])] = addrNode1s[hashFunction(p_adCt[i])];
    }

    // viết vào file các biến cho các bit phát hiện sườn lên
    for(int i = 1; i <= numberEdgUp; i++){
        fprintf((FILE*)FWirteDecla, "\n// khởi tạo các biến cho bít nhận biết sườn xuống EU%d, EU\n\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t EU%d = 0;\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t nhoEU%d = 0;\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t demHtEU%d = 0;\n", i);
    }

    // viết vào file các biến cho các bit phát hiện sườn xuống
    for(int i = 1; i <= numberEdgDo; i++){
        fprintf((FILE*)FWirteDecla, "\n// khởi tạo các biến cho bít nhận biết sườn xuống ED%d, ED\n\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t ED%d = 0;\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t nhoED%d = 0;\n", i);
        fprintf((FILE*)FWirteDecla, "uint16_t demHtED%d = 0;\n", i);
    }

    // viết vào file các biến đếm và hàm xử lý ngắt timer trong IDE
    fprintf((FILE*)FWirteDecla, "\n// Khởi tạo biến đếm phục vụ cho các bộ timer, counter\nuint32_t dem = 0;\n");
    fprintf((FILE*)FWirteDecla, "\n// Bộ timer chính phục vụ cho các bộ timer, sườn lên, sườn xuống\nvoid HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){\n");
    fprintf((FILE*)FWirteDecla, "\n	if(htim->Instance == htim11.Instance){\n");
    fprintf((FILE*)FWirteDecla, "\n		dem++;\n");
    fprintf((FILE*)FWirteDecla, "\n	}\n}\n");

    fclose(FWirteDecla);

/*-------------------------------------Khâu chuyển đổi các list có trong file List3 thành file.c để viết vào file main.c trong IDE STM32-------------------------------------------------------*/
    
    int numberNetWork  = 1;         // số network có trong chương trình LADDER
    int posJ = 0;                   // vị trí thứ j của mảng để bắt đầu ghi vào list_temp
    int EUx = 1;                    // định giá trị cho các biến của bít phát hiện sườn lên
    int EDx = 1;                    // định giá trị cho các biến của bít phát hiện sườn xuống
    FILE* FWriteMainC = NULL;

    while(List->next != NULL){
        if(strcmp(List->list, "N\0") == 0){
            numberNetWork++;
        }
        List = List->next;
    }
     
    // ------------------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>     xử lý từng network để viết vào file.c

    for( int i = 0; i < numberNetWork; i++){

        // -----------------------------------------------------viết vào List_temp------------------------------------>>>>>>>>>>>>>>>>>>>

        for(int j = 0; j < soListTrongFileList3; j++){
            if(j >= posJ){
                if(strcmp(p_List[j], "N\0") == 0){
                    addNewLastNode(&List_Temp, p_List[j]);                       // chèn thêm node có dữ liệu là kí tự N
                    posJ = j + 1;
                    break;
                }
                if(List_Temp == NULL){
                    addFirstNode(&List_Temp, &P_LinkList_Temp, p_List[j]);       // chèn vào vị trí đầu tiên của danh sách dữ liệu
                }
                else if(List_Temp != NULL){
                    addNewLastNode(&List_Temp, p_List[j]);                       // chèn vào vị trí cuối của danh sách dữ liệu
                }
            }
        }

        //------------------------------------------------ xử lý với List_temp để đưa tới List_Out------------------------>>>>>>>>>>>>>>>>>

        while(strcmp(List_Temp->list , "N\0") != 0){
            
            // nếu List_temp[i] == "(" hoặc == ")" thì chèn list_temp[i] vào cuối List_Out
            if((strcmp(List_Temp->list, "(\0") == 0) || (strcmp(List_Temp->list, ")\0") == 0)){               
                addLastNode(&List_Out,&P_Link_Out, List_Temp->list); 
            }

            // nếu List_Temp[i] == "LD" hoặc List_Temp[i] == "LDN"
            else if((strcmp(List_Temp->list, "LD\0") == 0) || (strcmp(List_Temp->list, "LDN\0") == 0)){
                addLastNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);                          // thêm vào cuối List_out node thứ list_temp[i+1]
            }

            // nếu List_Temp[i] == "A" hoặc List_Temp[i] == "AN"
            else if((strcmp(List_Temp->list, "A\0") == 0) || (strcmp(List_Temp->list, "AN\0") == 0)){
                if(check_2_dau_ngoac(List_Out) != 0){                          // nếu trong List_Out đã từng tồn tại hai list[i], và List[i+1] lần lượt là các kí tự ")" và "("
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", check_2_dau_ngoac(List_Out) + 1);        // thay thế ")(" thành ")(("
                    addLastNode(&List_Out, &P_Link_Out, "*\0");                                               // chèn thêm dấu "*" vào cuối list_out
                    addLastNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);                      // thêm vào cuối List_out node thứ list_temp[i+1]
                    addLastNode(&List_Out, &P_Link_Out, ")\0");                                               // chèn thêm dấu ")" vào cuối list_out                                      
                }
                else if(check_2_dau_ngoac(List_Out) == 0){
                    addFirstNode(&List_Out, &P_Link_Out, "(\0");                                              // chèn vào đầu List_Out kí tự "("
                    addLastNode(&List_Out, &P_Link_Out, "*\0");                                               // chèn thêm dấu "*" vào cuối list_out
                    addLastNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);                      // thêm vào cuối List_out node thứ list_temp[i+1]
                    addLastNode(&List_Out, &P_Link_Out, ")\0");                                               // chèn thêm dấu ")" vào cuối list_out  
                } 
            }

            // nếu List_Temp[i] == "O" hoặc List_Temp[i] == "ON"
            else if((strcmp(List_Temp->list, "O\0") == 0) || (strcmp(List_Temp->list, "ON\0") == 0)){
                if(check_2_dau_ngoac(List_Out) != 0){                          // nếu trong List_Out đã từng tồn tại hai list[i], và List[i+1] lần lượt là các kí tự ")" và "("
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", check_2_dau_ngoac(List_Out) + 1);        // thay thế ")(" thành ")(("
                    addLastNode(&List_Out, &P_Link_Out, "+\0");                                               // chèn thêm dấu "+" vào cuối list_out
                    addLastNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);                      // thêm vào cuối List_out node thứ list_temp[i+1]
                    addLastNode(&List_Out, &P_Link_Out, ")\0");                                               // chèn thêm dấu ")" vào cuối list_out                                      
                }
                else if(check_2_dau_ngoac(List_Out) == 0){
                    addFirstNode(&List_Out, &P_Link_Out, "(\0");                                              // chèn vào đầu List_Out kí tự "("
                    addLastNode(&List_Out, &P_Link_Out, "+\0");                                               // chèn thêm dấu "+" vào cuối list_out
                    addLastNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);                      // thêm vào cuối List_out node thứ list_temp[i+1]
                    addLastNode(&List_Out, &P_Link_Out, ")\0");                                               // chèn thêm dấu ")" vào cuối list_out  
                } 
            }

            // nếu List_Temp[i] == "NOT"
            else if((strcmp(List_Temp->list, "NOT\0") == 0)){
                // nếu trong danh sách dữ liệu List_out có số dấu "(" > ")"
                if(numberChar_InList(List_Out, "(\0") > numberChar_InList(List_Out, ")\0")){
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", posNode_xLD(List_Out) + 1);
                        addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "!\0", posNode_xLD(List_Out) + 2);                    
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        addFirstNode(&List_Out, &P_Link_Out, "!\0");
                        addFirstNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }

                // nếu trong danh sách dữ liệu List_out có số dấu "(" = ")"
                else if(numberChar_InList(List_Out, "(\0") == numberChar_InList(List_Out, ")\0")){
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", posNode_xLD(List_Out) + 1);
                        addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "!\0", posNode_xLD(List_Out) + 2);                        
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }

                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        addFirstNode(&List_Out, &P_Link_Out, "!\0");
                        addFirstNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }
            }

            // nếu List_Temp[i] == "ALD"
            else if((strcmp(List_Temp->list, "ALD\0") == 0)){
                if(number_dau(List_Out) == 1){
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "*\0", posNode_xLD(List_Out) + 1);        // thay thế ")(" thành ")*("
                    addFirstNode(&List_Out, &P_Link_Out, "(\0");
                    addLastNode(&List_Out, &P_Link_Out, ")\0");
                }

                else if(number_dau(List_Out) != 1){
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "*\0", posNode_xLD(List_Out) + 1);        // thay thế ")(" thành ")*("
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", posNode_xLD(List_Out) + 1);  
                    addLastNode(&List_Out, &P_Link_Out, ")\0");     
                }
            }

            // nếu List_Temp[i] == "OLD"
            else if((strcmp(List_Temp->list, "OLD\0") == 0)){
                if(number_dau(List_Out) == 1){
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "+\0", posNode_xLD(List_Out) + 1);        // thay thế ")(" thành ")+("
                    addFirstNode(&List_Out, &P_Link_Out, "(\0");
                    addLastNode(&List_Out, &P_Link_Out, ")\0");
                }

                else if(number_dau(List_Out) != 1){
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "+\0", posNode_xLD(List_Out) + 1);        // thay thế ")(" thành ")+("
                    addNodeAnyLocaUpd(&List_Out, &P_Link_Out, "(\0", posNode_xLD(List_Out) + 1);  
                    addLastNode(&List_Out, &P_Link_Out, ")\0");     
                }
            }

            // nếu List_Temp[i] == "="
            else if((strcmp(List_Temp->list, "=\0") == 0)){
                addFirstNode(&List_Out, &P_Link_Out, "=\0");                                        //  chèn vào vị trí dầu node dấu "="
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);               //  chèn vào vị trí dầu node list_temp[i+1]

                // viết chương trình cho biểu thức đầu ra
                file_Out(FWriteMainC, List_Out);
            }

            // nếu List_Temp[i] == "EU"
            else if((strcmp(List_Temp->list, "EU\0") == 0)){
                noi_chuoi(List_Temp->list, EUx);                 // nối chuỗi để khai báo biến cho các bít phát hiện xung lên
                EUx++;

                // nếu trong danh sách dữ liệu List_out có số dấu "(" > ")"
                if(numberChar_InList(List_Out, "(\0") > numberChar_InList(List_Out, ")\0")){
                    
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);   // cộng 1 vì lấy vị trị của dấu "(" là node sau node có kí tự ")"
                        addLastNode(&List_Temp2, &P_LinkList_Temp2, ")\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa các node từ vị trí node có vị trị là kí tự "(" cho đến hết của danh sách dữ liệu
                        clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }

                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, 0);
                        addLastNode(&List_Temp2, &P_LinkList_Temp2, ")\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa hết các node trong danh sách dữ liệu List_Out và copy biến sườn lên vào danh sách liên kết
                        clear_Nodes(&List_Out, &P_Link_Out, 0);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }

                // nếu trong danh sách dữ liệu List_out có số dấu "(" = ")"
                else if(numberChar_InList(List_Out, "(\0") == numberChar_InList(List_Out, ")\0")){
                     
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);
                        
                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa các node từ vị trí node có vị trị là kí tự "(" cho đến hết của danh sách dữ liệu
                        clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }

                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, 0);
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa hết các node trong danh sách dữ liệu List_Out và copy biến sườn lên vào danh sách liên kết
                        clear_Nodes(&List_Out, &P_Link_Out, 0);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }
                List_Temp2 = P_LinkList_Temp2;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp2);
            }

            // nếu List_Temp[i] == "ED"
            else if((strcmp(List_Temp->list, "ED\0") == 0)){
                noi_chuoi(List_Temp->list, EDx);                 // nối chuỗi để khai báo biến cho các bít phát hiện xung xuống
                EDx++;

                // nếu trong danh sách dữ liệu List_out có số dấu "(" > ")"
                if(numberChar_InList(List_Out, "(\0") > numberChar_InList(List_Out, ")\0")){
                    
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);   // cộng 1 vì lấy vị trị của dấu "(" là node sau node có kí tự ")"
                        addLastNode(&List_Temp2, &P_LinkList_Temp2, ")\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "!\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa các node từ vị trí node có vị trị là kí tự "(" cho đến hết của danh sách dữ liệu
                        clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }

                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, 0);
                        addLastNode(&List_Temp2, &P_LinkList_Temp2, ")\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "!\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa hết các node trong danh sách dữ liệu List_Out và copy biến sườn lên vào danh sách liên kết
                        clear_Nodes(&List_Out, &P_Link_Out, 0);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }

                // nếu trong danh sách dữ liệu List_out có số dấu "(" = ")"
                else if(numberChar_InList(List_Out, "(\0") == numberChar_InList(List_Out, ")\0")){
                     
                    // nếu trong danh sách liên kết có 2 node liền kề lần lượt là các kí tự ")" và "("
                    if(posNode_xLD(List_Out) != 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "!\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);
                        
                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa các node từ vị trí node có vị trị là kí tự "(" cho đến hết của danh sách dữ liệu
                        clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }

                    // nếu trong danh sách liên kết không có 2 node liền kề lần lượt là các kí tự ")" và "("
                    else if(posNode_xLD(List_Out) == 0){
                        coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, 0);
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "!\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                        addFirstNode(&List_Temp2, &P_LinkList_Temp2, List_Temp->list);

                        // viết vào file main.c xử lý câu lệnh EU
                        file_EUx((FILE*)FWriteMainC, List_Temp2);

                        // xóa hết các node trong danh sách dữ liệu List_Out và copy biến sườn lên vào danh sách liên kết
                        clear_Nodes(&List_Out, &P_Link_Out, 0);
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, "(\0");
                        addLastNode(&List_Out, &P_Link_Out, List_Temp->list);
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                        addLastNode(&List_Out, &P_Link_Out, ")\0");
                    }
                }
                List_Temp2 = P_LinkList_Temp2;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp2);
            }

            // nếu List_Temp[i] == "TON"
            else if((strcmp(List_Temp->list, "TON\0") == 0)){
                addFirstNode(&List_Out, &P_Link_Out, "=\0");
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);

                // viết vào file main.c xử lý câu lệnh TON
                file_TON((FILE*)FWriteMainC, List_Out);
            }

            // nếu List_Temp[i] == "TOF"
            else if((strcmp(List_Temp->list, "TOF\0") == 0)){
                addFirstNode(&List_Out, &P_Link_Out, "=\0");
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);

                // viết vào file main.c xử lý câu lệnh TON
                file_TOF((FILE*)FWriteMainC, List_Out);
            }
            
            // nếu List_Temp[i] == "CTU"
            else if((strcmp(List_Temp->list, "CTU\0") == 0)){
                coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);   // cộng 1 vì lấy vị trị của dấu "(" là node sau node có kí tự ")"
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, addrListGo(List_Temp, 1)->list);

                // xóa các node từ vị trí kí tự "(" cho đến hết các node của danh sách liên kết List_Out
                clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                addFirstNode(&List_Out, &P_Link_Out, "=\0");
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);

                // viết vào file main.c xử lý câu lệnh CTU
                file_CTU((FILE*)FWriteMainC, List_Out, List_Temp2);

                List_Temp2 = P_LinkList_Temp2;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp2);
            }

            // nếu List_Temp[i] == "CTD"
            else if((strcmp(List_Temp->list, "CTD\0") == 0)){
                coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);   // cộng 1 vì lấy vị trị của dấu "(" là node sau node có kí tự ")"
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, addrListGo(List_Temp, 1)->list);

                // xóa các node từ vị trí kí tự "(" cho đến hết các node của danh sách liên kết List_Out
                clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                addFirstNode(&List_Out, &P_Link_Out, "=\0");
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);

                // viết vào file main.c xử lý câu lệnh CTU
                file_CTD((FILE*)FWriteMainC, List_Out, List_Temp2);

                List_Temp2 = P_LinkList_Temp2;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp2);
            }

            // nếu List_Temp[i] == "CTUD"
            else if((strcmp(List_Temp->list, "CTUD\0") == 0)){
                // coppy biểu thức reset
                coppy_List(&List_Temp3, List_Out, &P_LinkList_Temp3, posNode_xLD(List_Out) + 1);
                addFirstNode(&List_Temp3, &P_LinkList_Temp3, "=\0");
                addFirstNode(&List_Temp3, &P_LinkList_Temp3, addrListGo(List_Temp, 1)->list);

                // coppy biểu thức giam
                clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                coppy_List(&List_Temp2, List_Out, &P_LinkList_Temp2, posNode_xLD(List_Out) + 1);
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, "=\0");
                addFirstNode(&List_Temp2, &P_LinkList_Temp2, addrListGo(List_Temp, 1)->list);

                // coppy biểu thức tăng
                clear_Nodes(&List_Out, &P_Link_Out, posNode_xLD(List_Out) + 1);
                addFirstNode(&List_Out, &P_Link_Out, "=\0");
                addFirstNode(&List_Out, &P_Link_Out, addrListGo(List_Temp, 1)->list);

                file_CTUD((FILE*)FWriteMainC, List_Out, List_Temp2, List_Temp3);

                List_Temp2 = P_LinkList_Temp2;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp2);

                List_Temp3 = P_LinkList_Temp3;
                // xóa hết dữ liệu trong List_Temp2
                freeAllNode(&List_Temp3);
            }


            List_Temp = List_Temp->next;
        }


        
        
        List_Temp = P_LinkList_Temp;
        List_Out = P_Link_Out;
        
        // xóa hết dữ liệu trong List_temp
        freeAllNode(&List_Temp);
        // xóa hết dữ liệu trong List_Out
        freeAllNode(&List_Out);
        
    }




/*-----------------------------------------------------------------------giải phóng bộ nhớ khi đã cấp phép-------------------------------------------------------------------------------------*/

    for(int i = 0; i < numberList; i++){                            // giải phóng bộ nhớ cho numberList mảng con
        free(p_ArrayTowDirec[i]);
    }  

    free(p_ArrayTowDirec);                                          // giải phóng bộ nhớ cho mảng mẹ 
    free(listLine);  

    free(p_FReadList3);
    for(int i = 0; i < soListTrongFileList3; i++){                            // giải phóng bộ nhớ cho numberList mảng con
        free(p_List[i]);
    }  

    free(p_adIn);
    for(int i = 0; i < so_dau_vao; i++){
        free(p_adIn[i]);
    }

    free(p_adOut);
    for(int i = 0; i < so_dau_ra; i++){
        free(p_adOut[i]);
    }

    free(p_adTg);
    for(int i = 0; i < so_bien_trung_gian; i++){
        free(p_adTg[i]);
    }

    free(p_adCt);
    for(int i = 0; i < so_Bo_counter; i++){
        free(p_adCt[i]);
    }

    for(int i = 0; i < so_Bo_timer; i++){
        free(p_adTm[i]);
    }
    free(p_adTm);
    

    free(HashMap);
    free(addrNode1s);

    
  
    return 0;
} 

/*==================================================================================================================================================
END Hàm main, hàm xử lý chính (bộ não của chương trình), code ra kết quả như không biết tại sao nó lại ra :)))))     
====================================================================================================================================================*/
