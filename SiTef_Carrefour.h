#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
	#include <windows.h>
#else
	#include<dlfcn.h>
#endif

#define MESSAGE_BUFFER_SIZE 20000

typedef int(WINAPI* LPFN_ConfiguraIntSiTefInterativo)(char*, char*, char*, short);
typedef int(WINAPI* LPFN_IniciaFuncaoSiTefInterativo)(int, char*, char*, char*, char*, char*, char*);
typedef int(WINAPI* LPFN_ContinuaFuncaoSiTefInterativo)(int*, long*, short*, short*, char*, int, int);
typedef int(WINAPI* LPFN_FinalizaFuncaoSiTefInterativo)(short, char*, char*, char*, char*);

LPFN_ConfiguraIntSiTefInterativo ConfiguraIntSiTefInterativo;
LPFN_IniciaFuncaoSiTefInterativo IniciaFuncaoSiTefInterativo;
LPFN_ContinuaFuncaoSiTefInterativo ContinuaFuncaoSiTefInterativo;
LPFN_FinalizaFuncaoSiTefInterativo FinalizaFuncaoSiTefInterativo;

static void* lib = NULL;


int iLoadSiTef( char* szIPSiTef, char* szIdLoja, char* szIdTerminal);
int iUnloadSiTef( );
bool bSaleTransaction(int iPaymentMethod, char* szTotal, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour, char* szOperator, char* szMessage,
						bool* pbDisplayMessageCust,  bool* pDisplayMessageOp, bool* pbInputInfo, bool* pbCancelCmd, );
int iCompleteSaleTransaction();

int Comando;
	long TipoCampo;
	short TamMinimo, TamMaximo;
	char buffer[20000];
	resultado=1000;
	
    int i=1;
	for(; i<20; i++)
	{
		resultado = ContinuaFuncaoSiTefInterativo(&Comando, &TipoCampo, &TamMinimo, &TamMaximo, buffer, 20000, 0);
		
		printf("Resultado ContinuaFuncaoSiTefInterativo %d %da vez\n", resultado, i);
		printf("Comando %d \nTipoCampo %d \nMensagem: \"%s\" \n\n", Comando, TipoCampo, (strlen(buffer)>0)?buffer:"");
		
		if(Comando ==21)
		{
			memset(buffer, 0, 20000);
			strcpy(buffer, "1");
		}
		
		system("PAUSE");
	}	
