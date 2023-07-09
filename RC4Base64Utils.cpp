#include "RC4Base64Utils.h"
#include <string.h>

namespace RC4Base64Utils
{
	//获取字符在base数组的位置
	char GetIndex(char ch)
	{
		char* ptr = (char*)strrchr(base, ch);
		return char(ptr - base);
	}

	int RC4(unsigned char* pSrc, int nSrcLen, const char* pKey, int nKeyLen)
	{
		if (!pSrc || nSrcLen <= 0 || nSrcLen > RC4_SRC_MAX_SIZE 
			|| !pKey || nKeyLen <= 0 || nKeyLen > MAX_SIZE_256)
		{
			//源字符串或Key为空,或长度不符合范围
			return -1;
		}

		unsigned char S[MAX_SIZE_256] = { 0 }, T[MAX_SIZE_256] = { 0 }, R[RC4_SRC_MAX_SIZE] = { 0 };

		int i = 0;
		for (i = 0; i < MAX_SIZE_256; ++i)
		{
			S[i] = i;
			T[i] = pKey[i % nKeyLen];
		}

		int j = 0;
		for (i = 0; i < MAX_SIZE_256; ++i)
		{
			j = (j + S[i] + T[i]) % MAX_SIZE_256;

			unsigned char temp = S[i];
			S[i] = S[j], S[j] = temp;
		}

		i = 0, j = 0;
		for (int m = 0; m < RC4_SRC_MAX_SIZE; ++m)
		{
			i = (i + 1) % MAX_SIZE_256;
			j = (j + S[i]) % MAX_SIZE_256;

			unsigned char temp = S[i];
			S[i] = S[j], S[j] = temp;

			int t = (S[i] + S[j]) % MAX_SIZE_256;
			R[m] = S[t];
		}

		for (i = 0; i < nSrcLen; ++i)
		{
			pSrc[i] ^= R[i];
		}

		return 0;
	}

	void EncodeBinary2String(const void* src, int lenSrc, char*& res, int& lenRes)
	{
		int cp = (lenSrc % 3 == 0 ? 0 : 3 - (lenSrc % 3));

		lenSrc += cp;
		lenRes = lenSrc * 4 / 3;

		unsigned char* newSrc = new unsigned char[lenSrc];
		memcpy(newSrc, src, lenSrc);

		int i;
		for (i = 0; i < cp; ++i)
		{
			newSrc[lenSrc - 1 - i] = 0;
		}

		res = new char[lenRes + 1];

		int nTmp = 0;
		for (i = 0; i < lenSrc; i += 3)
		{
			nTmp = i / 3 * 4;
			if (nTmp <= lenRes)
			{
				res[nTmp] = base[newSrc[i] >> 2];
			}

			if (nTmp + 1 <= lenRes && i + 1 < lenSrc)
			{
				res[nTmp + 1] = base[((newSrc[i] & 3) << 4) + (newSrc[i + 1] >> 4)];
			}

			if (nTmp + 2 <= lenRes && i + 2 < lenSrc)
			{
				res[nTmp + 2] = base[((newSrc[i + 1] & 15) << 2) + (newSrc[i + 2] >> 6)];
			}

			if (nTmp + 3 <= lenRes && i + 2 < lenSrc)
			{
				res[nTmp + 3] = base[(newSrc[i + 2] & 63)];
			}
		}

		for (i = 0; i < cp; ++i)
		{
			res[lenRes - 1 - i] = '=';
		}
		res[lenRes] = '\0';

		delete[]newSrc;
		return;
	}

	void DecodeString2Binary(const char* src, int lenSrc, char*& res, int& lenRes)
	{
		lenRes = lenSrc * 3 / 4;
		res = new char[lenRes];

		int cp = 0;
		if (src[lenSrc - 1] == '=')
		{
			lenRes--, cp++;
		}

		if (src[lenSrc - 2] == '=')
		{
			lenRes--, cp++;
		}

		int lenNew = lenSrc - cp;
		char* newSrc = new char[lenNew];
		int i;
		for (i = 0; i < lenNew; ++i)
		{
			newSrc[i] = GetIndex(src[i]);
		}

		int nTmp = 0;
		for (i = 0; i < lenRes; i += 3)
		{
			nTmp = i / 3 * 4;
			if (nTmp + 1 < lenNew)
			{
				res[i] = (newSrc[nTmp] << 2) + (newSrc[nTmp + 1] >> 4);
			}

			if (i + 1 < lenRes && nTmp + 2 < lenNew)
			{
				res[i + 1] = ((newSrc[nTmp + 1] & 15) << 4) + (newSrc[nTmp + 2] >> 2);
			}

			if (i + 2 < lenRes && nTmp + 3 < lenNew)
			{
				res[i + 2] = ((newSrc[nTmp + 2] & 3) << 6) + (newSrc[nTmp + 3]);
			}
		}

		delete[]newSrc;
		return;
	}

	int EncodeRC4Base64(const char* pSrc, char* pDst, const char* pKey)
	{
		if (!pSrc || !pDst || !pKey)
		{
			return -1;
		}

		int nSrcLen = strlen(pSrc);
		int nKeyLen = strlen(pKey);

		char* pSrcCopy = new char[nSrcLen + 1];
		memcpy(pSrcCopy, pSrc, nSrcLen + 1);

		RC4((unsigned char*)pSrcCopy, nSrcLen, pKey, nKeyLen);

		int nResLen = 0;
		char* pRes = NULL;
		EncodeBinary2String(pSrcCopy, nSrcLen, pRes, nResLen);

		if (pRes)
		{
			memcpy(pDst, pRes, nResLen);
			pDst[nResLen] = '\0';

			delete[]pRes, pRes = NULL;
		}

		if (pSrcCopy)
		{
			delete[]pSrcCopy, pSrcCopy = NULL;
		}

		return 0;
	}

	int DecodeBase64RC4(const char* pSrc, char* pDst, const char* pKey)
	{
		if (!pSrc || !pDst || !pKey)
		{
			return -1;
		}

		int nSrcLen = strlen(pSrc);
		int nKeyLen = strlen(pKey);

		int nResLen = 0;
		char* pRes = NULL;
		DecodeString2Binary(pSrc, nSrcLen, pRes, nResLen);

		if (pRes)
		{
			RC4((unsigned char*)pRes, nResLen, pKey, nKeyLen);
			memcpy(pDst, pRes, nResLen);
			pDst[nResLen] = '\0';

			delete[]pRes, pRes = NULL;
			return nResLen;
		}

		return 0;
	}
}