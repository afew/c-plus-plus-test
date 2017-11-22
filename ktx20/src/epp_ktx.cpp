
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "uthash.h"

#include "ktx.h"
#include "ktxint.h"
#include "uthash.h"

#include "epp_ktx.h"


int KTX_LoadTextureF(	FILE* fp, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn, int mipLvlEnd)
{
	int hr = (int)ktxLoadTextureF(	fp, pTexture, (GLenum*)pTarget,
									(KTX_dimensions*)pDimensions, pIsMipmapped,
									(GLenum*) pGlerror, pKvdLen, ppKvd,
									mipLvlBgn, mipLvlEnd);

	return hr;
}


// has internal file pointer. call ktxLoadTextureF.
int KTX_LoadTextureN(	const char* const filename, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn, int mipLvlEnd)
{
	int hr = (int)ktxLoadTextureN(	filename, pTexture, (GLenum*)pTarget,
									(KTX_dimensions*)pDimensions, pIsMipmapped,
									(GLenum*)pGlerror, pKvdLen, ppKvd,
									mipLvlBgn, mipLvlEnd);
	return hr;
}


int KTX_LoadTextureM(	const void* bytes, int size, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn, int mipLvlEnd)
{
	int hr = (int)ktxLoadTextureM(	bytes, (GLsizei)size, (GLuint*)pTexture, (GLenum*)pTarget,
									(KTX_dimensions*)pDimensions, pIsMipmapped,
									(GLenum*)pGlerror, pKvdLen, ppKvd,
									mipLvlBgn, mipLvlEnd);
	return hr;
}


int KTX_WriteKTXF(	FILE* fp, const KTX_TEXTURE* imageInfo, int bytesOfKeyValueData,
					const void* keyValueData, unsigned int numImages, KTX_IMAGE* images)
{
	int hr = (int)ktxWriteKTXF(	fp, (const KTX_texture_info*)imageInfo, (GLsizei)bytesOfKeyValueData,
								keyValueData, (GLuint)numImages, (KTX_image_info*) images);

	return hr;
}

int KTX_WriteKTXN(	const char* dstname, const KTX_TEXTURE* imageInfo, int bytesOfKeyValueData,
					const void* keyValueData, unsigned int numImages, KTX_IMAGE* images)
{
	int hr = (int)ktxWriteKTXN(dstname, (const KTX_texture_info*)imageInfo, (GLsizei)bytesOfKeyValueData,
								keyValueData, (GLuint)numImages, (KTX_image_info*)images);
	return hr;
}


const char* const KTX_ErrorString(int error)
{
	return ktxErrorString((KTX_error_code)error);
}

KTX_HASH_TABLE KTX_HashTable_Create()
{
	return (KTX_hash_table)ktxHashTable_Create();
}

void KTX_HashTable_Destroy(KTX_HASH_TABLE This)
{
	ktxHashTable_Destroy((KTX_hash_table)This);
}

int KTX_HashTable_AddKVPair(KTX_HASH_TABLE This, const char* key, unsigned int valueLen, const void* value)
{
	int hr = (int)ktxHashTable_AddKVPair((KTX_hash_table)This, key, valueLen, value);
	return hr;
}

int KTX_HashTable_FindValue(KTX_HASH_TABLE This, const char* key, unsigned int* pValueLen, void** pValue)
{
	int hr = (int)ktxHashTable_FindValue((KTX_hash_table)This, key, pValueLen, pValue);
	return hr;
}

int KTX_HashTable_Serialize(KTX_HASH_TABLE This, unsigned int* kvdLen, unsigned char** kvd)
{
	int hr = (int)ktxHashTable_Serialize((KTX_hash_table)This, kvdLen, kvd);
	return hr;
}

int KTX_HashTable_Deserialize(unsigned int kvdLen, void* kvd, KTX_HASH_TABLE* pKvt)
{
	int hr = (int)ktxHashTable_Deserialize(kvdLen, kvd, (KTX_hash_table*)pKvt);
	return hr;
}


