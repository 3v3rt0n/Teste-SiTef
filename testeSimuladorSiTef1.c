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
	
	printf("Resultado iLoadSiTef %d\n", iRes);
	
	
	char Valor[6]="50,00";
	char CupomFiscal[10]="1234567890";
	char DataFiscal[8]="20170110";
	char HoraFiscal[6]="120000";
	char Operador[20]="Joao da Silva";
	int Comando;
	long TipoCampo;
	short TamMinimo, TamMaximo;
	char buffer[20000];
	int resultado=1000;
	bool bDispMsgCust, bDispMsgOpr, bInputInfo, bCancelCmd, bStartedTransaction = false;
	short sConfirma = 1;
	
	printf("Chamando bSaleTransaction:\n");
	
 	while(bSaleTransaction(bStartedTransaction, 3, Valor, CupomFiscal, DataFiscal, HoraFiscal, Operador, buffer, &bDispMsgCust, &bDispMsgOpr, &bInputInfo, bCancelCmd ))
	{
		bStartedTransaction=true;
		
		if(bDispMsgCust)
			printf("\nDisplay Cliente: %s\n", buffer);
		else
			printf("\nDisplay Cliente:    \n");
			
		if(bDispMsgCust)
			printf("\nDisplay Operador: %s\n", buffer);
		else
			printf("\nDisplay Operador:    \n");
		
		if(bInputInfo)
		{
			printf("\nInsira a informação solicitada:\n");
			gets(buffer);
			bInputInfo=false;
		}
		
	}
	 	
	
	
	
	resultado = iCompleteSaleTransaction(sConfirma, CupomFiscal, DataFiscal, HoraFiscal);
	
	printf("Resultado iCompleteSaleTransaction %d\n", resultado);
	

	
	int descarregou;
#ifdef WIN32
		descarregou = FreeLibrary((HMODULE) lib);
#else
		descarregou = dlclose(lib);
#endif

	system("PAUSE");
	return 0;
}
