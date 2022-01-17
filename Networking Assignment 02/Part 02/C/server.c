
#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char **argv)
{
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;
    int max_clients = 10;
    pthread_t thread_id[max_clients];
    int numThreads = 0;

    // sent when ,client disconnected
    signal(SIGPIPE, SIG_IGN);

    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family = AF_INET;                // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port = htons(PORT);              // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) == -1)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // server loop
    while (numThreads < max_clients)
    {

        // accept connection to client
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
        {
            perror("Error accepting client");
        }
        else
        {

            if (pthread_create(&thread_id[numThreads], NULL, handle_client, &client_socket) < 0)
            {
                numThreads = numThreads + 1;

                printf("\nAccepted client\n");
                //handle_client(client_socket);
            }
            else
            {
                if (server_socket < 0)
                {
                    printf("\n\n well now idk what im doing");
                }
                else
                {
                    numThreads = numThreads + 1;
                    // handle_client(client_socket);
                }
            }
        }
    }

    for (int index = 0; index <= numThreads; index++)
    {
        pthread_join(thread_id[index], NULL);
    }
}

/************************************************************************
 * handle client
 ************************************************************************/

void *handle_client(void *client_socket)
{
    char input;
    int client = *(int *)client_socket;
    int keep_going = TRUE;

    while (keep_going)
    {
        // read char from client
        switch (read(client, &input, sizeof(char)))
        {
        case 0:
            keep_going = FALSE;
            printf("\nEnd of stream, returning ...\n");
            break;
        case -1:
            perror("Error reading from network!\n");
            keep_going = FALSE;
            break;
        }

        printf("%c", input);

        // check if we terminate
        if (input == 'q')
        {
            keep_going = FALSE;
        }

        // send result back to client
        write(client, &input, sizeof(char));
    }
    // cleanup
    if (close(client) == -1)
    {
        perror("Error closing socket");
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("\nClosed socket to client, exit");
        pthread_exit(NULL);
    }
    return NULL;
}
