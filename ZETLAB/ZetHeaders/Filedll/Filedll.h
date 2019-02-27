extern "C"
{
 extern int GetConf(LPSTR dname,LPSTR dpth);
//  �������� ��p�p�� ������� � �p���� ������
//  dname  - ��� �p���� ������
//  ����p������ ��������:
//  dpth   - ��p�p�� �������
//   0  - �������� ����p�����
//  -1  - ���������� ����-��������� �������p����
//  -2  - ���������� ���� �������p���� �������
//  -3  - ���������� ��� �p���� ������

 extern void SetOwnDir(char *fdir); 
// ��������� �������

 extern void OwnDir(char *fdir);    
// ��������� �������

 extern int OpenPrm(char *fln); 
//  ���p��� ���� ��p����p�� ��� ������
//  fln - ������ ��� �����
//  ����������:
//   0 - �������� ����������
//  -2 - ��� �����

 extern int OpenPrmRW(char *fln);
//  ���p��� ���� ��p����p��  ��� ������
//  fln - ������ ��� �����
//  ����������:
//   0 - �������� ����������
//  -2 - ������

 extern int OpenPrmWR(char *fln); 
// �������� �� ������ ����� � ����������� ��������
//  fln - ������ ��� �����
//  ����������:
//   0 - �������� ����������
//  -2 - ������

 extern int RewindPrm(void);
 extern int RewindPrmW(void);
 extern void SwabHandle(void);
 
 extern int SetWFileEnd(void); 
// ��������� � ������ �����, ��������� �� ������

 extern void ClosePrm(void);   
// ���p��� ���� ��p����p��

 extern int RewindPrm(void);   
// ��������� � ������ ����� (��� ������)

 extern int OpenPrmW(LPSTR fln);

 extern int ReadStr(LPSTR pname,LPSTR str);
//  �p������� ��p��� �� ����� �� ����� ����������
//  pname - ��� �����
//   0 - �������� ����������
//  -1 - ��� ������ �����
//  -2 - ���� �� ������

 extern int ReadStrN(LPSTR pname,LPSTR str);
//  �p������� ��������� ��p��� �� ����� �� ����� ����������
//  pname - ��� �����
//   0 - �������� ����������
//  -1 - ��� ������ �����
//  -2 - ���� �� ������
//  -3 - ����� �����

 extern int ReadStrNs(LPSTR pname,LPSTR str);
//  �p������� �� ��������� ������ �� ������� �� �������� ������
// (����������� ��� ����������������� ������ ����� � ����������� �������)
//  ���� �������� ����� ������ ������ �� �������� ��� ������
//  pname - ��� �����
//   0 - �������� ����������
//  -1 - ��� ������ �����
//  -2 - ���� �� ������
//  -3 - ����� �����

 extern int WriteStr(LPSTR str);
//  ������ ��p��� � ���� ��p����p��
//   0 - �������� ����������
//  -2 - ���� �� ������

 extern int GetNumbFln(char *PathStr,char *KeyWord);
// �������� ����� ������ �� �����

 extern int GetNumbMaskFile(char *PathStr,char *KeyWord,
                            int NumbFln,int *Ndi,FILETIME *Ft);
// �������� ������ ������ ���� REGnnnn � ����������� PathStr 
// KeyWord - ��. ����� �����������,
// NumbDir - ������ ������� Ndi;

 extern int GetListFln(char *PathStr,char **FileList);
// �������� ������ ������ �� �����

 extern LPSTR GetWordN(LPSTR str,LPSTR sto,int n);
// ������� �� ������ ����� � ������� n
// ���������� "" ���� ������ ���������

 extern void xname(LPSTR fln,LPSTR str,int n,LPSTR ext,int m);
// ����������� ��� ����� ����: [str]nnnn.[ext][mmm]
// (��������: 1024.han, a256.e1 � �.�.)

 extern void cname(char *fln,float fr, int n, char c);
// ��������� ��������� ��� ����� ����: xxcxxxx.enn
//  x   - �����, ������������ �������� fr (� ���������
//        �� pr ������) � n (�� ���� ������)
//  c   - ��������� ��������� ���������� �����
}

