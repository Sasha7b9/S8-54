#pragma once
#include "SCPI/MacrosesSCPI.h"

/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/

class ParameterDouble;
class String;
class SimpleMessage;

typedef const char *(*FuncSCPI)(pchar);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(String *);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    const char *key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    const char *hint;

    FuncHint funcHint;

    bool IsEmpty() const { return key[0] == '\0'; }; //-V2563
    bool IsNode() const { return strct != nullptr; };   // ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    // ��������� �������� "������" ������, ����� ��������� ������� func
};


namespace SCPI
{
    // ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(pchar buffer, uint length);

    void Update();
    // ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(pchar *bufer);
    // ������� �����
    void SendAnswer(pchar message);
    // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    // ����� ���������� nullptr.
    pchar BeginWith(pchar buffer, pchar word);
    // ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    void ProcessHint(String *message, pString names[]); //-V2504
};
