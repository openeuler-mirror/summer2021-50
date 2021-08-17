#include "Enclave1_t.h"
#include <string.h>
#include <string>
#include <vector>
#include <cstdlib>
#define MAX_LEN 1024

using namespace std;

namespace calc {
	const int L = 16;
	const std::string hexChars = "0123456789abcdef";
	int dec(char ch) {
		if ('0' <= ch && ch <= '9') {
			return ch - '0';
		}
		if ('a' <= ch && ch <= 'f') {
			return ch - 'a' + 10;
		}
		if ('A' <= ch && ch <= 'F') {
			return ch - 'A' + 10;
		}
		return -1;
	}
	std::string hex(unsigned int x) {
		std::string result = "";
		for (int j = 28; j >= 0; j -= 4) {
			result += hexChars[x >> j & 0x0F];
		}
		return result;
	}
	std::string hex(unsigned long long x) {
		std::string result = "";
		for (int j = 60; j >= 0; j -= 4) {
			result += hexChars[x >> j & 0x0F];
		}
		return result;
	}
	std::string bin(unsigned int x) {
		std::string result = "";
		for (int j = 31; j >= 0; j--) {
			result += hexChars[x >> j & 0x01];
		}
		return result;
	}
	std::string bin(unsigned long long x) {
		std::string result = "";
		for (int j = 63; j >= 0; j--) {
			result += hexChars[x >> j & 0x01];
		}
		return result;
	}
	unsigned int rightRotate(unsigned int x, int u) {
		unsigned long long xx = (unsigned long long)x << 32 | x;
		return (unsigned int)((xx >> u)&(unsigned int)-1);
	}
	std::string _format(std::string src, int len = 8) {
		int Len = src.length();
		std::string result = "";
		for (int i = 0; i < Len; i += len) {
			result += (src.substr(i, len) + " \n"[(!((i + 8) & 31))]);
		}
		return result;
	}
	struct bigNum {
		int siz, hexLen;
		std::vector<unsigned long long>num;
		bigNum(int _siz = L) {
			siz = _siz; hexLen = siz << 4;
			num.resize(_siz, 0);
		}
		bigNum(std::string src) {
			int len = src.length(); hexLen = src.length();
			siz = (len + 15) >> 4;
			num.resize(siz, 0);
			for (int i = 0, cnt = 0; i < len; i++, cnt++) {
				num[cnt >> 4] |= (unsigned long long)dec(src[i]) << (((cnt & 0x0F) ^ 15) << 2);
			}
		}
		bigNum FromU32(uint32_t src) {
			bigNum result = bigNum(1);
			result.num[0] = src;
			return result;
		}
		std::string hex() {
			std::string result = "";
			for (int i = 0; i < siz; i++) {
				for (int j = 60; j >= 0; j -= 4) {
					result += hexChars[num[i] >> j & 0x0F];
				}
			}
			result.erase(hexLen);
			return result;
		}
		std::string bin() {
			std::string result = "b";
			for (int i = 0; i < siz; i++) {
				for (int j = 63; j >= 0; j--) {
					result += hexChars[num[i] >> j & 0x01];
				}
			}
			result.erase(hexLen << 2);
			return result;
		}
		bigNum& operator^=(const bigNum &rhs) {
			bigNum result = *this;
			/*while (siz < rhs.siz) {
				++siz;
				num.push_back(0);
			}*/
			int lmt = siz;
			if (rhs.siz < lmt) lmt = rhs.siz;
			for (int i = 0; i < lmt; i++) {
				result.num[i] ^= rhs.num[i];
			}
			return *this = result;
		}
		friend bigNum operator^(const bigNum& lhs, const bigNum& rhs) {
			if(lhs.siz>=rhs.siz) return bigNum(lhs) ^= rhs;
			else return bigNum(rhs) ^= lhs;
		}
		friend bigNum operator||(const bigNum& lhs, const bigNum& rhs) {
			bigNum result = bigNum(bigNum(lhs).hex() + bigNum(rhs).hex());
			return result;
		}
	};
	bigNum sha256(bigNum src) {
		const unsigned int k[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
		unsigned int h[8] = {
			0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
		std::string tmpstr = bigNum(src).hex();
		tmpstr += "8";
		while (tmpstr.length() % 128 != 112) {
			tmpstr += "0";
		}
		tmpstr += hex((unsigned long long)src.hexLen << 2);
		bigNum tmpnum = bigNum(tmpstr);

		for (int i = 0; i < tmpnum.siz; i += 8) {
			unsigned int w[64] = {};
			for (int j = 0; j < 16; j += 2) {
				w[j] = (unsigned int)(tmpnum.num[i + (j >> 1)] >> 32);
				w[j + 1] = (unsigned int)(tmpnum.num[i + (j >> 1)]);
			}
			for (int j = 16; j < 64; j++) {
				unsigned int
					s0 = rightRotate(w[j - 15], 7) ^ rightRotate(w[j - 15], 18) ^ (w[j - 15] >> 3),
					s1 = rightRotate(w[j - 2], 17) ^ rightRotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
				w[j] = w[j - 16] + s0 + w[j - 7] + s1;
			}
			unsigned int t[8] = {};
			for (int j = 0; j < 8; j++) {
				t[j] = h[j];
			}
			for (int j = 0; j < 64; j++) {
				unsigned int S0, S1, ch, maj, tmp1, tmp2;
				S1 = rightRotate(t[4], 6) ^ rightRotate(t[4], 11) ^ rightRotate(t[4], 25);
				ch = (t[4] & t[5]) ^ ((~t[4])&t[6]);
				tmp1 = t[7] + S1 + ch + k[j] + w[j];
				S0 = rightRotate(t[0], 2) ^ rightRotate(t[0], 13) ^ rightRotate(t[0], 22);
				maj = (t[0] & t[1]) ^ (t[0] & t[2]) ^ (t[1] & t[2]);
				tmp2 = S0 + maj;
				t[7] = t[6];
				t[6] = t[5];
				t[5] = t[4];
				t[4] = t[3] + tmp1;
				t[3] = t[2];
				t[2] = t[1];
				t[1] = t[0];
				t[0] = tmp1 + tmp2;
			}
			for (int j = 0; j < 8; j++) {
				h[j] = h[j] + t[j];
			}
		}

		bigNum result = bigNum(4);
		result.num[0] = ((unsigned long long)h[0] << 32) | h[1];
		result.num[1] = ((unsigned long long)h[2] << 32) | h[3];
		result.num[2] = ((unsigned long long)h[4] << 32) | h[5];
		result.num[3] = ((unsigned long long)h[6] << 32) | h[7];
		return result;
	}

	struct smallNum {
		int siz;
		std::vector<unsigned long long>num;
		smallNum(int _siz = 1) {
			siz = _siz;
			num.resize(_siz, 0);
		}
		smallNum(char ch) {
			siz = 1;
			num.push_back(ch - '0');
		}
		smallNum(std::string src) {
			int len = src.length();
			siz = (len + 15) >> 4;
			num.resize(siz, 0);
			for (int i = len - 1, cnt = 0; i >= 0; i--, cnt++) {
				num[cnt >> 4] |= (unsigned long long)dec(src[i]) << ((cnt & 0x0F) << 2);
			}
		}
		smallNum fromBin(std::string src) {
			smallNum result = smallNum();
			int len = src.length();
			if (!len) {
				return smallNum();
			}
			result.siz = (len + 63) >> 6;
			result.num.resize(result.siz, 0);
			for (int i = len - 1, cnt = 0; i >= 0; i--, cnt++) {
				result.num[cnt >> 6] |= (unsigned long long)dec(src[i]) << (cnt & 0x3F);
			}
			return result;
		}
		smallNum(bigNum src) {
			std::string srcstr = src.hex();
			*this = smallNum(srcstr);
		}
		smallNum fromBigNum(bigNum src) {
			std::string srcstr = src.hex();
			return smallNum(srcstr);
		}
		bigNum toBigNum(smallNum src, int minHexLength = 0) {
			std::string srcstr = src.hex();
			return bigNum(srcstr);
		}
		smallNum e() {
			smallNum result = smallNum(1);
			result.num[0] = 1;
			return result;
		}
		smallNum FromU32(uint32_t src) {
			smallNum result = smallNum(1);
			result.num[0] = src;
			return result;
		}
		std::string hex() {
			std::string result = "";
			for (int i = siz - 1; i >= 0; i--) {
				for (int j = 60; j >= 0; j -= 4) {
					result += hexChars[num[i] >> j & 0x0F];
				}
			}
			return result;
		}
		std::string bin() {
			std::string result = "";
			for (int i = siz - 1; i >= 0; i--) {
				for (int j = 63; j >= 0; j--) {
					result += hexChars[num[i] >> j & 0x01];
				}
			}
			return result;
		}
		smallNum& operator^=(const smallNum &rhs) {
			smallNum result = *this;
			/*while (siz < rhs.siz) {
				++siz;
				num.push_back(0);
			}*/
			int lmt = rhs.siz;
			for (int i = 0; i < lmt; i++) {
				result.num[i] ^= rhs.num[i];
			}
			return *this = result;
		}
		friend smallNum operator^(const smallNum& lhs, const smallNum& rhs) {
			if(lhs.siz>rhs.siz) return smallNum(lhs) ^= rhs;
			else return smallNum(rhs) ^= lhs;
		}
		friend bool operator<(const smallNum& lhs, const smallNum& rhs) {
			if (lhs.siz^rhs.siz) {
				return lhs.siz < rhs.siz;
			}
			for (int i = lhs.siz - 1; i >= 0; i--) {
				if (lhs.num[i] ^ rhs.num[i]) {
					return lhs.num[i] < rhs.num[i];
				}
			}
			return 0;
		}
		friend bool operator<=(const smallNum& lhs, const smallNum& rhs) {
			if (lhs.siz^rhs.siz) {
				return lhs.siz < rhs.siz;
			}
			for (int i = lhs.siz - 1; i >= 0; i--) {
				if (lhs.num[i] ^ rhs.num[i]) {
					return lhs.num[i] < rhs.num[i];
				}
			}
			return 1;
		}
		friend bool operator>(const smallNum& lhs, const smallNum& rhs) {
			return smallNum(rhs) < smallNum(lhs);
		}
		friend bool operator>=(const smallNum& lhs, const smallNum& rhs) {
			return smallNum(rhs) <= smallNum(lhs);
		}
		friend bool operator==(const smallNum& lhs, const smallNum& rhs) {
			if (lhs.siz^rhs.siz) {
				return 0;
			}
			for (int i = lhs.siz - 1; i >= 0; i--) {
				if (lhs.num[i] ^ rhs.num[i]) {
					return 0;
				}
			}
			return 1;
		}
		friend bool operator!=(const smallNum& lhs, const smallNum& rhs) {
			if (lhs.siz^rhs.siz) {
				return 1;
			}
			for (int i = lhs.siz - 1; i >= 0; i--) {
				if (lhs.num[i] ^ rhs.num[i]) {
					return 1;
				}
			}
			return 0;
		}
		smallNum& operator+=(const smallNum& rhs) {
			while (siz < rhs.siz) {
				++siz; num.push_back(0ULL);
			}
			unsigned long long carry0 = 0ULL, carry1 = 0ULL;
			for (int i = 0; i < rhs.siz; i++) {
				if (((num[i] >> 1) + (rhs.num[i] >> 1) + (((num[i] & 1ULL) + (rhs.num[i] & 1ULL) + carry0) >> 1)) >> 63) {
					if (i + 1 == siz) {
						++siz; num.push_back(0ULL);
					}
					carry1 = 1ULL;
				}
				num[i] += rhs.num[i] + carry0;
				carry0 = carry1;
				carry1 = 0ULL;
			}
			for (int i = rhs.siz; i < siz; i++) {
				if (((num[i] >> 1) + (((num[i] & 1ULL) + carry0) >> 1)) >> 63) {
					if (i + 1 == siz) {
						++siz; num.push_back(0ULL);
					}
					carry1 = 1ULL;
				}
				num[i] += carry0;
				carry0 = carry1;
				carry1 = 0ULL;
			}
			return *this;
		}
		friend smallNum operator+(const smallNum& lhs, const smallNum&rhs) {
			return smallNum(lhs) += rhs;
		}
		smallNum& operator-=(const smallNum& rhs) {
			std::string lhsBin, rhsBin;
			lhsBin = smallNum(*this).bin();
			rhsBin = smallNum(rhs).bin();
			int lhsLen = (int)lhsBin.length(), rhsLen = (int)rhsBin.length();
			for (int i = 1; i <= rhsLen; i++) {
				lhsBin[lhsLen - i] -= rhsBin[rhsLen - i] ^ 48;
				if (lhsBin[lhsLen - i] < 48) {
					lhsBin[lhsLen - i - 1] -= 1;
					lhsBin[lhsLen - i] = lhsBin[lhsLen - i] + 2;
				}
			}
			int pos = 0;
			while (pos < lhsLen&&lhsBin[(unsigned long)pos] ^ 49) {
				++pos;
			}
			if (pos) {
				lhsBin.erase(0, (unsigned long)pos);
			}
			return *this = smallNum().fromBin(lhsBin);
		}
		friend smallNum operator-(const smallNum& lhs, const smallNum&rhs) {
			return smallNum(lhs) -= rhs;
		}
		smallNum& operator*=(const smallNum& rhs) {
			smallNum result = smallNum(), u = smallNum(*this);
			std::string rhsBin = smallNum(rhs).bin();
			int rhsLen = rhsBin.length();
			for (int i = rhsLen - 1; i >= 0; i--) {
				if (rhsBin[i] & 1) {
					result = result + u;
				}
				u = u + u;
			}
			return *this = result;
		}
		friend smallNum operator*(const smallNum& lhs, const smallNum&rhs) {
			return smallNum(lhs) *= rhs;
		}
		smallNum pow(unsigned long long rhs) {
			smallNum result = smallNum().e(), u = smallNum(*this);
			while (rhs) {
				if (rhs & 1) {
					result *= u;
				}
				u *= u;
				rhs >>= 1;
			}
			return result;
		}
		smallNum& operator%=(const smallNum& rhs) {
			smallNum result = smallNum();
			std::string lhsBin = smallNum(*this).bin();
			int lhsLen = lhsBin.length();
			for (int i = 0; i < lhsLen; i++) {
				result = result + result;
				if (lhsBin[i] & 1) {
					result.num[0] |= 1;
				}
				if (rhs <= result) {
					result = result - rhs;
				}
			}
			return *this = result;
		}
		friend smallNum operator%(const smallNum& lhs, const smallNum&rhs) {
			return smallNum(lhs) %= rhs;
		}
		smallNum pow_mod(unsigned long long rhs, smallNum _mod) {
			smallNum result = smallNum().e(), u = smallNum(*this);
			while (rhs) {
				if (rhs & 1) {
					result = (result*u) % _mod;
				}
				u = (u*u) % _mod;
				rhs >>= 1;
			}
			return result;
		}
		smallNum pow_mod(smallNum rhs, smallNum _mod) {
			smallNum result = smallNum().e(), u = smallNum(*this);
			std::string rhsstr = rhs.bin();
			int rhsLen = rhsstr.length();
			for (int i = rhsLen - 1; i >= 0; i--) {
				if (rhsstr[i] & 1) {
					result = (result*u) % _mod;
				}
				u = (u*u) % _mod;
			}
			return result;
		}
		smallNum fromDec(std::string src) {
			if (src == "") {
				return smallNum();
			}
			smallNum result = smallNum();
			int siz = src.length();
			for (int i = 0; i < siz; i++) {
				result *= smallNum("a");
				result += smallNum(src[i]);
			}
			return result;
		}
	};
	smallNum big2Small(bigNum src) {
		std::string srcstr = src.hex();
		return smallNum(srcstr);
	}
	bigNum small2Big(smallNum src, int minHexLength = 0) {
		std::string srcstr = src.hex();
		return bigNum(srcstr);
	}
}

namespace interface {
	using namespace calc;
	smallNum rsa_d(smallNum rsa_e, smallNum d, smallNum n) {
		return rsa_e.pow_mod(d, n);
	}
}

char tmpresult[512];
char* transferFromString(string src) {
	int siz = src.length();
	if (siz < 512) {
		for (int i = 0; i <= siz; i++) {
			tmpresult[i] = src[i];
		}
	}
	return tmpresult;
}

string transferToString(char* src) {
	string result;
	for (int i = 0; src[i]; i++) {
		result += src[i];
	}
	return result;
}

unsigned int randSeed = 0;

unsigned int getNextRand(unsigned int preRand) {
	if (!preRand) return -1;
	unsigned x = preRand;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

unsigned int getNextRand(unsigned int preRand,int rounds) {
	if (!preRand) return -1;
	unsigned x = preRand;
	for (int i = 0; i < rounds; i++) {
		x = getNextRand(x);
	}
	return x;
}

uint32_t Keys[2];

void sealKeys() {
	Keys[0] ^= getNextRand(12345678, 32);
	Keys[1] ^= getNextRand(87654321, 32);
}

void decryptKeys(uint32_t *keybuf) {
	keybuf[0] = Keys[0] ^ getNextRand(12345678, 32);
	keybuf[1] = Keys[1] ^ getNextRand(87654321, 32);
}

void initKeys(uint32_t *src) {
	randSeed = src[0];
	randSeed ^= getNextRand(randSeed, 16);
	Keys[0] = getNextRand(randSeed, 555); // Ku
	Keys[1] = getNextRand(randSeed, 777); // Kw
	sealKeys();
}

void setKeys(uint32_t *K) {
	memcpy(Keys, K, sizeof Keys);
}

void getKeys(uint32_t *K) {
	memcpy(K, Keys, sizeof Keys);
}

using namespace calc;
string DebugStr;
string Dstr, Nstr, CDstr, NPWstr;

void encryptD() {
	uint32_t Dkeys[2] = {};
	decryptKeys(Dkeys);
	CDstr = (smallNum(Dstr) ^ big2Small(sha256(bigNum().FromU32(Dkeys[0]) || bigNum(Nstr)))).hex();
	Dstr = "";
}

void decryptD() {
	uint32_t Dkeys[2] = {};
	decryptKeys(Dkeys);
	Dstr = (smallNum(CDstr) ^ big2Small(sha256(bigNum().FromU32(Dkeys[0]) || bigNum(Nstr)))).hex();
}

void setCD(char* src, size_t len) {
	char source[MAX_LEN];
	if (len < MAX_LEN) {
		memcpy(source, src, len + 1);
	}
	CDstr = transferToString(source);
}

void setD(char* src, size_t len){
	char source[MAX_LEN];
	if (len < MAX_LEN) {
		memcpy(source, src, len + 1);
	}
	Dstr = transferToString(source);
	encryptD();
}

void setN(char* src, size_t len){
	char source[MAX_LEN];
	if (len < MAX_LEN) {
		memcpy(source, src, len + 1);
	}
	Nstr = transferToString(source);
}

void getCD(char *buf, size_t len) {
	char* result = transferFromString(CDstr);
	if (len > strlen(result)) {
		memcpy(buf, result, strlen(result) + 1);
	}
}

string RSAsource;

void setRSAsource(char* src, size_t len) {
	char source[MAX_LEN];
	if (len < MAX_LEN) {
		memcpy(source, src, len + 1);
	}
	RSAsource = transferToString(source);
}

void getRSAresult(char *buf, size_t len) {
	decryptD();
	string hpw = interface::rsa_d(smallNum(RSAsource), smallNum(Dstr), smallNum(Nstr)).hex();
	uint32_t Dkeys[2] = {};
	decryptKeys(Dkeys);
	string npw = (smallNum(hpw) ^ big2Small(sha256(bigNum().FromU32(Dkeys[0]) || bigNum(Dstr)))).hex();
	char* result = transferFromString(npw);
	Dstr = "";
	if (len > strlen(result)) {
		memcpy(buf, result, strlen(result) + 1);
	}
}

uint32_t _uints[3];

void registerSeed(uint32_t *src){
	randSeed = src[0];
	randSeed ^= getNextRand(randSeed, 16);
	_uints[0] = getNextRand(randSeed, 111) % 2001 + 2000; // x
	_uints[1] = getNextRand(randSeed, 222) % 1001 + 2000; // mu
	_uints[2] = getNextRand(randSeed, 333) % 15 + 1; // k
}

void setNPW(char* src, size_t len) {
	char source[MAX_LEN];
	if (len < MAX_LEN) {
		memcpy(source, src, len + 1);
	}
	NPWstr = transferToString(source);
}

string _xStr, _muStr, _kStr;

void calcEmbeddingResult() {
	decryptD();
	string hwid = interface::rsa_d(smallNum(RSAsource), smallNum(Dstr), smallNum(Nstr)).hex();
	uint32_t Dkeys[2] = {};
	decryptKeys(Dkeys);
	string nwid = (smallNum(hwid) ^ big2Small(sha256(bigNum().FromU32(Dkeys[1]) || bigNum(Dstr)))).hex();
	string hpw = (smallNum(NPWstr) ^ big2Small(sha256(bigNum().FromU32(Dkeys[0]) || bigNum(Dstr)))).hex();
	smallNum hashPart = sha256(bigNum(hpw) || bigNum(hwid)).hex();
	_xStr = (smallNum().FromU32(_uints[0])*smallNum("001001001001001001001001001001001001001001001001001001001001001") ^ hashPart).hex();
	_muStr = (smallNum().FromU32(_uints[1])*smallNum("001001001001001001001001001001001001001001001001001001001001001") ^ hashPart).hex();
	_kStr = (smallNum().FromU32(_uints[2])*smallNum("1111111111111111111111111111111111111111111111111111111111111111") ^ hashPart).hex();
	Dstr = "";
	NPWstr = "";
}

void getX(char *buf, size_t len) {
	char* result = transferFromString(_xStr);
	if (len > strlen(result)) {
		memcpy(buf, result, strlen(result) + 1);
	}
}

void getMu(char *buf, size_t len) {
	char* result = transferFromString(_muStr);
	if (len > strlen(result)) {
		memcpy(buf, result, strlen(result) + 1);
	}
}

void getK(char *buf, size_t len) {
	char* result = transferFromString(_kStr);
	if (len > strlen(result)) {
		memcpy(buf, result, strlen(result) + 1);
	}
}
