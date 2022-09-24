#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;

int port; 

int main(int argc, char *argv[])
{

  int sd;
  int index;
  struct sockaddr_in server; 
  char msg[100];
  char raspuns[100];
  int raspunsint = 0;
  char destinatar[100];
  char mesaj[500];
  char nou[100];
  char user[100];
  char necitite[100];
  char mesaj_necitit[300];
  char mesaj_reply[300][300];

  port = atoi(argv[2]);
  sd = socket(AF_INET, SOCK_STREAM, 0);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(port);

  connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr));

  printf("Bine ati venit la Offline-Messenger! \n");
  printf("Comenzile disponibile in aplicatie sunt: \n");
  printf("  >>'Login' - pentru a te loga\n");
  printf("  >>'Logout' - pentru a te deloga\n");
  printf("  >>'Create-account' - pentru a crea un cont\n");
  printf("  >>'Users-list' - pentru a afisa lista utilizatorilor\n");
  printf("  >>'Send-message' - pentru a trimite un mesaj unui utilizator\n");
  printf("  >>'View-conversation' - pentru a vizualiza conversatia cu un utilizator\n");
  printf("  >>'Reply' - pentru a raspunde specific la un mesaj primit\n");
  printf("  >>'Commands' - pentru a vizualiiza comenzile disponibile\n");
  printf("  >>'Refresh' - pentru a reimprospata aplicatia\n");
  printf("  >>'Quit' - pentru a inchide clientul\n\n\n");
_start:

  read(sd, &raspunsint, 4);
  if (raspunsint == 1)
  {
    read(sd, &raspunsint, 4);
    if (raspunsint == 0)
    {
      printf("Nu aveti mesaje necitite\n");
    }
    else
    {
      printf("Aveti mesaje necitite!\n\n");
      printf("Doriti sa le vedeti[y] sau continuati[n]\n");
      printf("  >>");
      bzero(necitite, 100);
      scanf("%s", necitite);
      write(sd, necitite, 100);
      if (strcmp(necitite, "y") == 0)
      {
        for (int i = 0; i < raspunsint; i++)
        {
          bzero(mesaj_necitit, 300);
          read(sd, mesaj_necitit, 300);
          printf("%s", mesaj_necitit);
        }
      }
    }
  }

  bzero(msg, 100);
  fflush(stdout);
  printf("\nIntroduceti o comanda: \n");
  printf("  >>");

  scanf("%s", msg);

  write(sd, msg, 100);

  if (strcmp(msg, "Login") == 0)
  {
    printf("Intrduceti un nume de utilizator:\n");
    printf("  >>");
    bzero(msg, 100);
    scanf("%s", msg);
    write(sd, msg, 100);
    read(sd, &raspunsint, 4);
    if (raspunsint == 0)
      printf("Nu a fost gasit un utilizator cu acest nume!\n");
    else
    {
      printf("Introduceti parola:\n");
      printf("  >>");
      bzero(msg, 100);
      scanf("%s", msg);
      write(sd, msg, 100);
      read(sd, &raspunsint, 4);
      if (raspunsint == 1)
      {
        printf("\nBine ati revenit in contul dumneavoastra!\n\n");
      }
      else
        printf("\nParola gresita. Incercati din nou!\n\n\n");
    }
  }

  else if (strcmp(msg, "Users-list") == 0)
  {
    read(sd, &raspunsint, 4);
    if (raspunsint == 1)
    {
      bzero(msg, 100);
      read(sd, msg, 100);
      printf("Lista cu utilizatorii:\n\n");
      printf("%s\n", msg);
    }
    else
      printf("Nu sunteti logat\n");
  }

  else if (strcmp(msg, "Logout") == 0)
  {
    read(sd, &raspunsint, 4);
    if (raspunsint == 1)
      printf("Ati iesit din contul dumneavoastra\n\n\n");
    else
      printf("Nu sunteti logat pentru a va deloga\n\n\n");
  }

  else if (strcmp(msg, "Send-message") == 0)
  {
    read(sd, &raspunsint, 4);
    if (raspunsint == 1)
    {
      printf("\nIntroduceti destinatarul: \n");
      printf("  >>");
      bzero(destinatar, 100);
      scanf("%s", destinatar);
      write(sd, destinatar, 100);
      read(sd, &raspunsint, 4);
      if (raspunsint == 1)
      {
        printf("\nIntroduceti mesajul:\n");
        printf("  >>");
        bzero(mesaj, 500);
        read(0, mesaj, 500);
        write(sd, mesaj, 500);
      }
      else
      {
        printf("\nNu exista un destinatar cu acest username\n");
      }
    }
    else
      printf("Trebuie sa fiti logat pentru a trimite un mesaj\n\n");
  }

  else if (strcmp(msg, "Create-account") == 0)
  {
  create:
    printf("Introduceti username nou:\n");
    printf("  >>");
    bzero(nou, 100);
    scanf("%s", nou);
    write(sd, nou, 100);

    read(sd, &raspunsint, 4);
    if (raspunsint == 0)
    {
      printf("Exista deja un utiliazator cu acest nume\n");
      goto create;
    }
    else
    {
      printf("\nIntroduceti parola noua:\n");
      printf("  >>");
      bzero(nou, 100);
      scanf("%s", nou);
      write(sd, nou, 100);
      bzero(nou, 100);
      read(sd, nou, 100);
      printf("%s\n", nou);
      goto _start;
    }
  }

  else if (strcmp(msg, "View-conversation") == 0)
  {
    read(sd, &raspunsint, 4);
    if (raspunsint == 1)
    {
      printf("\nIntroduceti utilizatorul cu care doriti sa vedeti conversatia:\n");
      printf("  >>");
      bzero(user, 100);
      scanf("%s", user);
      write(sd, user, 100);
      read(sd, &raspunsint, 4);
      if (raspunsint == 0)
      {
        printf("\nNu exista un utilizator cu acest username\n\n");
        goto _start;
      }
      int lungime_mesaje = 0;
      char mesaje[500];
      bzero(mesaje, 500);
      read(sd, &lungime_mesaje, 4);

      for (int i = 0; i < lungime_mesaje; i++)
      {
        read(sd, mesaje, 500);
        printf("%s", mesaje);
      }
    }
    else
    {
      printf("Trebuie sa fiti logat pentru a va vedea conversatiile\n\n");
      goto _start;
    }
  }

  else if(strcmp(msg,"Reply")==0)
  {
    read(sd,&raspunsint,4);
    for(int i=0;i<raspunsint;i++)
    {
      bzero(mesaj_reply[i],300);
      read(sd,mesaj_reply[i],300);
      printf("[%d]  %s",i,mesaj_reply[i]);
    }

    printf("Introduceti indexul mesajului la care doriti sa raspundeti\n");
    printf("  >>");
    scanf("%d",&index);
    write(sd,&index,4);
    if(index < 0 || index >= raspunsint)
    {
      printf("Introduceti un index valid\n");
      goto _start;
    }
    printf("Introduceti raspunsul pentru mesajul: \n'%s'\n",mesaj_reply[index]);
    printf("  >>");
    bzero(mesaj,500);
    fflush(stdout);
    read(0,mesaj,500);
    printf("MESAJ: %s\n",mesaj);

    write(sd,mesaj,300);
    bzero(mesaj,500);
    goto _start;
   }
  
  else if (strcmp(msg, "Commands") == 0)
  {
    printf("\nComenzile disponibile in aplicatie sunt: \n");
    printf("  >>'Login' - pentru a te loga\n");
    printf("  >>'Logout' - pentru a te deloga\n");
    printf("  >>'Create-account' - pentru a crea un cont\n");
    printf("  >>'Users-list' - pentru a afisa lista utilizatorilor\n");
    printf("  >>'Send-message' - pentru a trimite un mesaj unui utilizator\n");
    printf("  >>'View-conversation' - pentru a vizualiza conversatia cu un utilizator\n");
    printf("  >>'Reply' - pentru a raspunde specific la un mesaj primit\n");
    printf("  >>'Commands' - pentru a vizualiiza comenzile disponibile\n");
    printf("  >>'Refresh' - pentru a reimprospata aplicatia\n");
    printf("  >>'Quit' - pentru a inchide clientul\n\n\n");
  }

  else if(strcmp(msg,"Quit")==0)
  {
    printf("\nAti parasit aplicatia Offline-Messenger.\n\n\n");
    close(sd);
    exit(0);
  }
  else if(strcmp(msg,"Refresh")==0)
  {
    goto _start;
  }
  else
  {
    printf("\nComanda nu a fost recunoscuta.\n");
  }

  goto _start;
  close(sd);
}

