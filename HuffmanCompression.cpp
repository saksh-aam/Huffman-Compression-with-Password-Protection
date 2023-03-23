#include <bits/stdc++.h>
#include "Utilities.hpp"
#include "RSA_Cryptosystem.hpp"
#define ll long long int
#define boost ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)


class Node
{
public:
    // data represents the freq of character letter
    ll data;
    char letter;
    Node *left;
    Node *right;

    Node(ll data, char letter)
    {
        this->data = data;
        this->letter = letter;
        left = NULL;
        right = NULL;
    }
};

class HuffmanCompression
{
public:
    Node *Treemaker(unordered_map<char, ll> &frequency)
    {
        // maintaining the priority queue to get the node with least data value
        priority_queue<pair<ll, Node *>, vector<pair<ll, Node *>>, greater<pair<ll, Node *>>> min_heap;

        for (auto &itr : frequency)
        {
            Node *temp = new Node(itr.second, itr.first);
            // adding all the nodes in the min_heap
            min_heap.push({itr.second, temp});
        }

        // removing the top 2 nodes of the heap till root node of the tree is left in the heap
        while (min_heap.size() > 1)
        {
            Node *left = min_heap.top().second;
            min_heap.pop();
            Node *right = min_heap.top().second;
            min_heap.pop();

            Node *newnode = new Node(left->data + right->data, '~');
            newnode->left = left;
            newnode->right = right;
            min_heap.push({newnode->data, newnode});
        }
        return min_heap.top().second;
    }

    // function to encode each character to binary string of 0s and 1s 
    void dfsTraversal(Node *root, string s, unordered_map<char, string> &char_mapped_encodedbits)
    {
        if (!root)
        {
            return;
        }
        if (root->letter != '~')
        {
            char_mapped_encodedbits[root->letter] = s;
        }
        else
        {
            dfsTraversal(root->left, s + "0", char_mapped_encodedbits);
            dfsTraversal(root->right, s + "1", char_mapped_encodedbits);
        }
    }
};

int main(int argc, char *argv[])
{
    boost;

    //  open the input file to read
    FILE *inputFile = fopen(argv[1], "r");

    //  if no file is provided as input-> throw the error and return
    if (inputFile == NULL)
    {
        cout << "Error opening input file\n";
        return 1;
    }

    //  instantiating the Utils object to get the file details like name and file type
    Utils fileDetails;
    fileDetails.setter(argv[1]);

    string outputFileName = fileDetails.fileName + "-compressed.bin";

    // creating and opening the output file to write in binary format
    FILE *outputFile = fopen(outputFileName.c_str(), "wb");

    //  if no output file was created ->  throw the error and return
    if (outputFile == NULL)
    {
        cout << "Error creating output file\n";
        return 2;
    }

    // if numeric password was used for security of file
    if (argv[2] != NULL)
    {
        string password = argv[2];

        // encoding the password using RSA cryptography to store in output file
        RSA encoder(0ll, 0ll);
        encoder.encrypt(stoll(password));
        string encodedString = to_string(encoder.cipher) + "-" + to_string(encoder.p) + "-" + to_string(encoder.q);
        fwrite(encodedString.c_str(), encodedString.size() + 1, 1, outputFile);
    }
    else
    {
        // adding indicator in the output file if password was used while compressing
        string content = "NotEncrypted";
        fwrite(content.c_str(), content.size() + 1, 1, outputFile);
    }

    char buff[1];
    unordered_map<char, ll> frequency;
    string inputString = "", binaryData = "";

    // reading char by char from input file and maintaining their frequency of occurence
    while (fread(buff, 1, 1, inputFile))
    {
        frequency[buff[0]]++;
        inputString += buff[0];
    }
    // close the input file to free the resources
    fclose(inputFile);

    // instantiating the HuffmanCompression object
    HuffmanCompression compressor;
    // constructing the tree 
    Node *tree = compressor.Treemaker(frequency);

    unordered_map<char, string> char_mapped_encodedbits;
    string s = "";

    // storing the key-value pair of encoding in char_mapped_encodedbits
    compressor.dfsTraversal(tree, s, char_mapped_encodedbits);

    // taking the respected binary string of each char 
    for (int i = 0; i < inputString.size(); i++)
        binaryData += char_mapped_encodedbits[inputString[i]];

    // calcuating padding so the size of the file during reading is divisible by 8, so we can take out 1 byte at a time
    ll padding = 0;
    while ((binaryData.size()) % 8)
    {
        binaryData.push_back('\0');
        padding++;
    }

    // first of all adding the concatenated string of key-value pair
    for (auto itr : char_mapped_encodedbits)
    {
        string code = itr.first + itr.second;
        fwrite(code.c_str(), code.size() + 1, 1, outputFile);
        // code.size()+1 for NULL character in the end
    }

    // writing one extra Null charcter to indicate encodes have ended
    char nullChar = '\0';
    fwrite(&nullChar, 1, 1, outputFile);

    // writing padding
    char paddingString = padding + '0';
    fwrite(&paddingString, 1, 1, outputFile);

    // writing null char
    fwrite(&nullChar, 1, 1, outputFile);

    // creating series of bytes from binaryData and writing it to output file
    for (int i = 0; i < binaryData.size(); i += 8)
    {
        int bit_pos = 7;
        unsigned char byte = 0b00000000;
        for (int j = i; j < i + 8; j++, bit_pos--)
        {
            if (binaryData[j] == '1')
                byte |= (1 << bit_pos);
        }
        buff[0] = byte;
        fwrite(buff, 1, 1, outputFile);
    }

    cout << "File Compressed!!" << endl;
    // calcuating the total time used to compress the file
    cerr << fixed << setprecision(10);
    cerr << "Time taken to Compress: " << (float)clock() / CLOCKS_PER_SEC << " secs" << endl;
   
    return 0;
}