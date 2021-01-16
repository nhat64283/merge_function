#include"ServerAPI.h"

void registerAccount(int client_sock, int *status) { //done
	char username[255];
	char password[64];
	char buffer[BUFF_SIZE];
	int bytes_sent, bytes_received;

	// if already logged in, cannot registerAccount
	if(*status == 1) {
		fprintf(stderr, "You are already logged in. Cannot register account.\n");
		bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}
		return;
	}

	// send username to server
	while(1) {
		printf("Please enter the username: ");
		fgets(username, 255, stdin);
		username[strlen(username)-1] = '\0';

		// enter nothing, cancel registerAccount
		if(username[0] == '\0') {
			bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
			if (bytes_sent <= 0) {
				fprintf(stderr, "Failed to connect to server. Try again.\n");
				return;
			}
			return;
		}

		bytes_sent = send(client_sock, username, strlen(username), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}

		bzero(buffer, BUFF_SIZE);
		bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
		if(bytes_received <= 0) {
			fprintf(stderr, "Failed to verify username. Try again.\n");
			return;
		}
		else 
			buffer[bytes_received] = '\0';

		if(strcmp(buffer, MSG_DUP) == 0) {
			fprintf(stderr, "Account existed.\n");
			continue;
		}
		else if(strcmp(buffer, MSG_ERROR) == 0) {
			fprintf(stderr, "Error occurred. Try again.\n");
			return;
		}
		else 
			break;
	}
	
	// send password to server
	printf("Please enter the password: ");
	fgets(password, 64, stdin);
	password[strlen(password)-1] = '\0';

	bytes_sent = send(client_sock, password, strlen(password), 0);
	if (bytes_sent <= 0) {
		fprintf(stderr, "Failed to connect to server. Try again.\n");
		return;
	}

	bzero(buffer, BUFF_SIZE);
	bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
	if(bytes_received <= 0) {
		fprintf(stderr, "Failed to verify account. Try again.\n");
		return;
	}
	else 
		buffer[bytes_received] = '\0';

	if(strcmp(buffer, MSG_ERROR) == 0) 
		fprintf(stderr, "Error occurred. Try again.\n");
	else 
		printf("Successful registration.\n");

	return;
}

void signInAccount(int client_sock, int *status, char **sessionID) { // done
	char username[255];
	char password[64];
	char buffer[BUFF_SIZE];
	int bytes_sent, bytes_received;

	if(*status == 1) {
		fprintf(stderr, "You are already logged in.\n");
		bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}
		return;
	}
	// send username to server
	printf("Please enter the username: ");
	fgets(username, 255, stdin);
	username[strlen(username)-1] = '\0';
	// enter nothing, cancel signInAccount
	if(username[0] == '\0') {
		bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}
		return;
	}

	bytes_sent = send(client_sock, username, strlen(username), 0);
	if (bytes_sent <= 0) {
		fprintf(stderr, "Failed to connect to server. Try again.\n");
		return;
	}

	bzero(buffer, BUFF_SIZE);
	bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
	if(bytes_received <= 0) {
		fprintf(stderr, "Failed to verify username. Try again.\n");
		return;
	}
	else
		buffer[bytes_received] = '\0';

	if(strcmp(buffer, MSG_FALSE) == 0) { // account not found
		fprintf(stderr, "Account not found. Try again.\n");
		return;
	}
	else if(strcmp(buffer, MSG_ERROR) == 0) {	// Other error occurred
		fprintf(stderr, "Error occurred. Try again.\n");
		return;
	}
	else if(strcmp(buffer, MSG_BLOCKED) == 0){
		fprintf(stderr, "Account was blocked.Please contact your system administrator.\n");
		return;
	}

	for(int index = 0; index < 3; index++) {
		printf("Please enter the password: ");
		fgets(password, 64, stdin);
		password[strlen(password)-1] = '\0';

		bytes_sent = send(client_sock, password, strlen(password), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}

		bzero(buffer, BUFF_SIZE);
		bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
		if(bytes_received <= 0) {
			fprintf(stderr, "Failed to verify account. Try again.\n");
			return;
		}
		else
			buffer[bytes_received] = '\0';

		if(strcmp(buffer, MSG_FALSE) == 0) {
			fprintf(stderr, "Password is incorrect.\n");
		}
		else {
			printf("Hello %s\n", buffer);
			*status = 1;
			// sessionID will be kept at sessionID
			*sessionID = strdup(buffer);
			return; //break loop
		}
	}
	// Wrong password more than 3 times. Block account.
	printf("Account \"%s\" is blocked.\n", username);
	return;
}

void searchAccount(int client_sock) { //done 
	char username[255];
	char buffer[BUFF_SIZE];
	int bytes_sent, bytes_received;

	printf("Please enter the username: ");
	fgets(username, 255, stdin);
	username[strlen(username)-1] = '\0';
	// enter nothing, cancel searchAccount
	if(username[0] == '\0') {
		bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to connect to server. Try again.\n");
			return;
		}
		return;
	}

	bytes_sent = send(client_sock, username, strlen(username), 0);
	if (bytes_sent <= 0) {
		fprintf(stderr, "Failed to connect to server. Try again.\n");
		return;
	}

	bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
	if(bytes_received <= 0) {
		fprintf(stderr, "Failed to verify username. Try again.\n");
		return;
	}
	else
		buffer[bytes_received] = '\0';

	if(strcmp(buffer, MSG_TRUE) == 0) // return true mean user found and active
		printf("Account is active\n");
	else if(strcmp(buffer, MSG_FALSE) == 0) //return false mean user found and not active
		printf("Account is blocked\n");
	else 
		printf("Cannot find account.\n");

	return;
}

void signOutAccount(int client_sock, int *status, char *sessionID) { // done
	char buffer[BUFF_SIZE];
	int bytes_sent, bytes_received;

	if(*status == 0) {
		fprintf(stderr, "Account is not signed in.\n");
		bytes_sent = send(client_sock, MSG_FALSE, strlen(MSG_FALSE), 0);
		if (bytes_sent <= 0) {
			fprintf(stderr, "Failed to signed out. Try again.\n");
			return;
		}
		return;
	}

	bytes_sent = send(client_sock, sessionID, strlen(sessionID), 0);
	if (bytes_sent <= 0) {
		fprintf(stderr, "Failed to signed out. Try again.\n");
		return;
	}

	bzero(buffer, BUFF_SIZE);
	bytes_received = recv(client_sock, buffer, BUFF_SIZE, 0);
	if(bytes_received <= 0) {
		fprintf(stderr, "No message from server. Connection lost.\n");
		return;
	}
	else
		buffer[bytes_received] = '\0';

	if(strcmp(buffer, MSG_TRUE) == 0) {
		printf("Goodbye %s.\n", sessionID);
		free(sessionID); // free sessionID
		*status = 0;
	}
	else{
		fprintf(stderr, "Error occurred. Try again.\n");
	}
		
	return;
}

void fileTransfer(int client_sock) {
	int bytes_sent;
	char buff[BUFF_SIZE];

	int status;
	int choice;
	int cache;

	//Step 4: Communicate with server			
	//send message
	do{
		printf("Simple TCP-based download/upload application\n");
		printf("---------------------------------------------\n");
		printf("	1. Upload\n");
		printf("	2. Download\n");
		printf("	3. Delete\n");
		printf("Your choice (1-3, other to quit):\n");
		scanf("%d", &choice);
		while((cache = getchar()) != '\n' && cache != EOF);
		// send choice to server
		sprintf(buff, "%d", choice);
		bytes_sent = send(client_sock, buff, strlen(buff), 0);
		if(bytes_sent < 0){
			perror("\nError: ");
			return;
		}

		switch(choice) {
			case 1:
				printf("Insert string to send:");
				memset(buff,'\0',(strlen(buff)+1));
				fgets(buff, BUFF_SIZE, stdin);	
				buff[strlen(buff)-1] = '\0'; //remove trailing newline	
				status = upload(client_sock, buff);
				break;
			case 2:
				status = download(client_sock, "Downloaded");
				break;
			case 3:
				status = delete(client_sock);
				break;
			default:
				break;
		}

	} while(choice > 0 && choice < 4);

	return;
}