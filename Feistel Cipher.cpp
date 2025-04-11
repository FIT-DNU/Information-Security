#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

void encBlock(unsigned int* v, unsigned int* w, unsigned int* k)
{
	register unsigned int v0 = v[0], v1 = v[1], i, sum = 0;
	// Magic constant: 2^32/(golden ratio) in HEX for key scheduling
	register unsigned int delta = 0x9E3779B9;
	for (i = 0; i < 32; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
		sum += delta;	// increment the cumulitive delta
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
	}
	w[0] = v0; w[1] = v1;
}

void encipher(const std::string& str, const std::string& key, std::string* out)
{
	unsigned int v[2], w[2], k[4], keyBuff[4];

	// Set array values to 0 (windows sets them to default values)
	memset(v, 0, sizeof(v));
	memset(w, 0, sizeof(w));
	memset(k, 0, sizeof(k));
	memset(keyBuff, 0, sizeof(keyBuff));

	// Check key size, store in key buffer and divide it into 4 subkeys
	int len = key.length();
	if (len > 16)
		len = 16;
	memcpy(keyBuff, key.c_str(), len);
	for (int i = 0; i < 4; ++i)	// ++i more efficient
		k[i] = keyBuff[i];	// store each of the 4 parts of key into array k[]

	// Copy input string to a buffer of size multiple of 4
	int strBuffLen = str.length();
	if (strBuffLen == 0)
		return;
	if ((strBuffLen % 4) > 0)
		strBuffLen += 4 - (strBuffLen % 4);
	unsigned char* strBuff = new unsigned char[strBuffLen];

	// Initialize and copy input string into the string buffer
	memset(strBuff, 0, strBuffLen);
	memcpy(strBuff, str.c_str(), str.length());

	// Encode
	out->clear();
	v[1] = 0;
	for (int i = 0; i < strBuffLen; i += 4)
	{
		v[0] = *(unsigned int*)&strBuff[i];
		encBlock(&v[0], &w[0], &k[0]);	// encode one block with a piece of given string
		out->append((char*)&w[0], 4);	// add encoded part to output string
		v[1] = w[1];
	}
	out->append((char*)&v[1], 4);
	delete[] strBuff;
}
void decBlock(unsigned int* v, unsigned int* w, unsigned int* k)
{
	register unsigned int v0 = v[0], v1 = v[1], i, sum = 0xC6EF3720; // initial sum derived from TEA algorithm definition
	register unsigned int delta = 0x9E3779B9;
	for (i = 0; i < 32; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
		sum -= delta;	// decrement the cumulitive delta
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
	}
	w[0] = v0; w[1] = v1;
}

void decipher(const std::string& str, const std::string& key, std::string* out)
{
	unsigned int v[2], w[2], k[4], keyBuff[4];

	// Set array values to 0 (windows set them to default values)
	memset(v, 0, sizeof(v));
	memset(w, 0, sizeof(w));
	memset(k, 0, sizeof(k));
	memset(keyBuff, 0, sizeof(keyBuff));

	// Calculate number of blocks and passes
	int numBlocks = str.length() / 4;
	int numPasses = numBlocks - 1;

	if (numPasses <= 0)
		return;

	// Check key size, store in key buffer and divide it into 4 subkeys
	int len = key.length();
	if (len > 16)
		len = 16;
	memcpy(keyBuff, key.c_str(), len);
	for (int i = 0; i < 4; ++i)
		k[i] = keyBuff[i];

	// Make buffer to store ouput
	unsigned char* temp_buffer = new unsigned char[(numPasses + 1) * 4 ];
	memset(temp_buffer, 0, numPasses * 4 + 4);

	// Decode
	out->clear();
	const char* p = str.c_str();
	v[1] = *(unsigned int*)&p[numPasses * 4];
	for (int i = 0; i < numPasses; ++i)
	{
		v[0] = *(unsigned int*)&p[(numPasses - i - 1) * 4];
		decBlock(&v[0], &w[0], &k[0]);	// encode one block with a piece of given string
		*(unsigned int*)&temp_buffer[(numPasses - i - 1) * 4] = w[0];
		v[1] = w[1];
	}
	out->assign((char *)temp_buffer, numPasses * 4);
	delete[] temp_buffer;
}

int main()
{
	ifstream fileIn;
	ofstream fileOut;

	string f1;			// holds key
	string *key = &f1;	// pointer to key
	string str;			// holds user input to encipher
	string outdata;		// holds enciphered text
	string decoded;		// holds deciphered text
	char choice;		// holds user menu choice

	// Delete older files
	remove("cipher.fstl");
	remove("output.txt");

	// MENU
	do {
		cout << "__________\n";
		cout << "1: Encrypt\n2: Decrypt\n3: Exit\n";
		cout << "__________\n";
		cin >> choice;

		switch (choice) {
		case '1': {
			cout << "Enter the encryption Key" << endl;
			cin.ignore();
			getline(cin, *key);
			cout << "Enter the string to encrypt" << endl;
			getline(cin, str);

			// Encipher and display
			encipher(str, f1, &outdata);
			str = "0";	// clear string
			cout << "\nCiphertext: " << outdata << endl;

			// Write ciphertext to cipher.fstl
			fileOut.open("cipher.fstl", ios::out | ios::binary);
			if (fileOut.good())
			{
				fileOut.write(outdata.c_str(), outdata.size());
				fileOut.close();
				cout << "Ciphertext saved in cipher.fstl\n" << endl;
			}
			else
				cout << "Error opening file for writing\n";
			system("pause");

			break;
		}
		case '2': {
			cout << "Enter the decryption Key" << endl;
			cin.ignore();
			getline(cin, *key);

			// Open file "cipher.fstl" and read the ciphertext into variable ciphertext
			cout << "Opening cipher.fstl to read ciphertext..." << endl;
			fileIn.open("cipher.fstl", ios::in | ios::binary);
			string ciphertext((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
			fileIn.close();

			// Decipher and display
			decipher(ciphertext, f1, &decoded);
			ciphertext = "0";
			cout << "Decoded: " << decoded << endl;

			// Write decoded text to output.txt
			fileOut.open("output.txt", ios::out | ios::binary);
			if (fileOut.good())
			{
				fileOut.write(decoded.c_str(), decoded.size());
				fileOut.close();
				cout << "Decoded message saved in output.txt\n" << endl;
			}
			else
				cout << "Error opening file for writing\n";
			system("pause");

			break;
		}
		case '3': {
			exit(1);
		}
		default: cout << "Please choose 1, 2 or 3\n";
		}
	} while (choice != 3);
}
