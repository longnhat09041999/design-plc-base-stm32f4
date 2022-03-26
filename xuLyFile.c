#include"xuLyFile.h"

/*==================================================================================================================================================
                                                Các hàm xử lý để chuyển file.txt thành STL                                    
====================================================================================================================================================*/

//  Đếm số dòng trong file.txt
int countRowNumber(const char *file_path){
    char buffer[40];                                     // quy định một dòng lệnh trong file.txt có nhiều nhất 40 kí tự
    FILE *f = fopen(file_path, "r");

    if(f == NULL){                                       // Nếu file f không có gì cả thì hàm trả về giá trị -1
        printf("Khong the mo duoc file.\n");
        return -1;
    }

    int row = 0;
    while(fgets(buffer, 40, (FILE*)f)){                  // hàm fget trả về giá trị NULL khi gặp dòng cuối cùng
        row++;
    }

    fclose(f);
    return row;                                          // Trả về giá trị số dòng
}

// Xác định dòng lệnh thứ [i] có phải của dạng STL hay không?
bool stlCommandType(char *command){
    char temp[4];
                                                                    // xử lý các dòng lệnh của file.txt để gán các kí tự giống 
    for(int i=0; i <= 4; i++){                                      // như các câu lệnh STL (LD, A, AN, 0, VV..) của PLC  vào 
        if((command[i] == ' ') || (command[i] == '\n')){            // biến tạm, tiếp theo là so sánh các kí tự của biến tạm
            temp[i] = '\0'; 
            break;                                                  // xem có giống với kí hiệu câu lệnh của PLC hay không?
        }                                                           // đúng thì trả về "true", sai thì trả về false
        else {
            temp[i] = command[i];
        }
    }

    if( (strcmp(temp,"LD\0") == 0)   ||                       // A tương ứng với lệnh nhân với biến thường mở 
        (strcmp(temp,"LDN\0") == 0)  ||
        (strcmp(temp,"A\0") == 0)   ||
        (strcmp(temp,"AN\0") == 0)   ||                       // AN tương ứng với lệnh nhân với biến thường đóng
        (strcmp(temp,"O\0") == 0)    ||                       // O tương ứng với lệnh cộng biến thường mở
        (strcmp(temp,"ON\0") == 0)   ||                       // ON tương ứng với lệnh cộng biến thường đóng
        (strcmp(temp,"NOT\0") == 0)  ||                       // NOT tương ứng với lệnh đảo ngõ ra 
        (strcmp(temp,"ALD\0") == 0)  ||                       // ALD tương ứng với nhân 2 nhánh lệnh nhỏ trong network
        (strcmp(temp,"OLD\0") == 0)  ||                       // OLD tương ứng với cộng 2 nhánh lệnh nhỏ trong network
        (strcmp(temp,"EU\0") == 0)   ||                       // EU tương ứng với lệnh xác định sườn lên của ngõ vào                     
        (strcmp(temp,"ED\0") == 0)   ||                       // ED tương ứng với lệnh xác định sườn xuống của ngõ vào
        (strcmp(temp,"=\0") == 0)    ||                       // = tương ứng với lệnh khai báo ngõ ra
        (strcmp(temp,"TON\0") == 0)  ||                       // TON tương ứng với lệnh gọi chương trình timer đóng chậm
        (strcmp(temp,"TOF\0") == 0) ||                       // TOF tương ứng với lệnh gọi chương trình timer mở chậm
        (strcmp(temp,"CTD\0") == 0)  ||                       // CTD tương ứng với lệnh gọi chương trình đếm xuống
        (strcmp(temp,"CTU\0") == 0)  ||                       // CTU tương ứng với lệnh gọi chương trình đếm lên
        (strcmp(temp,"CTUD\0") == 0) ){                       // CTUD tương ứng với lệnh gọi chương trình đếm lên xuống
        return (true);
    }
    else {
        return false;
    }
}

// Xác định dòng lệnh [i] có phải là kết thúc của một network hay không?
bool endNetWorkCommand(char *command){

    char temp[7];

    if(strcmp(command, "Network 1 // Network Title\n\0") == 0){                  
        return (false);
    }
    else if(strcmp(command, "END_ORGANIZATION_BLOCK\n\0") == 0){
        return (true);
    }
    else {
        //int lenght = strlen(command);                       // xử lý các dòng lệnh của file.txt để xem các dòng lệnh 
        for(int i=0; i <= 7; i++){                            // có phải là kết thúc của 1 network hay không
            if(command[i] != ' '){                            // 
                temp[i] = command[i];                         // 
            }                                                 // 
            else {
                temp[i] = '\0';
                break;
            }
        }
    }

    if(strcmp(temp, "Network\0") == 0){
        return true;                                           // trả về kiểu giá trị true nếu là kết thúc của một network
    }
    else {
        return false;                                          // trả về kiểu giá trị false nếu là kết thúc của một network
    }
}

// viết một dòng lệnh vào file của chương trình STL
void writeCommandInFile(FILE *F, char *command){
    fputs(command,(FILE*)F);
}

// Đọc 1 dòng lệnh trên file và lưu vào một chuỗi
void readCommandInfile(FILE *F, char *command, int charMax){
     fgets(command,charMax,(FILE *)F);
}

// cộng dồn số kí tự có trong file list
void add_Number_CharList(elementCommandStlLine_t element, int *number_List, int commandType){
    if(commandType == 0){
        *number_List += (strlen(element.commandStl) + 4);
    }
    else if(commandType == 1){                                  // lệnh có dạng <'TON', 'T36', ',', '1000', >
        *number_List += (strlen(element.commandStl) + 4);       // số kí tự trong file list cộng thêm độ dài của lệnh stl và công thêm 4 kí tự <'', >
        *number_List += (strlen(element.timeStl) + 4);
        *number_List += (strlen(element.variableStl) + 4);
        *number_List += 5;                                      // cộng thêm 5 kí tự <',', >  
    }
    else{
        *number_List += (strlen(element.commandStl) + 4);
        *number_List += (strlen(element.variableStl) + 4);
    }
}

/*==================================================================================================================================================
                                                Các hàm xử lý để chuyển đổi file STL thành file list                                    
====================================================================================================================================================*/

// bóc tách 1 dòng lệnh trong file STL để lưu trữ vào cấu trúc dữ liệu (elementCommandStlLine_t) 
int dissectionCommandStl(char *command,elementCommandStlLine_t *elements){
    int lenght = strlen(command);
    char buffer1[40], *p_buffer1;                                                 // dùng để lưu vào biến commandStl của cấu trúc dữ liệu elementCommandStlLine_t
    char buffer2[40], *p_buffer2;                                                 // dùng để lưu vào biến variableStl của cấu trúc dữ liệu elementCommandStlLine_t
    char buffer3[40], *p_buffer3;                                                 // dùng để lưu vào biến timerStl của cấu trúc dữ liệu elementCommandStlLine_t
    char command_Temp[40];                                                        // chuỗi này dùng để lưu trữ giá trị của chuỗi  command  
    char command_Temp1[40];
    char command_Temp2[40];

    p_buffer1 = buffer1;
    p_buffer2 = buffer2;
    p_buffer3 = buffer3;
    
    // copy chuỗi command vào chuỗi command_Temp để tránh thay đổi giá trị của chuỗi command khi xử lý các câu lệnh sau này
    // chúng ta chỉ thao tác trên chuỗi command_Temp
    for(int i=0;i<=lenght;i++){
        command_Temp[i] = command[i];
    }                                                           
    
    // coppy các lệnh của Plc for example (LD, A, ON, = ,....) into  string buffer1
    char *p_temp = strtok(command_Temp, " ");                           // câu lệnh strstok() trả về giá trị là địa chỉ trong ô nhớ lưu trữ 
    int lenght_p_temp = strlen(p_temp);                                 // hàm strtok() khi gặp kí tự được truyền vào lần đầu tiên thì nó sẽ 
    for(int i = 0; i <= lenght_p_temp; i++){                            // gán kí tự NULL vào vị trí của kí tự đó đồ thời giá trị của chuỗi 
        *(p_buffer1 + i) = *(p_temp + i);                               // được truyền vào sẽ bị thay đổi
    }
    
    
    int lenght_buffer1 = strlen(buffer1);
    int lenght_command = strlen(command);
    int j=0;
    
    // --------------câu lệnh thuộc dạng <N>, <NOT>, <EU>, <ED>, <OLD>, <ALD> -------------
    if(checkCharacter(command, ' ') == false){                      // kiểm tra xem câu lệnh có kí tự dấu cách hay không nếu không có thì trả về false
        char temp_command_type[4];

        // xóa tất cả các kí tự khoảng trắng của chuỗi command_Temp và sao chép vào chuỗi command_Temp1
            int lenght_command_Temp = strlen(command);

            for(int i=0;i<=lenght_command_Temp;i++){
                if((command_Temp[i] == '\n') || (command_Temp[i] == '\r') || (command_Temp[i] == ' ')){
                    continue;
                }
                command_Temp1[j] = command_Temp[i];
                    j++;
            }
            j = 0;

        elements->commandStl = command_Temp1;
        elements->variableStl = NULL;
        elements->timeStl = NULL;
        return 0;                                                   // trả về giá trị bằng 0 khi dòng lệnh stl có dạng <N>
    }


    else {

        //------------câu lệnh thuộc dạng <TON     T38, 100>------------
        if(checkCharacter(command,',') == true){
            // sao chép các kí tự còn lại của câu lệnh stl vào command_Temp
            for(int i=0; i <= lenght_command; i++){
                if(i >= lenght_buffer1){
                    command_Temp[j] = command[i];
                    j++;
                }
            }
            j = 0;
            // xóa tất cả các kí tự khoảng trắng của chuỗi command_Temp và sao chép vào chuỗi command_Temp1
            int lenght_command_Temp = strlen(command_Temp);

            for(int i=0;i<=lenght_command_Temp;i++){
                if((command_Temp[i] == '\n') || (command_Temp[i] == '\r') || (command_Temp[i] == ' ')){
                    continue;
                }
                command_Temp1[j] = command_Temp[i];
                    j++;
            }
            j = 0;


            // tách chuỗi command_Temp1 vào buffer2

            int lenght_command_Temp1 = strlen(command_Temp1);
            // copy chuỗi command_Temp1 vào chuỗi command_Temp2 
            for(int i  =0; i <= lenght_command_Temp1; i++){
                command_Temp2[i] = command_Temp1[i];
            }   

            p_temp = strtok(command_Temp2, ",");
            int lenght_p_temp = strlen(p_temp);                                 // 
            for(int i = 0; i <= lenght_p_temp; i++){                            // 
              *(p_buffer2 + i) = *(p_temp + i);                                 // 
            }

            // tách chuỗi command_Temp2 vào buffer3
            int lenght_command_Temp2 = strlen(command_Temp2);

            for(int i=0;i<=lenght_command_Temp1;i++){
                if(i>=(lenght_command_Temp2+1)){ 
                    buffer3[j] = command_Temp1[i];
                    j++;
                }
            }
            j = 0;
            elements->commandStl = buffer1;
            elements->variableStl = buffer2;
            elements->timeStl = buffer3;

            return 1;                                                  // trả về giá trị bằng 1 nếu dạng dòng lệnh stl là  <TON    T37, 50>             
        }

        //--------------câu lệnh thuộc dạng <AN     M0.0>--------------
        else {
            // sao chép các kí tự còn lại của câu lệnh stl vào command_Temp
            for(int i=0; i <= lenght_command; i++){
                if(i >= lenght_buffer1){
                    command_Temp[j] = command[i];
                    j++;
                }
            }
            j = 0;
            // xóa tất cả các kí tự khoảng trắng của chuỗi command_Temp và sao chép vào chuỗi command_Temp1
            int lenght_command_Temp = strlen(command_Temp);

            for(int i=0; i<=lenght_command_Temp; i++){
                if((command_Temp[i] == '\n') || (command_Temp[i] == '\r') || (command_Temp[i] == ' ')){
                    continue;
                }
                command_Temp1[j] = command_Temp[i];
                    j++;
            }
            j = 0;
            
            int lenght_command_Temp1 = strlen(command_Temp1);
             for(int i = 0; i <= lenght_command_Temp1; i++){
                 buffer2[i] = command_Temp1[i];
             }   
            elements->commandStl = buffer1;
            elements->variableStl = buffer2;
            elements->timeStl = NULL;

            return 2;                                            // trả về giá trị bằng 2 nếu dạng dòng lệnh stl là <LD     Q0.0>
        }
    }
}

// kiểm tra xem trong câu lệnh có 1 kí tự tham chiếu vào hàm hay không?
bool checkCharacter(char *command, const char character){
    int lenght_command = strlen(command);                        // độ dài của câu lệnh cần tính

    for(int i=0; i<=lenght_command; i++){
        if(command[i] == character){
        return true;
        }
    }
    return false;
}

// Ghi câu lệnh dạng <N> vào file list có  dạng <'N',> (nếu dòng lệnh này không phải cuối cùng của file  STL) ngược lại có dạng <'N'>
void write1Type(FILE *F, elementCommandStlLine_t element, int i, int rowNumberStl){
    if(i == rowNumberStl){
           fprintf((FILE*)F,"'%s'",element.commandStl); 
    }
    else {
           fprintf((FILE*)F,"'%s', ",element.commandStl);  
    }
}

// ghi câu lệnh dạng <TON    T37, 50> vào file list có dạng      <'T97',',','1000',>
void write2Type(FILE *F, elementCommandStlLine_t element){
    fprintf((FILE*)F,"'%s', '%s', ',', '%s', ",element.commandStl,element.variableStl,element.timeStl);  
}

// ghi câu lệnh dạng <LD     I0.0> vào file list có dạng        <'LD','I0.0',>         
void write3Type(FILE *F, elementCommandStlLine_t element){
    fprintf((FILE*)F,"'%s', '%s', ",element.commandStl,element.variableStl);
}

/*==================================================================================================================================================
                                                 Các hàm xử lý file list giai đoạn 2                                     
====================================================================================================================================================*/

// xử lý lệnh trong file list và lưu vào từng list[]
void save_into_lists(char **P_list, int row, char *command){
    char *temp_command;
    char *temp;
    int j = 0;

    int lenght_command = strlen(command);                      // độ dài kí tự có trong file list

    temp_command = malloc(sizeof(char)*(lenght_command + 1));

    // Sao chép chuỗi command vào chuỗi tạm temp_command
    
    for(int i = 0; i <= lenght_command; i++){                  // duyệt từ 0 đến hết file list để sao chép vào chuỗi temp_command[]
        temp_command[i] = command[i];
    }

    temp = strtok(temp_command, "'");

        for(int i = 0; i < row; i++){                          // Bắt đầu sao chép từ list[] cho đến list[] cuối cùng. 
            temp_command = strtok(NULL, "'");                  // Lấy địa chỉ của các list[] trong file list.
            int lenght_list = strlen(temp_command);            // Độ dài của list[] vừa lấy được.
            for(int j = 0; j <= lenght_list; j++){             // Sao chép từng kí tự trong 1 list[]
                P_list[i][j] = temp_command[j];
            }
            temp = strtok(NULL, "'");                          // gán các kí tự thừa vào temp
        }
    
    free(temp_command);
}

/* sau khi đã lưu trữ các list ta thực hiện chuyển đổi sau
   dấu <.> thay bằng dấu <_> 
   nếu các list là các câu lệnh <LDN>, <AN>, <ON> ta thêm kí tự <H> ở đằng sau list đó
*/
void change_List(char **P_list, int row){

    // xử lý kí tự của các list
    for(int i = 0; i < row; i++){
        int lenght_P_list = strlen(P_list[i]);                 // Tính độ dài của list.
        if(checkCharacter(P_list[i], '.') == true){            // Nếu độ dài ở trong list thứ i có kí tự <.> thì thay đổi băng dấu <_>. 
            for(int j = 0; j <= lenght_P_list; j++){
                if(P_list[i][j] == '.'){
                    P_list[i][j] = '_';                
                }
            }
        }

        // gán kí tự H sau các list[i+1] nếu list[i] bằng bằng các kí tự sau: <LD>, <ON>, <AN>
        if((strcmp(P_list[i],"LDN\0") == 0) || (strcmp(P_list[i],"AN\0") == 0) || (strcmp(P_list[i],"ON\0") == 0)){
            int lenght_P_list_after = strlen(P_list[i + 1]);
            P_list[i+1][lenght_P_list_after] = 'H';
            P_list[i+1][lenght_P_list_after + 1] = '\0';
        }
    }
}

// Lưu các list vào file.
void save_Into_File_List(FILE *F, char **P_list, int row){
    fprintf((FILE *)F, "listcode = [\0");
    for(int i = 0; i < row; i++){
        if(i == (row - 1)){
            fprintf((FILE *)F, "'N']\0");
        }
        else if(strcmp(P_list[i], ",\0") != 0){
            fprintf((FILE *)F, "'%s',", P_list[i]);
        }
    }
}

// hàm đếm xem trong chương trình LADDER có bao nhiêu: đầu vào khi kí tự truyền vào là <I>
//                                                     đầu ra khi kí tự truyền vào là <Q>
//                                                     biến trung gian khi kí tự truyền vào là <M>
//                                                     bộ timer khi kí tự truyền vào là <T>
//                                                     bộ counter khi kí tự truyền vào là <C> 
//      Đã được khai báo trong chương trình LADDER (không tính đến việc các biến khai báo lặp lại)
int numberInstruc(LinkList data, const char x){
    char temp[6];
    int numberVar = 0;                    // Khởi tạo số lần các biến được khai báo (không tính lặp lại trong chương trình ladder)
    int nodeCurrent = 0;                  // chỉ định số node hiện tại của danh sách dữ liệu

    while(data->next != NULL){

        // Đếm số biến đầu vào I trong chương trình ladder ("KHÔNG" tính số lần được lặp lại trong ladder)
        if(x == 'I'){
            strncpy(temp, data->list, 1);         // coppy kí tự đầu tiên vào chuỗi tạm
            temp[1] = '\0';
            if(strcmp(temp, "I\0") == 0 && checkEqualNode(data, nodeCurrent) == false){         // so sách chuỗi tạm với kí tự <I> chưa từng tồn tại dữ liệu trong các node trước đó
                numberVar++;
            }
            
        }

        // Đếm số biến đầu ra Q trong chương trình ladder ("KHÔNG" tính số lần được lặp lại trong ladder)
        if(x == 'Q'){
            strncpy(temp, data->list, 1);         // coppy kí tự đầu tiên vào chuỗi tạm
            temp[1] = '\0';
            if(strcmp(temp, "Q\0") == 0 && checkEqualNode(data, nodeCurrent) == false){         // so sách chuỗi tạm với kí tự <Q> 
                numberVar++;
            }
        }

        // Đếm số biến trung gian M trong chương trình ladder ("KHÔNG" tính số lần được lặp lại trong ladder)
        if(x == 'M'){
            strncpy(temp, data->list, 1);         // coppy kí tự đầu tiên vào chuỗi tạm
            temp[1] = '\0';
            if(strcmp(temp, "M\0") == 0 && checkEqualNode(data, nodeCurrent) == false){         // so sách chuỗi tạm với kí tự <M> 
                numberVar++;
            }
        }

        // Đếm số bộ timer trong chương trình ladder ("KHÔNG" tính số lần được lặp lại trong ladder)
        if(x == 'T'){
            strncpy(temp, data->list, 4);           // coppy 4 kí tự đầu tiên vào chuỗi tạm
            temp[4] = '\0';
            if(strcmp(temp, "TON\0") == 0){         // so sách chuỗi tạm với kí tự <TON> 
                numberVar++;
            }

            if(strcmp(temp, "TOF\0") == 0){        // so sách chuỗi tạm với kí tự <TOFF> 
                numberVar++;
            }     
        }

        // Đếm số bộ counter trong chương trình ladder ("KHÔNG" tính số lần được lặp lại trong ladder)
        if(x == 'C'){
            strncpy(temp, data->list, 4);           // coppy 4 kí tự đầu tiên vào chuỗi tạm
            temp[4] = '\0';
            if(strcmp(temp, "CTU\0") == 0){         // so sách chuỗi tạm với kí tự <CTU> 
                numberVar++;
            }

            if(strcmp(temp, "CTD\0") == 0){         // so sách chuỗi tạm với kí tự <CTD> 
                numberVar++;
            }

            if(strcmp(temp, "CTUD\0") == 0){        // so sách chuỗi tạm với kí tự <CTUD> 
                numberVar++;
            }      
        }

        // sau khi đã xử lý xong ở node nay thì ta tiếp tục trỏ đến node tiếp theo của danh sách dữ liệu
        data = data->next;
        nodeCurrent++;
    }
    return numberVar;
}

// kiểm tra xem đã tồn tại dữ liệu trước đó của mảng hai chiều
bool checkArray2Dir(char **p, char *data, int posCurrent){

    for(int i = 0; i < posCurrent; i++){
        if(strcmp(p[i], data) == 0){
            return true;
        }
    }
    return false;
}

// kiểm tra chuỗi dữ liệu có phải là các biến vào, ra trung gian, timer, hay counter
bool checkVar (char *data, const char c){

    char temp = data[0];
    if(c == 'I' || c == 'Q' || c == 'M'){
        if(temp == c){
            return true;
        }
    }

    if(c == 'C'){
        if(strcmp(data, "CTU\0") == 0 || strcmp(data, "CTD\0") == 0 || strcmp(data, "CTUD\0") == 0){
            return false;
        }
        if(checkCharFormTxH(data) == true){
            return false;
        }
        if(temp == c){
            return true;
        }
    }

    if(c ==  'T'){
        if(strcmp(data, "TON\0") == 0 || strcmp(data, "TOF\0") == 0){
            return false;
        }
        if(checkCharFormTxH(data) == true){
            return false;
        }
        if(temp == c){
            return true;
        }
    }
    return false;
}

// Kiểm tra xem chuỗi dữ liệu là dạng T34H, C46H hay không?
bool checkCharFormTxH(char *data){
    int a = strlen(data);
    if(data[strlen(data) - 1] == 'H'){
        return true;
    }
    return false;
}


// sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
void dispArray2dir(char **p, int line){
    for(int i = 0; i < line-1; i++){
        for(int j = line-1; j > i; j--){
            if(strcmp(p[j], p[j-1]) < 0){
                swabchar(p[j], p[j-1]);
            }
        }
    }
}

// hoán vị hai chuỗi dữ liệu 
void swabchar(char *x, char*y){
    char *temp;
    temp = (char*)malloc(sizeof(char)*5);
    strcpy(temp, x);
    strcpy(x, y);
    strcpy(y, temp);  
    free(temp);  
}

// Kiểm tra xem dữ liệu có trong danh sách dữ liệu có kí H đã thêm hay không đối với biến I, Q, M for example: <I0_0M>......
bool checkCharH(char *data){
    char x;
    x = data[4];
    if(x == 'H'){
        return true;
    }
    return false;
}

// hàm lấy tên của timer, counter, giá trị preset value của mỗi bộ
void name_value(LinkList data, char *name, int *value){
    data = data->next;
    strcpy(name, data->list);
    data = data->next;
    *value = atoi(data->list);
}

// hàm ghép số nguyên vào cuối chuỗi truyền vào
void noi_chuoi(char* data, int i){
    char temp[4];
    itoa(i, temp, 10);
    strcat(data, temp);
}

// viết chương trình cho biểu thức đầu ra 
void file_Out(FILE* file, LinkList data){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file 

    int lengh_List_Out = 0;                                 // độ dài của List_Temp_Out   

    lengh_List_Out = countList(data);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức đầu ra %s--------------------\n	  ", data->list);

    for(int i = 0; i < lengh_List_Out; i++){
        fprintf((FILE*)file, "%s", data->list);
        data = data->next;
    }
    fprintf((FILE*)file, ";\n");
    fclose(file);
}

// viết chương trính cho biểu thức xung sườn lên
void file_EUx(FILE* file, LinkList data){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file 

    int lengh_List_Out = 0;                                 // độ dài của List_Temp_Out   

    lengh_List_Out = countList(data);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức xung sườn lên %s--------------------\n	  ", data->list);

    for(int i = 0; i < lengh_List_Out; i++){
        fprintf((FILE*)file, "%s", data->list);
        data = data->next;
    }
    fprintf((FILE*)file, ";\n");
    data = P_LinkList_Temp2; 

    fprintf((FILE*)file, "	  if(%s == 0){\n", data->list);
    fprintf((FILE*)file, "		  nho%s = 0;\n", data->list);
    fprintf((FILE*)file, "		  demHt%s = dem;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 1 && nho%s == 0){\n", data->list, data->list);
    fprintf((FILE*)file, "		  %s = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 1 && (dem - demHt%s) > 5){\n", data->list, data->list);
    fprintf((FILE*)file, "		  %s = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trính cho biểu thức xung sườn xuống
void file_EDx(FILE* file, LinkList data){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file 

    int lengh_List_Out = 0;                                 // độ dài của List_Temp_Out   

    lengh_List_Out = countList(data);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức xung sườn xuống %s--------------------\n	  ", data->list);

    for(int i = 0; i < lengh_List_Out; i++){
        fprintf((FILE*)file, "%s", data->list);
        data = data->next;
    }
    fprintf((FILE*)file, ";\n");
    data = P_LinkList_Temp2; 

    fprintf((FILE*)file, "	  if(%s == 0){\n", data->list);
    fprintf((FILE*)file, "		  nho%s = 1;\n", data->list);
    fprintf((FILE*)file, "		  demHt%s = dem;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 1 && nho%s == 1){\n", data->list, data->list);
    fprintf((FILE*)file, "		  %s = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 1 && (dem - demHt%s) > 5){\n", data->list, data->list);
    fprintf((FILE*)file, "		  %s = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trình cho bộ timer TON
void file_TON(FILE* file, LinkList data){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file

    int lengh_List_Out = 0;                                 // độ dài của List_Temp_Out   

    lengh_List_Out = countList(data);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức timer TON, %s--------------------\n	  vao", data->list);

    for(int i = 0; i < lengh_List_Out; i++){
        fprintf((FILE*)file, "%s", data->list);
        data = data->next;
    }
    fprintf((FILE*)file, ";\n");
    data = P_Link_Out; 

    fprintf((FILE*)file, "	  if(vao%s == 0){\n", data->list);
    fprintf((FILE*)file, "		  demHt%s = dem;\n", data->list);
    fprintf((FILE*)file, "		  en_%s = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(vao%s ==1 && (dem - demHt%s) >= tre%s ){\n", data->list, data->list, data->list);
    fprintf((FILE*)file, "		  en_%s = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  %s = en_%s;\n", data->list, data->list);
    fprintf((FILE*)file, "	  if(en_%s == 1){\n", data->list);
    fprintf((FILE*)file, "	  		  %sH = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  else{\n");
    fprintf((FILE*)file, "		  %sH = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trình cho bộ timer TOF
void file_TOF(FILE* file, LinkList data){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file

    int lengh_List_Out = 0;                                 // độ dài của List_Temp_Out   

    lengh_List_Out = countList(data);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức timer TOF, %s--------------------\n	  vao", data->list);

    for(int i = 0; i < lengh_List_Out; i++){
        fprintf((FILE*)file, "%s", data->list);
        data = data->next;
    }
    fprintf((FILE*)file, ";\n");
    data = P_Link_Out; 

    fprintf((FILE*)file, "	  if(vao%s == 1){\n", data->list);
    fprintf((FILE*)file, "		  demHt%s = dem;\n", data->list);
    fprintf((FILE*)file, "		  en_%s = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(vao%s ==0 && (dem - demHt%s) >= tre%s ){\n", data->list, data->list, data->list);
    fprintf((FILE*)file, "		  en_%s = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  %s = en_%s;\n", data->list, data->list);
    fprintf((FILE*)file, "	  if(en_%s == 1){\n", data->list);
    fprintf((FILE*)file, "	  		  %sH = 0;\n", data->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  else{\n");
    fprintf((FILE*)file, "		  %sH = 1;\n", data->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trình cho bộ counter CTU, list1 là biểu thức tangCx = y, list2 là biểu thức resetCx = y
void file_CTU(FILE* file, LinkList list1, LinkList list2){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file

    int lenght_List1 = countList(list1);
    int lenght_List2 = countList(list2);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức counter CTU, %s--------------------\n	  tang", list1->list);

    for(int i = 0; i < lenght_List1; i++){
        fprintf((FILE*)file, "%s", list1->list);
        list1 = list1->next;
    }
    fprintf((FILE*)file, ";\n");
    list1 = P_Link_Out; 

    fprintf((FILE*)file, "	  if(tang%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  nhoTang%s = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(tang%s == 1 && nhoTang%s == 0 && reset%s == 0){\n", list1->list, list1->list, list1->list);
    fprintf((FILE*)file, "			  valueHt%s++;\n", list1->list);
    fprintf((FILE*)file, "			  nhoTang%s = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(valueHt%s >= value%s){\n", list1->list, list1->list);
    fprintf((FILE*)file, "		  %s = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if (valueHt%s < value%s){\n", list1->list, list1->list);
    fprintf((FILE*)file, "		  %s = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  %sH = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  else{\n");
    fprintf((FILE*)file, "		  %sH = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");

    fprintf((FILE*)file, "	  reset");

    for(int i = 0; i < lenght_List2; i++){
        fprintf((FILE*)file, "%s", list2->list);
        list2 = list2->next;
    }
    fprintf((FILE*)file, ";\n");
    list2 = P_LinkList_Temp2; 

    fprintf((FILE*)file, "	  if(reset%s == 1){\n", list2->list);
    fprintf((FILE*)file, "		  valueHt%s = 0;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trình cho bộ counter CTD, list1 là biểu thức giamCx = y, list2 là biểu thức resetCx = y
void file_CTD(FILE* file, LinkList list1, LinkList list2){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file

    int lenght_List1 = countList(list1);
    int lenght_List2 = countList(list2);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức counter CTD, %s--------------------\n	  giam", list1->list);

    for(int i = 0; i < lenght_List1; i++){
        fprintf((FILE*)file, "%s", list1->list);
        list1 = list1->next;
    }
    fprintf((FILE*)file, ";\n");
    list1 = P_Link_Out; 

    fprintf((FILE*)file, "	  if(giam%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  nhoGiam%s = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(giam%s == 1 && nhoGiam%s == 0 && reset%s == 0){\n", list1->list, list1->list, list1->list);
    fprintf((FILE*)file, "		  if(valueHt%s > 0){\n", list1->list);
    fprintf((FILE*)file, "			  valueHt%s--;\n", list1->list);
    fprintf((FILE*)file, "			  nhoGiam%s = 1;\n", list1->list);
    fprintf((FILE*)file, "		  }\n");
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(valueHt%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  %s = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if (valueHt%s >= 1){\n", list1->list);
    fprintf((FILE*)file, "		  %s = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  %sH = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  else{\n");
    fprintf((FILE*)file, "		  %sH = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");

    fprintf((FILE*)file, "	  reset");

    for(int i = 0; i < lenght_List2; i++){
        fprintf((FILE*)file, "%s", list2->list);
        list2 = list2->next;
    }
    fprintf((FILE*)file, ";\n");
    list2 = P_LinkList_Temp2; 

    fprintf((FILE*)file, "	  if(reset%s == 1){\n", list2->list);
    fprintf((FILE*)file, "		  valueHt%s = value%s;\n", list2->list, list2->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

// viết chương trình cho bộ counter CTUD, list1 là biểu thức tangCx = y, list2 là biểu thức giamCx = y, list3 là biểu thức resetCx = y
void file_CTUD(FILE* file, LinkList list1, LinkList list2, LinkList list3){
    file = fopen(fileMainTemp, "a+");                       //  mở để ghi vào cuối file

    int lenght_List1 = countList(list1);
    int lenght_List2 = countList(list2);
    int lenght_List3 = countList(list3);

    fprintf((FILE*)file, "\n	  //----------------------biểu thức counter CTUD, %s--------------------\n	  tang", list1->list);

    for(int i = 0; i < lenght_List1; i++){
        fprintf((FILE*)file, "%s", list1->list);
        list1 = list1->next;
    }
    fprintf((FILE*)file, ";\n");
    list1 = P_Link_Out; 

    fprintf((FILE*)file, "	  if(tang%s == 0){\n", list1->list);
    fprintf((FILE*)file, "		  nhoTang%s = 0;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(tang%s == 1 && nhoTang%s == 0 && reset%s == 0){\n", list1->list, list1->list, list1->list);
    fprintf((FILE*)file, "			  valueHt%s++;\n", list1->list);
    fprintf((FILE*)file, "			  nhoTang%s = 1;\n", list1->list);
    fprintf((FILE*)file, "	  }\n");

    fprintf((FILE*)file, "	  giam");

    for(int i = 0; i < lenght_List2; i++){
        fprintf((FILE*)file, "%s", list2->list);
        list2 = list2->next;
    }
    fprintf((FILE*)file, ";\n");
    list2 = P_LinkList_Temp2; 

    fprintf((FILE*)file, "	  if(giam%s == 0){\n", list2->list);
    fprintf((FILE*)file, "		  nhoGiam%s = 0;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(giam%s == 1 && nhoGiam%s == 0 && reset%s == 0){\n", list2->list, list2->list, list2->list);
    fprintf((FILE*)file, "		  if(valueHt%s > 0){\n", list2->list);
    fprintf((FILE*)file, "			  valueHt%s--;\n", list2->list);
    fprintf((FILE*)file, "			  nhoGiam%s = 1;\n", list2->list);
    fprintf((FILE*)file, "		  }\n");
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(valueHt%s >= value%s){\n", list2->list, list2->list);
    fprintf((FILE*)file, "		  %s = 1;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if (valueHt%s < value%s){\n", list2->list, list2->list);
    fprintf((FILE*)file, "		  %s = 0;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  if(%s == 0){\n", list2->list);
    fprintf((FILE*)file, "		  %sH = 1;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");
    fprintf((FILE*)file, "	  else{\n");
    fprintf((FILE*)file, "		  %sH = 0;\n", list2->list);
    fprintf((FILE*)file, "	  }\n");

    fprintf((FILE*)file, "	  reset");

    for(int i = 0; i < lenght_List3; i++){
        fprintf((FILE*)file, "%s", list3->list);
        list3 = list3->next;
    }
    fprintf((FILE*)file, ";\n");
    list3 = P_LinkList_Temp3; 

    fprintf((FILE*)file, "	  if(reset%s == 1){\n", list3->list);
    fprintf((FILE*)file, "		  valueHt%s = 0;\n", list3->list);
    fprintf((FILE*)file, "	  }\n");

    fclose(file);
}

