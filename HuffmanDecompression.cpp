#include <bits/stdc++.h>
#include "Utilities.hpp"
#include "RSA_Cryptosystem.hpp"
#define ll long long int
#define boost ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)

// converts a byte into binary string of 0s and 1s for decoding
string byte_to_encode(unsigned char n)
{
    string ans = "";
    while (n > 0)
    {
        if (n & 1)
            ans.push_back('1');
        else
            ans.push_back('0');
        n >>= 1;
    }

    while (ans.size() < 8)
        ans.push_back('0');
    reverse(ans.begin(), ans.end());
    return ans;
}

int main(int argc, char *argv[])
{
    boost;

    //  open the compressed input file to read in binary format
    FILE *inputFile = fopen(argv[1], "rb");

    //  if no file is provided as input-> throw the error and return
    if (inputFile == NULL)
    {
        cout << "Input File Could Not be Opened" << endl;
        return 0;
    }
    else
    {
        //  instantiating the Utils object to get the file details like name and file type
        Utils fileDetails;
        fileDetails.setter(argv[1]);

        auto pos = fileDetails.fileName.find("-");
        string outputFileName = fileDetails.fileName.substr(0, pos) +"-decompressed.txt";

        char buff[1];

        // reading password set during compression
        string encodedString = "";

        // vector contins 3 elements - {cipertext, p, q}
        vector<string> cipher;
        ll hifenCnt = 0;
        fread(buff, 1, 1, inputFile);
        while (buff[0] != '\0')
        {
            if (buff[0] == '-')
            {
                cipher.push_back(encodedString);
                encodedString = "";
                fread(buff, 1, 1, inputFile);
                continue;
            }
            encodedString += buff[0];
            fread(buff, 1, 1, inputFile);
        }
        cipher.push_back(encodedString);

        //  checking whether the file was compressed using password
        if (encodedString != "NotEncrypted")
        {
            // if password not provided -> throw error and return
            if (argv[2] == NULL)
            {
                cout << "File is protected with a Password! Make sure to provide it." << endl;
                return 1;
            }
            // decoding cipher in the file
            RSA decoder(stoll(cipher[1]), stoll(cipher[2]));
            decoder.decrypt(stoll(cipher[0]));
            string decryptedPassword = to_string(decoder.plain);

            // if password not provided is incorrect -> throw error and return
            if (argv[2] != decryptedPassword)
            {
                cout << "Incorrect Password. Try Again!!" << endl;
                return 2;
            }
        }

        // creating and opening the output file to write
        FILE *outputFile = fopen(outputFileName.c_str(), "w");
        if (outputFile == NULL)
        {
            cout << "Output File Could Not be Opened" << endl;
            return 0;
        }

        // reading the encoding till  two consecutive NULL characters occurs
        unordered_map<string, char> decodedbits_mapped_char;
        int null = 1;
        string encodes = "";
        while (null < 2)
        {
            fread(buff, 1, 1, inputFile);
            if (buff[0] == '\0')
            {
                if (encodes.size() > 0)
                    decodedbits_mapped_char[encodes.substr(1)] = encodes[0],
                    encodes = "";
                null++;
            }
            else
            {
                encodes.push_back(buff[0]);
                null = 0;
            }
        }

        // reading total padding
        fread(buff, 1, 1, inputFile);
        int padding = buff[0] - '0';
        fread(buff, 1, 1, inputFile);

        // now, the main file
        string binaryData = "";
        while (fread(buff, 1, 1, inputFile))
        {
            string num = byte_to_encode(buff[0]);
            binaryData += num;
        }

        // removing padded characters
        while (padding-- > 0)
            binaryData.pop_back();

        // writing the decoded character to output file
        string temp = "";
        for (int i = 0; i < binaryData.size(); i++)
        {
            temp.push_back(binaryData[i]);
            if (decodedbits_mapped_char.find(temp) != decodedbits_mapped_char.end())
            {
                fwrite(&decodedbits_mapped_char[temp], 1, 1, outputFile);
                temp = "";
            }
        }

        fclose(inputFile);
        fclose(outputFile);

        cout << "File Decompressed!!" << endl;
        // calcuating the total time used to decompress the file
        cerr << fixed << setprecision(10);
        cerr << "Time taken to decompress: " << (float)clock() / CLOCKS_PER_SEC << " secs" << endl;
    }

    return 0;
}