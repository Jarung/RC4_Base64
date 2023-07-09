#pragma once
namespace RC4Base64Utils
{
	//Base64编码字符串数组
	const char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	//RC4算法源字符串最大长度
	const int RC4_SRC_MAX_SIZE = 10 * 1024;
	//RC4算法密钥最大长度
	const int MAX_SIZE_256 = 256;

	//RC4编解码
	int RC4(unsigned char* pSrc, int nSrcLen, const char* pKey, int nKeyLen);

	//base64编码
	void EncodeBinary2String(const void* src, int lenSrc, char*& res, int& lenRes);

	//base64解码
	void DecodeString2Binary(const char* src, int lenSrc, char*& res, int& lenRes);

	//RC4->base64编码
	int EncodeRC4Base64(const char* pSrc, char* pDst, const char* pKey);

	//base64->RC4解码
	int DecodeBase64RC4(const char* pSrc, char* pDst, const char* pKey);
};
