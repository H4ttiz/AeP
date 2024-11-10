#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define XOR_KEY 0x5A  			

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
int menu(int lin1, int col1, int qtd, char lista[5][40]);
void textColor(int letras, int fundo);
void menuLogin();
void addUser(char username[15], char password[15]);
void listUsers();
bool validSenha(char password[15]);
void ExcluUsers();
bool validUsuario(char username[15], char password[15]);
void criptoSENHA(char senha[15], char senhaCriptografada[15]);
void criptoUSUARIO(char usuario[15], char usuarioCriptografado[15]);
void salvarUsuariosEmArquivo();
void carregarUsuariosDeArquivo();

enum {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
    DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
    LIGHTMAGENTA, YELLOW, WHITE
};


enum {
    _BLACK = 0, _BLUE = 16, _GREEN = 32, _CYAN = 48,
    _RED = 64, _MAGENTA = 80, _BROWN = 96, _LIGHTGRAY = 112,
    _DARKGRAY = 128, _LIGHTBLUE = 144, _LIGHTGREEN = 160,
    _LIGHTCYAN = 176, _LIGHTRED = 192, _LIGHTMAGENTA = 208,
    _YELLOW = 224, _WHITE = 240
};

char usernames[100][15];
char passwords[100][15];
int userCount = 0;
int tp;

void clearScreen() {
    system("cls");
    textColor(WHITE, _BLACK); 
}

void ordemAlfa();
void alterarUsuario();
	
int main() {
    int opc;
    
    carregarUsuariosDeArquivo();
    ordemAlfa();
    
    char lista[5][40] = { " Incluir ", " Alterar ", " Excluir ", " Listar  ", " Fim     " };

    setlocale(LC_ALL, "Portuguese");
    while (true) {
        clearScreen();
        opc = menu(10, 10, 5, lista);
        if (opc == 5 || opc == 0) {
            break;
        } else if (opc == 1) {
            menuLogin();
            salvarUsuariosEmArquivo();
        } else if (opc == 2){
        	alterarUsuario();	
		} else if (opc == 4) {
            listUsers();
        } else if (opc == 3) {
            ExcluUsers();
        }
        linhaCol(1, 1);
        textColor(WHITE, _BLACK);
        printf("\nEscolheu a opção %d", opc);
    }
    textColor(WHITE, _BLACK);
    linhaCol(24, 1);
    printf("");

    return 0;
}

//---------------------------------------------------------------------------------------------
void textColor(int letra, int fundo) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}
//---------------------------------------------------------------------------------------------
void linhaCol(int lin, int col) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){ col - 1, lin - 1 });
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
//---------------------------------------------------------------------------------------------
void box(int lin1, int col1, int lin2, int col2) {
	int i,j;
    for (i = col1; i <= col2; i++) {
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for ( i = lin1; i <= lin2; i++) {
        linhaCol(i, col1);
        printf("%c", 179);
        linhaCol(i, col2);
        printf("%c", 179);
    }

    for ( i = lin1 + 1; i < lin2; i++) {
        for ( j = col1 + 1; j < col2; j++) {
            linhaCol(i, j);
            printf(" ");
        }
    }

    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}
//---------------------------------------------------------------------------------------------
int menu(int lin1, int col1, int qtd, char lista[5][40]) {
    int opc = 1, lin2, col2, linha, i, tamMaxItem, tecla = 0;

    tamMaxItem = strlen(lista[0]);
    for (i = 1; i < qtd; i++) {
        if (strlen(lista[i]) > tamMaxItem) {
            tamMaxItem = strlen(lista[i]);
        }
    }
    lin2 = lin1 + (qtd * 2 + 2);
    col2 = col1 + tamMaxItem + 4;

    textColor(WHITE, _BLUE);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");
    while (tecla != 27 && tecla != 13) {
        linha = lin1 + 2;
        for (i = 0; i < qtd; i++) {
            if (i + 1 == opc) textColor(BLACK, _LIGHTGREEN);
            else textColor(WHITE, _BLUE);
            linhaCol(linha, col1 + 2);
            printf("%s", lista[i]);
            linha += 2;
        }

        linhaCol(1, 1);
        tecla = getch();
        linhaCol(22, 1);

        if (tecla == 224 || tecla == 0) {
            tecla = getch();
        }

        switch (tecla) {
            case 72: opc--; break; 
            case 80: opc++; break; 
        }

        if (opc == 0) opc = 5; 
        if (opc == qtd + 1) opc = 1; 
        if (tecla == 27) opc = 5;
    }
    return opc;
}

//---------------------------------------------------------------------------------------------
void addUser(char username[15], char password[15]) {
    if (userCount < 100) {
        strcpy(usernames[userCount], username);
        strcpy(passwords[userCount], password);
        userCount++;
    } else {
        printf("Limite de usuários alcançado.\n");
    }
}
//---------------------------------------------------------------------------------------------
void listUsers() {
    char mod;
    int i;
    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(2, 45, 26, 100); 
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(2, 48);
    printf("Usuarios e senhas\n");

 
    linhaCol(20, 48);
    printf("Pressione 1 para ver senhas descriptografadas\n"); 
    linhaCol(22, 56);
	printf("Pressione 2 para criptografadas\n");
	linhaCol(25, 47);
	printf("ESC para voltar\n");

   
    while (true) {
        mod = getch();
        if (mod == 49) {
           
            for ( i = 0; i < userCount; i++) {
                linhaCol(3 + i, 48);
                printf("Usuario: %s  |  Senha: %s\n", usernames[i],passwords[i]);
            }
            break;
        } else if (mod == 50) {
            // Mostrar senhas criptografadas
            for ( i = 0; i < userCount; i++) {
            	char senhaCriptografada[100][15];
            	char userCriptografada[100][15];
            	criptoUSUARIO(usernames[i],userCriptografada[i]);
                criptoSENHA(passwords[i], senhaCriptografada[i]);
                linhaCol(3 + i, 50);
                printf("Usuário: %s  |  Senha: %s\n",userCriptografada[i], senhaCriptografada[i]);
            }
            break;
        } else if (mod == 27) {
            // ESC para voltar
            break; 
        }
    }

    linhaCol(25, 47);
    printf("Pressione ESC para voltar");
    while (getch() != 27); 
}
//---------------------------------------------------------------------------------------------
void menuLogin() {
    char username[15];
    char password[15];
    int i,j;

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(12, 60, 16, 90);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(13, 62);
    printf("Username: ");

    linhaCol(13, 72);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(14, 60);
        textColor(WHITE, _BLUE);
        linhaCol(15, 62);
        linhaCol(14, 60);
        for ( i = 0; i < 30; i++) printf(""); 
        linhaCol(14, 62);
        printf("Password: ");

        i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) { 
            if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b"); 
            } else {
                password[i++] = ch;
                printf("*"); 
            }
        }
        password[i] = '\0'; 

        valid = validSenha(password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(15, 62);
            printf("Senha inválida! ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("A senha deve ter número!");
            Sleep(2000);
            linhaCol(15, 62);
            printf("letra maiúscula!        ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("letra minúscula!        ");
            Sleep(2000);
            linhaCol(15, 62);
            printf("caracter especial!      ");
            Sleep(2000);
            linhaCol(15, 62);
			textColor(WHITE, _BLUE);
        	linhaCol(15, 62);
        	for ( j = 0; j < 28; j++) printf(" ");
        	linhaCol(14, 62);
            for ( j = 0; j < 28; j++) printf(" "); 
        }
    }

    addUser(username, password);
    textColor(WHITE, _GREEN);
    linhaCol(15, 62);
    printf("Login feito com sucesso!\n");
    Sleep(2000);
}
//---------------------------------------------------------------------------------------------
bool validSenha(char password[15]) {
    int pontoM = 0, pontom = 0, pontoN = 0, pontoC = 0;
    int tam = strlen(password);
    int cont;

    for ( cont = 0; cont < tam; cont++) {
        char copia = password[cont];
        if (isupper(copia)) pontoM++;
        if (islower(copia)) pontom++;
        if (isdigit(copia)) pontoN++;
        if (!isalnum(copia)) pontoC++;
    }

    return (pontoM > 0 && pontom > 0 && pontoN > 0 && pontoC > 0);
}
//---------------------------------------------------------------------------------------------
void ExcluUsers() {
    char username[15];
    char password[15];

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(12, 70, 20, 100);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(13, 72);
    printf(" Deseja Excluir um Usuário ");
    textColor(WHITE, _BLUE);
    linhaCol(15, 72);
    printf("Username: ");

    linhaCol(15, 82);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(16, 72);
        printf("Password: ");
		int j;
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) {        
            if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b"); 
            } else {
                password[i++] = ch;
                printf("*"); 
            }
        }
        password[i] = '\0'; 

        valid = validUsuario(username, password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(17, 72);
            printf("Usuário ou SENHA "); 
            textColor(WHITE, _RED);
            linhaCol(18, 72);
            printf("Não identificado "); 
            Sleep(2000);
            linhaCol(17, 72);
            textColor(WHITE, _BLUE);
            linhaCol(16, 72);
			for ( j = 0; j < 28; j++) printf(" ");
            textColor(WHITE, _BLUE);
            linhaCol(17, 72);
            for ( j = 0; j < 28; j++) printf(" "); 
            textColor(WHITE, _BLUE);
            linhaCol(18, 72);
            for ( j = 0; j < 28; j++) printf(" "); 
        }
    }
	int i,j;
    for ( i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0) {
            // Move os usuários para cima
            for ( j = i; j < userCount - 1; j++) {
                strcpy(usernames[j], usernames[j + 1]);
                strcpy(passwords[j], passwords[j + 1]);
            }
            userCount--; // Diminui o contador de usuários
            break;
        }
    }

    textColor(WHITE, _GREEN);
    linhaCol(17, 72);
    printf("USUÁRIO EXCLUIDO!\n");
    Sleep(2000); // Pausa para que o usuário veja a mensagem
    ordemAlfa();
    salvarUsuariosEmArquivo();
}
//---------------------------------------------------------------------------------------------
bool validUsuario(char username[15], char password[15]) {
	int i;
    for (i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0 && strcmp(passwords[i], password) == 0) {
            return true; // Usuário e senha válidos
        }
    }
    return false; // Usuário ou senha inválidos
}
//---------------------------------------------------------------------------------------------
void criptoSENHA(char senha[15],char senhaCriptografada[15]) {
	int i;
    int tamanho = strlen(senha);
    for (i = 0; i < tamanho; i++) {
        senhaCriptografada[i] = senha[i] ^ XOR_KEY; // Criptografia simples
    }
    senhaCriptografada[tamanho] = '\0'; // Termina a string
}
//---------------------------------------------------------------------------------------------
void criptoUSUARIO(char usuario[15],char usuarioCriptografado[15]) {
	int i;
    int tamanho = strlen(usuario);
    for (i = 0; i < tamanho; i++) {
        usuarioCriptografado[i] = usuario[i] ^ XOR_KEY; // Criptografia simples
    }
    usuarioCriptografado[tamanho] = '\0'; // Termina a string
}
//---------------------------------------------------------------------------------------------
void ordemAlfa() {
    char tmp1[15], tmp2[15];
    int x, y;

    for (x = 0; x < userCount - 1; x++) {
        for (y = x + 1; y < userCount; y++) {            
            if (strcmp(usernames[y], usernames[x]) < 0) {                
                strcpy(tmp1, usernames[x]);
                strcpy(tmp2, passwords[x]);
                strcpy(usernames[x], usernames[y]);
                strcpy(passwords[x], passwords[y]);
                strcpy(usernames[y], tmp1);
                strcpy(passwords[y], tmp2);
            }
        }
    }
}
//---------------------------------------------------------------------------------------------
void salvarUsuariosEmArquivo() {
	int i;
    FILE *file = fopen("AEP LISTA.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir arquivo para salvar usuários!\n");
        return;
    }
	
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %s\n", usernames[i], passwords[i]);
    }
	ordemAlfa();
    fclose(file);
    printf("Usuários salvos com sucesso no arquivo!\n");
}
//---------------------------------------------------------------------------------------------
void carregarUsuariosDeArquivo() {
    FILE *file = fopen("AEP LISTA.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir arquivo para carregar usuários!\n");
        return;
    }
	ordemAlfa();
    while (fscanf(file, "%s %s", usernames[userCount], passwords[userCount]) != EOF) {
        userCount++;
    }

    fclose(file);
    printf("Usuários carregados com sucesso do arquivo!\n");
}
//---------------------------------------------------------------------------------------------
void alterarUsuario() {
    char username[15];
    char password[15];
    char novoUsername[15];
    char novaSenha[15];

    linhaCol(12, 10);
    setlocale(LC_ALL, "C");
    box(10, 60, 25, 120);
    setlocale(LC_ALL, "");
    textColor(WHITE, _BLUE);
    linhaCol(10, 62);
    printf(" Alterar Usuário ");
    textColor(WHITE, _BLUE);
    linhaCol(12, 62);
    printf("Username atual: ");
    linhaCol(12, 78);
    scanf("%s", username);

    bool valid = false;
    while (!valid) {
        linhaCol(14, 62);
        printf("Senha atual: ");
        int j;
        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) {         
            if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b"); 
            } else {
                password[i++] = ch;
                printf("*"); 
            }
        }
        password[i] = '\0'; 

        // Valida o usuário e senha
        valid = validUsuario(username, password);
        if (!valid) {
            textColor(WHITE, _RED);
            linhaCol(16, 62);
            printf("Usuário ou senha incorretos! ");
            Sleep(2000);
            textColor(WHITE, _BLUE);
            linhaCol(16, 63);
            for (j = 0; j < 28; j++) printf(" "); 
            textColor(WHITE, _BLUE);
            linhaCol(16, 62);
            for (j = 0; j < 28; j++) printf(" ");
            textColor(WHITE, _BLUE);
            linhaCol(14, 62);
            for (j = 0; j < 28; j++) printf(" ");
        }
    }

    linhaCol(18, 75);
    textColor(WHITE, _BLUE);                      
    printf("Novo Username: ");
    linhaCol(18, 90);
    scanf("%s", novoUsername);
    int i;
	bool valida = false;
    while (!valida) {
        linhaCol(14, 60);
        textColor(WHITE, _BLUE);
        linhaCol(15, 62);
        linhaCol(14, 60);
        for (i = 0; i < 30; i++) printf(""); 
        linhaCol(19, 75);
        printf("Nova Senha: ");

        int i = 0;
        char ch;
        while ((ch = getch()) != '\r' && i < 29) { 
            if (ch == 8 && i > 0) { 
                i--;
                printf("\b \b"); 
            } else {
                novaSenha[i++] = ch;
                printf("*"); 
            }
        }
        novaSenha[i] = '\0'; 
		int j;
        valida = validSenha(novaSenha);
        if (!valida) {
            textColor(WHITE, _RED);
            linhaCol(20, 75);
            printf("Senha inválida! ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("A senha deve ter número!");
            Sleep(2000);
            linhaCol(20, 75);
            printf("letra maiúscula!        ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("letra minúscula!        ");
            Sleep(2000);
            linhaCol(20, 75);
            printf("caracter especial!      ");
            Sleep(2000);
            linhaCol(20, 75);
			textColor(WHITE, _BLUE);
        	linhaCol(20, 75);
        	for (j = 0; j < 28; j++) printf(" ");
        	linhaCol(19, 75);
            for (j = 0; j < 28; j++) printf(" "); 
        }
    }

    
    for ( i = 0; i < userCount; i++) {
        if (strcmp(usernames[i], username) == 0 && strcmp(passwords[i], password) == 0) {
            
            if (strlen(novoUsername) > 0) {
                strcpy(usernames[i], novoUsername);
            }
            
            if (strlen(novaSenha) > 0) {
                strcpy(passwords[i], novaSenha);
            }
            break;
        }
    }

    textColor(WHITE, _GREEN);
    linhaCol(20, 72);
    printf("Usuário alterado com sucesso!");
    Sleep(2000);
    ordemAlfa();
    salvarUsuariosEmArquivo();
}