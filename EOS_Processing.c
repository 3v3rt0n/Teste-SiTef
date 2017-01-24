__attribute__ ((unused))        static char *SVNid = "DTPLSVN$Id: EOS_Processing.c 20586 2010-10-20 12:54:45Z bine $";

/*----------------------------------------------------------------------
 *  Copyright by Wincor Nixdorf International GmbH, 33094 Paderborn
 *  Germany, 2005 - 2010
 *  All rights reserved.
----------------------------------------------------------------------*/

/*
 *  Holds sUser_EOS_Processing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calypso/db_def.h"
#include "calypso/db_empl.h"
#include "calypso/db_plu.h"
#include "calypso/db_move.h"
#include "calypso/db_dept.h"
#include "calypso/db_cmmedia.h"
#include "calypso/db_medlst.h"
#include "calypso/db_ejrecs.h"
#include "calypso/db_acc.h"
#include "calypso/db_bwlist.h"
#include "calypso/db_barcode.h"
#include "calypso/db_command.h"
#include "calypso/db_cust.h"
#include "calypso/db_datent.h"
#include "calypso/db_emprof.h"
#include "calypso/db_halo.h"
#include "calypso/db_itmz.h"
#include "calypso/db_keyb.h"
#include "calypso/db_medgrp.h"
#include "calypso/db_msr.h"
#include "calypso/db_pluext.h"
#include "calypso/db_poslit.h"
#include "calypso/db_postxt.h"
#include "calypso/db_restr.h"
#include "calypso/db_scf.h"
#include "calypso/db_tax.h"
#include "calypso/db_tcf.h"
#include "calypso/db_termtot.h"

#include "calypso/events.h"
#include "calypso/posdef.h"
#include "calypso/pospi.h"
#include "calypso/structs.h"
#include "calypso/poslits.h"
#include "calypso/environ.h"
#include "calypso/osys.h"
#include "calypso/devices.h"
#include "calypso/errserv.h"
#include "calypso/protos.h"
#include "calypso/config.h"
//#include "calypso/sysapi.h"
#include "calypso/discount.h"
#include "calypso/ueprotos.h"
#include "calypso/fiscal.h"

#include "syscom/nm_error.h" /* Containing errorcodes for SYSCOM */
#include "syscom/nm_ta.h"    /* Containing return and errorcodes for lSend_EJ */
#include "syscom/calprint.h"


// fernando

#include "inventti/nfce.h"
#include "inventti/nfe.h"
#include "inventti/sat.h"
#include "inventti/commons.h"
#include "inventti/enums.h"
// fernando


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    Description  : sUser_EOS_Processing() - This function is where a user
                   would link into the system with their own functions if
                   the user wanted to gain control at a End Of Sale.
                   User Exits are supplied at the following positions
                   in the processing of End Of Sale.

                   13. Upon entry into EOS. i.e. Set_Up_For_EOS.
                       13 is NOT a spelling mistake !!

                   2. Prior to displaying Change.

                   3. Prior to Final EOS Processing.

                   4. Immediately before going back to the event handler.

                   8.  When leaving EOS.

    Comments     : This is called from the event handler. EVENT 72

    Arguments    : None

    Returns      : USER_EXIT_RETURN The User must exit to the event handler with
                   a return(USER_EXIT_RETURN) after calling the function
                   Restore_Stack_For_User_Exit. 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short sUser_EOS_Processing(void)
{
  calprint("V1",CLP_POS_USEX, "userexit", "sUser_EOS_Processing US=<%d>",
    gstUserSave.uchUserPosition);
  

  /* correct the userexit position for PSV */
  /*
  if ((gstUserSave.uchUserPosition == 3) &&
      (gstRegStates.uchFuncType == POST_SALE_VOID_FUNC))
  {
    //gstUserSave.uchUserPosition++;
     *
     *

  }
  */

  // fernando
  if (gstUserSave.uchUserPosition == 3){
	// fernando
	// criar uma variavel emissor global para não finalizar o socket
	emissor_nfce * emissor;
	int retorno = 0;

	//Setup inicial
	//1. Carregar a DLL e chamar o método nfce_inicializa_emissor, seguindo de:
	emissor = nfce_inicializa_emissor();

	// fernando
	char strParametroNfceSat_tpAmbiente[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_modeloDocumento[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_endereco[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java

	char Ambiente[64];
	char modeloDocumento[64];
	char endereco[64];

	strcpy(strParametroNfceSat_tpAmbiente, "2");
	strcpy(strParametroNfceSat_modeloDocumento, "65");
	strcpy(strParametroNfceSat_endereco, "http://hom.inventti.com.br/nfce");

	strcpy(Ambiente, strParametroNfceSat_tpAmbiente);
	strcpy(modeloDocumento, strParametroNfceSat_modeloDocumento);
	strcpy(endereco, strParametroNfceSat_endereco);
	// fernando

	//retorno = sConfigurar_WebService(emissor, "2", "65", "http://hom.inventti.com.br/nfce", "", "", "", "", "", "", "", "", "", "", "");
	retorno = sConfigurar_WebService(emissor, Ambiente, modeloDocumento, endereco, "", "", "", "", "", "", "", "", "", "", "");
	printf("\nretorno sConfigurar_WebService = %d\n", retorno);

	//2. sConfigurar_Emissao
/*
	e
	Type:  emissor_nfce *
	Handle para o emissor_nfce

	cnpj
	Type:  const char *
	CNPJ do Emitente.

	tipoAmbiente
	Type:  const char *
	Tipo do ambiente que o agente irá operar.
	1 - Produção
	2 - Homologação

	serie
	Type:  const char *
	Série que será utilizada pelo PDV

	modelo
	Type:  const char *
	Modelo do documento que será utilizado pelo PDV

	naturezaOperacao
	Type:  const char *
	Natureza de operação que será utilizada nas vendas

	sequenciaAutorizacao
	Type:  const char *
	Sequência de autorização, separadas por virgulas. Os valores possíveis podem ser: NFC-e;NFC-e OffLine;SAT
	tipoControleNumeracao
	Type:  const char *
	Tipo de controle da numeração da venda. Os valores possíveis são: 10="Gerada automaticamente"; 20="Fornecida pelo PDV". Caso não informado, será assumido o valor 10="Gerada automaticamente"
	contingenciaAutomaticaDeRejeicao
	Type:  const char *
	Indica se o sistema irá emitir automaticamente em contingência caso a venda é rejeitada.
	0 - Não
	1 - Sim
	Esta configuração aplica-se somente para emissão de NFC-e ou SAT CF-e quando configurado para sequência de emissão "NFC-e;NFC-e OffLine;", "NFC-e;SAT;" ou "SAT;NFC-e";
	Quando este parâmetro não for informado ele assumirá por padrão o valor 1 onde que qualquer rejeição que ocorrer, a aplicação emitirá em contingência respeitando a sequência de opções informada no parâmetro "sequenciaAutorizacao".
	Mesmo quando informado o valor "0", a aplicação emitirá em modo de contingência se ocorrer uma das seguintes Rejeições abaixo:
	108 - Serviço Paralisado Momentaneamente (curto prazo)
	109 - Serviço Paralisado sem Previsão
	999 - Rejeição: Erro não catalogado

	diretorioParaSalvarRespostaProtocolada
	Type:  const char *
	Parâmetro opcional.
	Caso informado, o agente salvará o XML protocolado no diretório informado.

	O XML que será salvo depende da operação:
	-NF-e autorizado: será o XML de autorização retornado pela Sefaz, com nome no padrão NFe{chave}-procNFe.xml
	-NFC-e autorizado: será o XML de autorização retornado pela Sefaz, com nome no padrão NFe{chave}-procNFe.xml
	-CF-e autorizado: será o XML de autorização retornado pela Sefaz, com nome no padrão CFe{chave}.xml
	-NFC-e offline: será o XML gerado pelo agente, com nome no padrão NFe{chave}-nfe.xml
	-NF-e ou NFC-e emitido em contigência: será o XML validado e consolidado pela Retaguarda, com nome no padrão NFe{chave}-nfe.xml
	-Cancelamento de NF-e ou NFC-e: será o XML homologado retornado pela Sefaz, com nome no padrão {chave}-1-procCanc.xml
	-Cancelamento CF-e: será o XML homologado retornado pela Sefaz, com nome no padrão ADC{chave}.xml
	-Inutilização de NFC-e: será o XML homologado retornado pela Sefaz, com nome no padrão {chave}-procInutNFe.xml

	Na ocorrência de erro ao salvar o XML no diretório, entrar em contato com suporte.
*/


	// fernando
	char strParametroSat_cnpjAC[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroSat_signAC[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroSat_caminhoDLLSAT[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroSat_codigoAtivacao[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroSat_versaoDadosEntrada[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java

	char cnpjAC[64];
	char signAC[64];
	char caminhoDLLSAT[64];
	char codigoAtivacao[64];
	char versaoDadosEntrada[64];

	/*
	char cnpjAC[64] = "16716114000172";
	char signAC[64] = "SGR-SAT SISTEMA DE GESTAO E RETAGUARDA DO SAT";
	char caminhoDLLSAT[64] = "/usr/lib/libsat_v2_0_0_0_x86.so";
	char codigoAtivacao[64] = "12345678";
	char versaoDadosEntrada[64] = "0.06";
	*/

	strcpy(strParametroSat_cnpjAC, "16716114000172");
	strcpy(strParametroSat_signAC, "SGR-SAT SISTEMA DE GESTAO E RETAGUARDA DO SAT");
	strcpy(strParametroSat_caminhoDLLSAT, "/usr/lib/libsat_v2_0_0_0_x86.so");
	strcpy(strParametroSat_codigoAtivacao, "12345678");
	strcpy(strParametroSat_versaoDadosEntrada, "0.06");

/*
	strcpy(strParametroSat_cnpjAC, USERLITNNN);
	strcpy(strParametroSat_signAC, USERLITNNN);
	strcpy(strParametroSat_caminhoDLLSAT, USERLITNNN);
	strcpy(strParametroSat_codigoAtivacao, USERLITNNN);
	strcpy(strParametroSat_versaoDadosEntrada, USERLITNNN);

	strcpy(strParametroSat_cnpjAC, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroSat_signAC, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroSat_caminhoDLLSAT, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroSat_codigoAtivacao, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroSat_versaoDadosEntrada, gpstScfParam->aszLiterals[0]);
*/

	strcpy(cnpjAC, strParametroSat_cnpjAC);
	strcpy(signAC, strParametroSat_signAC);
	strcpy(caminhoDLLSAT, strParametroSat_caminhoDLLSAT);
	strcpy(codigoAtivacao, strParametroSat_codigoAtivacao);
	strcpy(versaoDadosEntrada, strParametroSat_versaoDadosEntrada);
	// fernando

	retorno = sat_VincularAgente(emissor, cnpjAC, signAC, caminhoDLLSAT, codigoAtivacao, versaoDadosEntrada, "", "", "", "", "");
	printf("\nretorno sat_VincularAgente = %d\n", retorno);

	// fernando
	char strParametroNfceSat_cnpj[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_tipoAmbiente[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_serie[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_modelo[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_naturezaOperacao[64]; // vetor de caracteres com no maximo 64 - pegar do bando criado pelo java
	char strParametroNfceSat_sequenciaAutorizacao[64]; // vetor de caracteres com no maximo 64 / deverá ser um combo com 2 opcoes "NFC-e;SAT", "SAT" - pegar do bando criado pelo java

	char cnpj[64];
	char tipoAmbiente[64];
	char serie[64];
	char modelo[64];
	char naturezaOperacao[64];
	char sequenciaAutorizacao[64];

	strcpy(strParametroNfceSat_cnpj, "45543915000777");
	strcpy(strParametroNfceSat_tipoAmbiente, "2");
	strcpy(strParametroNfceSat_serie, "001");
	strcpy(strParametroNfceSat_modelo, "65");
	strcpy(strParametroNfceSat_naturezaOperacao, "VENDA DE PRODUCAO DO ESTABELECIMENTO");
	strcpy(strParametroNfceSat_sequenciaAutorizacao, "NFC-e;SAT");

	/*
	strcpy(strParametroNfceSat_cnpj, USERLITNNN);
	strcpy(strParametroNfceSat_tipoAmbiente, USERLITNNN);
	strcpy(strParametroNfceSat_serie, USERLITNNN);
	strcpy(strParametroNfceSat_modelo, USERLITNNN);
	strcpy(strParametroNfceSat_naturezaOperacao, USERLITNNN);
	strcpy(strParametroNfceSat_sequenciaAutorizacao, USERLITNNN);

	strcpy(strParametroNfceSat_cnpj, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroNfceSat_tipoAmbiente, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroNfceSat_serie, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroNfceSat_modelo, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroNfceSat_naturezaOperacao, gpstScfParam->aszLiterals[0]);
	strcpy(strParametroNfceSat_sequenciaAutorizacao, gpstScfParam->aszLiterals[0]);
*/


	strcpy(cnpj, strParametroNfceSat_cnpj);
	strcpy(tipoAmbiente, strParametroNfceSat_tipoAmbiente);
	strcpy(serie, strParametroNfceSat_serie);
	strcpy(modelo, strParametroNfceSat_modelo);
	strcpy(naturezaOperacao, strParametroNfceSat_naturezaOperacao);
	strcpy(sequenciaAutorizacao, strParametroNfceSat_sequenciaAutorizacao);
	// fernando

	//sConfigurar_Emissao(emissor_nfce * e, const char *cnpj, const char *tipoAmbiente, const char *serie, const char *modelo, const char *naturezaOperacao, const char *sequenciaAutorizacao, const char *tipoControleNumeracao, const char *contingenciaAutomaticaDeRejeicao, const char *diretorioParaSalvarRespostaProtocolada, const char *reservado5, const char *reservado6, const char *reservado7, const char *reservado8, const char *reservado9);
	//retorno = sConfigurar_Emissao(emissor, "8723218000186", "2", "001", "65", "VENDA DE PRODUCAO DO ESTABELECIMENTO", "SAT", "", "", "", "", "", "", "", "");
	//retorno = sConfigurar_Emissao(emissor, "45543915000777", "2", "001", "65", "VENDA DE PRODUCAO DO ESTABELECIMENTO", "NFC-e;SAT", "", "", "", "", "", "", "", "");
	retorno = sConfigurar_Emissao(emissor, cnpj, tipoAmbiente, serie, modelo, naturezaOperacao, sequenciaAutorizacao, "", "", "", "", "", "", "", "");
	printf("\nretorno sConfigurar_Emissao = %d\n", retorno);

	//3. tVenda_StatusAgente
/*
e
Type:  emissor_nfce*
Handle para o emissor_nfce
codigoNFCe
Type:  char*
Código com o resultado da checagem da NFCe
0 - Não configurado
1 - Sucesso
2 - Erro
motivoNFCe
Type:  char*
Descrição do resultado da checagem da NFCe
codigoSAT
Type:  char*
Código com o resultado da checagem do SAT
0 - Não configurado
1 - Sucesso
2 - Erro
3 - Bloqueado
motivoSAT
Type:  char*
Descrição com o resultado da checagem do SAT
*/
	//tVenda_StatusAgente(emissor_nfce* e, char* codigoNFCe, char* motivoNFCe, char* codigoSAT, char* motivoSAT, char* reservado1, char* reservado2, char* reservado3, char* reservado4, char* reservado5, char* reservado6);
	char var1[64];
	char var2[64];
	char var3[64];
	char var4[64];
	char var5[64];
	char var6[64];
	char var7[64];
	char var8[64];
	char var9[64];

	retorno = tVenda_StatusAgente(emissor, var1, var2, var3, var4, "", "", "", "", "", "");
		printf("\nvar1 = %s\n", var1);
		printf("\nvar2 = %s\n", var2);
		printf("\nvar3 = %s\n", var3);
		printf("\nvar4 = %s\n", var4);
		printf("\nretorno tVenda_StatusAgente = %d\n", retorno);

	//4. eVenda_Abrir
/*
e
Type:  emissor_nfce *
Handle para o emissor_nfce
identificador
Type:  const char *
O PDV deve informar este identificador para que possa ser utilizado a consulta em casos de queda do PDV. Esse identificador deve ser único para cada Venda.
modoEmissao
Type:  const char *
Modo de emissão da venda. Os valores possíveis são 1="Normal" e 2="Em contingência". Este campo é necessário apenas se o tipo de controle de numeração informado no sConfigurar_Emissao for 20="Fornecedia pelo PFV"
*/
		// fernando
		char strParametroNfceSat_identificador[64]; // vetor de caracteres com no maximo 64 - pegar numero do caixa + transacao do tplinux

		char identificador[64];

		strcpy(strParametroNfceSat_identificador, "001");

		strcpy(identificador, strParametroNfceSat_identificador);
		// fernando

	//eVenda_Abrir(emissor_nfce * e, const char *identificador, const char *modoEmissao, const char *reservado2, const char *reservado3, const char *reservado4, const char *reservado5, const char *reservado6, const char *reservado7, const char *reservado8, const char *reservado9, const char *reservado10);
	retorno = eVenda_Abrir(emissor, identificador, "", "", "", "", "", "", "", "", "", "");
	printf("\nretorno eVenda_Abrir = %d\n", retorno);

	//5. eVenda_Item
/*
e
Type:  emissor_nfce *
Handle para o emissor_nfce
codigo
Type:  const char *
Código do produto vendido
descricao
Type:  const char *
Descrição do produto ou serviço
cEAN
Type:  const char *
GTIN (Global Trade Item Number) do produto, antigo código EAN ou código de barras do produto. Preencher com o código GTIN-8, GTIN-12, GTIN-13 ou GTIN-14
unidadeMedida
Type:  const char *
Informar a unidade de medida comercial do produto (KG, LT, CX)
quantidade
Type:  const char *
Quantidade de comercialização do produto
valorUnitario
Type:  const char *
Valor unitário de comercialização do produto
cEANTrib
Type:  const char *
GTIN (Global Trade Item Number) da unidade tributável do produto, antigo código EAN ou código de barras do produto. Preencher com o código GTIN-8, GTIN-12, GTIN-13 ou GTIN-14
unidadeMedidaTributavel
Type:  const char *
Informar a unidade de medida tributável do produto (KG, LT, CX)
quantidadeTributavel
Type:  const char *
Quantidade tributável do produto
valorUnitarioTributavel
Type:  const char *
Valor unitário de tributação do produto
tipoDescontoAcrescimo
Type:  const char *
Parâmetro para configurar o tipo do desconto ou acréscimo que será aplicado na venda do produto. "$" para valor ou "%" para percentual
valorAcrescimo
Type:  const char *
Valor de acréscimo do produto
valorDesconto
Type:  const char *
Valor de desconto do produto
arredondaTrunca
Type:  const char *
Parâmetro para indicar o tipo de cálculo que será efetuado na venda do item. "A" para arredondamento ou "T" para truncamento
ncm
Type:  const char *
Número do NCM (8 posições) para este item, ou gênero do item (2 posições)
cfop
Type:  const char *
Código fiscal de operações e prestações. Utilizar tabela de CFOP
informacaoAdicional
Type:  const char *
Informações adicionais do produto
cstIcms
Type:  const char *
Parâmetro responsável pela indicação do CST ICMS conforme a classificação da empresa. 00, 20, 40, 41, 60, 90
origemProduto
Type:  const char *
Parâmetro responsável pela indicação da origem do item. 0 - Nacional, 1 - Estrangeira (Importação direta), 2 - Estrangeira (Adquirida no mercado interno)
csosn
Type:  const char *
Código de situação das Operações para empresas enquadradas no regime tributário Simples Nacional. 101, 102, 103, 300, 400, 500, 900
percentualReducBCICMS
Type:  const char *
Percentual para redução da base de cálculo do ICMS
aliquotaICMS
Type:  const char *
Alíquota do ICMS
aliquotaCreditoICMSSN
Type:  const char *
Percentual para crédito do ICMS para empresas do Simples Nacional
CEST
Type:  const char *
Código Especificador da Substituição Tributária  CEST, que estabelece a sistemática de uniformização e identificação das mercadorias e bens passíveis de sujeição aos regimes de substituição tributária e de antecipação de recolhimento do ICMS
*/
	char codigo[64];
	char descricao[64];
	char cEAN[64];
	char unidadeMedida[64];
	char quantidade[64];
	char valorUnitario[64];
	char cEANTrib[64];
	char unidadeMedidaTributavel[64];
	char quantidadeTributavel[64];
	char valorUnitarioTributavel[64];
	char tipoDescontoAcrescimo[64];
	char valorAcrescimo[64];
	char valorDesconto[64];
	char arredondaTrunca[64];
	char ncm[64];
	char cfop[64];
	char informacaoAdicional[64];
	char cstIcms[64];
	char origemProduto[64];
	char csosn[64];
	char percentualReducBCICMS[64];
	char aliquotaICMS[64];
	char aliquotaCreditoICMSSN[64];
	char CEST[64];

	  short sLoop;
	  short sSign;
	  unsigned long ulThisFnxReturnValue = 0UL;

	  for (sLoop = 0; sLoop < (short)gstRegStates.usElectJrnlPos; sLoop++)
	  {
		  //		if ((gastEJ [sLoop].stItem.stHeader.uchRecType == ITEM_PLU_EJ_REC) &&
		  //				(strcmp((char *) gastEJ [sLoop].stItem..stItem.stPlu.szNmbr, pszPlu) == 0))
		  if (gastEJ [sLoop].stItem.stHeader.uchRecType == ITEM_PLU_EJ_REC)
		{
		  sSign = sGet_Sign(gastEJ [sLoop].stItem.unModFlags.stModBits);
		  ulThisFnxReturnValue += (unsigned long)
					(sSign * gastEJ[sLoop].stItem.ulQtySold);


			printf("\nsLoop = [%d]", sLoop);

			strcpy(codigo, "codigo001"); // pegar da aplicacao
			strcpy(descricao, gastEJ[sLoop].stItem.stItem.stPlu.szDesc); // pegar da aplicacao

			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.szDesc = [%s]", gastEJ[sLoop].stItem.stItem.stPlu.szDesc);

			//strcpy(cEAN, gastEJ[sLoop].stItem.szEntryNmbr); // pegar da aplicacao
			strcpy(cEAN, ""); // pegar da aplicacao
			strcpy(unidadeMedida, "KG"); // pegar da aplicacao
			sprintf(quantidade, "%ld", gastEJ[sLoop].stItem.stItem.stPlu.ulQty1/100); // pegar da aplicacao
			//sprintf(valorUnitario, "%ld", gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100); // pegar da aplicacao
			sprintf(valorUnitario, "%ld", (gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1 - gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt)/100); // pegar da aplicacao

			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulPrice1 [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1);
			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100 [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100);
			printf("\n(gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100) - gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt [%ld]", (gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1 - gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt)/100);

			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulQty1 = [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulQty1);
			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulPrice1 = [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1);
			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulPriceP = [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulPriceP);
			printf("\ngastEJ[sLoop].stItem.stItem.stPlu.ulMrpPrice1 = [%ld]", gastEJ[sLoop].stItem.stItem.stPlu.ulMrpPrice1);
			printf("\ngastEJ[sLoop].stItem.stPromoTotal.lPromoAmnt = [%ld]", (gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1 - gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt)/100);


			//strcpy(cEANTrib, gastEJ[sLoop].stItem.szEntryNmbr); // pegar da aplicacao
			strcpy(cEANTrib, ""); // pegar da aplicacao
			strcpy(unidadeMedidaTributavel, "KG"); // igual a de cima
			sprintf(quantidadeTributavel, "%ld", gastEJ[sLoop].stItem.stItem.stPlu.ulQty1); // igual a de cima
			//sprintf(valorUnitarioTributavel, "%ld", gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100); // igual a de cima
			sprintf(valorUnitarioTributavel, "%ld", (gastEJ[sLoop].stItem.stItem.stPlu.ulPrice1/100) - gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt); // igual a de cima
			strcpy(tipoDescontoAcrescimo, "$"); // passar o caractere "$"
			strcpy(valorAcrescimo, ""); // vazio
			//strcpy(valorDesconto, gastEJ[gstDiscGlobals.sEjCoupon].stPromo.llValue);
			//strcpy(valorDesconto, gastEJ[sLoop].stItem.stPromoTotal[1].lPromoAmnt/100); // pegar da aplicacao
			strcpy(valorDesconto, ""); // pegar da aplicacao
			strcpy(arredondaTrunca, "A"); // passar o caractere "A"
			strcpy(ncm, "19041000"); // pegar da aplicacao
			strcpy(cfop, "5102"); // pegar da aplicacao
			strcpy(informacaoAdicional, "informacaoAdicional001"); // pegar da aplicacao
			strcpy(cstIcms, "40"); // pegar da aplicacao
			strcpy(origemProduto, "0"); // pegar da aplicacao
			strcpy(csosn, ""); // vazio
			strcpy(percentualReducBCICMS, ""); // vazio
			strcpy(aliquotaICMS, ""); // pegar da aplicacao
			strcpy(aliquotaCreditoICMSSN, ""); // pegar da aplicacao
			strcpy(CEST, "1702401"); // pegar da aplicacao


			/*eVenda_Item(	emissor_nfce * e, 
							const char *codigo, 
							const char *descricao, 
							const char *cEAN, 
							const char *unidadeMedida, 
							const char *quantidade, 
							const char *valorUnitario, 
							const char *cEANTrib, 
							const char *unidadeMedidaTributavel, 
							const char *quantidadeTributavel, 
							const char *valorUnitarioTributavel, 
							const char *tipoDescontoAcrescimo, 
							const char *valorAcrescimo, 
							const char *valorDesconto, 
							const char *arredondaTrunca, 
							const char *ncm, 
							const char *cfop, 
							const char *informacaoAdicional, 
							const char *cstIcms, 
							const char *origemProduto, 
							const char *csosn, 
							const char *percentualReducBCICMS, 
							const char *aliquotaICMS, 
							const char *aliquotaCreditoICMSSN, 
							const char *CEST, 
							const char *reservado2, 
							const char *reservado3, 
							const char *reservado4, 
							const char *reservado5, 
							const char *reservado6, 
							const char *reservado7, 
							const char *reservado8, 
							const char *reservado9, 
							const char *reservado10, 
							const char *reservado11, 
							const char *reservado12, 
							const char *reservado13, 
							const char *reservado14, 
							const char *reservado15, 
							const char *reservado16, 
							const char *reservado17, 
							const char *reservado18, 
							const char *reservado19, 
							const char *reservado20);
	/*
			retorno = eVenda_Item(emissor,
			"codigo001",
			"descricao001",
			"",
			"KG",
			"001",
			"1.00",
			"",
			"KG",
			"1",
			"1.00",
			"$",
			"1.00",
			"1.00",
			"A",
			"19041000",
			"5102",
			"informacaoAdicional001",
			"40",
			"0",
			"",
			"",
			"",
			"",
			"1702401", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "");
	*/
			retorno = eVenda_Item(emissor,
			codigo,
			descricao,
			cEAN,
			unidadeMedida,
			quantidade,
			valorUnitario,
			cEANTrib,
			unidadeMedidaTributavel,
			quantidadeTributavel,
			valorUnitarioTributavel,
			tipoDescontoAcrescimo,
			valorAcrescimo,
			valorDesconto,
			arredondaTrunca,
			ncm,
			cfop,
			informacaoAdicional,
			cstIcms,
			origemProduto,
			csosn,
			percentualReducBCICMS,
			aliquotaICMS,
			aliquotaCreditoICMSSN,
			CEST,
			"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "");
			printf("\nretorno eVenda_Item = %d\n", retorno);

			char cst[64];

			strcpy(cst, "06"); // pegar da aplicacao

			/*
			retorno = eVenda_ItemPISNaoTributavel(
					emissor,
					"06",// pegar da aplicacao
					"",
					"",
					"",
					"",
					"",
					"");
					*/
			retorno = eVenda_ItemPISNaoTributavel(
					emissor,
					cst,// pegar da aplicacao
					"",
					"",
					"",
					"",
					"",
					"");
			printf("\nretorno ItemPISNaoTributavel = %d\n", retorno);

			//strcpy(cst ,); // pegar da aplicacao

		/*
			retorno = eVenda_ItemCOFINSNaoTributavel(
					emissor,
					"06",// pegar da aplicacao
					"",
					"",
					"",
					"",
					"",
					"");
					*/
			retorno = eVenda_ItemCOFINSNaoTributavel(
					emissor,
					cst,// pegar da aplicacao
					"",
					"",
					"",
					"",
					"",
					"");
			printf("\nretorno eVenda_ItemCOFINSNaoTributavel = %d\n", retorno);

		}
	  }



	//6. [...] demais métodos “eVenda” de acordo com a necessidade
/*
*/

	//7. eVenda_Pagamento
/*
e
Type:  emissor_nfce *
Handle para o emissor_nfce
tipo_pagamento
Type:  const char *
Este parâmetro indica a forma do pagamento, e deve ser informado um dos valores abaixo:
01- Dinheiro
02- Cheque
03- Cartão de Crédito
04- Cartão de Débito
05- Crédito Loja
10- Vale Alimentação
11- Vale Refeição
12- Vale Presente
13- Vale Combustível
99- Outros

valor_pagamento
Type:  const char *
Valor do pagamento. Utilizar sempre "," ou "." para a indicação das decimais
card_cnpj
Type:  const char *
CNPJ da credenciadora de cartão de crédito ou débito.
card_tp_band
Type:  const char *
Bandeira da operação de carção de crédito ou débito. Deve ser informado um dos valores abaixo:
01 - Visa
02 - Mastercard
03 - American Express
04 - Sorocred
99 - Outros

card_cAut
Type:  const char *
Número de autorização da operação do cartão de crédito ou débito.
card_cod_adm
Type:  const char *
Código da credenciadora de cartão de crédito ou débito
tp_integra
Type:  const char *
Tipo de Integração para pagamento:
1 - Pagamento integrado com o sistema de automação da empresa (Ex.: equipamento TEF, Comércio Eletrônico);
2 - Pagamento não integrado com o sistema de automação da empresa (Ex.: equipamento POS);

*/


	// fernando
	char tipo_pagamento[64];
	char valor_pagamento[64];
	char card_cnpj[64];
	char card_tp_band[64];
	char card_cAut[64];
	char card_cod_adm[64];
	char tp_integra[64];

	int iQuantidadeDePagamentos=1;
	int iNumeroDoPagamento=0;

	for(iNumeroDoPagamento=0; iNumeroDoPagamento < iQuantidadeDePagamentos; iNumeroDoPagamento++){
		//if (gastEJ [sLoop].stItem.stHeader.uchRecType == MEDIA_EJ_REC)
		//{
		printf("\niNumeroDoPagamento = [%d]", iNumeroDoPagamento);
		printf("\ngastEJ[iNumeroDoPagamento].stEJMedia.stMedia.uchMediaType [%d]", gastEJ[iNumeroDoPagamento].stEJMedia.stMedia.uchMediaType);
		printf("\ngastEJ[iNumeroDoPagamento].stEJMedia.stMedia.uchMediaNmbr [%d]", gastEJ[iNumeroDoPagamento].stEJMedia.stMedia.uchMediaNmbr);
		printf("\ngastEJ[iNumeroDoPagamento].stEJMedia.stMedia.lMediaAmt [%ld]", gastEJ[iNumeroDoPagamento].stEJMedia.stMedia.lMediaAmt);
		printf("\nlGet_Subtl()/100 [%ld]", lGet_Subtl()/100);

		//strcpy(tipo_pagamento, (char *)gastEJ[iNumeroDoPagamento].stEJMedia.stMedia.uchMediaType);
		strcpy(tipo_pagamento, "01");
		//sprintf(valor_pagamento, "%ld", gastEJ[iNumeroDoPagamento].stEJMedia.stMedia.lMediaAmt);
		sprintf(valor_pagamento, "%ld", lGet_Subtl()/100);

		printf("\nlGet_Subtl() [%ld]", lGet_Subtl());
		printf("\nlGet_Subtl()/100 [%ld]", lGet_Subtl()/100);


		strcpy(valor_pagamento, "0.00");

		if((strcmp(tipo_pagamento, "03") == 0) || (strcmp(tipo_pagamento, "04") == 0)){
			strcpy(card_cnpj, "");
			strcpy(card_tp_band, "");
			strcpy(card_cAut, "");
			strcpy(card_cod_adm, "");
			strcpy(tp_integra, "");
		}
		else{
			strcpy(card_cnpj, "");
			strcpy(card_tp_band, "");
			strcpy(card_cAut, "");
			strcpy(card_cod_adm, "");
			strcpy(tp_integra, "");
		}
		//}

		//eVenda_Pagamento(emissor_nfce * e, const char *tipo_pagamento, const char *valor_pagamento, const char *card_cnpj, const char *card_tp_band, const char *card_cAut, const char *tp_integra, const char *reservado2, const char *reservado3, const char *reservado4, const char *reservado5);
		//retorno = eVenda_Pagamento(emissor, "03", "1.00", "123.123.123.0001-12", "01", "001", "2", "", "", "", "");
		//retorno = eVenda_Pagamento(emissor, "01", "1.00", "", "", "", "", "", "", "", "");
		retorno = eVenda_Pagamento(emissor,
				tipo_pagamento,
				valor_pagamento,
				card_cnpj,
				card_tp_band,
				card_cAut,
				card_cod_adm,
				tp_integra, "", "", "");
		printf("\nretorno eVenda_Pagamento = %d\n", retorno);
	}
	// fernando


	//8. eVenda_Fechar
/*
e
Type:  emissor_nfce *
Handle para o emissor_nfce
inf_complementar
Type:  const char *
Informações complementares de interesse do contribuinte
inf_adicionais_fisco
Type:  const char *
Informações adicionais de interesse do Fisco
tipo_desconto
Type:  const char *
Tipo de desconto. 1 - Condicional (Não recalcula os impostos). 2 - Incondicional (Recalcula os impostos). Caso não informado, será aplicado valor 1 (Condicional)
*/

	char inf_complementar001[64];
	char inf_adicionais_fisco001[64];

	strcpy(inf_complementar001, "inf_complementar001");
	strcpy(inf_adicionais_fisco001, "inf_adicionais_fisco001");

	//eVenda_Fechar(emissor_nfce * e, const char *inf_complementar, const char *inf_adicionais_fisco, const char *tipo_desconto, const char *reservado1, const char *reservado2, const char *reservado3, const char *reservado4);
	//retorno = eVenda_Fechar(emissor, "inf_complementar001", "inf_adicionais_fisco001", "1", "", "", "", "");
	retorno = eVenda_Fechar(emissor, inf_complementar001, inf_adicionais_fisco001, "", "", "", "", "");
	printf("\nretorno eVenda_Fechar = %d\n", retorno);

	//9. rVenda_StatusUltimaEmissao
/*
e
Type:  emissor_nfce*
Handle para o emissor_nfce
chave
Type:  char*
O Buffer que receberá a chave da NFC-e (44 Caracteres)
situacaoOperacao
Type:  char*
O Buffer que receberá a Situação da Operação da NFC-e (1- Sucesso, 2 - Erro, 3 - Denegada) (2 Caracteres máx.).
status
Type:  char*
O Buffer que receberá o status da NFC-e (2 Caracteres máx.)
protocolo
Type:  char*
O Buffer que receberá o protocolo da NFC-e (15 Caracteres máx.)
digval
Type:  char*
O Buffer que receberá o DigVal da NFC-e (28 Caracteres máx.)
urlConsulta
Type:  char*
O Buffer que receberá a url de consulta da NFC-e (640 Caracteres máx.)
codigoErro
Type:  char*
O Buffer que contém a código do erro. (5 Caracteres máx.)
descricaoErro
Type:  char*
O Buffer que contém a descrição da rejeição, quando a NF-e está rejeitada. (510 Caracteres máx.)
podeReutilizarNumero
Type:  char*
O Buffer que contém a indicação se é possível reutilizar o número, quando não controlado pelo NFCePack (0: Não pode, 1; Pode)
*/
	//rVenda_StatusUltimaEmissao(emissor_nfce* e, char* chave, char* situacaoOperacao, char* status, char* protocolo, char* digval, char* urlConsulta, char* codigoErro, char* descricaoErro, char* podeReutilizarNumero, const char* reservado1, const char* reservado2, const char* reservado3, const char* reservado4, const char* reservado5);
	retorno = rVenda_StatusUltimaEmissao(emissor, var1, var2, var3, var4, var5, var6, var7, var8, var9, "", "", "", "", "");
	printf("\nvar1 = %s\n", var1);
	printf("\nvar2 = %s\n", var2);
	printf("\nvar3 = %s\n", var3);
	printf("\nvar4 = %s\n", var4);
	printf("\nvar5 = %s\n", var5);
	printf("\nvar6 = %s\n", var6);
	printf("\nvar7 = %s\n", var7);
	printf("\nvar8 = %s\n", var8);
	printf("\nvar9 = %s\n", var9);
	printf("\nretorno rVenda_StatusUltimaEmissao = %d\n", retorno);

/*
retorno = sat_ExtrairLogs(
			emissor,
			var1,
			var2,
			var3,
			var4,
			var5,
			"",
			"",
			"",
			"",
			"");

	printf("\nretorno = %d\n", retorno);
	printf("\nvar1 = %s\n", var1);
	printf("\nvar2 = %s\n", var2);
	printf("\nvar3 = %s\n", var3);
	printf("\nvar4 = %s\n", var4);
	printf("\nvar5 = %s\n", var5);
*/

/*
	//Operação do dia-a-dia:
	//1. tVenda_StatusAgente
	retorno = tVenda_StatusAgente(emissor_nfce* e, char* codigoNFCe, char* motivoNFCe, char* codigoSAT, char* motivoSAT, char* reservado1, char* reservado2, char* reservado3, char* reservado4, char* reservado5, char* reservado6);
	printf("\nretorno = %d\n", retorno);
	//2. eVenda_Abrir
	retorno = eVenda_Abrir(emissor_nfce * e, const char *identificador, const char *modoEmissao, const char *reservado2, const char *reservado3, const char *reservado4, const char *reservado5, const char *reservado6, const char *reservado7, const char *reservado8, const char *reservado9, const char *reservado10);
	printf("\nretorno = %d\n", retorno);
	//3. eVenda_Item
	retorno = eVenda_Item(emissor_nfce * e, const char *codigo, const char *descricao, const char *cEAN, const char *unidadeMedida, const char *quantidade, const char *valorUnitario, const char *cEANTrib, const char *unidadeMedidaTributavel, const char *quantidadeTributavel, const char *valorUnitarioTributavel, const char *tipoDescontoAcrescimo, const char *valorAcrescimo, const char *valorDesconto, const char *arredondaTrunca, const char *ncm, const char *cfop, const char *informacaoAdicional, const char *cstIcms, const char *origemProduto, const char *csosn, const char *percentualReducBCICMS, const char *aliquotaICMS, const char *aliquotaCreditoICMSSN, const char *CEST, const char *reservado2, const char *reservado3, const char *reservado4, const char *reservado5, const char *reservado6, const char *reservado7, const char *reservado8, const char *reservado9, const char *reservado10, const char *reservado11, const char *reservado12, const char *reservado13, const char *reservado14, const char *reservado15, const char *reservado16, const char *reservado17, const char *reservado18, const char *reservado19, const char *reservado20);
	printf("\nretorno = %d\n", retorno);
	//4. [...] demais métodos “eVenda” de acordo com a necessidade
	//5. eVenda_Pagamento
	retorno = eVenda_Pagamento(emissor_nfce * e, const char *tipo_pagamento, const char *valor_pagamento, const char *card_cnpj, const char *card_tp_band, const char *card_cAut, const char *tp_integra, const char *reservado2, const char *reservado3, const char *reservado4, const char *reservado5);
	printf("\nretorno = %d\n", retorno);
	//6. eVenda_Fechar
	retorno = eVenda_Fechar(emissor_nfce * e, const char *inf_complementar, const char *inf_adicionais_fisco, const char *tipo_desconto, const char *reservado1, const char *reservado2, const char *reservado3, const char *reservado4);
	printf("\nretorno = %d\n", retorno);
	//7. rVenda_StatusUltimaEmissao
	retorno = rVenda_StatusUltimaEmissao(emissor_nfce* e, char* chave, char* situacaoOperacao, char* status, char* protocolo, char* digval, char* urlConsulta, char* codigoErro, char* descricaoErro, char* podeReutilizarNumero, const char* reservado1, const char* reservado2, const char* reservado3, const char* reservado4, const char* reservado5);
	printf("\nretorno = %d\n", retorno);
	*/
	// fernando
  }

  vRestore_Stack_For_User_Return();
  return (USER_EXIT_RETURN);
}
