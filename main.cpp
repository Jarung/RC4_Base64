#include <stdio.h>
#include "RC4Base64Utils.h"

int main(int argc, char* argv[])
{
	const char* pKey = "github2023!@#";
	char* pSrc = "RC4Base64Utils=test";
	char res1[256] = { 0 }, res2[256] = { 0 };

	//编码
	RC4Base64Utils::EncodeRC4Base64(pSrc, res1, pKey);
	printf("EncodeRC4Base64, src: %s  -->  res: %s\n", pSrc, res1);

	//解码
	RC4Base64Utils::DecodeBase64RC4(res1, res2, pKey);
	printf("DecodeBase64RC4, src: %s  -->  res: %s\n", res1, res2);
	return 0;
}