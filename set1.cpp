#include <cctype>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

template<typename T>
ostream& operator<<(ostream &o, const vector<T>& vec) {
	for (auto x: vec) {
		o << x;
	}
	return o;
}

using Byte = uint8_t;
using Bytes = vector<Byte>;

constexpr char kHexTable[] = "0123456789abcdef";
constexpr char kB64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr char kB64Padding = '=';

inline
Byte hexToDecimal(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	}
	cerr << "Error! Char is is not hex digit: " << ch << endl;
	return (1 << 4);
}

// Assumes correct string(even length, hex digits)
Bytes hexToBytes(const string& hexString) {
	Bytes result;

	int hexLength = hexString.length();

	for (int i = 1; i < hexLength; i += 2) {
		result.push_back(
			(hexToDecimal(hexString[i - 1]) << 4) | hexToDecimal(hexString[i]));
	}

	return result;
}

string bytesToHex(const Bytes& bytes) {
	string result;

	for (auto byte: bytes) {
		result.push_back(kHexTable[byte >> 4]);
		result.push_back(kHexTable[byte & 0x0F]);
	}

	return result;
}

string bytesToBase64(const Bytes& bytes) {
	string result;

	int numBytes = bytes.size();

	for (int i = 0; i < numBytes; i += 3) {
		uint32_t chunk = (bytes[i] >> 2);

		result.push_back(kB64Table[chunk]);

		chunk = ((bytes[i] & 3) << 4);

		if (i + 1 < numBytes) {
			chunk |= (bytes[i + 1] >> 4);
			result.push_back(kB64Table[chunk]);

			chunk = ((bytes[i + 1] & 0x0F) << 2);

			if (i + 2 < numBytes) {
				chunk |= (bytes[i + 2] >> 6);
				result.push_back(kB64Table[chunk]);

				chunk = (bytes[i + 2] & 0x3F);
				result.push_back(kB64Table[chunk]);
			} else {
				result.push_back(kB64Table[chunk]);
				result.push_back(kB64Padding);
			}
		} else {
			result.push_back(kB64Table[chunk]);
			result.push_back(kB64Padding);
		}
	}

	return result;
}

inline
Bytes xorBytes(const Bytes& b1, const Bytes& b2) {
	Bytes result;
	int len = min(b1.size(), b2.size());

	for (int i = 0; i < len; i += 1) {
		result.push_back(b1[i] ^ b2[i]);
	}
	return result;
}

inline
Bytes xorBytes(const Bytes &bytes, Byte xorByte) {
	Bytes result;
	for (auto byte: bytes) {
		result.push_back(byte ^ xorByte);
	}
	return result;
}

inline
bool isAlpha(char ch) {
	if (ch >= 'a' && ch <= 'z') {
		return true;
	}
	if (ch >= 'A' && ch <= 'Z') {
		return true;
	}
	return false;
}

inline
bool isVowel(char ch) {
	switch (ch) {
	case 'a': case 'e': case 'i': case'o': case 'u':
		return true;
	}
	return false;
}



double englishScore(const Bytes& text) {
	constexpr double letterFrq[] = {
		8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
		0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
		2.758, 0.978, 2.361, 0.150, 1.974, 0.074
	};
	constexpr double spaceFrq = 19.18182;

	double score = 0.0;

	for (auto ch: text) {
		if (isAlpha(ch)) {
			score += letterFrq[tolower(ch) - 'a'];
		} else if (ch == ' ') {
			score += spaceFrq;
		}
	}
	return score;
}


void challange1() {
	static const string test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	static const string expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

	Bytes bytes = hexToBytes(test);
	// cout << bytes << endl;

	string result = bytesToBase64(bytes);
	// cout << result << endl;

	cout << "Challange 1 ";

	if (result == expected) {
		cout << "passed!" << endl;
	} else {
		cout << "failed!" << endl;
	}
}

void challange2() {
	static const string s1 = "1c0111001f010100061a024b53535009181c";
	static const string s2 = "686974207468652062756c6c277320657965";
	static const string expected = "746865206b696420646f6e277420706c6179";

	Bytes b1 = hexToBytes(s1);
	Bytes b2 = hexToBytes(s2);

	Bytes resultBytes = xorBytes(b1, b2);

	string resultHex = bytesToHex(resultBytes);

	// cout << resultHex << endl;

	cout << "Challange 2 ";

	if (resultHex == expected) {
		cout << "passed!" << endl;
	} else {
		cout << "failed!" << endl;
	}
}

void challange3() {
	static const string input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

	Bytes result;
	double bestScore = -1;

	Bytes bytes = hexToBytes(input);

	for (int i = 0; i < numeric_limits<uint8_t>::max(); i += 1) {
		Bytes xored = xorBytes(bytes, i);
		double score = englishScore(xored);
		if (score > bestScore) {
			bestScore = score;
			result = move(xored);
		}
	}

	cout << result << endl;
}

int main() {
	challange1();
	challange2();
	challange3();
	return 0;
}
