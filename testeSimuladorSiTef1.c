#include "SiTef_Carrefour.h"

int main() {
	int iRes=0;
	char ok;
	printf("Iniciando SiTef: ConfiguraIntSiTefInterativo\n");
	char IPSiTef[13] = "192.168.56.1";
	char IdLoja[9] = "00000000";
	char IdTerminal[9] = "AA000000";
	
	iRes=iLoadSiTef(IPSiTef, IdLoja, IdTerminal);
	
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
