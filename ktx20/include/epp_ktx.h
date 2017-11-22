// EPP KTX
//
//----------------------------------------------------------------------------------------------------------------------


#ifndef _EPP_KTX_H_
#define _EPP_KTX_H_


enum KTX_ERROR
{
	ERR_KTX_SUCCESS							= 0,	/*!< Operation was successful. */
	ERR_KTX_FILE_OPEN_FAILED				,		/*!< The target file could not be opened. */
	ERR_KTX_FILE_WRITE_ERROR				,		/*!< An error occurred while writing to the file. */
	ERR_KTX_GL_ERROR						,		/*!< GL operations resulted in an error. */
	ERR_KTX_INVALID_OPERATION				,		/*!< The operation is not allowed in the current state. */
	ERR_KTX_INVALID_VALUE					,		/*!< A parameter value was not valid */
	ERR_KTX_NOT_FOUND						,		/*!< Requested key was not found */
	ERR_KTX_OUT_OF_MEMORY					,		/*!< Not enough memory to complete the operation. */
	ERR_KTX_UNEXPECTED_END_OF_FILE			,		/*!< The file did not contain enough data */
	ERR_KTX_UNKNOWN_FILE_FORMAT				,		/*!< The file not a KTX file */
	ERR_KTX_UNSUPPORTED_TEXTURE_TYPE		,		/*!< The KTX file specifies an unsupported texture type. */
};

struct KTX_TEXTURE
{
	unsigned int	glType					;
	unsigned int	glTypeSize				;
	unsigned int	glFormat				;
	unsigned int	glInternalFormat		;
	unsigned int	glBaseInternalFormat	;
	unsigned int	pixelWidth				;
 	unsigned int	pixelHeight				;
	unsigned int	pixelDepth				;
	unsigned int	numberOfArrayElements	;
	unsigned int	numberOfFaces			;
	unsigned int	numberOfMipmapLevels	;
};

struct KTX_IMAGE
{
	int				size	;
	unsigned char*	data	;
};

struct KTX_DIMENSIONS
{
	int				width	;
	int				height	;
	int				depth	;
};

typedef void*		KTX_HASH_TABLE;


//int epp_ktx_read_from_file(FILE* fp);
//int epp_ktx_write_to_file(FILE* fp);
//
//int epp_ktx_read_from_buf(char* buf);
//int epp_ktx_write_to_buf (char* buf);


//----------------------------------------------------------------------------------------------------------------------
// low level ktx interface

int KTX_LoadTextureF (	FILE* fp, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn=0, int mipLvlEnd=0x0000FFFF);

// has internal file pointer. call ktxLoadTextureF.
int KTX_LoadTextureN (	const char* const filename, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn=0, int mipLvlEnd=0x0000FFFF);

int KTX_LoadTextureM(	const void* bytes, int size, unsigned int* pTexture, int* pTarget,
						KTX_DIMENSIONS* pDimensions, unsigned int* pIsMipmapped, int* pGlerror,
						unsigned int* pKvdLen, unsigned char** ppKvd, int mipLvlBgn=0, int mipLvlEnd=0x0000FFFF);


int KTX_WriteKTXF    (	FILE* fp, const KTX_TEXTURE* imageInfo, int bytesOfKeyValueData,
						const void* keyValueData, unsigned int numImages, KTX_IMAGE* images);

// has internal file pointer. call ktxWriteKTXF.
int KTX_WriteKTXN    (	const char* dstname, const KTX_TEXTURE* imageInfo, int bytesOfKeyValueData,
						const void* keyValueData, unsigned int numImages, KTX_IMAGE* images);


const char* const KTX_ErrorString(int error);
KTX_HASH_TABLE    KTX_HashTable_Create();

void KTX_HashTable_Destroy     (KTX_HASH_TABLE This);
int  KTX_HashTable_AddKVPair   (KTX_HASH_TABLE This, const char* key, unsigned int valueLen, const void* value);
int  KTX_HashTable_FindValue   (KTX_HASH_TABLE This, const char* key, unsigned int* pValueLen, void** pValue);
int  KTX_HashTable_Serialize   (KTX_HASH_TABLE This, unsigned int* kvdLen, unsigned char** kvd);
int  KTX_HashTable_Deserialize (unsigned int kvdLen, void* kvd, KTX_HASH_TABLE* pKvt);

//----------------------------------------------------------------------------------------------------------------------

#endif



