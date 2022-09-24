#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define PORT 7777

extern int errno;

char username[30][30];
char mesaje[500][500];
char mesaje_necitite[300][300];
char mesaje_reply[300][300];
int lungime_username;
int lungime_mesaje;
int lungime_mesaje_necitite;
int lungime_mesaje_reply;
char parola[30][30];
int da = 0;
int da2 = 0;
int k = 0;
int k1 = 0;
int k2 = 0;
int k3 = 0;
char nume_user[100];
char nume_user_aux[100];

static int callback_reply(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < 1; i++)
	{

		if (strcmp(argv[i + 1], nume_user) == 0)
		{
			strcpy(mesaje_reply[k3], argv[i]);
			strcat(mesaje_reply[k3], ": ");
			strcat(mesaje_reply[k3], argv[2]);
			strcat(mesaje_reply[k3], "\n");
			k3++;
		}
	}
	lungime_mesaje_reply = k3;

	return 0;
}

static int callback_nevazut(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < 1; i++)
	{
		if (strcmp(argv[3], "0") == 0 && strcmp(argv[1], nume_user) == 0)
		{
			strcpy(mesaje_necitite[k2], argv[i]);
			strcat(mesaje_necitite[k2], ": ");
			strcat(mesaje_necitite[k2], argv[2]);
			if(strcmp(argv[4],"")!=0)
			{
			strcat(mesaje_necitite[k2], "  ->replied to >> ");
			strcat(mesaje_necitite[k2], argv[4]);
			}
			else
			strcat(mesaje_necitite[k2], "\n");
			k2++;
		}
	}
	lungime_mesaje_necitite = k2;
	return 0;
}

static int callback_conversatie(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < 1; i++)
	{

		if ((strcmp(argv[i], nume_user) == 0 && strcmp(argv[i + 1], nume_user_aux) == 0) || (strcmp(argv[i + 1], nume_user) == 0 && strcmp(argv[i], nume_user_aux) == 0))
		{

			strcpy(mesaje[k1], argv[i]);
			strcat(mesaje[k1], ": ");
			strcat(mesaje[k1], argv[2]);
			if(strcmp(argv[4],"")!=0)
			{
			strcat(mesaje[k1], "  ->replied to >> ");
			strcat(mesaje[k1], argv[4]);
			}
			else
			strcat(mesaje[k1], "\n");

			k1++;
		}
	}
	lungime_mesaje = k1;

	return 0;
}

static int callback_insert(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
	}

	return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < 1; i++)
	{
		strcpy(username[k], argv[i]);
		strcpy(parola[k], argv[i + 1]);
		k++;
	}
	lungime_username = k;
	return 0;
}

int main()
{
	int raspunsint;
	struct sockaddr_in server; 
	struct sockaddr_in from;
	char msg[100];			 
	char msgrasp[100] = " "; 
	int sd;					 
	char destinatar[100];
	char mesaj[500];
	char necitite[100];
	char mesaj_aux[300];
	char parola_user_aux[100];
	int ok;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char *data = "";

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[server]Eroare la socket().\n");
		return errno;
	}
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");

	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);


	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("[server]Eroare la bind().\n");
		return errno;
	}

	if (listen(sd, 1) == -1)
	{
		perror("[server]Eroare la listen().\n");
		return errno;
	}

	while (1)
	{
		int loggedin = 0; 
		int client;
		int length = sizeof(from);

		fflush(stdout);

		client = accept(sd, (struct sockaddr *)&from, &length);

		if (client < 0)
		{
			perror("[server]Eroare la accept().\n");
			continue;
		}

		int pid;
		if ((pid = fork()) == -1)
		{
			close(client);
			continue;
		}
		else if (pid > 0)
		{
			close(client);
			continue;
		}
		else if (pid == 0)
		{
			int indicele_utilizatorului;
			k = 0;
			close(sd);
		_start:

			write(client, &loggedin, 4);
			if (loggedin == 1)
			{
				k2 = 0;
				rc = sqlite3_open("data7.db", &db);
				sql = "SELECT * from mesaje";
				rc = sqlite3_exec(db, sql, callback_nevazut, (void *)data, &zErrMsg);

				write(client, &lungime_mesaje_necitite, 4);
				if (lungime_mesaje_necitite != 0)
				{
					bzero(necitite, 100);
					read(client, &necitite, 100);
					if (strcmp(necitite, "y") == 0)
					{
						for (int i = 0; i < lungime_mesaje_necitite; i++)
							write(client, mesaje_necitite[i], 300);

						rc = sqlite3_open("data7.db", &db);
						sql = sqlite3_mprintf("UPDATE mesaje set VAZUT = 1 where DESTINATAR= '%s'", nume_user);
						rc = sqlite3_exec(db, sql, callback_insert, (void *)data, &zErrMsg);
					}
				}
			}

			k = 0;
			rc = sqlite3_open("data7.db", &db);
			sql = "SELECT * from utilizatori";
			rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
	
			bzero(msg, 100);
			fflush(stdout);

		
			read(client, msg, 100);

			if (strcmp(msg, "Login") == 0)
			{
				bzero(nume_user, 100);
				bzero(nume_user_aux, 100);

				bzero(msg, 100);
				read(client, msg, 100);
				bzero(nume_user_aux, 100);
				strcpy(nume_user_aux, msg);
				k = 0;
				rc = sqlite3_open("data7.db", &db);
				sql = "SELECT * from utilizatori";
				rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

				for (int k = 0; k < lungime_username; k++)
				{
					if (strcmp(username[k], msg) == 0)
					{
						da = 1;
						indicele_utilizatorului = k;
					}
				}

				if (da == 0)
				{
					int raspunsint = 0;

					write(client, &raspunsint, 4);
				}
				else
				{

					int raspunsint = 1;

					write(client, &raspunsint, 4);

					bzero(msg, 100);
					read(client, msg, 100);

					if (strcmp(parola[indicele_utilizatorului], msg) == 0)
						da2 = 1;

					if (da2 == 1)
					{

						int raspunsint = 1;
						loggedin = 1;
						strcpy(nume_user, nume_user_aux);

						write(client, &raspunsint, 4);
					}
					else
					{
						int raspunsint = 0;

						write(client, &raspunsint, 4);
					}
				}
			}
			if (strcmp(msg, "Users-list") == 0)
			{
				if (loggedin == 1)
				{
					raspunsint = 1;
					write(client, &raspunsint, 4);
					bzero(msgrasp, 100);
	
					for (int k = 0; k <= lungime_username; k++)
					{
						strcat(msgrasp, username[k]);
						strcat(msgrasp, "\n");
					}
					write(client, msgrasp, 100);
				}
				else
				{
					raspunsint = 0;

					write(client, &raspunsint, 4);
				}
			}
			if (strcmp(msg, "Logout") == 0)
			{
				if (loggedin == 1)
				{
					raspunsint = 1;
					loggedin = 0;
					da = 0;
					da2 = 0;
				}
				else
					raspunsint = 0;
				write(client, &raspunsint, 4);
			}

			if (strcmp(msg, "Send-message") == 0)
			{
				if (loggedin == 1)
				{
					raspunsint = 1;
					write(client, &raspunsint, 4);
					bzero(destinatar, 100);
					read(client, destinatar, 100);

					raspunsint = 0;
					for (k = 0; k < lungime_username; k++)
						if (strcmp(username[k], destinatar) == 0)
							raspunsint = 1;
					write(client, &raspunsint, 4);

					if (raspunsint == 1)
					{
						bzero(mesaj, 500);
						read(client, mesaj, 500);

						mesaj[strlen(mesaj) - 1] = '\0';
						rc = sqlite3_open("data7.db", &db);
						sql = sqlite3_mprintf("INSERT INTO mesaje (EXPEDITOR,DESTINATAR,MESAJ,VAZUT,REPLIED_TO) VALUES ('%s', '%s', '%s', 0, '%s');", nume_user, destinatar, mesaj,"");
						rc = sqlite3_exec(db, sql, callback_insert, 0, &zErrMsg);
					}
				}
				else
				{
					raspunsint = 0;
					write(client, &raspunsint, 4);
				}
			}

			if (strcmp(msg, "Create-account") == 0)
			{
				k = 0;
				rc = sqlite3_open("data7.db", &db);
				sql = "SELECT * from utilizatori";
				rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
			create:
				bzero(nume_user_aux, 100);
				read(client, nume_user_aux, 100);

				ok = 1;

				for (int k = 0; k < lungime_username; k++)
					if (strcmp(nume_user_aux, username[k]) == 0)
						ok = 0;

				if (ok == 1)
					raspunsint = 1;
				else
					raspunsint = 0;

				write(client, &raspunsint, 4);

				if (raspunsint == 0)
					goto create;

				bzero(parola_user_aux, 100);
				read(client, parola_user_aux, 100);
				rc = sqlite3_open("data7.db", &db);
				sql = sqlite3_mprintf("INSERT INTO utilizatori (USERNAME,PAROLA) VALUES ('%s', '%s');", nume_user_aux, parola_user_aux);
				rc = sqlite3_exec(db, sql, callback_insert, 0, &zErrMsg);

				bzero(parola_user_aux, 100);
				strcpy(parola_user_aux, "Cont creat cu succes. Acum va puteti loga.");
				write(client, parola_user_aux, 100);
				goto _start;
			}

			if (strcmp(msg, "View-conversation") == 0)
			{
				if (loggedin == 1)
					raspunsint = 1;
				else
					raspunsint = 0;

				write(client, &raspunsint, 4);
				if (loggedin == 1)
				{
					bzero(nume_user_aux, 100);
					read(client, nume_user_aux, 100);
					ok = 0;
					for (int k = 0; k < lungime_username; k++)
						if (strcmp(nume_user_aux, username[k]) == 0)
							ok = 1;

					write(client, &ok, 4);
					if (ok == 0)
						goto _start;

					k1 = 0;
					rc = sqlite3_open("data7.db", &db);
					sql = "SELECT * from mesaje";
					rc = sqlite3_exec(db, sql, callback_conversatie, (void *)data, &zErrMsg);

					write(client, &lungime_mesaje, 4);
					for (int k1 = 0; k1 < lungime_mesaje; k1++)
					{
						write(client, mesaje[k1], 500);
					}

					rc = sqlite3_open("data7.db", &db);
					sql = sqlite3_mprintf("UPDATE mesaje set VAZUT = 1 where DESTINATAR= '%s' AND EXPEDITOR= '%s'", nume_user, nume_user_aux);
					rc = sqlite3_exec(db, sql, callback_insert, (void *)data, &zErrMsg);
				}
				else
					goto _start;
			}
			if (strcmp(msg, "Reply") == 0)
			{
				k3 = 0;
				rc = sqlite3_open("data7.db", &db);
				sql = "SELECT * from mesaje";
				rc = sqlite3_exec(db, sql, callback_reply, (void *)data, &zErrMsg);

				write(client, &lungime_mesaje_reply, 4);
				for (int i = 0; i < lungime_mesaje_reply; i++)
				{
					write(client, mesaje_reply[i], 300);
				}
				bzero(&raspunsint, 4);
				read(client, &raspunsint, 4);
				if (raspunsint < 0 || raspunsint >= lungime_mesaje_reply)
				{
					goto _start;
				}

				bzero(mesaj_aux, 300);
				read(client, mesaj_aux, 300);
				bzero(destinatar, 100);
				int i = 0;
				for (i = 0; i < strlen(mesaje_reply[raspunsint]); i++)
				{
					if (mesaje_reply[raspunsint][i] == ':')
						break;
					destinatar[i] = mesaje_reply[raspunsint][i];
				}
				destinatar[i] = '\0';
			
				rc = sqlite3_open("data7.db", &db);
				sql = sqlite3_mprintf("INSERT INTO mesaje (EXPEDITOR,DESTINATAR,MESAJ,VAZUT,REPLIED_TO) VALUES ('%s', '%s', '%s', 0,'%s');", nume_user, destinatar, mesaj_aux, mesaje_reply[raspunsint]);
				rc = sqlite3_exec(db, sql, callback_insert, 0, &zErrMsg);
				goto _start;
			}
			if (strcmp(msg, "Quit") == 0)
			{
				exit(0);
			}

			goto _start;
			close(client);
			exit(0);
		}
	} 
	exit(0);
} 
