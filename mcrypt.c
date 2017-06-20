#include<stdio.h>
#include<mcrypt.h>
#include<stdlib.h>

int main()
{
	MCRYPT handle;
	int keysize;
	int iv_required = 0;	
	int key = 1234;	
	int check = 0;
	char plaintext[] = "HelloWorld";
	int len  = 0;

	handle = mcrypt_module_open("xtea",NULL,"ecb",NULL);
	if(handle == MCRYPT_FAILED)
	{
		printf("Module open Error\n");
		exit(1);
	}

	keysize = mcrypt_module_get_algo_key_size("xtea", NULL);
	printf("Keysize = %d\n",keysize);

	iv_required = mcrypt_enc_mode_has_iv(handle);
	if(iv_required == 1)
	{
		printf("IV is required\n");
	}
	else
	{
		printf("IV is not required\n");
	}

	check = mcrypt_generic_init(handle, &key, keysize, NULL);
	if(check < 0)
	{
		printf("Generic Init Error\n");
		exit(1);
	}

	len = mcrypt_module_get_algo_block_size("xtea", NULL);
	printf("BlockSize = %d\n",len);

	printf("String before encryption = %s\n", plaintext);
	mcrypt_generic(handle, plaintext, len);
	printf("String after encryption = %s\n", plaintext);


	mdecrypt_generic(handle, plaintext, len);
	printf("String after decryption = %s\n", plaintext);

	check = mcrypt_generic_deinit(handle);
	if(check < 0)
        {
                printf("Generic De-Init Error\n");
                exit(1);
        }

	
	mcrypt_module_close(handle);
	return 0;
}		
