#include "SiTef_Carrefour.h"
/*

	LoadSiTef( char* IPSiTef, char* IdLoja, char* IdTerminal)
	
	Method which loads SiTef interface to get function pointers that will be used by POS application.
	
	This method also calls ConfiguraIntSiTefInterativo to initialize communications with plugged PinPad.
	
	

*/
int iLoadSiTef( char* szIPSiTef, char* szIdLoja, char* szIdTerminal)
{
	char szError[MAX_PATH] = {0};
	int  iRes = 0;
	
	//Load CliSiTef interface to memory
#ifdef WIN32
	lib = LoadLibrary(TEXT("CliSiTef32I.dll"));
#else
	lib = dlopen("./libclisitef.so");
#endif

	if (!lib) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//exit(EXIT_FAILURE);
		return 0;
	}
	
	//Load function pointers from CliSiTef interafce
#ifdef WIN32
	ConfiguraIntSiTefInterativo = (LPFN_ConfiguraIntSiTefInterativo)GetProcAddress((HMODULE) lib, "ConfiguraIntSiTefInterativo");
#else
	ConfiguraIntSiTefInterativo = (LPFN_ConfiguraIntSiTefInterativo)dlsym(lib, "ConfiguraIntSiTefInterativo");
#endif

	if (!ConfiguraIntSiTefInterativo) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//printf("Erro ao carregar a funcao %s: %s\n", nomeFuncao, erro);
		return 0;
	}

#ifdef WIN32
	IniciaFuncaoSiTefInterativo = (LPFN_IniciaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "IniciaFuncaoSiTefInterativo");
#else
	IniciaFuncaoSiTefInterativo = (LPFN_IniciaFuncaoSiTefInterativo)dlsym(lib, "IniciaFuncaoSiTefInterativo");
#endif

	if (!IniciaFuncaoSiTefInterativo) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//printf("Erro ao carregar a funcao %s: %s\n", nomeFuncao, erro);
		return 0;
	}

#ifdef WIN32
	ContinuaFuncaoSiTefInterativo = (LPFN_ContinuaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "ContinuaFuncaoSiTefInterativo");
#else
	ContinuaFuncaoSiTefInterativo = (LPFN_ContinuaFuncaoSiTefInterativo)dlsym(lib, "ContinuaFuncaoSiTefInterativo");
#endif

	if (!ContinuaFuncaoSiTefInterativo) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//printf("Erro ao carregar a funcao %s: %s\n", nomeFuncao, erro);
		return 0;
	}

#ifdef WIN32
	FinalizaFuncaoSiTefInterativo = (LPFN_FinalizaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "FinalizaFuncaoSiTefInterativo");
#else
	FinalizaFuncaoSiTefInterativo = (LPFN_FinalizaFuncaoSiTefInterativo)dlsym(lib, "FinalizaFuncaoSiTefInterativo");
#endif

	if (!FinalizaFuncaoSiTefInterativo) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//printf("Erro ao carregar a funcao %s: %s\n", nomeFuncao, erro);
		return 0;
	}

	//Open communication with PinPad properly, calling ConfiguraIntSiTefInterativo method
	
	iRes = ConfiguraIntSiTefInterativo(szIPSiTef, szIdLoja, szIdTerminal, 0);
	
	//trace return from ConfiguraIntSiTefInterativo
	
	return iRes;	
}

int iUnloadSiTef( )
{
	char szError[MAX_PATH] = {0};
	int iRes = 0;

	if (lib) 
	{
#ifdef WIN32
		iRes = FreeLibrary((HMODULE) lib);
#else
		iRes = dlclose(lib);
#endif
	}

	if (!iRes) 
	{
#ifdef WIN32
		sprintf(szError, "%d", GetLastError());
#else
		szError = dlerror();
#endif
		//printf("Erro ao descarregar a DarumaFrameWork: %s\n", erro);
	}
	
	return iRes;
}

bool bSaleTransaction(bool bStartedTransaction, int iPaymentMethod, char* szTotal, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour, char* szOperator, char* szMessage,
						bool* pbDisplayMessageCust,  bool* pbDisplayMessageOp, bool* pbInputInfo, bool bCancelCmd )
{
	int iRes = 0;
	if(!bStartedTransaction)
	{ 
		//at first we should call function to initialize the sale transaction, if succeeds we continue 
		iRes = IniciaFuncaoSiTefInterativo(iPaymentMethod, szTotal, szFiscalCoupon, szFiscalData, szFiscalHour, szOperator, NULL);
		
		if(iRes != 0 && iRes != 10000&& iRes != -12)
		{
			//trace here the kind of error we got from IniciaFuncaoSiTefInterativo
			return false;
		}
	}
	//proceed with sale transaction
	int iComando;
	long lTipoCampo;
	short TamMinimo, TamMaximo;
	char buffer[20000];
	int resultado=1000;
	
    /*int i=1;
	do
	{*/
		if(bCancelCmd) iComando = 0;
		
		resultado = ContinuaFuncaoSiTefInterativo(&iComando, &lTipoCampo, &TamMinimo, &TamMaximo, szMessage, MESSAGE_BUFFER_SIZE, 0);
		
		switch(iComando )
		{
			case 1:
				*pbDisplayMessageOp=true;
				break;
			case 2:
				*pbDisplayMessageOp=true;
				break;
			case 3:
				*pbDisplayMessageOp=true;
				*pbDisplayMessageCust=true;
				break;
			case 4:
				*pbDisplayMessageOp=true;
				*pbDisplayMessageCust=true;
				break;
			case 11:
				*pbDisplayMessageOp=false;
				break;
			case 12:
				*pbDisplayMessageOp=false;
				break;
			case 13:
				*pbDisplayMessageOp=false;
				*pbDisplayMessageCust=false;
				break;
			case 14:
				*pbDisplayMessageOp=false;
				*pbDisplayMessageCust=false;
				break;
			case 21:
			case 22:
				*pbDisplayMessageOp=true;
				*pbDisplayMessageCust=true;
				*pbInputInfo=true;
				break;
			case 29:
			case 30:
				*pbInputInfo=true;
				break;
			default:
				break;
		}
		/*switch(lTipoCampo )
		{
			case 1:
				pDisplayMessageOp=true;
				break;
			case 2:
				pDisplayMessageOp=true;
				break;
			case 3:
				pDisplayMessageOp=true;
				pDisplayMessageCust=true;
				break;
			case 4:
				pDisplayMessageOp=true;
				pDisplayMessageCust=true;
				break;
			case 11:
				pDisplayMessageOp=false;
				break;
			case 12:
				pDisplayMessageOp=false;
				break;
			case 13:
				pDisplayMessageOp=false;
				pDisplayMessageCust=false;
				break;
			case 14:
				pDisplayMessageOp=false;
				pDisplayMessageCust=false;
				break;
			case 21:
			case 22:
				pDisplayMessageOp=true;
				pDisplayMessageCust=true;
				break;
			case 29:
			case 30:
				pbInputInfo=true;
				break;
			default:
				break;
		}*/
		
		//printf("\n\n****  Comando %d \nTipoCampo %d \nMensagem: \"%s\  ****\n\n", iComando, lTipoCampo, (strlen(buffer)>0)?buffer:"");
		if(iComando!=23)
		printf("\n\n****  Comando %d TipoCampo %d  ****\n\n%s\n", iComando, lTipoCampo, (strlen(szMessage)>0)?szMessage:"");
		/*if(Comando ==21)
		{
			memset(buffer, 0, 20000);
			strcpy(buffer, "1");
		}*/
		
/*		if(resultado!=10000)break;
	}while(true);*/
	return resultado==10000?true:false;
}

int iCompleteSaleTransaction(short sFinishTransaction, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour)
{
	return FinalizaFuncaoSiTefInterativo(sFinishTransaction, szFiscalCoupon, szFiscalData, szFiscalHour, NULL);
}
