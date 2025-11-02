#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TK_int 1
#define TK_float 2
#define TK_char 3
#define TK_struct 4
#define TK_if 5
#define TK_else 6


#define TK_id 7
#define TK_Abre_Colch 8
#define TK_Fecha_Colch 9
#define TK_Abre_Chaves 10
#define TK_Fecha_Chaves 11
#define TK_Fim_Arquivo 12
#define TK_Atrib 13
#define TK_Const_Int 14
#define TK_Mais 15
#define TK_Menos 16
#define TK_Mult 17
#define TK_Abre_Par 18
#define TK_Fecha_Par 19
#define TK_virgula 20
#define TK_pv 21
#define TK_Maior 22
#define TK_Menor 23
#define TK_Menor_Igual 24
#define TK_Maior_Igual 25
#define TK_Igual 26
#define TK_Diferente 27
#define TK_while 28
#define TK_and_bitwise 29
#define TK_and_logico 30
#define TK_or_logico 31
#define TK_do 32
#define TK_for 33
#define TK_break 34
#define TK_continue 35
#define TK_label 36
#define TK_goto 37
#define TK_Div 38

/***********************************************************************************/
/*                                                                                 */
/*  INICIO DO LEXICO - Nao entre a nao ser que tenha interesse pessoal em lexicos  */
/*                                                                                 */
/***********************************************************************************/


int linlex = 0, collex = 1;

typedef struct bloco
{
   char nome[20];
   int succs[10];
   char commands[2000];
   int proxSucc;
} bb;

bb grafo[50];
int grafoMudou = 0;
int blocoAtual = 0;
int proxBloco = 1;
int blocoSendoAvaliado = 0;

char tokens[][20] = {"", "TK_int",
                     "TK_float",
                     "TK_char",
                     "TK_struct",
                     "TK_if",
                     "TK_else",
                     "TK_id",
                     "TK_Abre_Colch",
                     "TK_Fecha_Colch",
                     "TK_Abre_Chaves",
                     "TK_Fecha_Chaves",
                     "TK_Fim_Arquivo",
                     "TK_Atrib",
                     "TK_Const_Int",
                     "TK_Mais",
                     "TK_Menos",
                     "TK_Mult",
                     "TK_Abre_Par",
                     "TK_Fecha_Par",
                     "TK_virgula",
                     "TK_pv",
                     "TK_Maior",
                     "TK_Menor",
                     "TK_Menor_Igual",
                     "TK_Maior_Igual",
                     "TK_Igual",
                     "TK_Diferente",
                     "TK_while",
                     "TK_and_bitwise",
                     "TK_and_logico",
                     "TK_or_logico",
                     "TK_do",
                     "TK_for",
                     "TK_break",
                     "TK_continue",
                     "TK_label",
                     "TK_goto",
                     "TK_Div"
                  };


typedef struct pal
{
   char palavra[20];
   int token;
} tpal;
tpal reservadas[] = {{"", 0},
                     {"int", TK_int},
                     {"float", TK_float},
                     {"char", TK_char},
                     {"struct", TK_struct},
                     {"if", TK_if},
                     {"else", TK_else},
                     {"while", TK_while},
                     {"do", TK_do},
                     {"for", TK_for},
                     {"break", TK_break},
                     {"continue", TK_continue},
                     {"goto", TK_goto},
                     {"fim", -1}};


FILE *arqin;
int token;
char lex[20];
char c;
// variaveis do marca - restaura


int tokenant;
long posarq;
char lexant[100];
char cant;
int marcou = 0;


void marca_pos()
{
   marcou = 1;
   tokenant = token;
   posarq = ftell(arqin);
   strcpy(lexant, lex);
   cant = c;
}


void restaura()
{
   fseek(arqin, posarq, SEEK_SET);
   token = tokenant;
   strcpy(lex, lexant);
   c = cant;
}


char le_char()
{
   if (fread(&c, 1, 1, arqin) == 0)
      return -1;
   if (c == '\n')
   {
      linlex++;
      collex = 1;
   }
   else
      collex++;
   return c;
};


int pal_res(char lex[])
{
   int tk = 0;
   while (strcmp(reservadas[tk].palavra, "fim") != 0)
   {
      if (strcmp(lex, reservadas[tk].palavra) == 0)
         return reservadas[tk].token;
      tk++;
   }

   if(lex[strlen(lex) - 1] == ':'){
      return TK_label;
   }
   return TK_id;
}


int le_token()
{
   static int pos = 0;
   static int estado = 0;
   strcpy(lex, "");
   while (1)
   {
      switch (estado)
      {
      case 0:
         if (c == ',')
         {
            c = le_char();
            return TK_virgula;
         }
         if (c == '+')
         {
            c = le_char();
            return TK_Mais;
         }
         if (c == '-')
         {
            c = le_char();
            return TK_Menos;
         }
         if (c == '*')
         {
            c = le_char();
            return TK_Mult;
         }
         if (c == '{')
         {
            c = le_char();
            return TK_Abre_Chaves;
         }
         if (c == '}')
         {
            c = le_char();
            return TK_Fecha_Chaves;
         }
         if (c == ';')
         {
            c = le_char();
            strcpy(lex, ";");
            return TK_pv;
         }
         if (c == '[')
         {
            c = le_char();
            return TK_Abre_Colch;
         }
         if (c == ']')
         {
            c = le_char();
            return TK_Fecha_Colch;
         }
         if (c == '(')
         {
            c = le_char();
            return TK_Abre_Par;
         }
         if (c == ')')
         {
            c = le_char();
            return TK_Fecha_Par;
         }
         if (c == '=')
         {
            c = le_char();
            if (c == '=')
            {
               c = le_char();
               return TK_Igual;
            }
            strcpy(lex, "=");
            return TK_Atrib;
         }
         if (c == '<')
         {
            c = le_char();
            if (c == '=')
            {
               c = le_char();
               return TK_Menor_Igual;
            }
            return TK_Menor;
         }
         if (c == '&')
         {
            c = le_char();
            if (c == '&')
            {
               c = le_char();
               return TK_and_logico;
            }
            return TK_and_bitwise;
         }
         if (c == '>')
         {
            c = le_char();
            if (c == '=')
            {
               c = le_char();
               return TK_Maior_Igual;
            }
            return TK_Maior;
         }
         if (c == '!')
         {
            c = le_char();
            if (c == '=')
            {
               c = le_char();
               return TK_Diferente;
            }
         }
         if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
         {
            lex[0] = c;
            c = le_char();
            pos = 1;
            estado = 1;
            break;
         }
         if (c >= '0' && c <= '9')
         {
            lex[0] = c;
            c = le_char();
            pos = 1;
            estado = 2;
            break;
         }
         if (c == -1)
            return TK_Fim_Arquivo;
         if (c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == ' ')
         {
            c = le_char();
            break;
         }
      case 1:
         if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9' || c == ':')
         {
            lex[pos++] = c;
            c = le_char();
            break;
         }
         else
         {
            estado = 0;
            lex[pos] = '\0';
            return pal_res(lex);
         }
      case 2:
         if (c >= '0' && c <= '9')
         {
            lex[pos++] = c;
            c = le_char();
            break;
         }
         else
         {
            estado = 0;
            lex[pos] = '\0';
            return TK_Const_Int;
         }
      }
   }
}


/********************/
/*                  */
/*  FIM DO L�XICO   */
/*                  */
/********************/


#define MAX_COD 1000

void mostra_t()
{
   printf("%s lex=%s na lin %d, col %d\n", tokens[token], lex, linlex, collex);
}

int geraBloco(char nomeBloco[20])
{
   for(int i = 0; i < proxBloco; i++){
      if(strcmp(grafo[i].nome, nomeBloco) == 0){
         return i;
      }
   }

   grafo[proxBloco].proxSucc = 0;
   strcpy(grafo[proxBloco].nome, nomeBloco);
   proxBloco++;
   grafoMudou = 1;
   return proxBloco - 1;
       
}

int obterIndiceBloco(char nomeBloco[20])
{
   for(int i = 0; i < proxBloco; i++){
      if(strcmp(grafo[i].nome, nomeBloco) == 0){
         return i;
      }
   }

   return -1;
       
}

void addSucc(char nomeBloco[]){
   if(blocoAtual == -1) return;

   for(int i = 0; i < proxBloco; i++){
      // Procura se sucessor já existe no grafo
      if(strcmp(grafo[i].nome, nomeBloco) == 0){

         // Verifica se successor ja foi adicionado no nodo atual
         for(int j = 0; j < grafo[blocoAtual].proxSucc; j++){
            if(grafo[blocoAtual].succs[j] == i){
               return;
            }
         }
         // Adiciona successor
         grafo[blocoAtual].succs[grafo[blocoAtual].proxSucc] = i;
         grafo[blocoAtual].proxSucc++;
         grafoMudou = 1;
         return;
      }
   }
   
   // Se não encontrar bloco sucessor, não adiciona o bloco 
   // (em alguns casos, o bloco só vai ser adicionado quando encontrar um GoTo para essa label que pode ainda não ter aparecido)
   return;
}


/****************/
/*              */
/*  EXPRESS�ES  */
/*              */
/****************/


int MultiplicativeExpression(char T_p[MAX_COD], char T_c[MAX_COD]);
int MultiplicativeExpressionRest(char S_hp[MAX_COD], char S_sp[MAX_COD], char S_hc[MAX_COD], char S_sc[MAX_COD]);
int AdditiveExpression(char E_p[MAX_COD], char E_c[MAX_COD]);
int AdditiveExpressionRest(char R_hp[MAX_COD], char R_sp[MAX_COD], char R_hc[MAX_COD], char R_sc[MAX_COD]);
int PrimaryExpression(char F_p[MAX_COD], char F_c[MAX_COD]);
int RelationalExpression(char E_p[MAX_COD], char E_c[MAX_COD]);
int RelationalExpressionRest(char R_hp[MAX_COD], char R_sp[MAX_COD], char R_hc[MAX_COD], char R_sc[MAX_COD]);
int AssignmentExpression(char A_p[MAX_COD], char A_c[MAX_COD]);
int Command(char Com_c[MAX_COD]);
int CommandList(char Com_c[MAX_COD]);
int Expression(char e_p[MAX_COD], char e_c[MAX_COD]);

int AssignmentExpression(char A_p[MAX_COD], char A_c[MAX_COD]){
   char A1_c[MAX_COD] = "", A1_p[MAX_COD] = ""; 
   marca_pos();
   if(token == TK_id) {
      sprintf(A1_c, "%s %s", A_c, lex);
      token = le_token();
      if(token == TK_Atrib) {
         sprintf(A1_c, "%s %s", A1_c, lex);
         token = le_token();
         if(AssignmentExpression(A1_p, A1_c)) {
            sprintf(A_c, "%s \n", A1_c);
            return 1;
         }
      }
   }
   restaura();
   if(RelationalExpression(A_p, A_c)){
      return 1;
   }

   return 0;
}

int RelationalExpression(char E_p[MAX_COD], char E_c[MAX_COD])
{
   char T_p[MAX_COD], T_c[MAX_COD], R_hp[MAX_COD], R_sp[MAX_COD], R_hc[MAX_COD], R_sc[MAX_COD];

   if (AdditiveExpression(E_p, E_c))
   {
      if (RelationalExpressionRest(E_p, R_sp, E_c, R_sc))
      {
         strcpy(E_c, R_sc);
         return 1;
      }
   }
   return 0;
}

int RelationalExpressionRest(char R_hp[MAX_COD], char R_sp[MAX_COD], char R_hc[MAX_COD], char R_sc[MAX_COD])
{
   char T_c[MAX_COD], R1_hc[MAX_COD], R1_sc[MAX_COD], T_p[MAX_COD], R1_hp[MAX_COD], R1_sp[MAX_COD];
   if (token == TK_Maior)
   {
      sprintf(R_sc, "%s >", R_hc, lex);
      token = le_token();
      if (AdditiveExpression(R_hp, R_sc))
      {
         if (RelationalExpressionRest(R_hp, R_sp, R1_hc, R1_sc))
         {
            return 1;
         }
      }
      return 0;
   }
   if (token == TK_Menor)
   {
      sprintf(R_sc, "%s <", R_hc, lex);
      token = le_token();
      if (AdditiveExpression(R_hp, R_sc))
      {
         if (RelationalExpressionRest(R_hp, R_sp, R1_hc, R1_sc))
         {
            return 1;
         }
      }
      return 0;
   }

   strcpy(R_sc, R_hc);
   strcpy(R_sp, R_hp);
   return 1;
}

int AdditiveExpression(char E_p[MAX_COD], char E_c[MAX_COD])
{
   char T_p[MAX_COD], T_c[MAX_COD], R_hp[MAX_COD], R_sp[MAX_COD], R_hc[MAX_COD], R_sc[MAX_COD];
   if (MultiplicativeExpression(T_p, E_c))
   {
      if (AdditiveExpressionRest(E_p, R_sp, E_c, R_sc))
      {
         strcpy(E_c, R_sc);
         return 1;
      }
   }
   return 0;
}


int AdditiveExpressionRest(char R_hp[MAX_COD], char R_sp[MAX_COD], char R_hc[MAX_COD], char R_sc[MAX_COD])
{
   char T_c[MAX_COD], R1_hc[MAX_COD], R1_sc[MAX_COD], T_p[MAX_COD], R1_hp[MAX_COD], R1_sp[MAX_COD];
   if (token == TK_Mais)
   {
      sprintf(R_sc, "%s +", R_hc);
      token = le_token();
      if (MultiplicativeExpression(R_hp, R_sc))
      {
         if (AdditiveExpressionRest(R_hp, R1_sp, R_sc, R1_sc))
         {
            strcpy(R_sc, R1_sc);
            return 1;
         }
      }
      return 0;
   }
   if (token == TK_Menos)
   {
      token = le_token();
      sprintf(R_sc, "%s -", R_hc);
      if (MultiplicativeExpression(R_hp, R_sc))
      {
         if (AdditiveExpressionRest(R_hp, R1_sp, R_sc, R1_sc))
         {
            strcpy(R_sc, R1_sc);
            return 1;
         }
      }
      return 0;
   }

   strcpy(R_sc, R_hc);
   strcpy(R_sp, R_hp);
   return 1;
}


int MultiplicativeExpression(char T_p[MAX_COD], char T_c[MAX_COD])
{
   char F_c[MAX_COD], F_p[MAX_COD], S_hp[MAX_COD], S_sp[MAX_COD], S_hc[MAX_COD], S_sc[MAX_COD];
   if (PrimaryExpression(T_p, T_c))
   {
      if (MultiplicativeExpressionRest(T_p, S_sp, T_c, S_sc))
      {
         return 1;
      }
   }
   return 0;
}


int MultiplicativeExpressionRest(char S_hp[MAX_COD], char S_sp[MAX_COD], char S_hc[MAX_COD], char S_sc[MAX_COD])
{
   char F_c[MAX_COD], S1_hc[MAX_COD], S1_sc[MAX_COD], F_p[MAX_COD], S1_hp[MAX_COD], S1_sp[MAX_COD];
   if (token == TK_Mult)
   {
      sprintf(S_sc, "%s *", S_hc);
      token = le_token();
      if (PrimaryExpression(F_p, F_c))
      {
         if (MultiplicativeExpressionRest(S1_hp, S1_sp, S1_hc, S1_sc))
         {
            return 1;
         }
      }
      return 0;
   }

   if (token == TK_Div)
   {
      sprintf(S_sc, "%s /", S_hc);
      token = le_token();
      if (PrimaryExpression(F_p, F_c))
      {
         if (MultiplicativeExpressionRest(S1_hp, S1_sp, S1_hc, S1_sc))
         {
            return 1;
         }
      }
      return 0;
   }
   return 1;
}

// F->id
// F->cte
int PrimaryExpression(char F_p[MAX_COD], char F_c[MAX_COD])
{
   if (token == TK_Const_Int)
   {
      sprintf(F_c, "%s %s", F_c, lex);
      token = le_token();
      return 1;
   }
   if (token == TK_id)
   {
      sprintf(F_c, "%s %s", F_c, lex);
      token = le_token();
      return 1;
   }
   return 0;
}

int JumpExpression(char Com_c[MAX_COD]) {
   if(token == TK_goto){
      token = le_token();
      if(token == TK_id){
         sprintf(Com_c, "%s goto %s", Com_c, lex);
         // Adiciona o bloco sucessor por conta do desvio incondicional
         geraBloco(lex);
         addSucc(lex);
         token = le_token();
         return 1;
      }
   }
   return 0;
}

int ifCount = 0;
int IfExpression(char If_c[MAX_COD])
{
   char blocoAbaixo[20];
   if (token == TK_if)
   {
      sprintf(If_c, "%s %s", If_c, lex);
      token = le_token();
     
         char E_p[MAX_COD], E_cod[MAX_COD];
         if (Expression(E_p, If_c))
         {
               char E2_cod[MAX_COD];
               if (JumpExpression(If_c))
               {
                  // Regra 3: Gera novo bloco devido ao desvio condicional 
                  sprintf(blocoAbaixo, "Seq_if_%d", ifCount++);
                  int i = geraBloco(blocoAbaixo);
                  // Adiciona o bloco sucessor por conta da execução sequencial (caso não atenda o if)
                  addSucc(blocoAbaixo);
                  strcpy(grafo[blocoAtual].commands, If_c);
                  strcpy(If_c, "");
                  blocoAtual = i;
                  blocoSendoAvaliado = blocoAtual;
                  return 1;
               }
            
         }
      
   }


   return 0;
}

int Expression(char e_p[MAX_COD], char e_c[MAX_COD]){
   if(AssignmentExpression(e_p, e_c)){
      return 1;
   }

   return 0;
}


int CommandList(char Com_c[MAX_COD]){
   char Command_c[MAX_COD], CommandList_c[MAX_COD];

   if(token == TK_Fim_Arquivo){
      return 1;
   }

   if(Command(Com_c)){
      if(CommandList(Com_c)){
         return 1;
      }
   }
   
   return 0;


}

int JumpLabel(char Com_c[MAX_COD]){
   char nomeBloco[20];
   strcpy(nomeBloco, lex);
   nomeBloco[strlen(lex) - 1] = '\0';

   // Regra 2: Novos blocos são gerados quando tem algum jump que aponta pra linha 
   // Nesse caso, ele gera quando encontra a label, mesmo que não tenha nenhum goto apontando pra ele

   if(token == TK_label){
       // Adiciona o bloco sucessor por conta do Jump Label indicar o início de um novo bloco na execução sequencial
      if(blocoAtual > 0) addSucc(nomeBloco);

      // Caso chegue em um trecho de código que sucede um GoTo incondicional,
      // O código a seguir será código morto a não ser que tenha uma label imediatamente após o GoTo
      if(blocoAtual == -1) {
         blocoAtual = obterIndiceBloco(nomeBloco);
      }

      token = le_token();
      return 1;
   }
   return 0;
}

int Command(char Com_c[MAX_COD])
{
   if (token == TK_if)
      return IfExpression(Com_c);
   else if (token == TK_label){
      int i = JumpLabel(Com_c);
      return i;
   } else if (token == TK_goto) {
      int blocoAtualAntes = blocoAtual;
      int result = JumpExpression(Com_c);
      strcpy(grafo[blocoAtualAntes].commands, Com_c);
      strcpy(Com_c, "");
      // Encontrou um GoTo Incondicional 
      // (Não sabe qual o bloco atual)
      blocoAtual = -1;
      return result;
   }
   char Com_p[MAX_COD];
   return Expression(Com_p, Com_c);
}

int CriarBlocos(char Com_c[MAX_COD]){
   int results = 0;

   // Do while para garantir que execute a primeira vez
   do {
      grafoMudou = 0;
      // Move ponteiro do arquivo de entrada para o inicio
      rewind(arqin);        // rewind limpa também feof/ferror
      clearerr(arqin);      // redundante com rewind, mas seguro se optar por fseek depois
      linlex = 0; collex = 1;
      c = le_char();
      ifCount = 0;
      blocoAtual = 0;

      for(int i = 0; i < proxBloco; i++){
         grafo[i].commands[0] = '\0';
      }

      token = le_token();
      // Regra 1: Primeiro bloco começa na primeira instrução
      char nomeBloco[20];
      if(token == TK_label){
         strcpy(nomeBloco, lex);
         nomeBloco[strlen(lex) - 1] = '\0';
         strcpy(grafo[0].nome, nomeBloco);
      }
      else {
         strcpy(grafo[0].nome, "Inicio");
      }

      results = CommandList(Com_c);
      strcpy(grafo[blocoAtual].commands, Com_c);
      strcpy(Com_c, "");
   
   } while(grafoMudou == 1);

   return results;
}

int printGrafo() {
   FILE *arqout;

   if ((arqout = fopen("./output.txt", "wt")) == NULL)
   {
      printf("Erro na abertura do arquivo de saida");
      exit(0);
   }

   for(int i = 0; i < proxBloco; i++){
      printf("<<%s>>\n", grafo[i].nome);
      fprintf(arqout, "<<%s>>\n", grafo[i].nome);
      printf("%s\n", grafo[i].commands);
      fprintf(arqout, "%s\n", grafo[i].commands);

      printf("Sucessores: \n");
      fprintf(arqout, "Sucessores: \n");
      for(int j = 0; j < grafo[i].proxSucc; j++){
         printf("%s ", grafo[grafo[i].succs[j]].nome);
         fprintf(arqout, "%s ", grafo[grafo[i].succs[j]].nome);

      }
      printf("\n\n");
      fprintf(arqout, "\n\n");
   }

   fclose(arqout);

   return 1;
}

int main()
{

   if ((arqin = fopen("./input.txt", "rt")) == NULL)
   {
      printf("Erro na abertura do arquivo");
      exit(0);
   }
   

   // token = le_token();
   // // Regra 1: Primeiro bloco começa na primeira instrução
   // char nomeBloco[20];
   // if(token == TK_label){
   //    strcpy(nomeBloco, lex);
   //    nomeBloco[strlen(lex) - 1] = '\0';
   //    strcpy(grafo[0].nome, nomeBloco);
   // }
   // else {
   //    strcpy(grafo[0].nome, "Inicio");
   // }

   char Com_c[MAX_COD] = "";
   if (CriarBlocos(Com_c) == 0)
      printf("Erro no comando!!!\n");
   else
   {
      printGrafo();
   }
   fclose(arqin);
}