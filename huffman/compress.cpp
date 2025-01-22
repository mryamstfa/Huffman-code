#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class ctree
{
public:
    int info;
    unsigned char byte;
    ctree* pright;
    ctree* pleft;

    ctree()
    {
        info = 0;
        byte = 0;
        pright = nullptr;
        pleft = nullptr;
    }
    ~ctree()
    {
        if (pright)
        {
            delete pright;
        }
        if (pleft)
        {
            delete pleft;
        }
    }
};

class cnode
{
public:
    int info;
    unsigned char byte;
    cnode* pnext;
    ctree* pdown;

    cnode()
    {
        info = 0;
        byte = 0;
        pnext = nullptr;
        pdown = nullptr;
    }
    ~cnode()
    {
        if (pdown)
        {
            delete pdown;
        }
    }
};

class clist
{
public:
    cnode* phead;

    clist()
    {
        phead = nullptr;
    }
    ~clist()
    {
        cnode* ptrav = phead;
        while (ptrav)
        {
            cnode* temp = ptrav;
            ptrav = ptrav->pnext;
            delete temp;
        }
    }

    void attach(cnode* pnn)
    {
        if (!phead || pnn->info < phead->info)
        {
            pnn->pnext = phead;
            phead = pnn;
        }
        else
        {
            cnode* ptrav = phead;
            while (ptrav->pnext && ptrav->pnext->info <= pnn->info)
            {
                ptrav = ptrav->pnext;
            }
            pnn->pnext = ptrav->pnext;
            ptrav->pnext = pnn;
        }
    }
};

class cnodetwo
{
public:
    unsigned char byte;
    string bin;
    cnodetwo* pnext;

    cnodetwo()
    {
        byte = 0;
        bin = "";
        pnext = nullptr;
    }
};

class clisttwo
{
public:
    cnodetwo* phead;

    clisttwo()
    {
        phead = nullptr;
    }
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


ctree* buildHuffmanTree(clist& mylist)
{
    while (mylist.phead && mylist.phead->pnext)
    {
        cnode* pback = mylist.phead;
        cnode* ptrav = mylist.phead->pnext;

        ctree* leftChild = pback->pdown;
        pback->pdown = nullptr;
        if (!leftChild) {
            leftChild = new ctree();
            leftChild->info = pback->info;
            leftChild->byte = pback->byte;
        }

        ctree* rightChild = ptrav->pdown;
        ptrav->pdown = nullptr;
        if (!rightChild) {
            rightChild = new ctree();
            rightChild->info = ptrav->info;
            rightChild->byte = ptrav->byte;
        }

        ctree* pnn = new ctree();
        pnn->info = leftChild->info + rightChild->info;
        pnn->pleft = leftChild;
        pnn->pright = rightChild;

        cnode* pnn2 = new cnode();
        pnn2->info = pnn->info;
        pnn2->pdown = pnn;
        mylist.attach(pnn2);

        mylist.phead = ptrav->pnext;
        delete pback;
        delete ptrav;
    }

    if (mylist.phead)
    {
        return mylist.phead->pdown;
    }
    else
    {
        return nullptr;
    }
}

void generateHuffmanCodes(ctree* proot, string code, clisttwo& mylisttwo)
{
    if (proot == nullptr) return;

    if (proot->pleft == nullptr && proot->pright == nullptr)
    {
        cnodetwo* pnn = new cnodetwo();
        pnn->byte = proot->byte;
        pnn->bin = code;
        mylisttwo.attachtwo(pnn);
    }

    generateHuffmanCodes(proot->pleft, code + "0", mylisttwo);
    generateHuffmanCodes(proot->pright, code + "1", mylisttwo);
}


void compressFile(const string& inputFile, const string& outputFile, clisttwo& mylisttwo)
{
    ifstream readfile(inputFile, ios::binary);
    ofstream outfile(outputFile, ios::binary);

    if (!readfile.is_open() || !outfile.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    char header[54];
    readfile.read(header, 54);
    outfile.write(header, 54);

    string binaryString;
    unsigned char ch;
    while (readfile.read((char*)&ch, sizeof(ch)))
    {
        cnodetwo* ptr = mylisttwo.phead;
        while (ptr)
        {
            if (ptr->byte == ch)
            {
                binaryString += ptr->bin;
                break;
            }
            ptr = ptr->pnext;
        }
    }

    unsigned char byte = 0;
    int bitCount = 0;
    for (char bit : binaryString)
    {
        byte = (byte << 1) | (bit - '0');
        bitCount++;
        if (bitCount == 8)
        {
            outfile.write((char*)&byte, sizeof(byte));
            byte = 0;
            bitCount = 0;
        }
    }

    if (bitCount > 0)
    {
        byte <<= (8 - bitCount);
        outfile.write((char*)&byte, sizeof(byte));
    }

    outfile.close();
    readfile.close();
}


void savelisttofile(const string& filename, clisttwo& mylisttwo)
{
    ofstream outfile(filename); 
    if (!outfile.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    cnodetwo* current = mylisttwo.phead;
    while (current)
    {
        
        outfile << (int)current->byte << " " << current->bin << endl;

       
        cout << "Character: " << current->byte << "| Huffman Code: " << current->bin << endl;

        current = current->pnext;
    }

    outfile.close();
}


int main()
{
    clist mylist;
    clisttwo mylisttwo;

    ifstream readfile("input.bmp", ios::binary);
    if (!readfile.is_open())
    {
        cerr << "Error opening input file!" << endl;
        return 1;
    }

    unsigned char ch;
    while (readfile.read((char*)&ch, sizeof(ch)))
    {
        cnode* ptrav = mylist.phead;
        while (ptrav)
        {
            if (ptrav->byte == ch)
            {
                ptrav->info++;
                break;
            }
            ptrav = ptrav->pnext;
        }
        if (!ptrav)
        {
            cnode* pnn = new cnode();
            pnn->info = 1;
            pnn->byte = ch;
            mylist.attach(pnn);
        }
    }
    readfile.close();

    ctree* huffmanTree = buildHuffmanTree(mylist);

    generateHuffmanCodes(huffmanTree, "", mylisttwo);

   
    cout << "Huffman Codes:" << endl;
    savelisttofile("mylisttwo.txt", mylisttwo);

    compressFile("input.bmp", "compressed.bin", mylisttwo);

    cout << "finished" << endl;

    delete huffmanTree;
    return 0;
}