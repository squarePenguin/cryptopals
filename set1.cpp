#include <cstdint>
#include <iostream>
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

constexpr char kHexTable[] = "0123456789ABCDEF";
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


void challange1() {
	static const string test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

	Bytes bytes = hexToBytes(test);
	cout << bytes << endl;

	string result = bytesToBase64(bytes);
	cout << result << endl;
}

int main() {
	challange1();
	return 0;
}
