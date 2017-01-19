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
		szError = (string) GetLastError();
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
		szError = (string) GetLastError();
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
		szError = (string) GetLastError();
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
		szError = (string) GetLastError();
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
		szError = (string) GetLastError();
#else
		szError = dlerror();
#endif
		//printf("Erro ao carregar a funcao %s: %s\n", nomeFuncao, erro);
		return 0;
	}

	//Open communication with PinPad properly, calling ConfiguraIntSiTefInterativo method
	
	res = ConfiguraIntSiTefInterativo(szIPSiTef, szIdLoja, szIdTerminal, 0);
	
	//trace return from ConfiguraIntSiTefInterativo
	
	return res;	
}

int iUnloadSiTef( )
{
	char szError[MAX_PATH] = {0};
	int iRes = 0;

	if (lib) 
	{
#ifdef WIN32
		iRes = FreeLibrary((HMODULE) bibliotecaDinamica);
#else
		iRes = dlclose(bibliotecaDinamica);
#endif
	}

	if (!iRes) 
	{
#ifdef WIN32
		szError = (string) GetLastError();
#else
		szError = dlerror();
#endif
		//printf("Erro ao descarregar a DarumaFrameWork: %s\n", erro);
	}
	
	return iRes;
}

bool bSaleTransaction(int iPaymentMethod, char* szTotal, char* szFiscalCoupon, char* szFiscalData, char* szFiscalHour, char* szOperator, char* szMessage,
						bool* pbDisplayMessageCust,  bool* pDisplayMessageOp, bool* pbInputInfo, bool* pbCancelCmd, );
{
	int iRes = 0;
	 
	//at first we should call function to initialize the sale transaction, if succeeds we continue 
	iRes = IniciaFuncaoSiTefInterativo(iPaymentMethod, szTotal, szFiscalCoupon, szFiscalData, HoraFiscal, szFiscalHour, NULL);
	
	if(iRes != 0)
	{
		//trace here the kind of error we got from IniciaFuncaoSiTefInterativo
		return false;
	}
		
	//proceed with sale transaction
	int Comando;
	long TipoCampo;
	short TamMinimo, TamMaximo;
	char buffer[20000];
	resultado=1000;
	
    int i=1;
	do
	{
		if(*pbCancelCmd) Comando = 0;
		
		resultado = ContinuaFuncaoSiTefInterativo(&Comando, &TipoCampo, &TamMinimo, &TamMaximo, buffer, MESSAGE_BUFFER_SIZE, 0);
		
		switch (
		printf("Resultado ContinuaFuncaoSiTefInterativo %d %da vez\n", resultado, i);
		printf("Comando %d \nTipoCampo %d \nMensagem: \"%s\" \n\n", Comando, TipoCampo, (strlen(buffer)>0)?buffer:"");
		
		if(Comando ==21)
		{
			memset(buffer, 0, 20000);
			strcpy(buffer, "1");
		}
		
		if(resultado!=1000L)
	}while(true);
	
}

int iCompleteSaleTransaction()
{
	
}
