#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <bitset>

using namespace std;


int len(unsigned char *str) {
    int i = -1;
    while(str[++i] != '\0') {}
    return i;
}


bool compareStr(unsigned char *a, unsigned char *b) {
    if(len(a) == len(b)) {
        for(int i = 0; i < len(a); ++i) {
            if(a[i] != b[i]) return false;
        }
        return true;
    }
    return false;
}


unsigned int toInt(unsigned int *bin) {
    int erg = 0;
    int pot = 1;
    for(int i = 7; i >=0; --i) {
        erg += bin[i]*pot;
        pot *= 2;
    }
    return erg;
}


vector<unsigned char> maskInput(vector<unsigned char> msg, vector<unsigned char> key) {
    vector<unsigned char> masked;
    int a, b, a1, b1;
    unsigned int res[8];
    for(int i = 0; i < msg.size(); ++i) {
        for(int j = 7; j >= 0; --j) {
            a1 = bitset<8>((unsigned int)msg[i])[j];
            b1 = bitset<8>((unsigned int)key[i%key.size()])[j];
            a = (!a1 && b1);
            b = (a1 && !b1);
            res[8-(j+1)] = (a || b);
        }
        masked.push_back((unsigned char)toInt(res));
    }
    return masked;
}


void decmat_turbocrypt(unsigned char *file, unsigned char *key) {
    // Hier wird der Dateityp bestimmt
    int fileLen = len(file);
    int index = fileLen;
    vector<unsigned char> fileType;
    fileType.push_back('\0');
    while(file[--index] != '.') {
        fileType.push_back(file[index]);
    }
    unsigned char type[fileType.size()];
    for(int i = 0; i < fileType.size(); ++i) {
        type[i] = fileType[fileType.size()-(i+1)];
    }

    if(compareStr(type, (unsigned char*)"mencpp")) {
        // Hier wird der Dateiname bestimmt
        vector<unsigned char> fileName;
        index = -1;
        while(file[++index] != '.') {
            fileName.push_back(file[index]);
        }
        fileName.push_back('\0');
        unsigned char name[fileName.size()];
        for(int i = 0; i < fileName.size(); ++i) {
            name[i] = fileName[i];
        }

        ifstream input((const char*)file, ios::binary );
        vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});

        vector<unsigned char> oldFileType;
        index = buffer.size();
        oldFileType.push_back('\0');
        while(buffer[--index] != '.') {
            oldFileType.push_back(buffer[index]);
        }
        unsigned char oFileType[oldFileType.size()];
        for(int i = 0; i < oldFileType.size(); ++i) {
            oFileType[i] = oldFileType[oldFileType.size()-(i+1)];
        }

        vector<unsigned char> newName;
        for(int i = 0; i < len(name); ++i) {
            newName.push_back(name[i]);
        }
        newName.push_back((unsigned char)'.');
        for(int i = 0; i < len(oFileType); ++i) {
            newName.push_back(oFileType[i]);
        }
        newName.push_back((unsigned char)'\0');
        unsigned char nName[newName.size()];
        for(int i = 0; i < newName.size(); ++i) {
            nName[i] = newName[i];
        }

        ofstream output((const char*)nName, ios::binary );

        vector<unsigned char> keyMask;
        for(int i = 0; i < len(key); ++i) {
            keyMask.push_back(key[i]);
        }

        vector<unsigned char> primes;
        int counter;
        primes.push_back(2);
        for(int i = 3; i < 10001; ++i) {
            counter = 0;
            for(int j = 2; j < i; ++j) {
                if(i%j == 0) counter++;
            }
            if(counter == 0) primes.push_back((unsigned char)i);
        }

        vector<unsigned char> buffer2;
        for(int i = 0; i < buffer.size()-(len(oFileType)+1); ++i) {
            buffer2.push_back(buffer[i]);
        }

        // Maske 1
        vector<unsigned char> masked1 = maskInput(buffer2, primes);

        // Maske 2
        vector<unsigned char> masked2 = maskInput(masked1, keyMask);

        for(int i = 0; i < masked2.size(); ++i) {
            output<<masked2[i];
        }
        output.close();

    } else {
        cout<<"Du kannst nur .mencpp Dateien entschlüsseln!";
    }
}


void encmat_turbocrypt(unsigned char *file, unsigned char *key) {
    // Hier wird der Dateityp bestimmt, bis Z.38
    int fileLen = len(file);
    int index = fileLen;
    vector<unsigned char> fileType;
    fileType.push_back('\0');
    while(file[--index] != '.') {
        fileType.push_back(file[index]);
    }
    unsigned char type[fileType.size()];
    for(int i = 0; i < fileType.size(); ++i) {
        type[i] = fileType[fileType.size()-(i+1)];
    }

    // Hier wird der Dateiname bestimmt, bis Z.50
    vector<unsigned char> fileName;
    index = -1;
    while(file[++index] != '.') {
        fileName.push_back(file[index]);
    }
    fileName.push_back('\0');
    unsigned char name[fileName.size()];
    for(int i = 0; i < fileName.size(); ++i) {
        name[i] = fileName[i];
    }

    vector<unsigned char> newName;
    for(int i = 0; i < len(name); ++i) {
        newName.push_back(name[i]);
    }
    newName.push_back((unsigned char)'.');
    newName.push_back((unsigned char)'m');
    newName.push_back((unsigned char)'e');
    newName.push_back((unsigned char)'n');
    newName.push_back((unsigned char)'c');
    newName.push_back((unsigned char)'p');
    newName.push_back((unsigned char)'p');
    newName.push_back((unsigned char)'\0');
    unsigned char nName[newName.size()];
    for(int i = 0; i < newName.size(); ++i) {
        nName[i] = newName[i];
    }

    ifstream input((const char*)file, ios::binary );
    ofstream output((const char*)nName, ios::binary );
    vector<unsigned char> buffer(istreambuf_iterator<char>(input), {});
    vector<unsigned char> keyMask;
    for(int i = 0; i < len(key); ++i) {
        keyMask.push_back(key[i]);
    }

    vector<unsigned char> primes;
    int counter;
    primes.push_back(2);
    for(int i = 3; i < 10001; ++i) {
        counter = 0;
        for(int j = 2; j < i; ++j) {
            if(i%j == 0) counter++;
        }
        if(counter == 0) primes.push_back((unsigned char)i);
    }

    // Maske 1
    vector<unsigned char> masked1 = maskInput(buffer, keyMask);

    // Maske 2
    vector<unsigned char> masked2 = maskInput(masked1, primes);

    for(int i = 0; i < masked2.size(); ++i) {
        output<<masked2[i];
    }
    output<<"."<<type;
    output.close();
}


int main(int argc, unsigned char **argv)
{
    if(argc == 4 && compareStr(argv[1], (unsigned char*)"-e")) {
        encmat_turbocrypt(argv[2], argv[3]);
    } else if(argc == 4 && compareStr(argv[1], (unsigned char*)"-d")) {
        decmat_turbocrypt(argv[2], argv[3]);
    } else if(argc != 4) {
        cout<<"Es fehlen Parameter! Aufbau:\nmencpp3 (-e/-d) [file] [key]";
    }
    return 0;
}
