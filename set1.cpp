#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
ostream& operator<<(ostream &o, const vector<T>& vec) {
	o << '[';
	for (auto x: vec) {
		o << x << ' ';
	}
	o << ']';
	return o;
}

using Byte = uint8_t;
using Bytes = vector<Byte>;

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

Bytes hexToBytes(const string& hexString) {
	Bytes result;

	int hexLength = hexString.length();
	int startIndex = 0;

	if (hexLength & 1) {
		// We have odd number of hex digits. We pad with one leading 0.
		result.push_back(hexToDecimal(hexString[0]));
		startIndex = 1;
	}

	for (int i = startIndex; i < hexLength; i += 2) {
		result.push_back(
			(hexToDecimal(hexString[i]) << 4) | hexToDecimal(hexString[i + 1]));
	}
	return result;
}

string bytesToBase64(const Bytes& bytes) {
	string result;

	int numBytes = bytes.size();
	int startIndex = 0;

	if (numBytes % 3 != 0) {
		// Wee need 3 bytes for 4 b64 digits. We pad as needed.
		uint32_t firstChunk = 0;
		for (int i = 0; i < numBytes % 3; i += 1) {
			firstChunk = (firstDigit << 8) | bytes[i];
		}
		startIndex = numBytes % 3;
	}

	for (int i = startIndex; i < numBytes; i += 3) {
		uint32_t chunk = (numBytes[i] << 16) | (numBytes[i + 1] << 8) | numBytes[i + 2];

	}
}


void challange1() {
	static const string test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

	Bytes bytes = hexToBytes(test);
	cout << bytes << endl;
}

int main() {
	challange1();
	return 0;
}
