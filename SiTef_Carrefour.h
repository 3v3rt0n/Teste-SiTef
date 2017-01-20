#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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


int iLoadSiTef(char* szIPSiTef, char* szIdLoja, char* szIdTerminal);
int iUnloadSiTef( );
bool bSaleTransaction(bool bStartedTransaction, int iPaymentMethod, char* szTotal, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour, char* szOperator, char* szMessage,
						bool* pbDisplayMessageCust,  bool* pDisplayMessageOp, bool* pbInputInfo, bool bCancelCmd );
int iCompleteSaleTransaction(short sFinishTransaction, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour);

int iCarregaSiTef(char* szIPSiTef, char* szIdLoja, char* szIdTerminal);
int iDescarregaSiTef();
int iIniciaVenda(int iFuncao, char* szValor, char* szCupomFiscal, char* szDataFiscal, char* szHoraFiscal);
bool bContinuaVenda(int* piComando, long* plTipoComando, short* psTamMinimo, short* psTamMaximo, char* szBuffer, int iTamBuffer, int iContinua);
