#ifndef xuLyFile_h
#define xuLyFile_h

#include"denGiaoThong.h"
#include"LinkList.h"

// định nghĩa kiểu dữ liệu cho các thành phần trong 1 dòng lệnh STL
typedef struct{
    char *commandStl;                    // Lưu trữ chuỗi kí tự của câu lệnh STL như (LD, ON, A,...)
    char *variableStl;                   // Lưu trữ chuỗi kí tự của các biến vào ra, biến trung gian, timer, counter như (I0.0, M0.1,...)
    char *timeStl;                       // Lưu trữ thời gian trong các bộ timer và counter (nếu có) như (1000, 5000, ...)
}elementCommandStlLine_t;

elementCommandStlLine_t element;         


// Tổng hợp các hàm xử lý chương trình stl thành C
int countRowNumber(const char *file_path);
bool stlCommandType(char *command);  
bool endNetWorkCommand(char *command);
void writeCommandInFile(FILE *F, char *command);
void readCommandInfile(FILE *F, char *command, int charMax);
void add_Number_CharList(elementCommandStlLine_t element, int *number_List, int commandType);
int  dissectionCommandStl(char *command,elementCommandStlLine_t *elements);
bool checkCharacter(char *command, const char character);
void write1Type(FILE *F, elementCommandStlLine_t element, int i, int rowNumberStl);
void write2Type(FILE *F, elementCommandStlLine_t element);
void write3Type(FILE *F, elementCommandStlLine_t element);
void save_into_lists(char **P_list, int row, char *command);
void change_List(char **P_list, int row);
void save_Into_File_List(FILE *F, char **P_list, int row);
int numberInstruc(LinkList data, const char x);    // hàm đếm xem trong chương trình LADDER có bao nhiêu: đầu vào, ra, timer, counter khi kí tự truyền vào là <I>, <Q>, <M>, <T>, <C> 
                                                   // đã được đưa vào chương trình ladder (không tính cả số lần các biến được lặp lại)
bool checkArray2Dir(char **p, char *data, int posCurrent);   // kiểm tra xem đã tồn tại dữ liệu trước đó của mảng hai chiều                                               
bool checkVar (char *data, const char c);                    // kiểm tra chuỗi dữ liệu có phải là các biến vào, ra trung gian, timer, hay counter
bool checkCharFormTxH(char *data);
void dispArray2dir(char **p, int line);                      // sắp xếp mảng hai chiều biến đầu vào theo thứ tự tăng dần của các port và chân
void swabchar(char *x, char*y);                              // hoán vị hai chuỗi dữ liệu 
bool checkCharH(char *data);                                 // Kiểm tra xem dữ liệu có trong danh sách dữ liệu có kí H đã thêm hay không 
void name_value(LinkList data, char *name, int *value);
void noi_chuoi(char* data, int i);                           // ép giá trị i thành 1 chuỗi và nối nó vào chuỗi data

void file_Out(FILE* file, LinkList data);                    // viết chương trình cho biểu thức đầu ra  
void file_EUx(FILE* file, LinkList data);                    // viết chương trính cho biểu thức xung sườn lên
void file_EDx(FILE* file, LinkList data);                    // viết chương trính cho biểu thức xung sườn xuống
void file_TON(FILE* file, LinkList data);                    // viết chương trình cho bộ timer TON
void file_TOF(FILE* file, LinkList data);                    // viết chương trình cho bộ timer TOF
void file_CTU(FILE* file, LinkList list1, LinkList list2);   // viết chương trình cho bộ CTU
void file_CTD(FILE* file, LinkList list1, LinkList list2);   // viết chương trình cho bộ CTD
void file_CTUD(FILE* file, LinkList list1, LinkList list2, LinkList list3); // viết chương trình cho bộ counter CTUD

#endif 