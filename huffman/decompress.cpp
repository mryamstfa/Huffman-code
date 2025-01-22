#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class cnodetwo 
{
public:
    unsigned char byte;
    string bin;
    cnodetwo* pnext;

    cnodetwo() : byte(0), bin(""), pnext(nullptr) {}
};


class clisttwo 
{
public:
    cnodetwo* phead;

    clisttwo() : phead(nullptr) {}
    ~clisttwo() 
    {
        cnodetwo* ptrav = phead;
        while (ptrav) 
        {
            cnodetwo* temp = ptrav;
            ptrav = ptrav->pnext;
            delete temp;
        }
    }

    void attachtwo(cnodetwo* pnn) 
    {
        if (!phead) 
        {
            phead = pnn;
        }
        else {
            cnodetwo* ptrav = phead;
            while (ptrav->pnext) 
            {
                ptrav = ptrav->pnext;
            }
            ptrav->pnext = pnn;
        }
    }
};

void loadlistfromfile(const string& filename, clisttwo& mylisttwo)
{
    ifstream infile(filename); 
    if (!infile.is_open()) 
    {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    mylisttwo = clisttwo(); 

    int byteInt; 
    string bin;
    while (infile >> byteInt >> bin) 
    { // byte = integer , bin = string
        cnodetwo* newNode = new cnodetwo();
        newNode->byte = static_cast<unsigned char>(byteInt); //integer safely converted to  single byte
        newNode->bin = bin;
        mylisttwo.attachtwo(newNode);
    }

    infile.close();
   
}

void decompressFile(const string& compressedFile, const string& outputFile, clisttwo& mylisttwo) 
{
    ifstream readcompressedfile(compressedFile, ios::binary);
    ofstream outputFileTWO(outputFile, ios::binary);

    if (!readcompressedfile.is_open() || !outputFileTWO.is_open()) 
    {
        cout << "Error opening file!" << endl;
        return;
    }

    
    char header[54];
    readcompressedfile.read(header, 54);
    outputFileTWO.write(header, 54);

    string binaryString;
    unsigned char ch;
    while (readcompressedfile.read((char*)&ch, sizeof(ch))) 
    {
        for (int i = 7; i >= 0; i--) 
        {
            binaryString += ((ch >> i) & 1) ? '1' : '0';
        }
    }

    string currentCode;
    for (char bit : binaryString) 
    {
        currentCode += bit;
        cnodetwo* ptr = mylisttwo.phead;
        while (ptr) 
        {
            if (ptr->bin == currentCode) 
            {
                outputFileTWO.write((char*)&ptr->byte, sizeof(ptr->byte));
                currentCode.clear();
                break;
            }
            ptr = ptr->pnext;
        }
    }

    readcompressedfile.close();
    outputFileTWO.close();
   
}

int main() 
{
    clisttwo mylisttwo;

   
    loadlistfromfile("mylisttwo.txt", mylisttwo);

 
    decompressFile("compressed.bin", "decompressed.bmp", mylisttwo);

    cout << "finished";

    return 0;
}