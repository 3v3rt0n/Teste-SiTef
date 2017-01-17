#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
	#include <windows.h>
#else
	#include<dlfcn.h>
#endif

static void* lib = NULL;
typedef int(WINAPI* LPFN_ConfiguraIntSiTefInterativo)(char*, char*, char*, short);
typedef int(WINAPI* LPFN_IniciaFuncaoSiTefInterativo)(int, char*, char*, char*, char*, char*, char*);
typedef int(WINAPI* LPFN_ContinuaFuncaoSiTefInterativo)(int*, long*, short*, short*, char*, int, int);
typedef int(WINAPI* LPFN_FinalizaFuncaoSiTefInterativo)(short, char*, char*, char*, char*);

LPFN_ConfiguraIntSiTefInterativo ConfiguraIntSiTefInterativo;
LPFN_IniciaFuncaoSiTefInterativo IniciaFuncaoSiTefInterativo;
LPFN_ContinuaFuncaoSiTefInterativo ContinuaFuncaoSiTefInterativo;
LPFN_FinalizaFuncaoSiTefInterativo FinalizaFuncaoSiTefInterativo;

int main() {

#ifdef WIN32
	lib = LoadLibrary(TEXT("CliSiTef32I.dll"));
#else
	lib = dlopen("./libclisitef.so");
#endif

#ifdef WIN32
	ConfiguraIntSiTefInterativo = (LPFN_ConfiguraIntSiTefInterativo)GetProcAddress((HMODULE) lib, "ConfiguraIntSiTefInterativo");
#else
	ConfiguraIntSiTefInterativo = (LPFN_ConfiguraIntSiTefInterativo)dlsym(lib, "ConfiguraIntSiTefInterativo");
#endif

#ifdef WIN32
	IniciaFuncaoSiTefInterativo = (LPFN_IniciaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "IniciaFuncaoSiTefInterativo");
#else
	IniciaFuncaoSiTefInterativo = (LPFN_IniciaFuncaoSiTefInterativo)dlsym(lib, "IniciaFuncaoSiTefInterativo");
#endif

#ifdef WIN32
	ContinuaFuncaoSiTefInterativo = (LPFN_ContinuaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "ContinuaFuncaoSiTefInterativo");
#else
	ContinuaFuncaoSiTefInterativo = (LPFN_ContinuaFuncaoSiTefInterativo)dlsym(lib, "ContinuaFuncaoSiTefInterativo");
#endif

#ifdef WIN32
	FinalizaFuncaoSiTefInterativo = (LPFN_FinalizaFuncaoSiTefInterativo)GetProcAddress((HMODULE) lib, "FinalizaFuncaoSiTefInterativo");
#else
	FinalizaFuncaoSiTefInterativo = (LPFN_FinalizaFuncaoSiTefInterativo)dlsym(lib, "FinalizaFuncaoSiTefInterativo");
#endif

	
	char ok;
	printf("Iniciando SiTef: ConfiguraIntSiTefInterativo\n");
	char IPSiTef[13] = "192.168.56.1";
	char IdLoja[9] = "00000000";
	char IdTerminal[9] = "AA000000";
	int resultado = ConfiguraIntSiTefInterativo(IPSiTef, IdLoja, IdTerminal, 0);
	/*/printf("aperte qualquer tecla para continuar\n");
	scanf("%c", ok);*/
	
	printf("Resultado ConfiguraIntSiTefInterativo %d\n", resultado);
	
	printf("Chamando IniciaFuncaoSiTefInterativo:\n");
	
	char Valor[5]="50,00";
	char CupomFiscal[10]="1234567890";
	char DataFiscal[8]="20170110";
	char HoraFiscal[6]="120000";
	char Operador[20]="Joao da Silva";
	
	resultado = IniciaFuncaoSiTefInterativo(3, Valor, CupomFiscal, DataFiscal, HoraFiscal, Operador, NULL);
	
	printf("Resultado IniciaFuncaoSiTefInterativo %d\n", resultado);
	
	printf("Chamando ContinuaFuncaoSiTefInterativo:\n");
	
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
	
	printf("Chamando FinalizaFuncaoSiTefInterativo:\n");
	
	resultado = FinalizaFuncaoSiTefInterativo(1, CupomFiscal, DataFiscal, HoraFiscal, NULL);
	
	printf("Resultado FinalizaFuncaoSiTefInterativo %d\n", resultado);
	printf("Comando %d TipoCampo %d", Comando, TipoCampo);

	
	int descarregou;
#ifdef WIN32
		descarregou = FreeLibrary((HMODULE) lib);
#else
		descarregou = dlclose(lib);
#endif

	system("PAUSE");
	return 0;
}
